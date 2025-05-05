#ifndef HEADER_H
#define HEADER_H

#include <stdbool.h>
#define MAX_HEADERS 20
#define MAX_BUFFER_SIZE 1024
typedef struct header {
  char *key;
  char *value;
} Header;

typedef struct {
  Header *items;
  int size;

} Headers;

bool headers_write(int fd, Headers *headers);

char *header_get_value(Headers *headers, char *key);

int find_crlf_index(char *buffer, int total_bytes_read);

Headers *headers_read(int fd);

void headers_free(Headers *headers);

#endif