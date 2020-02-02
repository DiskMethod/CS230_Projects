/*
 * NAME: Lucas Chagas
 * COURSE: CS230 / Spring, 2019
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mystring.h"
#include "mywordhist.h"

void myprint(char *input) {
  ssize_t ret_val;
  char *s = input;

  if (input == NULL) {
    return;
  }

  ret_val = write(STDOUT_FILENO, s, len(s));
  if (ret_val == -1) {
    exit(1);
  }
}

char getlower(char c) {
  if (c >= 65 && c <= 90) {
    return c + 32;
  }
  return c;
}

int len(char *input) {
  int i;
  for (i = 0; input[i] != '\0'; ++i)
    ;
  return i;
}

void lower(char *input) {
  int length = len(input);
  for (int i = 0; i < length; ++i) {
    input[i] = getlower(input[i]);
  }
}

void slice(char *input, int start, int end) {
  int n_start = start;
  for (int i = 0; i < (end - start) + 1; ++i) {
    input[i] = input[n_start++];
  }
  input[end - start] = '\0';
}

int count(char *input, char c) {
  int counter = 0;
  for (int i = 0; input[i] != '\0'; ++i) {
    if (input[i] == c) {
      ++counter;
    }
  }
  return counter;
}

void delete_char(char *input, int index) {
  int length = len(input);
  for (int i = index; i < length; ++i) {
    input[i] = input[i + 1];
  }
}

void remove_nonletters(char *input) {
  for (int i = 0; i < len(input); ++i) {
    if ((!is_letter(input[i])) && (input[i] != 32)) {
      delete_char(input, i);
      i = 0;
    }
  }
  if ((!is_letter(input[0])) && (input[0] != 32)) {
    remove_nonletters(input);
  }
}

bool is_letter(char c) {
  if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
    return true;
  }
  return false;
}

void strip(char *input) {
  int length = len(input);

  for (int i = length - 1; i >= 0; i--) {
    if (input[i] == ' ') {
      input[i] = '\0';
    } else {
      break;
    }
  }

  while (true) {
    if (*input == ' ' || *input == '\r' || *input == '\n') {
      input++;
    } else {
      break;
    }
  }
}

int split(char *input) {
  remove_nonletters(input);
  lower(input);

  int num_words = 0;

  while (len(input) > 0) {
    strip(input);
    for (int i = 0; i < len(input); ++i) {
      if (input[i] == ' ' || input[i + 1] == '\0') {
        char *temp = malloc(len(input) + 1);

        strcpy(temp, input);
        slice(temp, 0, i + 1);
        strip(temp);

        if (!compare(temp, "")) {
          if (temp[0] == ' ') {
            add_word(++temp);
          } else {
            add_word(temp);
          }
        }

        slice(input, i + 1, len(input));
        strip(input);

        ++num_words;
        i = 0;
      }
    }
  }

  return num_words;
}

bool compare(char *input1, char *input2) {
  if (len(input1) != len(input2)) {
    return false;
  }
  for (int i = 0; i < len(input1); ++i) {
    if (input1[i] != input2[i]) {
      return false;
    }
  }
  return true;
}
