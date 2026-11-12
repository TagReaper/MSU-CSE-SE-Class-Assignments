#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

// Defining Names
#define R_P1 "referee_to_player1"
#define R_P2 "referee_to_player2"
#define P1_R "player1_to_referee"
#define P2_R "player2_to_referee"
#define END 999

char *R_TO_P[2] = {R_P1, R_P2};
char *P_TO_R[2] = {P1_R, P2_R};

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

void msSleep(long miliseconds){
  struct timespec ts;

  ts.tv_sec = miliseconds / 1000;
  ts.tv_nsec = (miliseconds % 1000) * 1000000L;

  //nanosleep()
  checkError(nanosleep(&ts, NULL), "Sleep Interupted");
}

// Function Assignment
void referee();

// Main Function
int main() {
  mkfifo(R_P1, S_IRUSR | S_IWUSR);
  mkfifo(R_P2, S_IRUSR | S_IWUSR);
  mkfifo(P1_R, S_IRUSR | S_IWUSR);
  mkfifo(P2_R, S_IRUSR | S_IWUSR);

  referee();

  // Clean up
  unlink(R_P1);
  unlink(P1_R);
  unlink(R_P2);
  unlink(P2_R);

  return 0;
}

// Referee Function
void referee() {
  // Seeding rng
  srand(time(NULL) ^ getpid());

  // Setting arrays
  int wins[2] = {0,0};
  int cmp[2];
  int guess[2];

  int wP1 = checkError(open(R_TO_P[0], O_WRONLY), "Failed to open P1 write...");
  int wP2 = checkError(open(R_TO_P[1], O_WRONLY), "Failed to open P2 write...");
  int rP1 = checkError(open(P_TO_R[0], O_RDONLY), "Failed to open P1 read...");
  int rP2 = checkError(open(P_TO_R[1], O_RDONLY), "Failed to open P2 read...");

  // Internal Mechanism
  for (int a = 1; a <= 10; a++) {
    // Print game Statistics
    printf("\n\n=== Game %d of 10 === (Score P1/P2: %d/%d)\n\n", a, wins[0], wins[1]);

    int target = rand() % 100 + 1;
    int round = 0;
    while (1) {
      msSleep(200);

      // Reading Guesses
      checkError(read(rP1, &guess[0], sizeof(int)), "Player 1 read failed...");
      checkError(read(rP2, &guess[1], sizeof(int)), "Player 2 read failed...");

      // Printing Guesses
      printf("   Round %d: P1 guessed %d, P2 guessed %d\n\n", round, guess[0], guess[1]);

      // Setting cmp
      cmp[0] = guess[0] - target;
      cmp[1] = guess[1] - target;

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

      checkError(write(wP1, &cmp[0], sizeof(int)), "Failed to write to Player1...");
      checkError(write(wP2, &cmp[1], sizeof(int)), "Failed to write to Player2...");


      // Checking if game is ended, and printing the winner
      if (end) {
        printf("   Target was %d\n\n", target);
        int msg;
        if (a == 10) {
          msg = END;
        } else {
          msg = 0;
        }
        checkError(write(wP1, &msg, sizeof(int)), "Failed to write to Player1...");
        checkError(write(wP2, &msg, sizeof(int)), "Failed to write to Player2...");
        break;
      }
    }
  }

  // Printing Game Statistics
  printf("\n=== Final Results ===\n");
  printf("Player 1: %d wins\n", wins[0]);
  printf("Player 2: %d wins\n", wins[1]);

  close(rP1);
  close(wP1);
  close(rP2);
  close(wP2);
}