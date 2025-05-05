#include "response.h"

void send_text_response(int fd) {

  if (strncmp(req->path, "/calc/", 6) == 0) {
    printf("CALCULATE: %s", calculate_from_path(req->path));
    write(sock_fd, calculate_from_path(req->path), );
  }
}