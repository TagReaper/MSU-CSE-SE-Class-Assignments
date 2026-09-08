#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>


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

int main() {
  //variable assignment
  int i = 0;
  int fd = 0;
  double arrD[9] = {0};
  // Acceleration Data
  double ax = 0.0;
  double ay = 0.0;
  double az = 0.0;
  // Angular Velocity
  double wx = 0.0;
  double wy = 0.0;
  double wz = 0.0;
  // Angle
  double roll = 0.0;
  double pitch = 0.0;
  double yaw = 0.0;
  ssize_t bytesRd = 0;

  //opens the file
  fd = checkError(open("data.dat",O_RDONLY), "failed to open the file");
  bytesRd = checkError(read(fd, arrD, 9*sizeof(double)), "Failed to read \"data.dat\"");

  //Reads each data entry stored in data.dat
   for (i = 0; i < bytesRd / sizeof(double); i++) {
    switch (i) {
      case 0:
        ax = arrD[i];
        break;
      case 1:
        ay = arrD[i];
        break;
      case 2:
        az = arrD[i];
        break;
      case 3:
        wx = arrD[i];
        break;
      case 4:
        wy = arrD[i];
        break;
      case 5:
        wz = arrD[i];
        break;
      case 6:
        roll = arrD[i];
        break;
      case 7:
        pitch = arrD[i];
        break;
      case 8:
        yaw = arrD[i];
        break;
      }
    }


  //Temp Printing Data
  printf("Acceleration:\n X: %f\n Y: %f\n Z: %f\n", ax, ay, az);
  printf("Angular Velocity:\n X: %f\n Y: %f\n Z: %f\n", wx, wy, wz);
  printf("Angle:\n Roll: %f\n Pitch: %f\n Yaw: %f\n", roll, pitch, yaw);  

  close(fd);

  return 0;
}