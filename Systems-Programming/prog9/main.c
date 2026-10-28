#include <stdio.h>
#include <threads.h>
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
    int min = 0;
    int max = 100;
    pthread_mutex_lock(&mtx[2]);
    while (!rdy[playerNum + 2]) {
      pthread_cond_wait(&cnd[2], &mtx[2]);
    }
    rdy[playerNum + 2] = 0;
    pthread_mutex_unlock(&mtx[2]);

    while (1) {
      switch (playerNum) {
        case 0:
          guess[0] = (max-min)/2;
          break;
        case 1:
          guess[1] = min + rand() % (max - min + 1);
          break;
      }
      pthread_mutex_lock(&mtx[playerNum]);
      while (!rdy[playerNum]) {
        pthread_cond_wait(&cnd[playerNum], &mtx[playerNum]);
      }
      rdy[playerNum] = 0;
      pthread_mutex_unlock(&mtx[playerNum]);

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
  int wins[2] = {0, 0};
  
  for (int a = 0; a<10; a++) {
    pthread_mutex_lock(&mtx[2]);
    int target = rand() % 100 + 1;
    rdy[2] = 1;
    rdy[3] = 1;
  }
  return NULL;
}