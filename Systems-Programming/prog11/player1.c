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

// Function Assignment
void child_main(int playerNum);

// Main Function
int main() {
  mkfifo(R_P1, S_IRUSR | S_IWUSR);
  mkfifo(R_P2, S_IRUSR | S_IWUSR);
  mkfifo(P1_R, S_IRUSR | S_IWUSR);
  mkfifo(P2_R, S_IRUSR | S_IWUSR);

  child_main(0);
  return 0;
}

// Child-Player Function
void child_main(int playerNum) {
  // Seeding rng
  srand(time(NULL) ^ getpid());

  // Setting FIFOs + EndVar
  int rfd = checkError(open(R_TO_P[playerNum], O_RDONLY), "Player open read failed...");
  int wfd = checkError(open(P_TO_R[playerNum], O_WRONLY), "Player open write failed...");
  int final = 0;

  // Forever Loop
  while (1) {
    // Setting min and max
    int min = 1;
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

      // Sending guess to referee
      checkError(write(wfd, &guess, sizeof(int)), "Failed to write to Referee...");

      // Comparison Check
      int cmp = 0;
      checkError(read(rfd, &cmp, sizeof(int)), "Failed to read from the Referee...");
      
      if (cmp < 0) {
        min = guess + 1;
      } else if (cmp > 0) {
        max = guess - 1;
      } else {
        break;
      }
    }
    checkError(read(rfd, &final, sizeof(int)), "Failed to read from the Referee...");
    if (final == END) {
      break;
    }
  }
  close(rfd);
  close(wfd);
}