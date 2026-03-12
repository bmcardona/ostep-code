<!-- 

To run the program, type:
prompt> ./mlfq.py

Use the help flag (-h) to see the options:
Usage: mlfq.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -n NUMQUEUES, --numQueues=NUMQUEUES
                        number of queues in MLFQ (if not using -Q)
  -q QUANTUM, --quantum=QUANTUM
                        length of time slice (if not using -Q)
  -a ALLOTMENT, --allotment=ALLOTMENT
                        length of allotment (if not using -A)
  -Q QUANTUMLIST, --quantumList=QUANTUMLIST
                        length of time slice per queue level, specified as
                        x,y,z,... where x is the quantum length for the
                        highest priority queue, y the next highest, and so
                        forth
  -A ALLOTMENTLIST, --allotmentList=ALLOTMENTLIST
                        length of time allotment per queue level, specified as
                        x,y,z,... where x is the # of time slices for the
                        highest priority queue, y the next highest, and so
                        forth
  -j NUMJOBS, --numJobs=NUMJOBS
                        number of jobs in the system
  -m MAXLEN, --maxlen=MAXLEN
                        max run-time of a job (if randomly generating)
  -M MAXIO, --maxio=MAXIO
                        max I/O frequency of a job (if randomly generating)
  -B BOOST, --boost=BOOST
                        how often to boost the priority of all jobs back to
                        high priority
  -i IOTIME, --iotime=IOTIME
                        how long an I/O should last (fixed constant)
  -S, --stay            reset and stay at same priority level when issuing I/O
  -I, --iobump          if specified, jobs that finished I/O move immediately
                        to front of current queue
  -l JLIST, --jlist=JLIST
                        a comma-separated list of jobs to run, in the form
                        x1,y1,z1:x2,y2,z2:... where x is start time, y is run
                        time, and z is how often the job issues an I/O request
  -c                    compute answers for me -->

1. Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.
    - We'll run two jobs:
        - Job 1: 
            - startTime: 0
            - runTime: 5
            - ioFreq: 0
        - Job 2: 
            - startTime: 2
            - runTime: 7
            - ioFreq: 0
    - To give things a little more pizazz, I'll also set the boost to 4 and quantum to 1.
    - `python mlfq.py -l 0,5,0:2,7,0 -n 2 -B 4 -q 1 -c`
    - Note:
        - Quantum — the length of a single time slice; how long a job runs before being interrupted and the scheduler checks what to run next.
        - Allotment — the number of quanta a job gets at a given priority level before being demoted to the next queue. For example, an allotment of 1 means the job gets demoted after just 1 quantum; an allotment of 10 means it gets 10 quanta worth of CPU time before demotion.

2. How would you run the scheduler to reproduce each of the examples in the chapter?
    <!-- Note: Some of these solutions are slightly incorrect since I don't think we can 
    start a job on a queue that doesn't have the highest priority -->
    - Figure 8.2 
        - `python mlfq.py -l 0,200,0 -n 3 -B 0 -q 10 -a 1 -c`
        - NUMQUEUES --> -n 3 
        - BOOST --> -B 0
        - QUANTUM --> -q 10
        - ALLOTMENT --> -a 1 
    - Figure 8.3 
        - `python mlfq.py -l 0,180,0:100,20,0 -n 3 -B 0 -q 10 -a 1 -c`
        - NUMQUEUES --> -n 3 
        - BOOST --> -B 0
        - QUANTUM --> -q 10
        - ALLOTMENT --> -a 1
    - Figure 8.4 --
        - `python mlfq.py -l 1,180,0:0,20,1 -n 3 -q 1 -A 1,1,10 -S -i 9 -c`
        - NUMQUEUES --> -n 3 
        - BOOST --> -B 0
        - QUANTUM --> -q 1
        - ALLOTMENT --> -A 1,1,10
        - Stay --> -S 
        - IOTIME --> -i 9
    - Figure 8.5 --
        <!-- Left -->
        - `python mlfq.py -l 0,100,0:100,50,1:101,50,1 -n 3 -B 0 -q 10 -A 1,1,1 -S -i 1 -c`
        - NUMQUEUES --> -n 3 
        - BOOST --> -B 0
        - QUANTUM --> -q 10
        - ALLOTMENT --> -A 1,1,1
        - Stay --> -S 
        - IOTIME --> -i 1
        <!-- Right -->
        <!-- Not sure how to switch back and forth between 1st and 2nd job without
        also switching back and forth with 0th job... -->
        <!-- - `python mlfq.py -l 0,130,0:100,35,1:101,35,1 -n 3 -B 100 -q 10 -A 1,1,1 -S -i 1 -c`
        - NUMQUEUES --> -n 3 
        - BOOST --> -B 100
        - QUANTUM --> -q 10
        - ALLOTMENT --> -A 1,1,1
        - Stay --> -S 
        - IOTIME --> -i 1 -->
    <!-- I think I understand the concept. Will come back to these problems later if I need a refresher... -->
    - Figure 8.6 --
    - Figure 8.7 --

3. How would you configure the scheduler parameters to behave just like a round-robin scheduler?
    <!-- Round-robin scheduler w/ 3 jobs, 1 queue, no boost, and quantum of 5 ms -->
    - `python mlfq.py -l 0,100,0:0,100,0:0,100,0 -n 1 -B 0 -q 5 -c`
    - NUMQUEUES --> -n 1
    - BOOST --> -B 0
    - QUANTUM --> -q 5

4. Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval.
        <!-- Job 0 games scheduler for first 1000 ms -->
        - `python mlfq.py -l 0,1000,99:0,1000,0 -n 3 -q 100 -A 1,1,1 -S -i 1 -I -c`
        - NUMQUEUES --> -n 3 
        - QUANTUM --> -q 100
        - ALLOTMENT --> -A 1,1,1
        - Stay --> -S 
        - IOTIME --> -i 1
        - -I
5. Given a system with a quantum length of 10ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single long-running (and potentially-starving) job gets at least 5% of the CPU?
    - 200 ms? Not sure if this question is considering that there may be a large
    number of jobs? If there are a large number of jobs, even with 200 ms 
    a single long-running job would not be guaranteed at least 5% of CPU time...

