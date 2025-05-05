
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "connection_handler.h"
#define LISTEN_BACKLOG 10
#define BUFFER_SIZE 1024
bool V_ARG = false;

int main(int argc, char *argv[]) {

  int port = 8000;
  for (int xx = 1; xx < argc; xx++) {
    if (strcmp(argv[xx], "-p") == 0) {
      sscanf(argv[xx + 1], "%d", &port);
      xx++;
    } else if (strcmp(argv[xx], "-v") == 0) {
      V_ARG = true;
    }
  }

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in socket_address;
  memset(&socket_address, '\0', sizeof(socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
  socket_address.sin_port = htons(port);

  printf("binding to port %d\n", port);

  int returnval;

  returnval = bind(socket_fd, (struct sockaddr *)&socket_address,
                   sizeof(socket_address));
  if (returnval == -1) {
    perror("bind");
    return 1;
  }

  returnval = listen(socket_fd, LISTEN_BACKLOG);

  struct sockaddr_in client_address;
  socklen_t client_address_len = sizeof(client_address);
  while (1) {
    pthread_t thread;
    int *client_fd_buf = malloc(sizeof(int));
    *client_fd_buf = accept(socket_fd, (struct sockaddr *)&client_address,
                            &client_address_len);

    pthread_create(&thread, NULL, (void *(*))(void *)handleConnection,
                   (void *)client_fd_buf);
  }

  return 0;
}
