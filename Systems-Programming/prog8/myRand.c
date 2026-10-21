#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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
  //1. Generate a new file name dataX.dat where X is a random number between 0 and 255.
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  srand(time(NULL) * ts.tv_nsec);
  int x = rand() % 256;

  char file_name[15];
  snprintf(file_name, 15, "data%d.dat", x);

  int fd = checkError(open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "Failed to open or create file...");
  
  //2. Generate 60 values between 0 and 100 and store them in the file.
  for (int a = 0; a < 60; a++) {
    int y = rand() % 101;
    
    checkError(write(fd, &y, sizeof(int)), "Failed to write to file...");
  }
  // 3. Exit with an exit status of X.
  exit(x);
}