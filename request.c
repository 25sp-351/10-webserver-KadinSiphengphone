#include "request.h"

#include "header.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define NOT_FOUND -1
#define CRLF "\r\n"
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

  return request;
}

void request_print(Request *req) {
  printf("Request ///\n");
  printf("Method: %s\n", req->method);
  printf("Path: %s\n", req->path);
  printf("Version: %s\n", req->version);
  printf("Headers:\n");
  for (int i = 0; i < req->headers->size; i++) {
    printf("%s: %s\n", req->headers->items[i].key,
           req->headers->items[i].value);
  }
  printf("Body: %s\n", req->body);
}

int find_crlf_index(char *buffer, int bytes_read) {
  int crlf_index = NOT_FOUND;

  for (int xx = 0; xx < bytes_read; xx++) {
    if (buffer[xx] == '\r' && buffer[xx + 1] == '\n') {
      crlf_index = xx;
      break;
    }
  }

  return crlf_index;
}

bool read_request_line(int fd, Request *req) {
  char buffer[MAX_BUFFER_SIZE];
  int bytes_read = read(fd, buffer, sizeof(buffer) - 1);

  if (bytes_read < 0) {
    return false;
  }

  int crlf_index = find_crlf_index(buffer, bytes_read);

  if (crlf_index == NOT_FOUND) {
    return false;
  }

  buffer[crlf_index] = '\0';

  char method[MAX_BUFFER_SIZE];
  char path[MAX_BUFFER_SIZE];
  char version[MAX_BUFFER_SIZE];

  if (sscanf(buffer, "%s %s %s", method, path, version) != 3) {
    return false;
  }

  req->method = strdup(method);
  req->path = strdup(path);
  req->version = strdup(version);

  return true;
}

bool read_body(int fd, Request *req) {

  char buffer[MAX_BUFFER_SIZE];
  int content_length = 0;

  char *content_length_str = header_get_value(req->headers, "Content-Length");
  if (content_length_str == NULL) {
    return false;
  }

  char *content_type = header_get_value(req->headers, "Content-Type");
  if (content_type == NULL) {
    return false;
  }

  if (sscanf(content_length_str, "%d", &content_length) != 1 ||
      content_length <= 0) {
    return false;
  }

  int bytes_read = read(fd, buffer, content_length);

  if (bytes_read != content_length) {
    request_free(req);
    req->body = NULL;
    return false;
  }

  req->body = strndup(buffer, content_length);
  req->body_length = content_length;
}

Request *request_read_from_fd(int fd) {
  printf("Reading request from fd %d\n", fd);

  Request *req = request_create();

  if (read_request_line(fd, req) == false) {
    printf("Failed to read request line\n");
    request_free(req);
    return NULL;
  }

  req->headers = headers_read(fd);

  if (req->headers == NULL) {
    printf("Failed to read headers\n");
    request_free(req);
    return NULL;
  }

  // if (read_body(fd, req) == false) {
  //   printf("Failed to read body\n");
  //   req->free(req);
  //   return NULL;
  // }

  return req;
}
