#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
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

void child(int fd[2]) {
  
}

void parent(int fd[2]) {
  
}

int main() {
  // Variable Creation
  int fd[2];
  pid_t pid;

  // Creating a pipe
  checkError(pipe(fd), "Failed to pipe...");

  // Forking the program
  pid = checkError(fork(), "Failed to fork...");

  if (pid == 0) {
    child(fd);
  } else {
    parent(fd);
  }
  
  return 0;
}