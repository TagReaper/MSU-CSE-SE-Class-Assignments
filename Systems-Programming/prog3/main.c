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
  // Variable assignment
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
  // Directory Assignment
  const char *dir_path = "values";
  const char *accl_path = "values/accl.dat";
  const char *rota_path = "values/rota.dat";
  const char *angl_path = "values/angl.dat";

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

  close(fd);

  //Directory creation and checking
  mkdir(dir_path, S_IRWXU);

  // Writing acceleration data
  fd = checkError(open(accl_path,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "failed to open the file");

  checkError(write(fd,&ax,sizeof(double)), "failed to write data");
  checkError(write(fd,&ay,sizeof(double)), "failed to write data");
  checkError(write(fd,&az,sizeof(double)), "failed to write data");

  close(fd);

  // Writing rotation data
  fd = checkError(open(rota_path,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "failed to open the file");

  checkError(write(fd,&wx,sizeof(double)), "failed to write data");
  checkError(write(fd,&wy,sizeof(double)), "failed to write data");
  checkError(write(fd,&wz,sizeof(double)), "failed to write data");

  close(fd);

  // Writing angle data
  fd = checkError(open(angl_path,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "failed to open the file");

  checkError(write(fd,&roll,sizeof(double)), "failed to write data");
  checkError(write(fd,&pitch,sizeof(double)), "failed to write data");
  checkError(write(fd,&yaw,sizeof(double)), "failed to write data");

  close(fd);

  return 0;
}