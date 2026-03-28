
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

int
main() {
    int fd = open("test.txt", O_RDONLY | O_CREAT, 0644);
    char buffer[1];
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < 1000000; i++) {
        read(fd, buffer, 0);
    };
    gettimeofday(&end, NULL);
    long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec; 
    double avg = (double)elapsed / 1000000.0;
    printf("Average time per read: %.3f microseconds\n", avg);
    return 0;
}

