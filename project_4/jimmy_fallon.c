/*
 * NAME: Lucas Chagas
 * COURSE: CS230 / Spring, 2019
 *
 * DESCRIPTION:
 *
 * Project Requirements:
 *  a. Access to the connected variable is protected on lines: (101-106,
 *      118-126, 151-153)
 *  b. Access to the operators variable is protected on lines: (138-148)
 *  c. Semaphores are intialized on lines: (92-93) and destroyed on
 *     lines: (58-59)
 *  d. The thread function is named phonecall, prototyped on line 14.
 *  e. next_id is a global variable on line 9, and is updated on line 82.
 *  f. Yes, as shown on lines: (101-106, 118-126, 151-153)
 *  g. Yes, as shown on lines: (98, 104, 122, 136, 141, 147, 156)
 *  h. Yes, as shown on lines: (84-87)
 *
 * Usage: ./jimmy_fallon [NUMBER OF CALLERS]
 *
 * Overview:
 * There are a few choices that I made that deviates a little from the
 * project description. For example, intially I was initializing my semaphores
 * within the main function. However, I realized that I wasn't using the
 * NUM_OPERATORS variable (as described in the project description). To fix
 * this, I ended up creating another static variable named "call" which would
 * act as a boolean. Using this variable I was then able to initialize my
 * semaphores within the thread function, and in-turn use the NUM_OPERATORS
 * variable.
 * 
 * Another thing is that I decided to print out whenever a caller was waiting
 * for an operator. To do this I used sem_getvalue(3).
 *
 * Video: https://youtu.be/Pf_Palbnwys
 */

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function prototypes
void phonecall(void *arg);

// Global variables
int next_id;
sem_t connected_lock; // binary semaphore
sem_t operators;      // counting sempahore

int main(int argc, char *argv[]) {

  int NUM_CALLS;

  if (argc > 1) {
    NUM_CALLS = strtol(argv[1], NULL, 10);
  } else {
    exit(EXIT_FAILURE);
  }

  next_id = 0;

  // IDs for phonecalls
  pthread_t id[NUM_CALLS];

  // Return values for pthread_create and pthread_join
  int ret1, ret2;

  // Creates phonecall threads
  for (int i = 0; i < NUM_CALLS; ++i) {
    ret1 = pthread_create(&(id[i]), NULL, (void *)&phonecall, NULL);
    if (ret1 != 0) {
      perror("pthread_create");
    }
  }

  // Waits for each thread to finish running
  for (int j = 0; j < NUM_CALLS; ++j) {
    ret2 = pthread_join(id[j], NULL);
    if (ret2 != 0) {
      perror("pthread_join");
    }
  }

  sem_destroy(&connected_lock);
  sem_destroy(&operators);

  return 0;
} // main function

/*
    phonecall thread function algorithm
    1. Print that an attempt to connect has been made.
    2. Check if the connection can be made:
        a. You'll need to test connected in a critical section
        b. If the line is busy, exit the critical section, print a message, and
           try again. c. If the line is not busy, update connected, exit the
           critical section, and print a message, and continue to the next
           step.
    3. Wait for an operator to be available (use a counting semaphore)
    4. Print a message that the order is being taken by an operator
    5. Simulate a ticker order by sleeping for a few seconds (sleep(3))
    6. Print a message that the order is complete (and update the semaphore)
    7. Update connected (using a binary semaphore)
    8. Print a message that the call is over
*/
void phonecall(void *arg) {

  int id = ++next_id;

  static bool call = true;
  static int NUM_OPERATORS = 3;
  static int NUM_LINES = 5;
  static int connected = 0; // Callers that are connected

  // Prevents semaphores from being initialized more than once
  if (call) {
    sem_init(&connected_lock, 0, 1);
    sem_init(&operators, 0, NUM_OPERATORS);
    call = false;
  }

  // Step One
  printf("Caller [%i] is attempting to connect.\n", id);

  // Step Two
  sem_wait(&connected_lock);
  if (connected == NUM_LINES) {
    printf("Caller [%i] is calling line, busy signal\n", id);
  }
  sem_post(&connected_lock);
  /*
      It's necessary to use a semaphore here since as soon as a phone line has
     opened up, all the threads that haven't made a connection will attempt to
     connect all at once resulting in printf being called by multiple threads
     that claim to have found an "availiable line," when in reality it was taken
     by another thread before it could do anything. However, I also don't want
     one thread to be hogging access to the connected variable while it's
     waiting for a line to open up. That's why after each iteration of the loop
     the thread releases access to the connected variable.
  */
  for (;;) {
    sem_wait(&connected_lock);
    if (connected < NUM_LINES) // Line available
    {
      ++connected;
      printf("Caller [%i] has available line, call ringing\n", id);
      sem_post(&connected_lock);
      break;
    }
    sem_post(&connected_lock);
  } // for loop

  // Gets the number of operators that are available
  int sem_val;
  sem_getvalue(&operators, &sem_val);

  // Step Three
  if (sem_val == 0) {
    printf("Caller [%i] is waiting for an operator to be available\n", id);
  }
  sem_wait(&operators);

  // Step Four
  printf("Caller [%i] is speaking to operator\n", id);

  // Step Five
  sleep(1);

  // Step Six
  printf("Caller [%i] has bought a ticket!\n", id);
  sem_post(&operators);

  // Step Seven
  sem_wait(&connected_lock);
  --connected;
  sem_post(&connected_lock);

  // Step Eight
  printf("Caller [%i] has hung up!\n", id);

  pthread_exit(EXIT_SUCCESS);
} // phonecall function
