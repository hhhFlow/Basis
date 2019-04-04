
// #include <pthread/pthread.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/msg_protocol.h"

int main(int argc, char *argv[]) {
  const char *ip = argv[1];
  int port = atoi(argv[2]);

  struct sockaddr_in address, ret_address;
  memset(&address, '\0', sizeof(address));

  address.sin_family = AF_INET;
  inet_pton(AF_INET, ip, &address.sin_addr);
  address.sin_port = htons(port);

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);

  int ret = connect(sockfd, (struct sockaddr *)&address, sizeof(address));

  // msgPkg mp;
  // mp.msg_header.type = TYPE_MSG;

  // strncpy(mp.msg_body.msg_msg.src_usr, "12345678", 10);
  // strncpy(mp.msg_body.msg_msg.dst_usr, "112233", 10);
  // char *buff = "Hello, how are you!";

  // strncpy(mp.msg_body.msg_msg.msg_buff, buff, strlen(buff));
  // mp.msg_header.length =
  //     sizeof(Msg_header) + 20 + strlen(mp.msg_body.msg_msg.msg_buff);

  msgPkg mp;
  mp.msg_header.type = TYPE_REQ_LOGIIN;
  strncpy(mp.msg_body.msg_login.user_id, "12345678", 10);
  mp.msg_header.length = sizeof(Msg_header) + 10;


  send(sockfd, &mp, mp.msg_header.length, 0);

  // getchar();
  close(sockfd);


  return 0;
}