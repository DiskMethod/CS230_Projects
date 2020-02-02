/*
 * NAME: Lucas Chagas
 * COURSE: CS230 / Spring, 2019
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mywordhist.h"
#include "mystring.h"

WORD_t **words = NULL;
int word_size = 1;
int word_count = 0;

void add_word(char *input) {
  WORD_t *w = (WORD_t *)malloc(sizeof(WORD_t));
  w->s = input;
  w->count = 1;

  if (word_count == 0) {
    words = (WORD_t **)malloc(sizeof(WORD_t *));
    words[0] = w;
    ++word_count;
  } else {
    if (word_size == word_count) {
      words = (WORD_t **)realloc(words, sizeof(WORD_t *) * word_size * 2);
      word_size *= 2;
    }
    if (contains(w)) {
      ++(get_word(w)->count);
    } else {
      words[word_count] = w;
      ++word_count;
    }
  }
}

bool contains(WORD_t *w) {
  for (int i = 0; i < word_count; ++i) {
    if (compare(w->s, words[i]->s)) {
      return true;
    }
  }
  return false;
}

WORD_t *get_word(WORD_t *w) {
  for (int i = 0; i < word_count; ++i) {
    if (compare(w->s, words[i]->s)) {
      return words[i];
    }
  }
  return NULL;
}

#define ATOI_DIGITS_MAX 5

// Converts integer to ascii
static void myitoa(unsigned int val, char buf[ATOI_DIGITS_MAX]) {
  int i;
  unsigned int lVal = val;

  // Set buffer to default values
  for (i = 0; i < (ATOI_DIGITS_MAX - 1); i++) {
    buf[i] = ' ';
  }
  buf[ATOI_DIGITS_MAX - 1] = '0';

  // Populate buffer with digits (right justified)
  for (i = ATOI_DIGITS_MAX - 1; lVal != 0; i--) {
    buf[i] = (lVal % 10) + 48;
    lVal /= 10;
  }
}

void print_hist(void) {
  char ascii_str[ATOI_DIGITS_MAX];

  for (int i = 0; i < word_count; i++) {
    myprint(words[i]->s);
    if (len(words[i]->s) >= 7) {
      myprint(":\t");
    } else {
      myprint(":\t\t");
    }
    myitoa(words[i]->count, ascii_str);
    myprint(ascii_str);
    myprint("\n");
  }
}

void cleanup(void)
{
  if (words != NULL) {
    free(words);
  }
}
