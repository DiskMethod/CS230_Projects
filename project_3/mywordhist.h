/*
 * NAME: Lucas Chagas
 * COURSE: CS230 / Spring, 2019
 */

#ifndef MY_WORD_HIST
#define MY_WORD_HIST

#include <stdbool.h>

typedef struct _word {
  char *s;
  int count;
} WORD_t, *pWORD_t;

extern WORD_t **words;
extern int word_count;
// Adds a word to the histogram
extern void add_word(char *input);
// Checks to see if a word is already in the histogram
extern bool contains(WORD_t *w);
// Gets the pointer to a word in words
extern WORD_t *get_word(WORD_t *w);
// Prints out a list of words with their frequency
extern void print_hist(void);
// Free up memory
extern void cleanup(void);
#endif
