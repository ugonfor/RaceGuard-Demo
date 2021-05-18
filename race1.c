#include <stdio.h>
#include <pthread.h>
#include <time.h>

int count = 0;


void print_count(char* data){
    for (int i = 0; i < 10; i++)
    {
        printf("%s : %d\n", data, count);
        count++;
    }
    
}

int main(int argc, char *argv[])
{
    pthread_t thread1, thread2;
    char threadname[2][20]={
        {"thread1"},
        {"thread2"}
    };

    pthread_create(&thread1, NULL, print_count, threadname[0]);
    pthread_create(&thread2, NULL, print_count, threadname[1]);    

    sleep(1);
    printf("count : %d\n", count);
    return 0;
}