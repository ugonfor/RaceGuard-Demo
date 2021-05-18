#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX 0x100

int main(void){
    char* name = NULL;
    char template1[MAX] = "./raceXXXXXX";
    char text[100] = "This is text";
    name = mktemp(template1);
    struct stat buf;
    
    int check = stat(name, &buf);
    int fd = 0;
    if( check == -1 ){
        fd = open(name, O_CREAT | O_WRONLY, S_IRWXU|S_IRWXG|S_IRWXO);
        write(fd, text, strlen(text));
        close(fd);
    }
    else{
        printf("already exist file name %s\n", name);
    }

    return 0;
}
