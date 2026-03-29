# Homework: Page Replacement Policies

<!-- 
NOTES:

 -->

<!-- prompt> ./paging-policy.py --help
Usage: paging-policy.py [options]

Options:
-h, --help      show this help message and exit
-a ADDRESSES, --addresses=ADDRESSES
                a set of comma-separated pages to access; 
                -1 means randomly generate
-f ADDRESSFILE, --addressfile=ADDRESSFILE
                a file with a bunch of addresses in it
-n NUMADDRS, --numaddrs=NUMADDRS
                if -a (--addresses) is -1, this is the 
                number of addrs to generate
-p POLICY, --policy=POLICY
                replacement policy: FIFO, LRU, LFU, OPT, 
                                    UNOPT, RAND, CLOCK
-b CLOCKBITS, --clockbits=CLOCKBITS
                for CLOCK policy, how many clock bits to use
-C CACHESIZE, --cachesize=CACHESIZE
                size of the page cache, in pages
-m MAXPAGE, --maxpage=MAXPAGE
                if randomly generating page accesses, 
                this is the max page number
-s SEED, --seed=SEED  random number seed
-N, --notrace   do not print out a detailed trace
-c, --compute   compute answers for me -->

---

## Questions
- `python3 paging-policy.py -s 10 -n 3`
- `python3 paging-policy.py -C 3 -a 1,2,3,4,1,2,5,1,2,3,4,5`
    <!-- Assuming a replacement policy of FIFO, and a cache of size 3 pages,
    figure out whether each of the following page references hit or miss
    in the page cache. -->

    Access: 1  Hit/Miss? State of Memory?
    ------------ Miss ----- [1] ----------
    Access: 2  Hit/Miss?  State of Memory?
    ------------ Miss ----- [1, 2] -------
    Access: 3  Hit/Miss?  State of Memory?
    ------------ Miss ----- [1, 2, 3] ----
    Access: 4  Hit/Miss?  State of Memory?
    ------------ Miss ----- [2, 3, 4] ----
    Access: 1  Hit/Miss?  State of Memory?
    ------------ Miss ----- [3, 4, 1] ----
    Access: 2  Hit/Miss?  State of Memory?
    ------------ Miss ----- [4, 1, 2] ----
    Access: 5  Hit/Miss?  State of Memory?
    ------------ Miss ----- [1, 2, 5] ----
    Access: 1  Hit/Miss?  State of Memory?
    ------------ Hit ----- [1, 2, 5] ----   
    Access: 2  Hit/Miss?  State of Memory?
    ------------ Hit ----- [1, 2, 5] ----
    Access: 3  Hit/Miss?  State of Memory?
    ------------ Miss ----- [2, 5, 3] ----
    Access: 4  Hit/Miss?  State of Memory?
    ------------ Miss ----- [5, 3, 4] ----
    Access: 5  Hit/Miss?  State of Memory?
    ------------ Hit ----- [5, 3, 4] ----

- `python3 paging-policy.py -C 4 -a 1,2,3,4,1,2,5,1,2,3,4,5`
    <!-- Assuming a replacement policy of FIFO, and a cache of size 4 pages,
    figure out whether each of the following page references hit or miss
    in the page cache. -->

    Access: 1  Hit/Miss?  State of Memory?
    ------------ Miss ----- [1] ----------
    Access: 2  Hit/Miss?  State of Memory?
    ------------ Miss ----- [1, 2] -------
    Access: 3  Hit/Miss?  State of Memory?
    ------------ Miss ----- [1, 2, 3] ----  
    Access: 4  Hit/Miss?  State of Memory?
    ------------ Miss ----- [1, 2, 3, 4] ----  
    Access: 1  Hit/Miss?  State of Memory?
    ------------ Hit ----- [1, 2, 3, 4] ---- 
    Access: 2  Hit/Miss?  State of Memory?
    ------------ Hit ----- [1, 2, 3, 4] ---- 
    Access: 5  Hit/Miss?  State of Memory?
    ------------ Miss ----- [2, 3, 4, 5] ---- 
    Access: 1  Hit/Miss?  State of Memory?
    ------------ Miss ----- [3, 4, 5, 1] ---- 
    Access: 2  Hit/Miss?  State of Memory?
    ------------ Miss ----- [4, 5, 1, 2] ---- 
    Access: 3  Hit/Miss?  State of Memory?
    ------------ Miss ----- [5, 1, 2, 3] ---- 
    Access: 4  Hit/Miss?  State of Memory?
    ------------ Miss ----- [1, 2, 3, 4] ---- 
    Access: 5  Hit/Miss?  State of Memory?
    ------------ Miss ----- [2, 3, 4, 5] ---- 

    This is unusual in that, although both programs have the same page access sequence, when we increase the size of the page cache from 3 to 4, the number of cache hits DECREASES (even though we would expect it to increase!). This is similar to the example we saw in Chapter 21, and is due to the stack property of the FIFO replacement policy.

### Question 1

Generate random addresses with the following arguments: `-s 0 -n 10`, `-s 1 -n 10`, and `-s 2 -n 10`. Change the policy from FIFO, to LRU, to OPT. Compute whether each access in said address traces are hits or misses.
    <!-- For each of these problems, we assume a cache size of 3 pages -->
    - `python3 paging-policy.py -s 0 -n 10 -p FIFO -c`
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [8] =========
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7] ======
    Access: 4  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7, 4] ===
    Access: 2  Hit/Miss?  State of Memory? ====== Miss ======= [7, 4, 2] ===
    Access: 5  Hit/Miss?  State of Memory? ====== Miss ======= [4, 2, 5] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Hit ======== [4, 2, 5] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [2, 5, 7] ===
    Access: 3  Hit/Miss?  State of Memory? ====== Miss ======= [5, 7, 3] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Miss ======= [7, 3, 4] ===
    Access: 5  Hit/Miss?  State of Memory? ====== Miss ======= [3, 4, 5] ===
    - `python3 paging-policy.py -s 0 -n 10 -p LRU -c`
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [8] =========
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7] ======
    Access: 4  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7, 4] ===
    Access: 2  Hit/Miss?  State of Memory? ====== Miss ======= [7, 4, 2] ===
    Access: 5  Hit/Miss?  State of Memory? ====== Miss ======= [4, 2, 5] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Hit ======== [2, 5, 4] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [5, 4, 7] ===
    Access: 3  Hit/Miss?  State of Memory? ====== Miss ======= [4, 7, 3] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Hit ======== [7, 3, 4] ===
    Access: 5  Hit/Miss?  State of Memory? ====== Miss ======= [3, 4, 5] ===
    - `python3 paging-policy.py -s 0 -n 10 -p OPT -c`
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [8] =========
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7] ======
    Access: 4  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7, 4] ===
    Access: 2  Hit/Miss?  State of Memory? ====== Miss ======= [2, 7, 4] ===
    Access: 5  Hit/Miss?  State of Memory? ====== Miss ======= [5, 7, 4] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Hit ======== [5, 7, 4] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Hit ======== [5, 7, 4] ===
    Access: 3  Hit/Miss?  State of Memory? ====== Miss ======= [5, 3, 4] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Hit ======== [5, 3, 4] ===
    Access: 5  Hit/Miss?  State of Memory? ====== Hit ======== [5, 3, 4] ===
    - `python3 paging-policy.py -s 1 -n 10 -p FIFO -c`
    Access: 1  Hit/Miss?  State of Memory? ====== Miss ======= [1] =========
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [1, 8] ======
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [1, 8, 7] ===
    Access: 2  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7, 2] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Miss ======= [7, 2, 4] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Hit ======== [7, 2, 4] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Miss ======= [2, 4, 6] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [4, 6, 7] ===
    Access: 0  Hit/Miss?  State of Memory? ====== Miss ======= [6, 7, 0] ===
    Access: 0  Hit/Miss?  State of Memory? ====== Hit ======== [6, 7, 0] ===
    - `python3 paging-policy.py -s 1 -n 10 -p LRU -c`
    Access: 1  Hit/Miss?  State of Memory? ====== Miss ======= [1] =========
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [1, 8] ======
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [1, 8, 7] ===
    Access: 2  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7, 2] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Miss ======= [7, 2, 4] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Hit ======== [7, 2, 4] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Miss ======= [2, 4, 6] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [4, 6, 7] ===
    Access: 0  Hit/Miss?  State of Memory? ====== Miss ======= [6, 7, 0] ===
    Access: 0  Hit/Miss?  State of Memory? ====== Hit ======== [6, 7, 0] ===
    - `python3 paging-policy.py -s 1 -n 10 -p OPT -c`
    Access: 1  Hit/Miss?  State of Memory? ====== Miss ======= [1] =========
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [1, 8] ======
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [1, 8, 7] ===
    Access: 2  Hit/Miss?  State of Memory? ====== Miss ======= [1, 2, 7] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Miss ======= [4, 2, 7] ===
    Access: 4  Hit/Miss?  State of Memory? ====== Hit ======== [4, 2, 7] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Miss ======= [6, 2, 7] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Hit ======== [6, 2, 7] ===
    Access: 0  Hit/Miss?  State of Memory? ====== Miss ======= [6, 2, 0] ===
    Access: 0  Hit/Miss?  State of Memory? ====== Hit ======== [6, 2, 0] ===
    - `python3 paging-policy.py -s 2 -n 10 -p FIFO -c`
    Access: 9  Hit/Miss?  State of Memory? ====== Miss ======= [9] =========
    Access: 9  Hit/Miss?  State of Memory? ====== Hit ======== [9] =========
    Access: 0  Hit/Miss?  State of Memory? ====== Miss ======= [9, 0] ======
    Access: 0  Hit/Miss?  State of Memory? ====== Hit ======== [9, 0] ======
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [9, 0, 8] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [0, 8, 7] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7, 6] ===
    Access: 3  Hit/Miss?  State of Memory? ====== Miss ======= [7, 6, 3] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Hit ======== [7, 6, 3] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Hit ======== [7, 6, 3] ===
    - `python3 paging-policy.py -s 2 -n 10 -p LRU -c`
    Access: 9  Hit/Miss?  State of Memory? ====== Miss ======= [9] =========
    Access: 9  Hit/Miss?  State of Memory? ====== Hit ======== [9] =========
    Access: 0  Hit/Miss?  State of Memory? ====== Miss ======= [9, 0] ======
    Access: 0  Hit/Miss?  State of Memory? ====== Hit ======== [9, 0] ======
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [9, 0, 8] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [0, 8, 7] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Miss ======= [8, 7, 6] ===
    Access: 3  Hit/Miss?  State of Memory? ====== Miss ======= [7, 6, 3] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Hit ======== [7, 3, 6] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Hit ======== [7, 3, 6] ===
    - `python3 paging-policy.py -s 2 -n 10 -p OPT -c`
    Access: 9  Hit/Miss?  State of Memory? ====== Miss ======= [9] =========
    Access: 9  Hit/Miss?  State of Memory? ====== Hit ======== [9] =========
    Access: 0  Hit/Miss?  State of Memory? ====== Miss ======= [9, 0] ======
    Access: 0  Hit/Miss?  State of Memory? ====== Hit ======== [9, 0] ======
    Access: 8  Hit/Miss?  State of Memory? ====== Miss ======= [9, 0, 8] ===
    Access: 7  Hit/Miss?  State of Memory? ====== Miss ======= [7, 0, 8] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Miss ======= [6, 0, 8] ===
    Access: 3  Hit/Miss?  State of Memory? ====== Miss ======= [6, 3, 8] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Hit ======== [6, 3, 8] ===
    Access: 6  Hit/Miss?  State of Memory? ====== Hit ======== [6, 3, 8] ===
---

### Question 2

For a cache of size 5, generate worst-case address reference streams for each of the following policies: FIFO, LRU, and MRU (worst-case reference streams cause the most misses possible). For the worst case reference streams, how much bigger of a cache is needed to improve performance dramatically and approach OPT?
    <!-- I think this question is hinting that looping-sequential streams cause FIFO and LRU policies to perform extremely poorly. -->
    - `python3 paging-policy.py -C 5 -a 1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6 -p FIFO -c`
        - Let's increase the cache size from 5 to 6, and compare its performance to that of OPT (assuming OPT has a cache size of 6 as well):
            - `python3 paging-policy.py -C 6 -a 1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6 -p FIFO -c`
            - `python3 paging-policy.py -C 6 -a 1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6 -p OPT -c`
        - Indeed, by doing so we find that the performance of FIFO matches that of OPT!
    - `python3 paging-policy.py -C 5 -a 1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6 -p LRU -c`
        - Let's increase the cache size from 5 to 6, and compare its performance to that of OPT (assuming OPT has a cache size of 6 as well):
            - `python3 paging-policy.py -C 6 -a 1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6 -p LRU -c`
            - `python3 paging-policy.py -C 6 -a 1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6 -p OPT -c`
        - Indeed, by doing so we find that the performance of LRU matches that of OPT!
    - `python3 paging-policy.py -C 5 -a 1,2,3,4,5,6,5,6,5,6,5,6,5,6,5,6,5,6 -p MRU -c`
        - Let's increase the cache size from 5 to 6, and compare its performance to that of OPT (assuming OPT has a cache size of 6 as well):
            - `python3 paging-policy.py -C 6 -a 1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6 -p MRU -c`
            - `python3 paging-policy.py -C 6 -a 1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6 -p OPT -c`
        - Indeed, by doing so we find that the performance of MRU matches that of OPT!
---

### Question 3

Generate a random trace (use python or perl). How would you expect the different policies to perform on such a trace?
    - It is shown in Figure 22.2 in Chapter 21 that for a no-locality workload -- i.e., a workload in which each reference is to a random page within the set of accessed pages -- LRU, FIFO, and RAND perform roughly the same, increasing in hit rate as the cache size increases. Hence, I would expect LRU, FIFO, and RAND to perform roughly the same, given the same cache size. Moreover, I would expect LFU and Clock to perform roughly the same, for OPT to perform the best, and UNOPT to perform the worst. 
    - For the sake of completion, let's see how each policy performs on a random trace, provided they each have the same cache size.
         <!-- NOTE: Here, we use a cache size of 50, and use `random_addresses.txt`, which generates 1000 random addresses from 0 to 100 inclusive. -->
        - `python3 paging-policy.py -f random_addresses.txt -p LRU -C 50 -m 100 -N -c` ====== HIT RATE: 48.40 ========
        - `python3 paging-policy.py -f random_addresses.txt -p FIFO -C 50 -m 100 -N -c` ====== HIT RATE: 48.70 ========
        - `python3 paging-policy.py -f random_addresses.txt -p RAND -C 50 -m 100 -N -c` ====== HIT RATE: 48.90 ========
        - `python3 paging-policy.py -f random_addresses.txt -p LFU -C 50 -m 100 -N -c` ====== HIT RATE: 47.50 ========
            - Getting the message "Policy LFU is not yet implemented"?
        - `python3 paging-policy.py -f random_addresses.txt -p CLOCK --clockbits=2 -C 50 -m 100 -N -c` ====== HIT RATE: 48.80 ========
        - `python3 paging-policy.py -f random_addresses.txt -p OPT -C 50 -m 100 -N -c` ====== HIT RATE: 75.70 ========
        - `python3 paging-policy.py -f random_addresses.txt -p UNOPT -C 50 -m 100 -N -c` ====== HIT RATE: 2.80 ========
    - And thankfully, the policies behave as expected! 
--- 

### Question 4

Now generate a trace with some locality. How can you generate such a trace? How does LRU perform on it? How much better than RAND is LRU? How does CLOCK do? How about CLOCK with different numbers of clock bits?
    - Will need to create a new file `generate_addresses_with_locality.py` (which generates a file called `addresses_with_locality.txt`) that builds on top of our existing file `generate_random_addresses.py`
    - Note that we are again generating 1000 addresses, but this time with 80% of the addresses taking on values between 0 and 19 (inclusive), and the other 20% of addresses taking on values between 20 and 100 (inclusive) 
    - `python3 paging-policy.py -f addresses_with_locality.txt -p LRU -C 50 -m 100 -N -c` ====== HIT RATE: 85.30 ========
        - Note that using an LRU policy performs markedly better with a trace with locality as opposed to a trace with NO locality (see random trace from previous question)
    - `python3 paging-policy.py -f addresses_with_locality.txt -p RAND -C 50 -m 100 -N -c` ====== HIT RATE: 79.90 ========
        - LRU is noticeably better than RAND, with the former having a hit rate of 85.30 and the latter having a hit rate of 79.90.
    - `python3 paging-policy.py -f addresses_with_locality.txt -p CLOCK --clockbits=1 -C 50 -m 100 -N -c` ====== HIT RATE: 82.60 ========
    - `python3 paging-policy.py -f addresses_with_locality.txt -p CLOCK --clockbits=2 -C 50 -m 100 -N -c` ====== HIT RATE: 84.60 ========
    - `python3 paging-policy.py -f addresses_with_locality.txt -p CLOCK --clockbits=3 -C 50 -m 100 -N -c` ====== HIT RATE: 85.20 ========
    - `python3 paging-policy.py -f addresses_with_locality.txt -p CLOCK --clockbits=4 -C 50 -m 100 -N -c` ====== HIT RATE: 85.00 ========
---

### Question 5

Use a program like valgrind to instrument a real application and generate a virtual page reference stream. For example, running `valgrind --tool=lackey --trace-mem=yes ls` will output a nearly-complete reference trace of every instruction and data reference made by the program ls. To make this useful for the simulator above, you'll have to first transform each virtual memory reference into a virtual page-number reference (done by masking off the offset and shifting the resulting bits downward). How big of a cache is needed for your application trace in order to satisfy a large fraction of requests? Plot a graph of its working set as the size of the cache increases.
    - Skipping this question for now.

