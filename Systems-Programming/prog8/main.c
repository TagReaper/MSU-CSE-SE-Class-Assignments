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

int checkError(int val, const char *msg)
{
  if (val == -1)
    {
      perror(msg);
      exit(EXIT_FAILURE);
    }
  return val;
}


int main() {
  pid_t pid = checkError(fork(), "Failed to fork process...");
  ssize_t bytesRd = 0;

  // Child Process
  if (pid == 0) {
    checkError(execl("./myRand", "myRand", NULL), "Failed to execute...");
  }

  // Parent Process
  int x;
  checkError(waitpid(pid, &x, 0), "Something went wrong...");
  x = checkError(WEXITSTATUS(x), "myRand exited abnormally...");

  // Opening file
  char file_name[15];
  snprintf(file_name, 15, "data%d.dat", x);
  int fd = checkError(open(file_name, O_RDONLY), "Failed to open file...");

  // Reading and Calculating Data
  int data[60];
  int sum;
  float mean;
  bytesRd = checkError(read(fd, data, 60 * sizeof(int)),"Failed to read data from file...");
  close(fd);

  printf("---Values---");
  for (int a = 0; a < 60; a++) {
    if (a % 5 == 0) {
      printf("\n");
    }
    if (a == 59) {
      printf("%d\n", data[a]);
      sum += data[a];
    }else {
      printf("%d, ", data[a]);
      sum += data[a];
    }
  }
  mean = (float)sum/60.0;
  printf("---End of Values---\n\nMean: %f\n", mean);
  unlink(file_name);

  return 0;
}