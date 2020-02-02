/*
 * NAME: Lucas Chagas
 * COURSE: CS230 / Spring, 2019
 *
 * DESCRIPTION:
 *
 * Refer to assignment description in the PDF file.
 *
 */

#include <arpa/inet.h>
#include <errno.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Definitions
#define PARAMETER_MAX_LEN 255
#define BUFF_LEN 2048
#define SPLIT_WORDS_MAX 20

int PerformOperations(int sock);
void splitstring(char *str, char *words[], int words_maxlen);

// Display message on how use this application
void usage(void) {
  printf("USAGE:\n");
  printf("   ./client <netid>@umass.edu <ip_port> <ip_address>\n");
}

int main(int argc, char **argv) {
  char sId[PARAMETER_MAX_LEN], sIPAddr[PARAMETER_MAX_LEN];
  unsigned int iPort;
  int sockfd, status = 0;
  struct sockaddr_in serv_addr;
  char buff[BUFF_LEN];

  // Enough number of parameters provided?
  if (argc != 4) {
    usage();
    exit(1);
  }

  // ==========
  // Save parameters and validate input
  strcpy(sId, argv[1]);
  errno = 0; // Init 'errno' to distinguish success/failure after call
  iPort = strtol(argv[2], NULL, 10);
  if ((errno == ERANGE && (iPort == LONG_MAX || iPort == LONG_MIN)) ||
      (errno != 0 && iPort == 0)) {
    printf("ERROR: Invalid port number\n");
    exit(1);
  }

  strcpy(sIPAddr, argv[3]);
  // Convert IP address from text to binary form
  if (inet_pton(AF_INET, sIPAddr, (void *)&serv_addr.sin_addr) <= 0) {
    printf("ERROR: Invalid address: %s\n", sIPAddr);
    return -1;
  }
  // ==========

  // Create socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("ERROR: Socket creation failed\n");
    exit(1);
  }

  // Initialize sockaddr_in structure
  bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(iPort);

  // Connect to server
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR: Connection failed\n");
    exit(1);
  }

  // Sign into server
  sprintf(buff, "cs230 HELLO %s\n", sId);
  if ((send(sockfd, buff, strlen(buff), 0)) < 0) {
    printf("ERROR: Failed to send sign-in message to server\n");
    exit(1);
  }

  // Ready to comunicate with server to perform opetations
  status = PerformOperations(sockfd);

  close(sockfd);
  return status;
}

// Splits string into words delimitited by spaces
void splitstring(char *str, char *words[], int words_maxlen) {
  char delim[] = " ";
  int i = 0;

  char *ptr = strtok(str, delim);
  while ((ptr != NULL) && (i < words_maxlen)) {
    words[i] = ptr;
    ptr = strtok(NULL, delim);
    i++;
  }
}

// This function communicates with the server to perform all operations
int PerformOperations(int sock) {
  unsigned int n;
  int res, status = 0;
  char rBuff[BUFF_LEN], wBuff[BUFF_LEN];
  char *words[SPLIT_WORDS_MAX];

  for (;;) {
    // Read message from server
    n = read(sock, rBuff, BUFF_LEN);
    rBuff[n] = '\0'; // Place NULL string terminator
    printf("Server message: %s", rBuff);

    // Initialize array of words
    memset(words, 0, sizeof(words));

    // Parse message
    splitstring(rBuff, words, SPLIT_WORDS_MAX);

    // Message from server always starts with "cs230"
    if (strcmp(words[0], "cs230") != 0) {
      status = 1;
      break;
    }
    // End of session?
    if (strncmp(words[2], "BYE", 3) == 0) {
      // YES, then print hash value and quit
      printf("Hash value: %s\n", words[1]);
      status = 0;
      break;
    }
    // Operation?
    if (strcmp(words[1], "STATUS") == 0) {
      int num1, num2;
      char c;

      status = 0; // Assume success

      num1 = atoi(words[2]);
      num2 = atoi(words[4]);

      // YES, then process operation
      c = *words[3];
      switch (c) {
      case '+':
        res = num1 + num2;
        break;
      case '-':
        res = num1 - num2;
        break;
      case '*':
        res = num1 * num2;
        break;
      case '/':
        res = num1 / num2;
        break;
      default:
        printf("ERROR: Unknown operator: %c\n", c);
        status = 1;
        break; // Quit switch
      }

      // Any error detected?
      if (status) {
        // YES, then quit 'for' loop
        break;
      }
    } // if

    sprintf(wBuff, "cs230 %d\n", res);
    printf("%s", wBuff);
    n = write(sock, wBuff, strlen(wBuff));
    if (n < 0) {
      printf("ERROR: Writing to socket\n");
      status = 1;
      break; // Quit for loop
    }
  } // for

  return status;
}
