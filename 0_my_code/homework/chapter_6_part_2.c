#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#define READ_END 0
#define WRITE_END 1
#define NUM_ITERATIONS 1000

int main() {
    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    // create child process from parent process
    int pid = fork();
    if (pid == 0) {
        char send = 'x';
        char receive;
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            // child process writes to pipe 1 and reads from pipe 2
            write(pipe1[WRITE_END], &send, 1);
            read(pipe2[READ_END], &receive, 1);
        };
    } else {
        struct timeval start, end;
        char send = 'x';
        char receive;
        gettimeofday(&start, NULL);
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            // parent process reads from pipe 1 and writes to pipe 2
            read(pipe1[READ_END], &receive, 1);
            write(pipe2[WRITE_END], &send, 1);
        };
        gettimeofday(&end, NULL);
        long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec; 
        double avg = (double)elapsed / NUM_ITERATIONS; 
        printf("Time for two context switches and four system calls to execute (on my Mac's multi-core CPU): %.3f microseconds\n", avg); 
        wait(NULL);
    }
    return 0;
}
