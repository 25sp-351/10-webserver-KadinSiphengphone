#ifndef RESPONSE
#define RESPONSE
#include "request.h"
#define STATUS_CODE_OKE 200
#define MAX_BUFFER_SIZE 1024
typedef struct {
  int status;
  char *version;
  int body_length;
  Headers *headers;
  char *body;

} Response;

void send_text_response(int fd, int status, char *body);

bool send_file_response(int fd, char *path);

#endif