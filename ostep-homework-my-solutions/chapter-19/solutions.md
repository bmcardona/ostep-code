# Homework: TLB Measurement

---

## Questions

### Question 1

For timing, you'll need to use a timer such as that made available by gettimeofday(). How precise is such a timer? How long does an operation have to take in order for you to time it precisely? (this will help determine how many times, in a loop, you'll have to repeat a page access in order to time it successfully)
    ```bash
    SYNOPSIS
     #include <sys/time.h>

     int
     gettimeofday(struct timeval *restrict tp, void *restrict tzp);

     int
     settimeofday(const struct timeval *tp, const struct timezone *tzp);
    ```
    - From the man pages, we know that gettimeofday() is expressed in seconds and microseconds since midnight (hour 0), January 1, 1970. Moreover, the resolution of the system clock is hardware dependent, and the time may be updated continuously or in "ticks." 

---

### Question 2

Write the program, called tlb.c, that can roughly measure the cost of accessing each page. Inputs to the program should be: the number of pages to touch and the number of trials.
    - See `tlb.c` in the same directory as this file

---

### Question 3

Now write a script in your favorite scripting language (csh, python, etc.) to run this program, while varying the number of pages accessed from 1 up to a few thousand, perhaps incrementing by a factor of two per iteration. Run the script on different machines and gather some data. How many trials are needed to get reliable measurements?
    - I ended up just using a for loop in main() in `tlb.c` to vary the number of pages from 1 to 2 to ... to 2048. I only have one machine (my laptop!), so running on multiple machines is infeasible for now. I found that roughly 1000000 trials for each number of pages worked pretty reliably. 
    
---

### Question 4

Next, graph the results, making a graph that looks similar to the one above. Use a good tool like ploticus. Visualization usually makes the data much easier to digest; why do you think that is?
    - See `plot_tlb.py` and `1_plot_tlb_multiple_cores.png`
    - From our figure, I think it's safe to assume that our first-level TLB holds around 32-64 entries.
    - Visualization makes it much easier to see how two or more variables (in this case, Number Of Pages and Time Per Access (ns) are related to each other)

---

### Question 5

One thing to watch out for is compiler optimization. Compilers do all sorts of clever things, including removing loops which increment values that no other part of the program subsequently uses. How can you ensure the compiler does not remove the main loop above from your TLB size estimator?
    - I ended up just adding the `-O0` flag to ensure manually that optimizations were turned off 

---

### Question 6

Another thing to watch out for is the fact that most systems today ship with multiple CPUs, and each CPU, of course, has its own TLB hierarchy. To really get good measurements, you have to run your code on just one CPU, instead of letting the scheduler bounce it from one CPU to the next. How can you do that? (hint: look up "pinning a thread" on Google for some clues) What will happen if you don't do this, and the code moves from one CPU to the other?
    - See new code at top of main() in `tlb.c`, as well as `2_plot_tlb_single_core.png`
    - If our process moves from one CPU to another, then the rate at which we will get a TLB "hit" will be much less consistent. This is because each CPU has its own TLB, and when the OS scheduler moves our process to a different CPU, that CPU's TLB is cold (empty of our translations). We will need to go through several TLB "misses" on the new CPU before the TLB warms up again and we start getting "hits". This introduces noise and inconsistency into our measurements, making it harder to accurately determine the true TLB size and hit/miss costs.

---

### Question 7

Another issue that might arise relates to initialization. If you don't initialize the array a above before accessing it, the first time you access it will be very expensive, due to initial access costs such as demand zeroing. Will this affect your code and its timing? What can you do to counterbalance these potential costs?
    - Yes, this will affect my code; with num_pages = 1, specifically, it will make the first iteration through the array much slower relative to the remaining iterations. 
    - We can counteract these potential costs by initializing the array zeros. By implementing this change (see my implementation for getCostOfAccessingPage() in `tlb.c`), it seems that the access time per page (in nanoseconds) decrease slightly from 2.8 to 2.5. 