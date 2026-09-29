#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("Before Exec\n");
    //execl("/bin/ls", "ls", "-a", "-l", NULL); //Exec function blocks code if it doesn't fail
    system("ls -l -a"); // System continues running (non-blocking)
    printf("After Exec\n");
}