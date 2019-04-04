/*******************************/
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

#include "ethreadpool.h"
#include "msg_protocol.h"
/*******************************/


/***********************************************/
void* work(void*);

typedef struct {
  int status;
  struct sockaddr_in addr;
} client_info;
std::map<int, client_info> users;
pthread_mutex_t mutex;

/***********************************************/

int set_nonblocking(int fd) {
  int old_option = fcntl(fd, F_GETFL);
  int new_option = old_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_option);
  return old_option;
}

void addfd(int epollfd, int fd, bool one_shot) {
  epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
  if (one_shot) {
    event.events |= EPOLLONESHOT;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
  set_nonblocking(fd);
}

void handle_client_req(int fd) {
  char buff[128];
  char tem[32];
  memset(buff, '\0', 128);

  while (1) {
    // memset(tem, '\0', 32);
    int ret = recv(fd, tem, 32, 0);
    if (ret <= 0) break;
    strcat(buff, tem);
  }

  msgPkg* msgpkg = (msgPkg*)tem;
  if (msgpkg->msg_header.type == TYPE_REQ_LOGIIN) {
    char* user_id = msgpkg->msg_body.msg_login.user_id;
    printf("login_req : %s\n", user_id);
    // lock
    pthread_mutex_lock(&mutex);
    users[fd].status = 1;
    pthread_mutex_unlock(&mutex);
    // unlock
    return;
  }

  if (msgpkg->msg_header.type == TYPE_REQ_FRIENDS) {
    char* user_id = msgpkg->msg_body.msg_login.user_id;
    printf("friends_req : %s\n", user_id);
    // lock
    pthread_mutex_lock(&mutex);
    users[fd].status = 2;
    pthread_mutex_unlock(&mutex);
    // unlock
    close(fd);
    return;
  }
  return;
}

void accept_connect(int epfd, int lisenfd, int pfd) {
  struct sockaddr_in client_address;
  socklen_t client_addrlength = sizeof(struct sockaddr_in);
  client_info ci;
  char re;

  int connfd =
      accept(lisenfd, (struct sockaddr*)(&client_address), &client_addrlength);

  if (connfd < 0) {
    printf("error.\n");
    return;
  }
  addfd(epfd, connfd, false);
  ci.status = 1;
  ci.addr = client_address;
  // lock
  pthread_mutex_lock(&mutex);
  users[connfd] = ci;
  pthread_mutex_unlock(&mutex);
  // unlock
  while (1) {
    if ((read(pfd, &re, 1)) <= 0) return;
  }

}

void* work(void* args) {
  ethread_pool_t* pool = (ethread_pool_t*)args;
  int pipe_read_fd;
  for (int i = 0; i < pool->ethread_count; ++i) {
    if (pool->ethread_info[i].ethread == pthread_self()) {
      pipe_read_fd = pool->ethread_info[i].pipefd[0];
      break;
    }
  }

  int epollfd = epoll_create(1);
  addfd(epollfd, pool->listenfd, false);
  addfd(epollfd, pipe_read_fd, false);
  epoll_event events[50];

  while (1) {
    int number = epoll_wait(epollfd, events, 5, -1);
    if (number < 0) {
      printf("epoll failure\n");
      break;
    }

    for (int i = 0; i < number; i++) {
      int curr_sockfd = events[i].data.fd;

      if (curr_sockfd == pipe_read_fd) {
        printf("thread: %d\n", pthread_self());
        accept_connect(epollfd, pool->listenfd, curr_sockfd);
        continue;
      } else if (events[i].events & EPOLLRDHUP) {
        printf("connect close.\n");

        pthread_mutex_lock(&mutex);
        users.erase(curr_sockfd);
        pthread_mutex_unlock(&mutex);

        close(curr_sockfd);
        continue;
      } else if (events[i].events & EPOLLIN) {

        handle_client_req(curr_sockfd);

      } else if (events[i].events & EPOLLOUT) {

      }
    }
  }
}


int main(int argc, char* argv[]) {
  if (argc <= 2) {
    return 1;
  }

  const char* ip = argv[1];
  int port = atoi(argv[2]);

  int ret = 0;
  struct sockaddr_in address, client_address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);

  int listenfd = socket(PF_INET, SOCK_STREAM, 0);

  int on = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, (char*)&on, sizeof(on));

  ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
  ret = listen(listenfd, 5);

  epoll_event events[2];
  int epollfd = epoll_create(1);
  addfd(epollfd, listenfd, false);

  ethread_pool_t* pool = ethread_create(listenfd, 8, work);  //创建静态线程池
  pthread_mutex_init(&mutex, NULL);

  while (1) {
    int number = epoll_wait(epollfd, events, 5, -1);
    if (number < 0) {
      printf("epoll failure\n");
      break;
    }
    for (int i = 0; i < number; i++) {
      int curr_sockfd = events[i].data.fd;
      if (events[i].data.fd == listenfd) {
        ethread_sig_trigger(pool);
      }
    }
  }

  close(epollfd);
  close(listenfd);
  return 0;
}