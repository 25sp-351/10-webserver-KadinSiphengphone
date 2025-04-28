#include "request.h"
#include "header.h"

void request_free(Request *req) {
  free(req->method);
  free(req->path);
  free(req->version);
  free(req->body);
  headers_free(req->headers);
}

Request *request_create() {
  Request *request = malloc(sizeof(Request));
  request->method = NULL;
  request->path = NULL;
  request->version = NULL;
  request->headers = headers_create();
  request->body = NULL;
  request->body_length = 0;
}

Request *request_read_from_fd(int fd) {
  log("Reading request from fd %d\n", fd);

  Request *req = request_create();

  if (read_request_line(fd, req) == false) {
    printf("Failed to read request line\n");
    req->free(req);
    return NULL;
  }

  req->headers = headers_read(fd);

  if (req->headers == NULL) {
    printf("Failed to read headers\n");
    req->free(req);
    return NULL;
  }

  if (read_body(req, fd) == false) {
    printf("Failed to read body\n");
    req->free(req);
    return NULL;
  }

  return req;
}
