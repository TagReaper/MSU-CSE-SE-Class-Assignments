#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <setjmp.h>

volatile sig_atomic_t timeout = 0;

sigjmp_buf jump_buffer;

// For Cleaning the Terminal (IDK why, but this freezes if there is no text in
// the scanf. Not permanantly, just until you give some kind of input though)
void cleanIO()
{
  char garbage[100];
  static int once = 0;
  static struct termios old_tio, new_tio;
  if (!once)
    {
      tcgetattr(STDIN_FILENO, &old_tio);
      new_tio = old_tio;
      cfmakeraw(&new_tio);
      once = 1;
    }

  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  read(STDIN_FILENO, garbage, 100);

  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

// Starts the timer
void start_timer(int seconds) {
  timeout = 0;
  struct itimerval timer;
  timer.it_value.tv_sec = seconds;      // Seconds for the timer
  timer.it_value.tv_usec = 0;
  timer.it_interval.tv_sec = 0;         // Not Repeating
  timer.it_interval.tv_usec = 0;
  setitimer(ITIMER_REAL, &timer, NULL);
}

// Cancels the timer and returns seconds remaining
double stop_timer(void) {
  struct itimerval stop = {0, 0, 0, 0};
  struct itimerval remaining;
  setitimer(ITIMER_REAL, &stop, &remaining);
  return remaining.it_value.tv_sec + remaining.it_value.tv_usec / 1000000.0;
}


// Found siglongjump in Ch22 of the book
void handle_exit(int sig) {
  siglongjmp(jump_buffer, 2);
}

// Found siglongjump in Ch22 of the book
void handle_alarm(int sig) {
  timeout = 1;
  siglongjmp(jump_buffer, 1);
}

//Error checking and response
int checkError(int val, const char *msg)
{
  if (val == -1)
    {
      perror(msg);
      exit(EXIT_FAILURE);
    }
  return val;
}

// For SysProg Project5
int main() {
  int fd = 0;
  int score = 0;
  int qT = 0;
  int q = 1;
  char buffer[512];
  char target[32];
  char lineAns[32];
  char ans;

  //Alarm-Timer
  struct sigaction sa_timer;
  sa_timer.sa_handler = handle_alarm;
  sigemptyset(&sa_timer.sa_mask);
  sa_timer.sa_flags = 0;
  sigaction(SIGALRM, &sa_timer, NULL);

  // Exit Functionality
  struct sigaction sa_ext;
  sa_ext.sa_handler = handle_exit;
  sigemptyset(&sa_ext.sa_mask);
  sa_ext.sa_flags = 0;
  sigaction(SIGINT, &sa_ext, NULL);

  ssize_t bytesRead;

  // Informing the user
  printf(
      "You are about to begin a timed typing quiz.\nYou will have 15 seconds to answer each question.\n\nWould you like to continue? (Y/N):");
  scanf("%c", &ans);

  // Checking for procceding
  while (1) {
    if (ans == 'N' || ans == 'n') {
      printf("\nThank you, Goodbye...");
      exit(0);
    } else if (ans != 'Y' && ans != 'y') {
      printf("\n Invalid Response, Please type \"Y\" or \"N\".");
      scanf(" %c", &ans);
    } else {
      break;
    }
  }

  // Program Code
  // This program is to check if the typed text is the same as the line in the
  // test.txt document
  fd = checkError(open("test.txt", O_RDONLY), "Failed to open file.");

  while ((bytesRead = read(fd, buffer, 512)) > 0) {
    int count = 0;
    for (int i = 0; i < bytesRead; i++) {
      if (buffer[i] == '\n') {
        qT += 1;
        }
      }
    for (int i = 0; i < bytesRead; i++) {
      if (q < qT) {
        char c = buffer[i];
        if (c == '\n') {
          target[i-count] = '\0';
          // Run Program code Here
          start_timer(15);
          int result;
          int retry;
          int time_left = 15;

          // This should handle question skipping from the Exit signal
          // This is the same as a while loop, but it is exit controlled rather than entry controlled
          do {
            retry = 0;
            printf("\nType the text (after question #:) below and press enter; you have %d seconds.\nQuestion #%d: %s\n", time_left, q, target);

            int jump_val = sigsetjmp(jump_buffer, 1);

            if (jump_val == 0) {
              result = scanf(" %31[^\n]", lineAns);
            } else if (jump_val == 1) {
              // Timeout
              result = -1;
            } else if (jump_val == 2) {
              // Exit handler
              time_left = (int) stop_timer();   // pause the countdown while we ask
              char ans;
              printf("\n\nAre you sure you would like to exit? (Y/N): ");
              scanf(" %c", &ans);
              if (ans == 'Y' || ans == 'y') {
                  printf("\nThank you, Goodbye...\n");
                  exit(0);
              }
              printf("\nResuming quiz...\n");
              start_timer(time_left); // Resume the timer
              retry = 1;
            }
          } while (retry); // Do it again if there was an exit attemp, but keep the time


          if (timeout) {
            printf("\nTime's up.\n");
            cleanIO();
          } else if (result == 1 && strcmp(target, lineAns) == 0) {
            int time_remaining = (int)stop_timer();
            printf("Correct: +%d points\n", time_remaining);
            score += time_remaining;
          } else {
            printf("Incorrect.\n");
          }
          //After Program
          memset(target, 0, sizeof(target));
          count = i + 1;
          q += 1;
        } else {
          target[i - count] = buffer[i];
        }
      } else {
        break;
      }
    }
  }
  printf("\nTotal Score: %d/%d\n", score, (15*(q-1)));

  close(fd);

  return 0;
}