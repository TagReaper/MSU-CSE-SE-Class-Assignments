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
  close(fd[0]);
  checkError(dup2(fd[1], STDOUT_FILENO), "Failed dup2...");
  close(fd[1]);

  char *args[] = {"rand", "-c", "50", "-m", "200", "-M", "500", NULL};
  checkError(execvp("./rand", args), "Failed execution of rand...");
}

void parent(int fd[2]) {
  int sum = 0;
  int count = 0;
  int value;
  ssize_t n;

  close(fd[1]);

  while (1) {
    n = read(fd[0], &value, sizeof(int));

    if (n == 0 || n == -1) {
      break;
    }
    count++;
    printf("#%d: %d\n", count, value);
    sum += value;
  }

  close(fd[0]);
  wait(NULL);

  printf("Total: %d\n", sum);
  printf("Average: %f\n", (double)sum/count);
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