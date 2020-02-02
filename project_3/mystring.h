/*
 * NAME: Lucas Chagas
 * COURSE: CS230 / Spring, 2019
 */

#ifndef MY_STRING
#define MY_STRING

#include <stdbool.h>

// Splits a string at each occurence of a space.
// Also adds words to the histogram
extern int split(char *input);

// Converts all uppercase letters in a string to lowercase
extern void lower(char *input);

// Gets the length of a string
extern int len(char *input);

// Returns substring from start index up until but not including end index
extern void slice(char *input, int start, int end);

// Returns a string stripped of trailing whitespace
extern void strip(char *input);

// Removes a character from a string
extern void delete_char(char *input, int index);

// Gets lowercase of char c
extern char getlower(char c);

// Returns the number of occurrences in string input of char c
extern int count(char *input, char c);

// Returns true is a char c is a letter
extern bool is_letter(char c);

// Removes all nonletters from a string
extern void remove_nonletters(char *input);

// Compares two strings. Function is also case-sensitive
extern bool compare(char *input1, char *input2);

// Wrapper function for write
extern void myprint(char *input);

#endif
