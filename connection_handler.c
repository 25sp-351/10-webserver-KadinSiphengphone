#include "connection_handler.h"
#include "header.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>

void handleConnection(int *sock_fd_ptr) {
  int sock_fd = *sock_fd_ptr;
  free(sock_fd_ptr);

  Request *req = request_read_from_fd(sock_fd);
  printf("Handling connection on %d\n", sock_fd);
  request_print(req);
  printf("done with connection %d\n", sock_fd);
}