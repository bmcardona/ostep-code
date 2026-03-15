<!-- NOTE: Need to return to Chapter 10: Multiprocessor Scheduling (Advanced) after Part 2!  -->


<!-- Introduction to OSTEP -->

- Three major OS abstractions: CPU virtualization, memory virtualization, and the file system,
- With the CPU, the OS uses virtualization to give each process the illusion of having its own dedicated processor. With memory, each process gets its own private virtual address space, so one program can't accidentally (or maliciously) read another's memory. These abstractions exist largely to isolate processes from each other. 
    - The file system works differently. The OS doesn't give each process its own private virtual disk. Instead, files live in a shared, persistent space that any process with the right permissions can access. The design assumption is that sharing is actually desirable for files — you want programs to be able to pass data to each other.

- Multiprogramming and multithreading both improve CPU efficiency by allowing concurrent execution, but at different granularities. Multiprogramming keeps multiple independent programs (processes) in memory, switching between them to maximize CPU usage. Multithreading allows a single process to break tasks into multiple threads, which share resources to run concurrently, making it better for responsiveness and performance within a single application.

- A thread switch (or thread context switch) moves the CPU from one thread to another within the same process, requiring less overhead because they share memory, code, and resources. A context switch (or process context switch) generally refers to a process-level switch, which is more intensive as it swaps memory mappings, page tables, and flushes caches.

- The overall technique of rapidly switching between processes/threads to create the illusion they're running simultaneously is called time sharing or multitasking.

- The OS uses a timer interrupt to periodically interrupt the running process and decide whether to switch to another one - this is part of CPU scheduling.

- Context switch = the actual switching operation (for both processes and threads)
- Time sharing/multitasking = the overall technique of sharing CPU time
- Scheduling = deciding which process/thread to run next

- A program is generally considered running when it is loaded into main memory (RAM) and its instructions are actively being fetched, decoded, and executed by the CPU. While a program can be in a "ready" or "waiting" state in memory, it is not actively performing tasks (executing) until the CPU cycles are allocated to it.

- Process Control Block (PCB) - a C data structure that holds all the information about a single process (its state, registers, memory, open files, etc.).
- Process List - a collection of all the PCBs — one entry for every process currently on the system. The OS keeps this list so it can keep track of all processes at once.

- Pipes are really just an extension of the same redirection trick that was already discussed — instead of redirecting stdout to a file, you're redirecting it to a pipe that another process is reading from. The underlying mechanism is the same, just connecting two processes together instead of a process and a file.

- The Word "Trap" Is Overloaded; i.e., it's used in a few related but distinct ways in OS literature:
    1. The trap instruction
        - A specific CPU instruction (like int 0x80 on x86) that a program executes to deliberately trigger a switch from user mode to kernel mode. It's a hardware instruction with a precise meaning.
    2. The act of trapping
        - The event of transitioning from user mode to kernel mode. When people say "the process traps into the kernel" they mean the CPU switched from user mode to kernel mode and jumped to a kernel handler. This is the verb form.
    3. A trap as a category of event
        - More broadly, "trap" is sometimes used as an umbrella term for any synchronous event that causes a switch to kernel mode, including:
            - System calls (deliberate)
            - Page faults (program accessed invalid memory)
            - Division by zero (program did something illegal)
- No matter which way the word is used, "trap" always implies the same core thing: User mode code stopped, kernel mode code took over, at a well-defined entry point.
- Kernel handler — a piece of code in the kernel that runs in response to a trap. It's the function that actually handles the work (e.g. the open() handler reads from disk, the timer interrupt handler runs the scheduler). It's found via the trap table.
- Kernel stack — a region of memory dedicated to each process. It's where register state gets saved and where the kernel handler does its stack-based work (local variables, function calls within the kernel, etc.).
- REVISIT TABLE 6.3 (see LDEP.png in z_images directory)
- The process list is a list/array living in kernel memory
    - Each entry in that list is a process struct (PCB)
        - Each process struct (PCB) contains:
            - Saved register state
            - Scheduling information
            - Memory mappings
            - Open file descriptors
            - A pointer to that process's kernel stack
            - etc.
- PCB: Stores the complete state of a suspended process (program counter, registers, stack pointer) for long-term storage.
- KStack (Kernel Stack): Stores temporary register values while the process is still running but has temporarily transitioned to kernel mode.
- When you see "amortize" in systems/CS writing, just read it as: "make this fixed cost worth it by doing enough work to dwarf it."
    - A good mental image: imagine you pay a $5 toll every time you enter a highway. If you drive 1 mile then exit, that toll is enormous relative to your trip. If you drive 100 miles, the toll barely registers. You've amortized the toll cost over a long drive.

<!-- Chapter 8: SCHEDULING: THE MULTI-LEVEL FEEDBACK QUEUE -->
- The circumstances where a low-priority job runs without Rule 5 are actually pretty limited:
    - The high-priority queues happen to be empty (all short/interactive jobs have finished)
    - A high-priority job blocks (e.g. waiting for I/O), temporarily vacating the queue


<!-- Chapter 11: THE ABSTRACTION: ADDRESS SPACES -->
- Note that the heap and the stack are two regions of the address space that may grow (and shrink) while the program runs
- A 2^32-bit address space has a capacity of 2^32 bytes???
- CPU multiplexing → shared over time (one process runs, then another)
- Memory multiplexing → shared over space (each process gets a chunk simultaneously)

<!-- Chapter 14: Interlude: Memory API -->
- Stack memory -- allocations and deallocations are handled implicitly by the compiler
- Heap memory -- allocations and deallocations are handled by you, the programmer
- A pointer is a variable that stores a memory address.
- A pointer is just an address, and all addresses are the same size.
- the malloc() function returns a pointer to the first (lowest-addressed) byte of the successfully allocated virtual memory block.
- A segfault occurs when a process accesses an unmapped part of its own virtual address space. It's not about crossing into another process's memory — the virtual address space is already private to each process. It's about accessing a virtual address that has no real memory backing it.
    -  A segfault happens entirely within a process's own virtual address space — specifically when it touches an unmapped region. The OS maintains a table of which virtual addresses are valid and which aren't, and kills the process when it strays into invalid territory.
    - So the virtual address space is better thought of as mostly empty with a few mapped islands of real memory.
- Anytime a function needs to work with a whole string or array, you pass the pointer — because the pointer is the only thing that tells you where the data starts in memory.
- Never read from a memory location you haven't written to first
    - In practice this means either:
        - Initialize every element before reading it
        - Use calloc if you want all elements zeroed out automatically
- A dangling pointer is a pointer that points to a memory location which has already been deallocated or freed, leaving it invalid. It frequently occurs in C/C++ when an object is deleted but the pointer is not set to NULL.


<!-- Chapter 15: MECHANISM: ADDRESS TRANSLATION -->
- The program counter (which is a type of CPU register) is different from the base register!
- "Base and Bounds" = "Dynamic Relocation"
    - In the simplified memory virtualization at the start of the chapter, we 
      have: physical address = virtual address + base register (32 KB) 
- The **program counter** always holds the **virtual address** of the next instruction to be executed
- The **base register** always holds the starting **physical address** of where the program is loaded in RAM.
- The "dynamic" part of "dynamic relocation" refers to the fact that the address translation happens at runtime rather than being hardcoded ahead of time (i.e., at compile time).
"Dynamic relocation" doesn't mean processes are constantly being moved around in RAM. It just means the capability exists to place or move them anywhere, because the translation is computed live rather than hardcoded. In practice, most context switches involves (among other things) flipping the base register, no copying involved.
- Base register → physical address (where the process starts in physical RAM)
- Bounds register → virtual address (the size/limit of the process's virtual address space)
- Happy to know that other StackOverflow users were just as confused as I was about the difference between address space size and the value of the bounds (limit) register.
    - See a discussion here: https://stackoverflow.com/questions/63276454/what-is-the-difference-between-address-space-size-and-the-bound-when-using-bas
    