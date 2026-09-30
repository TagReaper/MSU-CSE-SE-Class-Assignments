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
  // Data Arrays
  unsigned char arr[20];
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

  int in_fd  = checkError(open("raw.dat", O_RDONLY), "Failed to open raw.dat");
  int out_fd = checkError(open("data.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "Failed to open output");

  while ((bytesRd = read(in_fd, arr, 20)) > 0) {
    // Assigning Calculations
    arrD[0] = 16.0 * (short)((arr[3] << 8) | arr[2]) / 32768.0;
    arrD[1] = 16.0 * (short)((arr[5] << 8) | arr[4]) / 32768.0;
    arrD[2] = 16.0 * (short)((arr[7] << 8) | arr[6]) / 32768.0;

    arrD[3] = 2000.0 * (short)((arr[9] << 8) | arr[8]) / 32768.0;
    arrD[4] = 2000.0 * (short)((arr[11] << 8) | arr[10]) / 32768.0;
    arrD[5] = 2000.0 * (short)((arr[13] << 8) | arr[12]) / 32768.0;

    arrD[6] = 180.0 * (short)((arr[15] << 8) | arr[14]) / 32768.0;
    arrD[7] = 180.0 * (short)((arr[17] << 8) | arr[16]) / 32768.0;
    arrD[8] = 180.0 * (short)((arr[19] << 8) | arr[18]) / 32768.0;

    printf("Acceleration:\n X: %f\n Y: %f\n Z: %f\n", arrD[0], arrD[1], arrD[2]);
    printf("Angular Velocity:\n X: %f\n Y: %f\n Z: %f\n", arrD[3], arrD[4], arrD[5]);
    printf("Angle:\n Roll: %f\n Pitch: %f\n Yaw: %f\n\n", arrD[6], arrD[7], arrD[8]);

    checkError(write(out_fd, arrD, sizeof(arrD)), "Failed to write data");
  }
  close(in_fd);
  close(out_fd);

  return 1;
}