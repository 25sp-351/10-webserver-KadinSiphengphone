
#include "header.h"
#include <stdlib.h>
Headers *headers_create() {
  Headers *result = malloc(sizeof(Headers));
  result->items = malloc(sizeof(Header) * MAX_HEADERS);
  result->size = 0;

  return result;
}

const char *headers_get(Headers *headers, char *key) {
  for (int xx = 0; xx < headers->size; xx++) {
    if (strcmp(headers->items[xx].key, key) == 0) {
      return headers->items[xx].value;
    }
  }
  return NULL;
}

bool headers_write(int fd, Headers *headers) {
  char buffer[MAX_BUFFER_SIZE];
  int buffer_length = 0;

  for (int xx = 0; xx < headers->size; xx++) {
    Header *header = &headers->items[xx];
    if (write(fd, header->key, strlen(header->key)) < 0) {
      return false;
    }
    if (write(fd, ": ", 2) < 0) {
      return false;
    }
    if (write(fd, header->value, strlen(header->value)) < 0) {
      return false;
    }
    if (write(fd, "\r\n", 2) < 0) {
      return false;
    }
  }
}

char *headers_get_value(Headers *headers, char *key) {
  for (int xx = 0; xx < headers->size; xx++) {
    if (strcmp(headers->items[xx].key, key) == 0) {
      return headers->items[xx].value;
    }
  }
}

void headers_free(Headers *headers) {
  for (int xx = 0; xx < headers->size; xx++) {
    free(headers->items[xx].key);
    free(headers->items[xx].value);
  }
}

Headers *headers_read(int fd) {
  Headers headers;

  headers.items = malloc(sizeof(Header) * MAX_HEADERS);
  headers.size = 0;

  char buffer[MAX_BUFFER_SIZE];

  int bytes_read = read(fd, buffer, sizeof(buffer));
  if (bytes_read <= 0)
    return (Headers *){NULL, 0};

  int crlf_index = 0;

  for (int xx = 0; xx < bytes_read; xx++) {
    if (buffer[xx] == '\r' && buffer[xx + 1] == '\n') {
      crlf_index = xx;
      break;
    }
  }

  char *colon_index = NULL;
  for (int xx = 0; xx < crlf_index; xx++) {

    colon_index = strchr(buffer + xx, ':');
    if (colon_index != NULL) {
    }
  }
}