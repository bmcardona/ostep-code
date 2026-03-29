
#include <stdlib.h>  // gives us malloc
#include <stdio.h>   // gives us printf

int
main() {
    double *x = (double *) malloc(sizeof(double));
    *x = 3.14;
    printf("%.2f\n", *x);
    return 0;
}


// gcc -Wall -Wextra -std=c17 allocate_memory.c -o program && ./program
