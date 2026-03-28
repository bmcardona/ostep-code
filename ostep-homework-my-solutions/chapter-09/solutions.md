<!-- prompt> ./lottery.py -h
Usage: lottery.py [options]

Options:
  -h, --help            
      show this help message and exit
  -s SEED, --seed=SEED  
      the random seed
  -j JOBS, --jobs=JOBS  
      number of jobs in the system
  -l JLIST, --jlist=JLIST
      instead of random jobs, provide a comma-separated list
      of run times and ticket values (e.g., 10:100,20:100
      would have two jobs with run-times of 10 and 20, each
      with 100 tickets)
  -m MAXLEN, --maxlen=MAXLEN
      max length of job
  -T MAXTICKET, --maxtick=MAXTICKET
      maximum ticket value, if randomly assigned
  -q QUANTUM, --quantum=QUANTUM
      length of time slice
  -c, --compute
      compute answers for me -->

1. Compute the solutions for simulations with 3 jobs and random seeds of 1, 2, 
   and 3.
    - `python3 lottery.py -j 3 -l -s 1 -c`
    - `python3 lottery.py -j 3 -l -s 2 -c`
    - `python3 lottery.py -j 3 -l -s 3 -c`

2. Now run with two specific jobs: each of length 10, but one (job 0) with 
   just 1 ticket and the other (job1) with 100 (e.g.,-l 10:1,10:100). What 
   happens when the number of tickets is so imbalanced? Will job 0 ever run 
   before job 1 completes? How often? In general, what does such a ticket 
   imbalance do to the behavior of lottery scheduling?
   - `python3 lottery.py -j 2 -l 10:1,10:100 -s 1 -c`
   - When the number of tickets is so unbalanced, job 0 will run very infrequently. 
   - Job 0 may run before job 1 completes, but only with a 1/101 chance each time a new ticket is selected.  
   - Such a ticket imbalance gives unfair CPU access to Job 1 (at least while Job 1 is still running)
   

3. When running with two jobs of length 100 and equal ticket allocations of 
   100 (-l 100:100,100:100), how unfair is the scheduler? Run with some different 
   random seeds to determine the (probabilistic) answer; let unfairness be 
   determined by how much earlier one job finishes than the other.
   - `python3 lottery.py -j 2 -l 100:100,100:100 -s 1 -c`
        - Unfairness: 196 / 200 = 0.98
   - `python3 lottery.py -j 2 -l 100:100,100:100 -s 2 -c`
        - Unfairness: 190 / 200 = 0.95
   - `python3 lottery.py -j 2 -l 100:100,100:100 -s 3 -c`
        - Unfairness: 196 / 200 = 0.98
   - `python3 lottery.py -j 2 -l 100:100,100:100 -s 4 -c`
        - Unfairness: 199 / 200 = 0.995 <-- Not bad :) 

4. How does your answer to the previous question change as the quantum size 
   (-q) gets larger?
    - As the quantum size gets larger, the unfairness becomes worse gradually, but not necessarily monotonically.
    - `python3 lottery.py -j 2 -l 100:100,100:100 -s 1 -q 5 -c`
        - Unfairness: 190 / 200 = 0.95
    - `python3 lottery.py -j 2 -l 100:100,100:100 -s 1 -q 10 -c`
        - Unfairness: 160 / 200 = 0.95
    - `python3 lottery.py -j 2 -l 100:100,100:100 -s 1 -q 20 -c`
        - Unfairness: 180 / 200 = 0.98
    - `python3 lottery.py -j 2 -l 100:100,100:100 -s 1 -q 40 -c`
        - Unfairness: 200 / 240 = 0.83
    - `python3 lottery.py -j 2 -l 100:100,100:100 -s 1 -q 100 -c`
        - Unfairness: 100 / 200 = 0.50

5. Can you make a version of the graph that is found in the chapter? What else 
   would be worth exploring? How would the graph look with a stride scheduler?    
   - To mimic Figure 9.2, we will gradually increase the job length from 1 to 
     1000... 
        - `python3 lottery.py -j 2 -l 1:100,1:100 -s 1 -c`
        - `python3 lottery.py -j 2 -l 3:100,3:100 -s 1 -c`
        - `python3 lottery.py -j 2 -l 50:100,50:100 -s 1 -c`
        - `python3 lottery.py -j 2 -l 100:100,100:100 -s 1 -c`
        - `python3 lottery.py -j 2 -l 250:100,250:100 -s 1 -c`
        - `python3 lottery.py -j 2 -l 500:100,500:100 -s 1 -c`
        - `python3 lottery.py -j 2 -l 1000:100,1000:100 -s 1 -c`
            - For random seed 1, the unfairness once the job length reaches 
              1000 is 1903 / 2000 = 0.9515
   - If we were using a stride scheduler instead of a proportional-share 
     scheduler for Figure 9.2, assuming no new jobs are introduced (say) halfway 
     as the jobs are executing, I would expect the curve to look more horizontal; i.e., 
     the level of unfairness would be fairly consistent, irrespective of the 
     job length of both jobs.  