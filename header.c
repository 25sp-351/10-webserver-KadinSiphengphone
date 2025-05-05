
#include "header.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NOT_FOUND -1
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

char *header_get_value(Headers *headers, char *key) {
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

int find_double_crlf_index(char *buffer, int bytes_read) {
  int crlf_index = NOT_FOUND;

  for (int xx = 0; xx < bytes_read; xx++) {
    if (buffer[xx] == '\r' && buffer[xx + 1] == '\n' &&
        buffer[xx + 2] == '\r' && buffer[xx + 3] == '\n') {
      crlf_index = xx;
      break;
    }
  }

  return crlf_index;
}
void read_lines_to_header(char *buffer, Headers *headers, int crlf_index) {
  int position = 0;
  while (position < crlf_index) {

    int total_line_length = position;

    while (total_line_length < crlf_index &&
           buffer[total_line_length] != '\n') {
      total_line_length++;
    }

    int line_length = total_line_length - position;
    char *line = strndup(buffer + position, line_length);

    char *colon = strchr(line, ':');
    if (colon == NULL) {
      free(line);
      break;
    }

    *colon = '\0';
    char *key = line;
    char *value = colon + 1;

    while (*value == ' ') {
      value++;
    }

    if (headers->size < MAX_HEADERS) {
      headers->items[headers->size].key = strdup(key);
      headers->items[headers->size].value = strdup(value);
      headers->size++;
    }

    free(line);
    position += total_line_length + 1;
  }
}

Headers *headers_read(int fd) {
  Headers *headers = malloc(sizeof(Headers));

  headers->items = malloc(sizeof(Header) * MAX_HEADERS);
  headers->size = 0;

  char buffer[MAX_BUFFER_SIZE];

  int total_bytes_read = 0;
  while (1) {
    int bytes_read = read(fd, buffer + total_bytes_read, sizeof(buffer) - 1);

    total_bytes_read += bytes_read;

    if (find_double_crlf_index(buffer, total_bytes_read) != NOT_FOUND) {
      break;
    }

    if (total_bytes_read == 0) {
      return NULL;
    }
  }
  // printf("total_bytes_read: %d\n", total_bytes_read);
  int crlf_index = find_double_crlf_index(buffer, total_bytes_read);

  buffer[crlf_index] = '\0';

  read_lines_to_header(buffer, headers, crlf_index);

  return headers;
}