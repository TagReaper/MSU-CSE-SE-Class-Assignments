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
  int i = 0;
  int fd = 0;
  int arr[20] = {0};
  double arrD[9] = {0};
  //Raw Bit Data
  int axL = 0;
  int axH = 0;
  int ayL = 0;
  int ayH = 0;
  int azL = 0;
  int azH = 0;
  int wxL = 0;
  int wxH = 0;
  int wyL = 0;
  int wyH = 0;
  int wzL = 0;
  int wzH = 0;
  int rollL = 0;
  int rollH = 0;
  int pitchL = 0;
  int pitchH = 0;
  int yawL = 0;
  int yawH = 0;
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

  fd = checkError(open("raw.dat", O_RDONLY), "Failed to open \"Raw.dat\"");

  bytesRd = checkError(read(fd, arr, 20*sizeof(int)), "Failed to read \"raw.dat\"");

  for (i = 0; i < bytesRd / sizeof(int); i++) {
    switch (i) {
      case 2:
        axL = arr[i];
        break;
      case 3:
        axH = arr[i];
        break;
      case 4:
        ayL = arr[i];
        break;
      case 5:
        ayH = arr[i];
        break;
      case 6:
        azL = arr[i];
        break;
      case 7:
        azH = arr[i];
        break;
      case 8:
        wxL = arr[i];
        break;
      case 9:
        wxH = arr[i];
        break;
      case 10:
        wyL = arr[i];
        break;
      case 11:
        wyH = arr[i];
        break;
      case 12:
        wzL = arr[i];
        break;
      case 13:
        wzH = arr[i];
        break;
      case 14:
        rollL = arr[i];
        break;
      case 15:
        rollH = arr[i];
        break;
      case 16:
        pitchL = arr[i];
        break;
      case 17:
        pitchH = arr[i];
        break;
      case 18:
        yawL = arr[i];
        break;
      case 19:
        yawH = arr[i];
        break;
    }
  }
  //Acceleration Calculations and "signing" data
  ax = 16.0 * (int16_t)((axH << 8) | axL) / 32768.0;
  ay = 16.0 * (int16_t)((ayH << 8) | ayL) / 32768.0;
  az = 16.0 * (int16_t)((azH << 8) | azL) / 32768.0;

  //Angular Velocity Calculations and "signing" data
  wx = 2000.0 * (int16_t)((wxH << 8) | wxL) / 32768.0;
  wy = 2000.0 * (int16_t)((wyH << 8) | wyL) / 32768.0;
  wz = 2000.0 * (int16_t)((wzH << 8) | wzL) / 32768.0;

  //Angle Calculations and "signing" data
  roll = 180.0 * (int16_t)((rollH << 8) | rollL) / 32768.0;
  pitch = 180.0 * (int16_t)((pitchH << 8) | pitchL) / 32768.0;
  yaw = 180.0 * (int16_t)((yawH << 8) | yawL) / 32768.0;

  printf("Acceleration:\n X: %f\n Y: %f\n Z: %f\n", ax, ay, az);
  printf("Angular Velocity:\n X: %f\n Y: %f\n Z: %f\n", wx, wy, wz);
  printf("Angle:\n Roll: %f\n Pitch: %f\n Yaw: %f\n", roll, pitch, yaw);  

  close(fd);

  //Writing to new file

  fd = checkError(open("data.dat",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "failed to open the file");

  for (i = 0; i < 9; i++){
    switch (i) {
      case 0:
        arrD[i] = ax;
        break;
      case 1:
        arrD[i] = ay;
        break;
      case 2:
        arrD[i] = az;
        break;
      case 3:
        arrD[i] = wx;
        break;
      case 4:
        arrD[i] = wy;
        break;
      case 5:
        arrD[i] = wz;
        break;
      case 6:
        arrD[i] = roll;
        break;
      case 7:
        arrD[i] = pitch;
        break;
      case 8:
        arrD[i] = yaw;
        break;
    }
  }

  for (i = 0; i < 9; i++)
    {
      checkError(write(fd,&arrD[i],sizeof(double)), "failed to write data");
    }

  close(fd);

  return 0;
}