#include "connection_handler.h"
#include "header.h"
#include "request.h"
#include "response.h"
#include <stdio.h>
#include <stdlib.h>

void handleConnection(int *sock_fd_ptr) {
  int sock_fd = *sock_fd_ptr;
  free(sock_fd_ptr);

  Request *req = request_read_from_fd(sock_fd);
  printf("Handling connection on %d\n", sock_fd);
  request_print(req);
  char *calculated_value = calculate_from_path(req->path);

  if (strncmp(req->path, "/calc/", 6) == 0) {
    if (calculated_value) {
      send_text_response(sock_fd, STATUS_CODE_OK, calculated_value);

    } else {
      send_text_response(sock_fd, STATUS_CODE_NOT_FOUND, "Error");
    }
  }

  printf("done with connection %d\n", sock_fd);
}