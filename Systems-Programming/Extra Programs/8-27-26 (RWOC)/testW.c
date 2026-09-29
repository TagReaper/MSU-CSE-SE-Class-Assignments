#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



int checkError(int val, const char *msg)
{
  if (val == -1)
    {
      perror(msg);
      exit(EXIT_FAILURE);
    }
  return val;
}

int main()
{
  int arr[20] = {0};
  int fd;
  int i = 0;

  fd = checkError(open("test.dat",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR), "failed to open the file");

  for (i = 0; i < 20; i++)
    {
      arr[i] = 5 * i;
    }

  for (i = 0; i < 20; i++)
    {
      checkError(write(fd,&arr[i],sizeof(int)), "failed to write data");
    }

  checkError(write(fd,arr,20*sizeof(int)),"failed to write data");


  close(fd);

  


  return 0;
}
