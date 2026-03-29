<!-- 

Usage: scheduler.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -j JOBS, --jobs=JOBS  number of jobs in the system
  -l JLIST, --jlist=JLIST
                        instead of random jobs, provide a comma-separated list
                        of run times
  -m MAXLEN, --maxlen=MAXLEN
                        max length of job
  -p POLICY, --policy=POLICY
                        sched policy to use: SJF, FIFO, RR
  -q QUANTUM, --quantum=QUANTUM
                        length of time slice for RR policy
  -c                    compute answers for me 

-->

Questions
1. Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers.
    <!-- SJF -->
    - `./scheduler.py -p SJF -l 200,200,200 -s 100`
    - Average response time: (0 - 0) + (200 - 0) + (400 - 0) / 3 = 600 / 3 = 200
    - Average turnaround time: (200 - 0) + (400 - 0) + (600 - 0) / 3 = 1200 / 3 = 400
    - Job 0: Response: 0, Turnaround: 200 
    - Job 1: Response: 200, Turnaround: 400
    - Job 2: Response: 400, Turnaround: 600
    
    <!-- FIFO -->
    - `./scheduler.py -p FIFO -l 200,200,200 -s 100`
    - Average response time: (0 - 0) + (200 - 0) + (400 - 0) / 3 = 600 / 3 = 200
    - Average turnaround time: (200 - 0) + (400 - 0) + (600 - 0) / 3 = 1200 / 3 = 400
    - Job 0: Response: 0, Turnaround: 200 
    - Job 1: Response: 200, Turnaround: 400
    - Job 2: Response: 400, Turnaround: 600

2. Now do the same but with jobs of different lengths: 100, 200, and 300.
    - `./scheduler.py -p SJF -l 100,200,300 -s 100`
    - Average response time: (0 - 0) + (100 - 0) + (300 - 0) / 3 = 400 / 3 = 133.33
    - Average turnaround time: (100 - 0) + (300 - 0) + (600 - 0) / 3 = 1000 / 3 = 333.33
    - Job 0: Response: 0, Turnaround: 100 
    - Job 1: Response: 100, Turnaround: 300
    - Job 2: Response: 300, Turnaround: 600

    - `./scheduler.py -p FIFO -l 100,200,300 -s 100`
    - Average response time: (0 - 0) + (100 - 0) + (300 - 0) / 3 = 400 / 3 = 133.33
    - Average turnaround time: (100 - 0) + (300 - 0) + (600 - 0) / 3 = 1000 / 3 = 333.33
    - Job 0: Response: 0, Turnaround: 100 
    - Job 1: Response: 100, Turnaround: 300
    - Job 2: Response: 300, Turnaround: 600

3. Now do the same, but also with the RR scheduler and a time-slice of 1.
    - `./scheduler.py -p RR -l 100,200,300 -s 100 -q 1`
    - Average response time: (0 - 0) + (1 - 0) + (2 - 0) / 3 = 3 / 3 = 1
    - Average turnaround time: (300 - 0) + (500 - 0) + (600 - 0) / 3 = 1400 / 3 = 466.66
    - Job 0: Response: 0, Turnaround: 300 
    - Job 1: Response: 1, Turnaround: 500
    - Job 2: Response: 2, Turnaround: 600

4. For what types of workloads does SJF deliver the same turnaround times as FIFO?
    - Workloads in which jobs are ordered shortest-to-longest. I.e., SJF and FIFO are **identical** whenever jobs are ordered shortest-to-longest. 

5. For what types of workloads and quantum lengths does SJF deliver the same response times as RR?
    - Workloads in which the size of each job is equal to k and the quantum length is equal to k.

6. What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?
    - The response time with SJF **increases** as job lengths increase.
    - `./scheduler.py -p SJF -l 100,200,300 -s 100 -c`
    - `./scheduler.py -p SJF -l 1000,2000,3000 -s 100 -c`
    - `./scheduler.py -p SJF -l 10000,20000,30000 -s 100 -c`

7. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?
    - `./scheduler.py -p RR -l 100,200,300 -s 100 -q 1 -c`
    - `./scheduler.py -p RR -l 100,200,300 -s 100 -q 2 -c`
    - `./scheduler.py -p RR -l 100,200,300 -s 100 -q 3 -c`
    - Response time with RR **increases** as quantum lengths increase. 
    - Worst-case response time (given N jobs) = quantum_length * (N - 1), assuming all jobs arrive at time t = 0.