#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>

int checkError(int val, const char *msg)
{
  if (val == -1)
    {
      perror(msg);
      exit(EXIT_FAILURE);
    }
  return val;
}

void *player(void* playernum) {
  /*
  Player Program

  Loop forever

      Set min to 0 and max to 100
      Lock mutex 3
      Using a while loop, wait on condition variable 3.
      In my code base this was implemented as a while(!rdy) cond_wait() loop.
  The following loop is for thread 1 while (rdy[2] == 0)
  pthread_cond_wait(&cnd[2], &mtx[2]); (note: both players are waiting on the
  cnd[2], mtx[2] pairing) Unlock mutex 3 and set the rdy flag you were looping
  over to 0 Loop forever Calculate the child’s guess using the proper method
  (See below) Store that guess in the proper global variable. Lock the mutex
  associated with the thread’s guess (See below) Using a while loop, wait on the
  associated condition variable (See below) Unlock the proper mutex (See below)
          Once this condition variable has been signaled, the value of the
  referee’s response should be stored in the cmp global. Using the proper
  element of the cmp global do the following: If cmp < 0, set min to the current
  guess If cmp > 0, set max to the current guess If cmp is 0, break.
  */
  return NULL;
}

void *referee() {
  /*
  Referee Program

  Loop from 0 to 10

      Lock mutex 3
      Generate the target for this game. Recall this is a random number between
  1 and 100 (inclusive). Set the two rdy flags associated with condition
  variable 3 to 1. I suggest using rdy[2] and rdy[3]. Broadcast condition
  variable 3 and unlock mutex 3 Print the game statistics (which game you are
  in, the current scores, etc) Loop forever Sleep for 1 second Lock mutexes 1
  and 2 Set the rdy flags for each mutex-condition variable pairing to 0. I
  suggest using rdy[0] for mutex1-condition variable 1 and rdy[1] for
  mutex2-condition variable 2. Set each cmp[0] to guess[0] - target and cmp[1]
  to guess[1] - target Set both rdy[0] and rdy[1] to 1. Broadcast condition
  variables 1 and 2 and unlock mutexes 1 and 2 If either thread made a correct
  guess, break. A thread made a correct guess if cmp is 0. Make certain to set
  both cmp's to 0 in this case.

  Print the final game statistics
  */
  return NULL;
}

int main() {
  // Variable Assignments
  pthread_t player1;
  pthread_t player2;
  pthread_t ref;

  int guess[2] = {};
  int cmp[2] = {};
  int rdy[3] = {};
  

  // Seeding the random number generator
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  srand(time(NULL) * ts.tv_nsec);

  // Setting ready flags

  // Spawning Threads
  pthread_create(&player1, NULL, player, (void*) 1);
  pthread_create(&player2, NULL, player, (void*) 1);
  pthread_create(&ref, NULL, referee, NULL);
  
  // Join Referee Thread
  pthread_join(ref, NULL);

  return 0;
}



