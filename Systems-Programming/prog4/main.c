#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

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

int main() {
  int i = 0;
  int fd = 0;
  double arr[3] = {0};
  char ans[10];
  // Angle
  double roll = 0.0;
  double pitch = 0.0;
  double yaw = 0.0;
  ssize_t bytesRd = 0;

  //opens the file
  fd = checkError(open("accl.dat",O_RDONLY), "failed to open the file");
  bytesRd = checkError(read(fd, arr, 3*sizeof(double)), "Failed to read \"data.dat\"");
  
  printf("Reading Data...\n\n");
  for (i = 0; i < bytesRd / sizeof(double); i++) {
    if (arr[i] > -20.0 && arr[i] < 20.0){
        //Inside Range
        strcpy(ans,"Inside");
    } else {
        //Outside Range
        strcpy(ans,"Outside");
    }
    switch (i) {
      case 0:
        printf("Reading Yaw...\n");
        msSleep(1000);
        printf("Yaw: %f\nYaw Range: %s\n\n", arr[i], ans);
        break;
      case 1:
        printf("Reading Pitch...\n");
        msSleep(1000);
        printf("Pitch: %f\nPitch Range: %s\n\n", arr[i], ans);  
        break;
      case 2:
        printf("Reading Roll...\n");
        msSleep(1000);
        printf("Roll: %f\nRoll Range: %s\n\n", arr[i], ans);
        break;
      }
  }

  return 1;
}
