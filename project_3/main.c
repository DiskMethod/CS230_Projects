/*
 * NAME: Lucas Chagas
 * COURSE: CS230 / Spring, 2019
 */

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "mystring.h"
#include "mywordhist.h"

#define SZ_1MB (1024 * 1024)
#define BUFFER_SIZE SZ_1MB * 16 // 16MB

static void cleanup_buff(char *pBuff) {
  char *s;

  if (pBuff != NULL) {

    s = pBuff;

    // WHILE not end of buffer
    while (*s != '\0') {
      // Character if NL or CR?
      if (*s == '\n' || *s == '\r') {
        // THEN, replace it with a space
        *s = ' ';
      }
      s++;
    }
  }
}

int main(int argc, char **argv) {
  int i;
  int fd; // File pointer
  off_t f_size;
  ssize_t bytes_read;
  size_t buff_size = BUFFER_SIZE;
  char *buff = malloc(buff_size);
  int buff_count = 0;
  char *env_var;

  // Allocation failed?
  if (buff == NULL) {
    // YES, then quit
    return 1;
  }
  // Text passed as parameter?
  if (argc > 1) {
    // YES, then read in file contents
    for (i = 1; i < argc; i++) {

      // Open file
      fd = open(argv[i], O_RDONLY);
      // File open failed?
      if (fd == -1) {
        // YES, then quit
        free(buff);
        return 1;
      }
      // Find file size
      f_size = lseek(fd, 0, SEEK_END);
      // Fetching file size failed?
      if (f_size == -1) {
        // YES, then quit
        close(fd);
        free(buff);
        return 1;
      }
      lseek(fd, 0, SEEK_SET); // Rewind file pointer

      // Check if there is enough buffer room left
      if (buff_count >= buff_size) {
        // Realocate buffer to next 1MB boundary
        size_t buff_inc = SZ_1MB - ((buff_count - buff_size) % SZ_1MB);
        char *buff_tmp;

        buff_size += buff_inc;
        buff_tmp = realloc(buff, buff_size);
        if (buff_tmp == NULL) {
          close(fd);
          free(buff);
          return 1;
        }
        buff = buff_tmp;
      }
      // Read in file
      bytes_read = read(fd, &buff[buff_count], f_size);
      // Error reading file?
      if (bytes_read == -1) {
        // YES, then quit
        close(fd);
        free(buff);
        return 1;
      }
      // Also check if the number of bytes match
      if (bytes_read != f_size) {
        close(fd);
        free(buff);
        return 1;
      }
      // Update buffer counter and loop again
      buff_count += bytes_read;
      buff[buff_count] = '\0'; // Set NULL terminator
    }

    cleanup_buff(buff);
    split(buff);
    print_hist();
    close(fd);
  }
  // File input passed in environment variable WORD_FREAK?
  else if ((env_var = getenv("WORD_FREAK")) != NULL) {
    // Open file
    fd = open(env_var, O_RDONLY);
    // File open failed?
    if (fd == -1) {
      // YES, then quit
      free(buff);
      return 1;
    }
    // Find file size
    f_size = lseek(fd, 0, SEEK_END);
    // Fetching file size failed?
    if (f_size == -1) {
      // YES, then quit
      close(fd);
      free(buff);
      return 1;
    }
    lseek(fd, 0, SEEK_SET); // Rewind file pointer

    // Check if there is enough buffer room left
    if (buff_count >= buff_size) {
      // Realocate buffer to next 1MB boundary
      size_t buff_inc = SZ_1MB - ((buff_count - buff_size) % SZ_1MB);
      char *buff_tmp;

      buff_size += buff_inc;
      buff_tmp = realloc(buff, buff_size);
      if (buff_tmp == NULL) {
        close(fd);
        free(buff);
        return 1;
      }
      buff = buff_tmp;
    }
    // Read in file
    bytes_read = read(fd, &buff[buff_count], f_size);
    // Error reading file?
    if (bytes_read == -1) {
      // YES, then quit
      close(fd);
      free(buff);
      return 1;
    }
    // Also check if the number of bytes match
    if (bytes_read != f_size) {
      close(fd);
      free(buff);
      return 1;
    }
    // Update buffer counter
    buff_count += bytes_read;
    buff[buff_count] = '\0'; // Set NULL terminator

    cleanup_buff(buff);
    split(buff);
    print_hist();
    close(fd);
  }
  // Text piped in?
  else if (!isatty(STDIN_FILENO)) {
    // YES, then read from stdin
    char c;

    while (read(STDIN_FILENO, &c, 1) != 0) {

      // Check if there is enough buffer room left
      if (buff_count >= buff_size) {
        // Realocate buffer to next 1MB boundary
        size_t buff_inc = SZ_1MB - ((buff_count - buff_size) % SZ_1MB);
        char *buff_tmp;

        buff_size += buff_inc;
        buff_tmp = realloc(buff, buff_size);
        if (buff_tmp == NULL) {
          free(buff);
          return 1;
        } // if
        buff = buff_tmp;
      } // if

      // Save character into buffer
      buff[buff_count] = c;
      buff_count++;
      buff[buff_count] = '\0'; // String NULL terminator
    }                          // while

    cleanup_buff(buff);
    split(buff);
    print_hist();
  } // if
  // ELSE, prompt user for text
  else {
    myprint("Usage:\n");
    myprint("    cat <filename> <filename> |  wordfreak\n");
    myprint("    wordfreak <filename> <filename>\n");
    myprint("    WORD_FREAK=<filename> wordfreak\n");
  }

  free(buff);
  cleanup();
  return 0;
}
