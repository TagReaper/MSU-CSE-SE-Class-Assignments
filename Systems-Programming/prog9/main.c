#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>

// Global Array Initialization
int guess[2] = {};
int cmp[2] = {};
int rdy[4] = {};
pthread_cond_t cnd[3] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};
pthread_mutex_t mtx[3] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

// Function Declaration
void *player(void *pnum);
void *referee();

// Error Checking
int checkError(int val, const char *msg)
{
  if (val == -1)
    {
      perror(msg);
      exit(EXIT_FAILURE);
    }
  return val;
}

// Main Function
int main() {
  // Variable Assignments
  pthread_t player1;
  pthread_t player2;
  pthread_t ref;

  // Seeding the random number generator
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  srand(time(NULL) * ts.tv_nsec);

  // Setting ready flags
  rdy[0] = 0;
  rdy[1] = 0;
  rdy[2] = 0;
  rdy[3] = 0;

  // Spawning Threads
  pthread_create(&player1, NULL, player, (void*) 0);
  pthread_create(&player2, NULL, player, (void*) 1);
  pthread_create(&ref, NULL, referee, NULL);
  
  // Join Referee Thread
  pthread_join(ref, NULL);

  return 0;
}

// Function Definitions

void *player(void *pnum) {
  // Type-casting pnum
  int playerNum = (int)(long)pnum;
  // Forever-Loop
  while (1) {
    // Setting min and max
    int min = 0;
    int max = 100;

    // Locking mutex 3
    pthread_mutex_lock(&mtx[2]);

    // Looping for cond-var 3
    while (!rdy[playerNum + 2]) {
      pthread_cond_wait(&cnd[2], &mtx[2]);
    }

    // Unlocking mutex 3 and setting ready flags
    rdy[playerNum + 2] = 0;
    pthread_mutex_unlock(&mtx[2]);

    // Forever-loop
    while (1) {
      // Guess calculations and store in guess[]
      switch (playerNum) {
        case 0:
          guess[0] = (max+min)/2;
          break;
        case 1:
          guess[1] = min + rand() % (max - min + 1);
          break;
      }

      // Lock Player Mutex
      pthread_mutex_lock(&mtx[playerNum]);

      // Looping for player cond-var
      while (!rdy[playerNum]) {
        pthread_cond_wait(&cnd[playerNum], &mtx[playerNum]);
      }

      // Unlocking Player Mutex
      pthread_mutex_unlock(&mtx[playerNum]);

      // Comparison Check
      if (cmp[playerNum] < 0) {
        min = guess[playerNum];
      } else if (cmp[playerNum] > 0) {
        max = guess[playerNum];
      } else {
        break;
      }
    }
  }
  return NULL;
}

void *referee() {
  // Setting win array
  int wins[2] = {0, 0};

  for (int a = 1; a <= 10; a++) {
    // Lock Mutex 3 and generate target 1-100
    pthread_mutex_lock(&mtx[2]);
    int target = rand() % 100 + 1;

    // Setting ready flags and cond-vars
    rdy[2] = 1;
    rdy[3] = 1;
    pthread_cond_broadcast(&cnd[2]);
    pthread_mutex_unlock(&mtx[2]);

    // Print game Statistics
    printf("\n=== Game %d of 10 === (Score P1/P2: %d/%d)\n", a, wins[0], wins[1]);

    int round = 0;
    while (1) {
      // Sleeping for 1 Sec
      sleep(1);

      // Locking Mutexes 1 & 2
      pthread_mutex_lock(&mtx[0]);
      pthread_mutex_lock(&mtx[1]);

      // Setting Ready flags and Comparisons
      rdy[0] = 0;
      rdy[1] = 0;
      cmp[0] = guess[0] - target;
      cmp[1] = guess[1] - target;

      // Increasing round number and printing guesses
      round++;
      printf("   Round %d: P1 guessed %d, P2 guessed %d\n\n", round, guess[0], guess[1]);

      // Checking if either player guessed the target
      // Putting this before the broadcast so they both stop at the same time
      int end = cmp[0] == 0 || cmp[1] == 0;
      if (end) {
        if (cmp[0] == 0) {
          wins[0]++;
        }
        if (cmp[1] == 0) {
          wins[1]++;
        }
        cmp[0] = 0;
        cmp[1] = 0;
      }

      // Setting ready flags, broadcasting cond-vars, and unlocking mutexes
      rdy[0] = 1;
      rdy[1] = 1;
      pthread_cond_broadcast(&cnd[0]);
      pthread_cond_broadcast(&cnd[1]);
      pthread_mutex_unlock(&mtx[0]);
      pthread_mutex_unlock(&mtx[1]);

      // Checking if game is ended, and printing the winner
      if (end) {
        printf("   Target was %d\n\n", target);
        break;
      }
    }
  }

  // Printing Game Statistics
  printf("\n=== Final Results ===\n");
  printf("Player 1: %d wins\n", wins[0]);
  printf("Player 2: %d wins\n", wins[1]);
  return NULL;
}