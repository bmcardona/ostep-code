
// ### Question 2

// Write the program, called tlb.c, that can roughly measure the cost of accessing each page. Inputs to the program should be: the number of pages to touch and the number of trials.

// Note:
    //  The structures pointed to by tp and tzp are defined in ⟨sys/time.h⟩ as:


    //  struct timeval {
    //          time_t       tv_sec;   /* seconds since Jan. 1, 1970 */
    //          suseconds_t  tv_usec;  /* and microseconds */
    //  };

    //  struct timezone {
    //          int     tz_minuteswest; /* of Greenwich */
    //          int     tz_dsttime;     /* type of dst correction to apply */
    //  };

// gcc -O0 -Wall tlb.c -o tlb
// ./tlb > tlb_data.csv
// cat tlb_data.csv

#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>

void getCostOfAccessingPage(int num_trials, int num_pages) {
    struct timeval start, end;
    int page_size = 4096;   // Here, we assume each page has size 4 KB or 4096 bytes
    int jump = page_size / sizeof(int);
    int *my_array = malloc(num_pages * page_size);

    for (int j = 0; j < num_pages * jump; j += jump) {
        my_array[j] = 0;
    }

    gettimeofday(&start, NULL);
    for (int i = 0; i < num_trials; i++) {
        for (int j = 0; j < num_pages * jump; j += jump) {
            my_array[j] += 1;
        };
    };
    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    long elapsed = seconds * 1000000 + microseconds;
    double cost_per_access = (double) elapsed / (num_trials * num_pages);

    printf("%d,%.6f\n", num_pages, cost_per_access);
}


int main() {
    thread_affinity_policy_data_t policy = { 0 };  
    thread_port_t thread = pthread_mach_thread_np(pthread_self());
    
    thread_policy_set(thread, 
                      THREAD_AFFINITY_POLICY,
                      (thread_policy_t)&policy,
                      THREAD_AFFINITY_POLICY_COUNT);

    int num_trials = 1000000;
    printf("num_pages,cost_microseconds\n");
    for (int num_pages = 1; num_pages < 2049; num_pages *= 2) {
        getCostOfAccessingPage(num_trials, num_pages);
    }
    return 0;
}