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

<!-- Chapter 16: SEGMENTATION -->
- offset = virtual address - where segment starts in virtual memory
- physical address = base + offset
- "The hardware uses the top two bits (11) to designate the segment, but then we are left with an offset of 3KB."
    - Here's the math:
        - 1100 0000 0000 = 1×2^11 + 1×2^10 + 0×2^9 + ... + 0×2^0 = 3072 bytes = 3 KB
        - (Here, we assume that 1 KB = 1024 bytes)
- A bit string of all 1's of length n is a binary number that converts to the decimal value 2ⁿ - 1. 
    - For example, a bit string of all 1's and length 4 ("1111") has a value of 2⁴ - 1 = 16 - 1 = 15.
- if (segment grows positive):
    physical address = base + offset        // forward translation
- else:
    negative offset = offset - max segment size
    physical address = base + negative offset  // backward translation
        - Note that the base is always the start of the segment; so for a stack it would
        be at the "end" of its segment; as opposed to the "beginning" for the other segments
- It's possible to have one physical segment in memory mapped to multiple virtual address spaces.
- With respect to memory allocation algorithms: "The fact that so many different algorithms exist to try to minimize external fragmentation is indicative of a stronger underlying truth: there is no one “best” way to solve the problem."
- The term "16-byte address space" means the address space contains 16 bytes — i.e., there are 16 addresses, each holding 1 byte. It's describing the capacity of the space, not the "width" of the addresses themselves.
- Recall that virtual address spaces (VAS's) and virtual addresses (VA's) are different concepts!
    - A virtual address space (VAS) is a contiguous range of memory addresses that the operating system assigns to a process, allowing it to act as if it has a large, private, and contiguous memory space. 

<!-- Chapter 17: FREE-SPACE MANAGEMENT -->
- A pointer in C is a variable that stores the memory address of another variable, function, or other object in the computer's memory
- External fragmentation vs. internal fragmentation:
    - External fragmentation: Waste is in the gaps between allocations (free but unusable)
    - Internal fragmentation: Waste is within allocations (allocated but unused)
- Worst-fit distributes the 'pain' of fragmentation across large blocks, keeping leftovers medium-sized, rather than best-fit which creates many unusably small fragments.

<!-- Chapter 18: PAGING: INTRODUCTION -->
- If the virtual address space of the process is 64 bytes, we need 6 bits total for our virtual address 
    - Since log2(64) = 6
- If the virtual address space of the process is 4 GB, we need 32 bits total for our virtual address 
    - Since 4GB = 4,294,967,296 bytes, and log2(4,294,967,296) = 32
    - More concisely, if the virtual address space of the process is 4 GB = 4,294,967,296 bytes, we need log2(4,294,967,296) = 32 bits total for our virtual address.
- *** A pointer is a programming variable that stores a memory address (typically a virtual one), while a virtual address is a logical address within a process's virtual address space ***
- Good-to-know general formulas:
    - Offset bits = log₂(page size in bytes)
    - VPN bits = (total address bits) - (offset bits)
- *** Page Table: Virtual Page Number --> Physical Frame Number ***

**When executing `movl 21, %eax`, there are two reads from memory:**
- Read #1: Instruction Fetch
    - What: Read the instruction bytes movl 21, %eax from memory
    - Where from: Whatever virtual address the Program Counter is pointing to (e.g., address 1000)
    - Translation needed: Virtual address 1000 → some physical address
- Read #2: Explicit Load
    - What: Read the data value stored at address 21
    - Where from: Virtual address 21
    - Translation needed: Virtual address 21 → physical address 117 (in the example)

So the full process is:
    - Instruction Fetch: CPU reads `movl 21, %eax` from memory (1st memory access)
    - Decode: CPU figures out what the instruction means
    - Execute (Explicit load, in this case): CPU reads the data from address 21 (2nd memory access)
    - Write: CPU puts that data into register eax
    - Both memory accesses require virtual-to-physical translation (and thus page table lookups), which is why the author was saying page tables can slow things down so much!
- Assembly code almost exclusively uses virtual addresses when referring to memory in modern operating systems. These addresses are mapped by the CPU's Memory Management Unit (MMU) to actual physical addresses at runtime.
- Based on the standard binary conversion where 1KB = 1024 bytes, 64 KB is equal to 65,536 bytes 
- On page 177, note that:
    - VPN 0: 0 to 1023 bytes
    - VPN 1: 1024 to 2047 bytes
    - VPN 2: 2048 to 3071 bytes
    - ...
    - More generally, VPN X: (X * 1024) - (X * 1024 + 1023). Hence,
    - VPN 39: 39936 - 40959
    - VPN 44: 43008 - 44031
        - These last two ranges thus show why the virtual pages for virtual addresses
          40000 to 44000 are VPN=39 ... VPN=44
- On page 178, the answer to the question "See if you can make sense of the patterns that show up in this visualization. In particular, what will change as the loop continues to run beyond these first five iterations? Which new memory locations will be accessed? Can you figure it out?" is ...
    - What stays the same as the loop continues:
            - Instructions (addresses 0x1024, 0x1028, 0x102c, 0x1030):
            - All 4 instructions remain on VPN 1 → PFN 4
            - The page table entry for VPN 1 keeps getting accessed repeatedly
            - The instruction physical addresses keep getting accessed repeatedly

    - What changes as the loop continues:
        - Array accesses:
            - Iteration 0: array[0] at virtual address 40000 (VPN 39 → PFN 7)
            - Iteration 1: array[1] at virtual address 40004 (VPN 39 → PFN 7)
            - Iteration 2: array[2] at virtual address 40008 (VPN 39 → PFN 7)
            ...
            - Iteration 255: array[255] at virtual address 41020 (VPN 40 → PFN 8) <- new page!

<!-- Chapter 19: Paging: Faster Translations (TLBs) -->
- The CPU generates the virtual address (also known as a logical address) while a program is running. The CPU then splits this virtual address into two parts: the Virtual Page Number (VPN) and the page offset.
- Q: How does the CPU choose the correct page table for a given process?
    - A:  Processors have a privileged register called the page table base register (PTBR), on x86 it is CR3. On a context switch, the OS changes the value of the PTBR so that the processor now knows which page table to use. In addition to the PTBR, many modern processors have a notion of an address space number (ASN). Processes are given an address space number (from a limited pool) and this ASN is set in a register on a context switch as well. This ASN is used as part of TLB matching and allows TLB entries from multiple address spaces to coexist. Only when an ASN is reused is it necessary to flush the TLB, and then only for entries matching that ASN. Most x86 implementations are more coarse grained than this and there is a notion of global pages (for shared libraries and shared data).
    - See the following StackOverflow link for more info: https://stackoverflow.com/questions/10880555/how-does-the-system-choose-the-right-page-table
- Instruction fetch vs. Explicit load/store
    - `movl 21, %eax`; Load data from address 21 into eax
    - Two memory accesses happen:
        - Instruction fetch (implicit): CPU reads the `movl 21, %eax` instruction from wherever it's stored in memory
        - Explicit load (explicit): CPU reads the data value from address 21 (because that's what the instruction tells it to do)
            - Side note:
                - Load = Read from memory
                - Store = Write to memory

- When the CPU switches to a "new" process (i.e., one that that the CPU has never seen before), the OS creates or loads a unique page table by allocating memory for it in the kernel space, mapping the process's virtual memory to physical frames, and updating the CPU's Page Table Base Register (e.g., CR3 on x86) to point to this new table.
- Segmentation fault -- Is memory valid?
- Protection fault -- Is memory accessible?
- In almost all modern computer architectures (x86, x64, ARM), virtual addresses are byte-addressable, meaning each unique address points to exactly 1 byte (8 bits) of data. While the pointer itself is 4 or 8 bytes long, the memory location it references holds 1 byte.
    - See more info here: https://stackoverflow.com/questions/37540853/does-memory-address-always-refer-to-one-byte-not-one-bit#:~:text=EX:%20In%20ARM%20cortex%20m4,int%2C%20string%2C%20..)
- The hardware saves a different PC depending on the trap type:
    - System call: Save PC (address of next instruction)
    - TLB miss/page fault: Save PC (address of current instruction; i.e., the one that faulted)
- A handler is just a function/routine in the OS that gets called when something specific happens
- When a TLB miss occurs, the OS needs to run code to handle it (look up the page table, load the correct translation into the TLB, etc.). But that handler code itself lives in memory, which means accessing it also requires address translation. If the handler code's translation isn't in the TLB, you get another TLB miss while trying to handle the first TLB miss, which triggers the handler again, causing another miss... infinite loop!
    - The Solutions:
        - Keep handlers in physical memory (unmapped): The handler code runs using physical addresses directly, bypassing the TLB entirely. This means no translation is needed, so no TLB lookup happens, so no miss can occur.
        - "Wire" TLB entries: Reserve some TLB slots that are "permanent" - they never get evicted. Load the handler code's translations into these wired entries at boot time. Now when the OS jumps to the handler, those translations are always in the TLB, guaranteed to hit, so no miss occurs.
- The operating system generally creates a page table (or a set of page tables) for a process immediately upon its creation to manage its virtual address space.
- The program counter (PC) is not strictly limited to the data or code segments. While it primarily tracks instructions in the code segment, it can point to any executable region within the entire virtual address space.
- The CPU continuously changes its own register values millions of times per second as it executes instructions (e.g., loading data, arithmetic operations). While the OS does change registers during context switches (saving/loading process states), the CPU itself modifies them constantly during normal operation.
- Imagine a carpenter at work. He has a few items in his hands (registers) and then, very close by on his workbench (cache) things he is frequently working on, but not using right this moment, and then in the workshop (main memory) things that pertain to the project at hand but that are not immediately important enough to be on the workbench.
- Every address within the same page shares the same TLB entry. (This makes sense of course -- VPN literally stands for Virtual Page Number, and each TLB entry only has one VPN and one PFN, in addition to other bits as well.) That's why TLBs are effective —- accessing nearby memory (spatial locality) doesn't thrash the TLB, since those addresses likely fall within the same page

<!-- Chapter 20: Paging: Smaller Tables -->
- Internal fragmentation -- waste is internal to the unit of allocation IN PHYSICAL MEMORY
- External fragmentation -- waste is external to the unit of allocation IN PHYSICAL MEMORY
- How it works in the hybrid approach:
    - Instead of:
        - Base register → start of segment in physical memory
        - Bounds register → size of segment
    - You have:
        - Base register → physical address of the segment's page table
        - Bounds register → number of valid pages in that segment's page table 
- Accessing memory outside a logical segment (or the allocated address space) is a classic cause of a segmentation fault.
- "A page of page-table entries"
    - The page table itself is just data sitting in memory. That data can be organized into pages, just like any other data.
    - Example:
        - Page size = 4 KB
        - Each PTE = 4 bytes
        - One page can hold: 4096 / 4 = 1024 PTEs
    - So "a page of PTEs" means: 1024 consecutive page table entries that happen to fit in one 4 KB page of memory.
- The page directory either can be used to tell you where a page of the page table is, or that the entire page of the page table contains no valid pages.
- For an N-level page table:
    - Total memory accesses = N + 1
        Where:
        N accesses = for walking through all N levels of the page table hierarchy
        +1 access = for fetching the actual data from physical memory

<!-- Chapter 22: BEYOND PHYSICAL MEMORY: POLICIES -->
- The operating system (OS) creates page tables primarily during process creation (e.g., when fork() is called in Unix-like systems). However, modern operating systems often use lazy allocation (demand paging) to optimize memory, meaning the full page table structure might not be created until the process actually accesses a memory page for the first time, triggering a page fault. 
    - Here is the breakdown of when page tables are created and updated:
        - Process Initiation: When a new process is created, the kernel creates a new mm_struct (in Linux) and allocates the top-level page table structure (e.g., Page Global Directory).
        - During exec(): When a binary is executed, the OS maps segments (code, data, stack) and sets up the initial memory mappings, which includes preparing the page table entries (PTEs).
        - On Demand (Page Faults): In modern OSs, the initial page table entries are often marked as "invalid" or "not present." When the program tries to read/write this memory, it triggers a "page fault." The OS page fault handler then allocates a physical page frame and updates the page table at that moment.
        - Kernel Bootstrapping: A specialized, separate page table is created by the OS during the initial booting process to map kernel memory.
        - Context Switch: While not the creation step, the OS must tell the Memory Management Unit (MMU) where the current process's page table is located in physical memory by updating a dedicated register (e.g., CR3 on x86). 
- In summary, the skeleton is created during process startup, but the actual mappings are usually added lazily during page faults. 
- Virtual addresses are embedded in a program's compiled binary. **During compilation and linking**, the binary is built assuming a specific virtual address layout, and instructions reference code and data using these virtual addresses. When the program runs, the **operating system** creates the actual virtual address space for the process and sets up page tables. The **MMU hardware** then translates virtual addresses to physical addresses, often lazily allocating physical memory on first access (demand paging).
    - I.e., the virtual address space is created and mapped to physical memory by the operating system at runtime when a process is started or during its execution. While the executable binary (compiled code) defines the structure, the OS creates the mapping and allocates virtual memory dynamically, allowing each process to have its own private, isolated address space
    - The main takeaway here is that virtual addresses are assigned during **compile time**, while mappings (VA -> PA) are assigned during **run time**! 

<!-- Chapter 25: THE VAX/VMS VIRTUAL MEMORY SYSTEM -->
- A NULL pointer stores the virtual memory address 0. (More precisely: it stores the value 0, which is interpreted as virtual memory address 0 when you dereference it.) When you dereference it, the CPU tries to access virtual memory address 0, which causes a segfault because that page is unmapped.
- The working set of a program is the set of memory pages (portions of code and data) currently loaded into physical RAM that a process is actively and frequently using.


<!-- Chapter 26: Concurrency: An Introduction -->
- Shared Between Threads
    Code (text segment)
    Heap
    Global/static data
    Open files
- NOT Shared (Per-Thread)
    Stack (each thread has its own stack)
    Registers (including program counter, stack pointer)
- The fetch-decode-execute cycle is the fundamental 3-step operation of a CPU, running continuously from boot-up to shutdown to process instructions. It fetches instructions from memory, decodes what action is needed, and executes that action, repeating this process billions of times per second. 
- Concurrency vs. Parallelism
    - Concurrency: Appears simultaneous (at least from the perspective of say a process or a thread)
    - Parallelism: Actually simultaneous

<!-- Chapter 27: Interlude: Thread API -->
- A pointer to a __ is a variable that holds the virtual address of another variable of type __. 
    - Here, __ could be any data type; e.g., int, char, float, etc.
- pthread_join needs to write a pointer into your variable. To write into a variable, you need its address.
    myret_t *m;                    // m is a pointer
    Pthread_join(p, (void **) &m); // Pass address of m so pthread_join can write into it

    m has type myret_t*
    &m has type myret_t** (address of a pointer)
    Cast to void** to match pthread_join's signature

    Analogy:
    void set_int(int *ptr) { *ptr = 42; }
    int x;
    set_int(&x);  // Pass &x so function can write into x
- int x = 100;        // Store 100 in an int
- void *p = (void *) 100;  // Store 100 in a pointer
- Dot vs Arrow
    - It depends on whether you have a struct or a pointer to a struct:
        myret_t r;     // r is a STRUCT
        r.x = 1;       // Use DOT (.)

        myret_t *r;    // r is a POINTER to a struct  
        r->x = 1;      // Use ARROW (->)
- The authors of OSTEP typically initialize a lock using the dynamic way (i.e., at run time), which involves making a call to pthread mutex init(), as follows:
    - int rc = pthread_mutex_init(&lock, NULL); assert(rc == 0); // always check success!
        - Note that a corresponding call to pthread cond destroy() should also be made, when you are done with the lock; see the manual page for all of details. 
- // Use this to keep your code clean but check for failures 
  // Only use if exiting program is OK upon failure
    void Pthread_mutex_lock(pthread_mutex_t *mutex) {
        int rc = pthread_mutex_lock(mutex);
        assert(rc == 0);
    }
- Synchronization primitives are programming tools used to coordinate concurrent threads or processes, preventing data races and ensuring orderly access to shared resources. Key types include mutexes (mutual exclusion), semaphores, condition variables, spinlocks, and atomic operations. They prevent unpredictable behavior by managing thread execution order and signaling.
- A condition variable is a synchronization primitive that allows threads to wait until a specific condition becomes true, efficiently putting them to sleep without consuming CPU resources. 
- To compile them, you must include the header pthread.h in your code. On the link line, you must also explicitly link with the pthreads library, by adding the -pthread flag.
- To compile a simple multi-threaded program, include the header pthread.h in your code, and include the `-pthread` flag to your command:
    - `gcc -o main main.c -Wall -pthread`