#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <sys/time.h>
#include <signal.h>
#include <math.h>
#include <sys/wait.h>
#include <errno.h>

#define LIMIT 20.0

volatile pid_t child_pid = 0;

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

void msSleep(long miliseconds){
  struct timespec ts;

  ts.tv_sec = miliseconds / 1000;
  ts.tv_nsec = (miliseconds % 1000) * 1000000L;

  //nanosleep()
  checkError(nanosleep(&ts, NULL), "Sleep Interupted");
}

void handle_child(int sig) {
  int er = errno;
  int child_state;
  pid_t pid;

  while (1) {
    pid = waitpid(-1, &child_state, WNOHANG);
    if (pid > 0) {
      write(STDOUT_FILENO, "Reaped a terminated child...\n", 29);
      continue;
    }
    if (pid == -1 && errno == ECHILD) {
      write(STDOUT_FILENO, "No children remaining, terminating process...\n", 46);
      _exit(0);
    }
    break;
  }
  errno = er;
}

void handle_exit(int sig) {
  char ans = 'n';
  printf("Exit: Are you sure (Y/n)?");
  while (1) {
    scanf(" %c", &ans);
    if (ans == 'Y'){
      printf("\nGoodbye!\n");
      if (child_pid > 0) {
        kill(child_pid, SIGTERM);
      }
      _exit(0);
    } else {
      printf("\nCanceling Exit...\n");
      break;
    }
  }
       
}

void handle_roll_OOB(int sig) {
  write(STDOUT_FILENO, "Warning! Roll outside of bounds\n", 32);
}

void handle_pitch_OOB(int sig) {
  write(STDOUT_FILENO, "Warning! Pitch outside of bounds\n", 33);
}

void child_terminator(int sig) {
  write(STDOUT_FILENO, "Child: SIGTERM recieved, teminating...\n", 39);
  _exit(0);
}

void child_main() {
  struct sigaction child_sa;
  double arr[3];
  int fd;
  ssize_t bytesRd;
  
  // Block SIGINT so Ctrl-C only reaches the parent
  sigemptyset(&child_sa.sa_mask);
  sigaddset(&child_sa.sa_mask, SIGINT);
  sigprocmask(SIG_BLOCK, &child_sa.sa_mask, NULL);

  child_sa.sa_flags = 0;
  sigemptyset(&child_sa.sa_mask);
  child_sa.sa_handler = child_terminator;
  sigaction(SIGTERM, &child_sa, NULL);

  fd = checkError(open("angl.dat", O_RDONLY), "Failed to open file.");

  while ((bytesRd = read(fd, arr, 3 * sizeof(double))) > 0) {
    if (fabs(arr[0]) > LIMIT){
        kill(getppid(), SIGUSR1);
    }
    if (fabs(arr[1]) > LIMIT){
        kill(getppid(), SIGUSR2);
    }
    msSleep(1000);
  }
  close(fd);

  printf("Child: End of data, exiting...\n");
  
  _exit(0);
}

int main() {
  //Signal Setup
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  // Assigning Signal Handlers
  sa.sa_handler = handle_child;
  sigaction(SIGCHLD, &sa, NULL);
  sa.sa_handler = handle_exit;
  sigaction(SIGINT, &sa, NULL);
  sa.sa_handler = handle_roll_OOB;
  sigaction(SIGUSR1, &sa, NULL);
  sa.sa_handler = handle_pitch_OOB;
  sigaction(SIGUSR2, &sa, NULL);

  printf("Spawning child...\n");
  child_pid = checkError(fork(), "Fork failed.");

  //Only runs for the child
  if (child_pid == 0) {
    child_main();
  }

  while (1){
    pause();
  }

  return 0;
}
