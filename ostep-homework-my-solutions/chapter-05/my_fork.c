#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Hello, world! (pid: %d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        printf("Hello, I'm child (pid: %d)\n", (int) getpid());
    }
    else {
        int wc = wait(NULL);
        printf("Hello, I'm parent of %d (wc: %d) (pid: %d)\n", rc, wc, (int) getpid());
    }
    return 0;
}

// gcc -Wall -Wextra -std=c17 my_fork.c -o filename && ./filename
