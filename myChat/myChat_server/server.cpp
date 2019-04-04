

/*******************************/
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
// #include <pthread/pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

// #include "../include/epoll.h"
// #include "../include/msg_protocol.h"
#include <map>
#include "msg_protocol.h"

typedef struct {
  int status;
  struct sockaddr_in addr;
}client_info;


int set_nonblocking_mode(int fd) {
  int old_option = fcntl(fd, F_GETFL);
  int new_option = old_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_option);
  return old_option;
}

void addfd(int epollfd, int fd, bool one_shot) {
  epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
  if (one_shot) {
    event.events |= EPOLLONESHOT;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
  set_nonblocking_mode(fd);
}


void handle_client_req(int fd, std::map<int, client_info>& users) {
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
    users[fd].status = 1;
    return;
  }
  if (msgpkg->msg_header.type == TYPE_REQ_FRIENDS) {
    char* user_id = msgpkg->msg_body.msg_login.user_id;
    printf("friends_req : %s\n", user_id);
    users[fd].status = 2;
    close(fd);
    return;
  }
  return;
}

void handle_client_ret(int fd, std::map<int, client_info>& users) {
    if(users[fd].status == 1){

    }
    if(users[fd].status == 2){

    }
  return;
}

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    return 1;
  }
  std::map<int, client_info> users;

  const char* ip = argv[1];
  int port = atoi(argv[2]);

  int ret = 0;
  client_info ci;
  struct sockaddr_in address, client_address;
  // struct sockaddr_in* client_address =
  //     (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
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

  epoll_event events[100];
  int epollfd = epoll_create(1);
  addfd(epollfd, listenfd, false);

  socklen_t client_addrlength = sizeof(struct sockaddr_in);
  while (1) {
    int number = epoll_wait(epollfd, events, 100, -1);
    if (number < 0) {
      printf("epoll failure\n");
      break;
    }
    for (int i = 0; i < number; i++) {
      int curr_sockfd = events[i].data.fd;
      if (events[i].data.fd == listenfd) {
        memset(&client_address, '\0', client_addrlength);
        int connfd = accept(curr_sockfd, (struct sockaddr*)(&client_address),
                            &client_addrlength);
        if (connfd < 0) continue;
        ci.status = 0;
        ci.addr = client_address;

        users[connfd] = ci;

        addfd(epollfd, connfd, false);
        printf("connected!\n");
      } else if (events[i].events & EPOLLRDHUP) {
        printf("connect close.\n");
        users.erase(curr_sockfd);
        close(curr_sockfd);
      } else if (events[i].events & EPOLLIN) {  //有数据可读
        handle_client_req(curr_sockfd, users);
      } else if (events[i].events & EPOLLOUT) {  //有
        handle_client_ret(curr_sockfd, users);
      }
    }
  }

  close(epollfd);
  close(listenfd);
  return 0;
}