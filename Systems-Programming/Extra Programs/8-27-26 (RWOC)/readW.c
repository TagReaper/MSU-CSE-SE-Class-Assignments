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
  int i = 0;
  int fd = 0;
  int arr[20] = {0};
  int val = 0;
  ssize_t bytesRd = 0;


  fd = checkError(open("test.dat",O_RDONLY),"failed to open file");

  bytesRd = checkError(read(fd,arr,20*sizeof(int)), "failed to read");

  for (i = 0; i < bytesRd/sizeof(int); i++)
    {
      printf("%s %d\n","The value is",arr[i]);
      printf("%2$d %1$s\n","The value is",arr[i]);
    }

  while ( (bytesRd = read(fd,&val,sizeof(int))) > 0)
  {
      printf("%s %d\n","The value is",val);
  }
  checkError(bytesRd,"failed to read");
  
  close(fd);
  
  return 0;

}
