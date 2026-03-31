# Homework: Concurrency: An Introduction

<!-- This program, x86.py, allows you to see how different thread interleavings either cause or avoid race conditions. See the README for details on how the program works and its basic inputs, then answer the questions below. -->

---
## Preliminary work
`python3 x86.py -p simple-race.s -t 1`
<!-- 
NOTES:
    - The values on the left show the memory/register contents AFTER the instruction on the right has executed.
-->
- `python3 x86.py -p looping-race-nolock.s -t 2 -a bx=1 -M 2000 -R ax,bx -c`

- `python3 x86.py -p looping-race-nolock.s -t 2 -a bx=1 -M 2000 -i 2 -c`
    - Q: Each thread is interrupt every 2 instructions, as we specify via the -i 2 flag. What is the value of memory[2000] throughout this run? What should it have been?
        - A: The value memory of memory[2000] changes from 0 to 1 throughout the run, when it should have changed from 0 to 1 to 2. 

 
   2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    0                            1000 mov 2000, %ax
    0                            1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1002 mov %ax, 2000
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1                            1004 test $0, %bx
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----
    1                            1006 halt 


    
## Questions

### Question 1

To start, let's examine a simple program, `loop.s`. First, just look at the program, and see if you can understand it: `cat loop.s`

Then, run it with these arguments: `python3 x86.py -p loop.s -t 1 -i 100 -R dx -c`

This specifies a single thread, an interrupt every 100 instructions, and tracing of register `%dx`. Can you figure out what the value of `%dx` will be during the run? Once you have, run the same above and use the `-c` flag to check your answers; note the answers, on the left, show the value of the register (or memory value) after the instruction on the right has run.


    dx          Thread 0         
    0   
    -1   1000 sub  $1,%dx
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    -1   1003 halt

---

### Question 2

Now run the same code but with these flags: `python3 x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx -c`

This specifies two threads, and initializes each `%dx` register to 3. What values will `%dx` see? Run with the `-c` flag to see the answers. Does the presence of multiple threads affect anything about your calculations? Is there a race condition in this code?


   dx          Thread 0                Thread 1         
    3   
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    0   1002 jgte .top
    -1   1000 sub  $1,%dx
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    -1   1003 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    1                            1000 sub  $1,%dx
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
    -1                            1000 sub  $1,%dx
    -1                            1001 test $0,%dx
    -1                            1002 jgte .top
    -1                            1003 halt 


---

### Question 3

Now run the following: `python3 x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx`

This makes the interrupt interval quite small and random; use different seeds with `-s` to see different interleavings. Does the frequency of interruption change anything about this program?

`python3 x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx -s 2`

   dx          Thread 0                Thread 1         
    3   
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    3   ------ Interrupt ------  ------ Interrupt ------  
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    1   1000 sub  $1,%dx
    2   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1001 test $0,%dx
    0   1002 jgte .top
    -1   1000 sub  $1,%dx
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1001 test $0,%dx
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1002 jgte .top
    -1                            1000 sub  $1,%dx
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1   1003 halt
    -1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    -1                            1001 test $0,%dx
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1                            1002 jgte .top
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1                            1003 halt


- Let's now change the random seed from 2 to 1...
`python3 x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx -s 1 -c`

   dx          Thread 0                Thread 1         
    3   
    2   1000 sub  $1,%dx
    3   ------ Interrupt ------  ------ Interrupt ------  
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    2   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 sub  $1,%dx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1001 test $0,%dx
    1   1002 jgte .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    1   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1002 jgte .top
    0   ------ Interrupt ------  ------ Interrupt ------  
    -1                            1000 sub  $1,%dx
    0   ------ Interrupt ------  ------ Interrupt ------  
    -1   1000 sub  $1,%dx
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1                            1001 test $0,%dx
    -1                            1002 jgte .top
    -1   ------ Interrupt ------  ------ Interrupt ------  
    -1   1003 halt
    -1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    -1                            1003 halt

- Reducing the the value of the frequency of interrupts (`-i`) results in more overall interrupts.

---

### Question 4

Next we'll examine a different program (`looping-race-nolock.s`). This program accesses a shared variable located at memory address 2000; we'll call this variable `x` for simplicity. Run it with a single thread and make sure you understand what it does, like this: `python3 x86.py -p looping-race-nolock.s -t 1 -M 2000`

What value is found in `x` (i.e., at memory address 2000) throughout the run? Use `-c` to check your answer.

 2000          Thread 0         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1006 halt

---

### Question 5

Now run with multiple iterations and threads: `python3 x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`

Do you understand why the code in each thread loops three times? What will the final value of `x` be?
    - The code in each thread loops three times because bx has an initial value of 3 and we are checking if bx > 0 each time we reach 100 jgt .top. 
    - The final value of 2000 will be 6, since the number of increments to `x` for each thread is equal to the number of successful loop iterations + 1. Each thread has two successful loop iterations, so for each thread `x` is incremented 2 + 1 = 3 times.  


 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1000 mov 2000, %ax
    1   1001 add $1, %ax
    2   1002 mov %ax, 2000
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   1000 mov 2000, %ax
    2   1001 add $1, %ax
    3   1002 mov %ax, 2000
    3   1003 sub  $1, %bx
    3   1004 test $0, %bx
    3   1005 jgt .top
    3   1006 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----  
    3                            1000 mov 2000, %ax
    3                            1001 add $1, %ax
    4                            1002 mov %ax, 2000
    4                            1003 sub  $1, %bx
    4                            1004 test $0, %bx
    4                            1005 jgt .top
    4                            1000 mov 2000, %ax
    4                            1001 add $1, %ax
    5                            1002 mov %ax, 2000
    5                            1003 sub  $1, %bx
    5                            1004 test $0, %bx
    5                            1005 jgt .top
    5                            1000 mov 2000, %ax
    5                            1001 add $1, %ax
    6                            1002 mov %ax, 2000
    6                            1003 sub  $1, %bx
    6                            1004 test $0, %bx
    6                            1005 jgt .top
    6                            1006 halt

---

### Question 6

Now run with random interrupt intervals: `python3 x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`

Then change the random seed, setting `-s 1`, then `-s 2`, etc. Can you tell, just by looking at the thread interleaving, what the final value of `x` will be? Does the exact location of the interrupt matter? Where can it safely occur? Where does an interrupt cause trouble? In other words, where is the critical section exactly?

`python3 x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`
- Here, the final value of `x` is 2. 
- The exact location of the interrupt *does* matter. The critical section is     
    0   XXXX mov 2000, %ax
    0   XXXX add $1, %ax
    1   XXXX mov %ax, 2000
    if this section is interrupted while it's being executed by a given thread (provided that both threads are still executing and the interrupt context switches from the one thread to the other), then the final value of `x` will be affected. Let's now see how it affects the following examples...

 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2                            1003 sub  $1, %bx
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1006 halt
- Here, the critical section is not interrupted while it's being executed by a given thread, so the final value of `x` (as mentioned above) is 2.

`python3 x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1`
- Here, the final value of `x` is 1. 
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov 2000, %ax
    0                            1001 add $1, %ax
    1                            1002 mov %ax, 2000
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1004 test $0, %bx
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1005 jgt .top
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1006 halt
- Here, the critical section is interrupted while it's being executed by Thread A, so the final value of `x` (as mentioned above) is 1.

`python3 x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2`
- Here, the final value of `x` is 2. 

 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2                            1003 sub  $1, %bx
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1004 test $0, %bx
    2   ------ Interrupt ------  ------ Interrupt ------  
    2                            1004 test $0, %bx
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1005 jgt .top
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1005 jgt .top
    2                            1006 halt
Here, the critical section is not interrupted while it's being executed by a given thread, so the final value of `x` (as mentioned above) is 2.

---

### Question 7

Now use a fixed interrupt interval to explore the program further: `python3 x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`

See if you can guess what the final value of the shared variable `x` will be. What about when you change `-i 2`, `-i 3`, etc.? For which interrupt intervals does the program give the "correct" final answer?

`python3 x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`
- The critical section is     
    0   XXXX mov 2000, %ax
    0   XXXX add $1, %ax
    1   XXXX mov %ax, 2000
- Since the critical section is interrupted while it's being executed by Thread 0, the final value of `x` will be 1.
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov 2000, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0   1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1004 test $0, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1006 halt

`python3 x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 2`
- Again, since the critical section is interrupted while it's being executed by Thread 0, the final value of `x` will be 1.
 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    0                            1000 mov 2000, %ax
    0                            1001 add $1, %ax
    0   ------ Interrupt ------  ------ Interrupt ------  
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1002 mov %ax, 2000
    1                            1003 sub  $1, %bx
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1004 test $0, %bx
    1                            1005 jgt .top
    1   ------ Interrupt ------  ------ Interrupt ------  
    1   1006 halt
    1   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1                            1006 halt

`python3 x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 3`
- Since the critical section is not interrupted while it's being executed by either thread, the final value of `x` will be 2. Hence, for the `-i 3` interrupt interval, the program gives the "correct" final answer.

 2000          Thread 0                Thread 1         
    0   
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   ------ Interrupt ------  ------ Interrupt ------  
    1                            1000 mov 2000, %ax
    1                            1001 add $1, %ax
    2                            1002 mov %ax, 2000
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2                            1003 sub  $1, %bx
    2                            1004 test $0, %bx
    2                            1005 jgt .top
    2   ------ Interrupt ------  ------ Interrupt ------  
    2   1006 halt
    2   ----- Halt;Switch -----  ----- Halt;Switch -----  
    2                            1006 halt

---

### Question 8

Now run the same code for more loops (e.g., set `-a bx=100`). What interrupt intervals, set with the `-i` flag, lead to a "correct" outcome? Which intervals lead to surprising results?
- Here's my guess: since the program begins with the critical section, and the critical section is 3 lines long, and we have a fixed interrupt interval without random (`-r`) interrupts, then so long as 3 modulo (the value of `-i`) == 0, then we will be left with the "correct" outcome. Moreover, if this modulo condition is satisfied, then `bx=y` will always lead to a value of 2 * y for `x`.
- Let's now test this hypothesis on different values of `-i`:
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 1 -c` 
        - Here, `-i` is set to 1, and 3 modulo (1) == 3, so this interval will lead to a surprising result (i.e., a value that isn't 2 * 100, where 100 comes from `bx=100`)
        - The final value of `x` is 100, and (as expected) is not 200. 
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 2 -c` 
        - Here, `-i` is set to 2, and 3 modulo (2) == 1, so this interval will lead to a surprising result
        - The final value of `x` is 100, and (as expected) is not 200.
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 3 -c`  
        - Here, `-i` is set to 3, and 3 modulo (3) == 0, so this interval will lead to a *correct outcome*
        - The final value of `x` (as expected) is 200.
- Let's now see if this hypothesis holds for other values of `-i x`, where 4 <= x <= 10.
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 4 -c` 
        - The final value of `x` is 150.
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 5 -c` 
        - The final value of `x` is 160.
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 6 -c`
        - The final value of `x` is 200. 
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 7 -c` 
        - The final value of `x` is 157.
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 8 -c` 
        - The final value of `x` is 150.
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 9 -c` 
        - The final value of `x` is 200.
    - `python3 x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 10 -c` 
        - The final value of `x` is 160.
- Hence, our hypothesis looks correct! 

---

### Question 9

We'll examine one last program in this homework (`wait-for-me.s`). Run the code like this: `python3 x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000`

This sets the `%ax` register to 1 for thread 0, and 0 for thread 1, and watches the value of `%ax` and memory location 2000 throughout the run. How should the code behave? How is the value at location 2000 being used by the threads? What will its final value be?

Here are the contents of `wait-for-me.s`:
<!-- 
    .main
    test $1, %ax     # ax should be 1 (signaller) or 0 (waiter)
    je .signaller

    .waiter	
    mov  2000, %cx
    test $1, %cx
    jne .waiter
    halt

    .signaller
    mov  $1, 2000
    halt 
-->

 2000      ax          Thread 0                Thread 1         
    0       1   
    0       1   1000 test $1, %ax
    0       1   1001 je .signaller
    1       1   1006 mov  $1, 2000
    1       1   1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0                            1000 test $1, %ax
    1       0                            1001 je .signaller
    1       0                            1002 mov  2000, %cx
    1       0                            1003 test $1, %cx
    1       0                            1004 jne .waiter
    1       0                            1005 halt

- Here, thread 0 and thread 1 are using memory location 2000 as a communication channel. The value of register `ax` (i.e., 1) for thread 0 is sent to memory location 2000, and is subsequently sent to Thread 1's register `cx`. 

---

### Question 10

Now switch the inputs: `python3 x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000`

How do the threads behave? What is thread 0 doing? How would changing the interrupt interval (e.g., `-i 1000`, or perhaps to use random intervals) change the trace outcome? Is the program efficiently using the CPU?

<!-- 
    .main
    test $1, %ax     # ax should be 1 (signaller) or 0 (waiter)
    je .signaller

    .waiter	
    mov  2000, %cx
    test $1, %cx
    jne .waiter
    halt

    .signaller
    mov  $1, 2000
    halt 
-->

`python3 x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000`
 2000      ax          Thread 0                Thread 1         
    0       0   
    0       0   1000 test $1, %ax
    0       0   1001 je .signaller
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       0   1002 mov  2000, %cx
    0       0   1003 test $1, %cx
    0       0   1004 jne .waiter
    0       1   ------ Interrupt ------  ------ Interrupt ------  
    0       1                            1000 test $1, %ax
    0       1                            1001 je .signaller
    1       1                            1006 mov  $1, 2000
    1       1                            1007 halt
    1       0   ----- Halt;Switch -----  ----- Halt;Switch -----  
    1       0   1002 mov  2000, %cx
    1       0   1003 test $1, %cx
    1       0   1004 jne .waiter    1       0   1005 halt


- Thread 0 is caught in a loop initially because its `ax` register is set to 0 and its `cx` register is also set to 0. However, once an interrupt occurs, thread 1 sends its `ax` register value (1) to memory location 2000, which thread 0 subsequently makes use of to set its `cx` register to 1 and halt. 
- Changing the interrupt interval to `-i 1000` makes the outcome worse, as we are caught in the loop of thread 0 for a longer span of time. 
- The CPU is not efficiently using the CPU, because even for moderate values of `-i` we are caught initially in thread 0's loop, making no progress on thread 1. 