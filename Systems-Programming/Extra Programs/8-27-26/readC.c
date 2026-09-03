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
  char str[51];
  int fd = 0;
  ssize_t bytes = 0;


  fd = checkError(open("out.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644),"failed to open");


  // permissions
  //  U   G   O
  // rwx rwx rwx
  // 110 000 000
  //  6   0   0


  printf("Saves text until you enter QUIT\n");
  
  while(1)
    {
      bytes = checkError(read(STDIN_FILENO, str,50),"failed to read");
      str[bytes] = 0;
      if (strcmp(str,"QUIT\n") == 0) break;

      checkError(write(fd,str,bytes),"failed to write");
    }

  close(fd);

  return 0;
}
