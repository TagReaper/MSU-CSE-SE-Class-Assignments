#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/wait.h>

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

// Function Assignment
void child_main(int playerNum);
void referee();

// Main Function
int main() {
  // Variables
  pid_t ids[3];

  // Seeding the random number generator
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  srand(time(NULL) * ts.tv_nsec);
  
  // Main Function Spawning
  for (int id = 0; id <= 1; id++) {
    pid_t p = fork();

    if (p < 0) {
      exit(1);
    }
    if (p == 0) {
      child_main(id);
      exit(0);
    }
    ids[id] = p;
  }

  ids[2] = checkError(fork(), "Referee Fork Failed.");
  if (ids[2] == 0) {
    referee();
    exit(0);
  }

  waitpid(ids[0], NULL, 0);
  waitpid(ids[1], NULL, 0);
  waitpid(ids[2], NULL, 0);

  return 0;
}

// Child-Player Function
void child_main(int playerNum) {
  // Forever Loop
  while (1) {
    // Setting min and max
    int min = 0;
    int max = 100;

    // Forever-loop
    while (1) {
      int guess = 0;
      // Guess calculations
      switch (playerNum) {
        case 0:
          guess = (max+min)/2;
          break;
        case 1:
          guess = min + rand() % (max - min + 1);
          break;
      }

      // Comparison Check
      
    }
  }
}

// Referee Function
void referee() {
  // Setting win array
  int wins[2];
  int cmp[2];

  // Internal Mechanism
  for (int a = 1; a <= 10; a++) {
    int target = rand() % 100 + 1;
    int round = 0;
    while (1) {
      // Setting cmp
      // player1 guess (Replace)
      // player2 guess (Replace)

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
}