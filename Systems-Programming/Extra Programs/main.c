#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

int checkError(int val, const char * msg){
    if (val == -1){
        perror(msg);
        exit(EXIT_FAULURE);
    }
    return val;
}

int main(){
    return 1;
}

