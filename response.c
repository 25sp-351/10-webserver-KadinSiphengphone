#include "response.h"
#include "calc.h"
#include "header.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Response *response_create(int status, Headers *headers, char *body) {
  Response *response = malloc(sizeof(Response));
  if (response == NULL) {
    perror("Failed to allocate memory for response");
    return NULL;
  }

  response->status = status;
  response->headers = headers;
  response->body = body;

  return response;
}

char *get_status_message(int status) {
  if (status == 200) {
    return "OK";
  } else if (status == 404) {
    return "Not Found";
  } else if (status == 500) {
    return "Internal Server Error";
  } else {
    return "Error";
  }
}

char *get_mime_type(char *path) {
  char *mime = strchr(path, '.');
  mime = mime;
  if (strcmp(mime, ".jpg") == 0 || strcmp(mime, ".jpeg") == 0) {
    return "image/jpeg";
  }
}

void send_text_response(int fd, int status, char *body) {

  char buffer[MAX_BUFFER_SIZE];
  int body_length = 0;
  int header_length = 0;

  if (body == NULL) {
    body = "";
  }

  body_length = strlen(body);

  header_length = snprintf(buffer, sizeof(buffer),
                           "HTTP/1.1 %d %s\r\n"
                           "Content-Type: text/plain\r\n"
                           "Content-Length: %d\r\n"
                           "\r\n",
                           status, get_status_message(status), body_length);

  if (header_length < 0) {
    return;
  }

  if (write(fd, buffer, header_length) < 0) {
    perror("Failed to write header");
    return;
  }

  if (body != NULL) {
    if (write(fd, body, body_length) < 0) {
      perror("Failed to write body");
      return;
    }
  }
}

bool send_file_response(int fd, char *path) {

  FILE *file = NULL;
  int file_size = 0;

  file = fopen(path, "rb");
  if (file == NULL) {
    perror("Failed to open file");
    return false;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    perror("Failed to seek to end of file");
    fclose(file);
    return false;
  }
  file_size = ftell(file);
  if (fseek(file, 0, SEEK_SET) != 0) {
    perror("Failed to seek to start of file");
    fclose(file);
    return false;
  }

  char buffer[MAX_BUFFER_SIZE];
  int header_length = 0;
  char *mime = get_mime_type(path);
  header_length = snprintf(buffer, sizeof(buffer),
                           "HTTP/1.1 200 OK\r\n"
                           "Content-Type: image/%s\r\n"
                           "Content-Length: %d\r\n"
                           "\r\n",
                           mime, file_size);

  if (header_length < 0) {
    perror("Failed to create header");
    fclose(file);
    return false;
  }

  if (write(fd, buffer, header_length) < 0) {
    perror("Failed to write header");
    fclose(file);
    return false;
  }

  char file_buffer[MAX_BUFFER_SIZE];
  int bytes_read;

  while (bytes_read = read(fd, file_buffer, sizeof(file_buffer)) > 0) {
    if (bytes_read < 0) {
      perror("Failed to read file");
      fclose(file);
      return false;
    }

    if (write(fd, file_buffer, bytes_read) < 0) {
      perror("Failed to write file");
      fclose(file);
      return false;
    }
  }

  fclose(file);
  return true;
}