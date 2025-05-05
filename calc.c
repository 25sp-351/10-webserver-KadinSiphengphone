#include "calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *calculate_from_path(char *path) {

  char operator[OPERATOR_SIZE];

  int first_number = 0;

  int second_number = 0;
  int result = 0;

  if (sscanf(path, "/calc/%[^/]/%d/%d", operator, & first_number,
             &second_number) != 3) {
    return NULL;
  }

  if (strcmp(operator, "add") == 0) {
    result = first_number + second_number;
  } else if (strcmp(operator, "mul") == 0) {
    result = first_number * second_number;
  } else if (strcmp(operator, "div") == 0) {
    if (second_number == 0) {
      return NULL;
    }
    result = first_number / second_number;
  } else {
    return NULL;
  }

  char result_str[MAX_BUFFER_SIZE];
  snprintf(result_str, sizeof(result_str), "%d", result);

  return strdup(result_str);
}