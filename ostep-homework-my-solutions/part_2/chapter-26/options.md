<!-- The full set of instructions simulated are:

mov immediate, register     # moves immediate value to register
mov memory, register        # loads from memory into register
mov register, register      # moves value from one register to other
mov register, memory        # stores register contents in memory
mov immediate, memory       # stores immediate value in memory

add immediate, register     # register  = register  + immediate
add register1, register2    # register2 = register2 + register1
sub immediate, register     # register  = register  - immediate
sub register1, register2    # register2 = register2 - register1

test immediate, register    # compare immediate and register (set condition codes)
test register, immediate    # same but register and immediate
test register, register     # same but register and register

jne                         # jump if test'd values are not equal
je                          #                       ... equal
jlt                         #     ... second is less than first
jlte                        #               ... less than or equal
jgt                         #            ... is greater than
jgte                        #               ... greater than or equal

xchg register, memory       # atomic exchange: 
                            #   put value of register into memory
                            #   return old contents of memory into reg
                            # do both things atomically

nop                         # no op

Notes:
'immediate' is something of the form $number
'memory' is of the form 'number' or '(reg)' or 'number(reg)' or 'number(reg,reg)' (as described above)
'register' is one of %ax, %bx, %cx, %dx
Finally, here are the full set of options to the simulator are available with the -h flag:

Usage: x86.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -t NUMTHREADS, --threads=NUMTHREADS
                        number of threads
  -p PROGFILE, --program=PROGFILE
                        source program (in .s)
  -i INTFREQ, --interrupt=INTFREQ
                        interrupt frequency
  -r, --randints        if interrupts are random
  -a ARGV, --argv=ARGV  comma-separated per-thread args (e.g., ax=1,ax=2 sets
                        thread 0 ax reg to 1 and thread 1 ax reg to 2);
                        specify multiple regs per thread via colon-separated
                        list (e.g., ax=1:bx=2,cx=3 sets thread 0 ax and bx and
                        just cx for thread 1)
  -L LOADADDR, --loadaddr=LOADADDR
                        address where to load code
  -m MEMSIZE, --memsize=MEMSIZE
                        size of address space (KB)
  -M MEMTRACE, --memtrace=MEMTRACE
                        comma-separated list of addrs to trace (e.g.,
                        20000,20001)
  -R REGTRACE, --regtrace=REGTRACE
                        comma-separated list of regs to trace (e.g.,
                        ax,bx,cx,dx)
  -C, --cctrace         should we trace condition codes
  -S, --printstats      print some extra stats
  -c, --compute         compute answers for me
Most are obvious. Usage of -r turns on a random interrupter (from 1 to intfreq as specified by -i), which can make for more fun during homework problems.

-L specifies where in the address space to load the code.
-m specified the size of the address space (in KB).
-S prints some extra stats
-c is not really used (unlike most simulators in the book); use the tracing or condition codes. -->