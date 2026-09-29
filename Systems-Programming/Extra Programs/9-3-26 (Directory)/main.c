#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>


int checkError(int val, const char * msg){
    if (val == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int main(){

    struct dirent *dir;
    DIR * dirp;

    dirp = opendir(".");
    if (dirp == NULL) {
        checkError(-1, "failed to open directory");
    }

    while ((dir = readdir(dirp)) != NULL){
        printf("%s\n", dir->d_name);
    }

    if (errno != 0) {
        checkError(-1, "failed to open directory");
    }

    closedir(dirp);

    return 0;
}

