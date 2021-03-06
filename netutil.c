#include <pcap.h>
#include <stdio.h>
#include <libnet.h>
#include <pthread.h>
#include <sys/socket.h>

void *connection_handler(void *sockfd) {
  int read_size;
  char recvbuff[2000];
  int sock = *(int*)sockfd;

  while (1) {
    read_size = recv(sock, recvbuff, 2000, 0)
    
    memset(recvbuff, 0, sizeof(recvbuff));
  }

  return 0;
}

void bind_tcp(const char *port){
  int fd = 0, connfd = 0, sockfd = 0, *new_sock;
  struct sockaddr_in serv_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("Error: opening socket\n");
    exit(-1);
  }

  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(port));

  fd = bind(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  if (fd == -1) {
    perror("Error");
    exit(-1);
  }
  fd = listen(sockfd, 50);
  if (fd == -1) {
    perror("Error");
    exit(-1);
  }

  while(1) {
    connfd = accept(sockfd, (struct sockaddr *)NULL, NULL);
    if (connfd != -1) {
      pthread_t sniffer_thread;
      new_sock = malloc(1);
      *new_sock = connfd;

      if(pthread_create(&sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0) {
            perror("could not create thread");
            exit(-1);
      }
    }
  }

}

int main(int argc, char const *argv[]) {
  size_t i;
  for (i = 0; i < argc; i++) {
    if (strncmp(argv[i], "-tcp", 4) == 0) {
      ++i;
      if (strncmp(argv[i], "-b", 2) == 0) {
        ++i;
        bind_tcp(argv[i]);
      }
    }
  }
  return 0;
}
