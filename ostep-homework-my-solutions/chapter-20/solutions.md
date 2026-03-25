# Homework: Multi-Level Paging

<!-- Run the program with the -h flag to see what options there are: -->

<!-- Options:
-s SEED, --seed=SEED       the random seed
-n NUM, --addresses=NUM    number of virtual addresses to generate
-c, --solve                compute answers for me -->

---

## Questions

### Question 1

With a linear page table, you need a single register to locate the page table, assuming that hardware does the lookup upon a TLB miss. How many registers do you need to locate a two-level page table? A three-level table?
    - I'm guessing the answer is just one? We need only know where to start the page table walk. But instead of a Page Table Base Register (PTBR) for a single-level table, we need a Page Directory Base Register (PDBR) for a multi-level table. 
---

### Question 2

Use the simulator to perform translations given random seeds 0, 1, and 2, and check your answers using the `-c` flag. How many memory references are needed to perform each lookup?
    - Before doing these translations, it's worth noting that yesterday I learned the total number of memory references required for an n-level page table is n + 1 (including the memory reference required to get the value stored at the actual physical address). Hence, if by "lookup" we just mean getting the address of a page table's page (from a page directory) or a PFN (from a page table), then for an n-level page table we require only n memory references. 
    <!-- 
        - NOTES: 
        - Page size = 32 bytes
        - VAS size = 1024 pages, or 1024 * 32 bytes = 32768 bytes = 32 KB (since 32768 / 1024 = 32)
        - Physical memory = 128 pages, or 128 * 32 = 4096 bytes = 4 KB (since 4096 / 1024 = 4)
        - **Virtual address** (15 bits, since log2(32768) = 15)
            - 5 for the offset, since log2(32) = 5
            - 10 for the VPN
        - **Physical address** (12 bits, since log2(4096) = 12)
            - 5 for the offset, since log2(32) = 5
            - 7 for the PFN 
        - The format of a PTE is thus:
            - VALID | PFN6 ... PFN0
            and is thus 8 bits or 1 byte.
        - The format of a PDE is essentially identical:
            - VALID | PT6 ... PT0
    -->
    - `python3  paging-multilevel-translate.py -s 0`
        <!-- PDBR: 108  (decimal) [This means the page directory is held in this page] -->
        **Virtual Address 611c: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 611c = 0110 0001 0001 1100 
                - VPN (page directory bits) = 11000 = 24 (decimal)
                - VPN (page table bits)     = 01000 = 8 (decimal)
                - Offset = 11100 
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[24] = a1 = 1 0100001
                - 1 = valid
                - 0100001 = 33 (decimal)
            - So page table starts at page 33
            <!-- Page Table Lookup -->
            - page33: `7f 7f 7f 7f 7f 7f 7f 7f b5 7f 9d 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f f6 b1 7f 7f 7f 7f`
            - Page33[8] = b5 = 1 0110101 
                - 1 = valid
                - 0110101 = 53 (decimal)
            <!-- Physical address -->
            - Physical address = 0110101 11100 
                - 11100 = 2^4 + 2^3 + 2^2 = 28
            - page  53: `0f 0c 18 09 0e 12 1c 0f 08 17 13 07 1c 1e 19 1b 09 16 1b 15 0e 03 0d 12 1c 1d 0e 1a 08 18 11 00`
            <!-- Fetch -->
            - Fetched value: 08 = 0000 1000 = 8 (decimal)
            - Answer: Physical address: 0110101 11100; Fetched Value: 8
        **Virtual Address 3da8: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 3da8 = 0011 1101 1010 1000
                - VPN (page directory bits) = 01111 = 15 (decimal)
                - VPN (page table bits)     = 01101 = 13 (decimal)
                - Offset = 01000
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[15] = 90 = 1 0010000
                - 1 = valid
                - 0010000 = 16 (decimal)
            - So page table starts at page 16
            <!-- Page Table Lookup -->
            - page16: `7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f ea 7f 7f 7f`
            - Page16[13] = 7f = 0 1111111
                - 0 = invalid
            - Answer: Page table entry not valid
        **Virtual Address 17f5: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 17f5 = 0001 0111 1111 0101
                - VPN (page directory bits) = 00101 = 5 (decimal)
                - VPN (page table bits)     = 11111 = 31 (decimal)
                - Offset = 10101
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[5] = d4 = 1 1010100
                - 1 = valid
                - 1010100 = 84 (decimal)
            - So page table starts at page 84
            <!-- Page Table Lookup -->
            - page 84: `7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 94 7f 7f 7f 7f 7f ce`
            - Page84[31] = ce = 1 1001110
                - 1 = valid
                - 1001110 = 78 (decimal)
            <!-- Physical address -->
            - Physical address = 1001110 10101
                - 10101 = 2^4 + 2^2 + 2^0 = 21
            - page 78: `0e 02 17 1b 1c 1a 1b 1c 10 0c 15 08 19 1a 1b 12 1d 11 0d 14 1e 1c 18 02 12 0f 13 1a 07 16 03 06`
            <!-- Fetch -->
            - Fetched value: page78[21] = 1c = 0001 1100 = 28 (decimal)
            - Answer: Physical address: 1001110 10101; Fetched Value: 28
        **Virtual Address 7f6c: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 7f6c = 0111 1111 0110 1100
                - VPN (page directory bits) = 11111 = 31 (decimal)
                - VPN (page table bits)     = 11011 = 27 (decimal)
                - Offset = 01100
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[31] = ff = 1 1111111
                - 1 = valid
                - 1111111 = 127 (decimal)
            - So page table starts at page 127
            <!-- Page Table Lookup -->
            - page 127: `7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f df 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 95 7f 7f`
            - Page127[27] = 7f = 0 1111111
                - 0 = invalid
            - Answer: Page table entry not valid 
        **Virtual Address 0bad: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 0bad = 0000 1011 1010 1101
                - VPN (page directory bits) = 00010 = 2 (decimal)
                - VPN (page table bits)     = 11101 = 29 (decimal)
                - Offset = 01101
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[2] = e0 = 1 1100000
                - 1 = valid
                - 1100000 = 96 (decimal)
            - So page table starts at page 96
            <!-- Page Table Lookup -->
            - page 96: `7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f b6 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f`
            - Page96[29] = 7f = 0 1111111
                - 0 = invalid
            - Answer: Page table entry not valid 
        **Virtual Address 6d60: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 6d60 = 0110 1101 0110 0000
                - VPN (page directory bits) = 11011 = 27 (decimal)
                - VPN (page table bits)     = 01011 = 11 (decimal)
                - Offset = 00000
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[27] = c2 = 1 1000010
                - 1 = valid
                - 1000010 = 66 (decimal)
            - So page table starts at page 66
            <!-- Page Table Lookup -->
            - page 66: `7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f d7 7f 7f`
            - Page66[11] = 7f = 0 1111111
                - 0 = invalid
            - Answer: Page table entry not valid
        **Virtual Address 2a5b: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 2a5b = 0010 1010 0101 1011
                - VPN (page directory bits) = 01010 = 10 (decimal)
                - VPN (page table bits)     = 10001 = 17 (decimal)
                - Offset = 11011
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[10] = d5 = 1 1010101
                - 1 = valid
                - 1010101 = 85 (decimal)
            - So page table starts at page 85
            <!-- Page Table Lookup -->
            - page 85: `7f 7f 7f 7f 7f 7f 7f 7f 9a 7f bf 7f 7f 7f 7f 7f 7f 7f 7f 7f af 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f`
            - Page85[17] = 7f = 0 1111111
                - 0 = invalid
            - Answer: Page table entry not valid
        **Virtual Address 4c5e: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 4c5e = 0100 1100 0101 1110
                - VPN (page directory bits) = 10011 = 19 (decimal)
                - VPN (page table bits)     = 00010 = 2 (decimal)
                - Offset = 11110
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[19] = f8 = 1 1111000
                - 1 = valid
                - 1111000 = 120 (decimal)
            - So page table starts at page 120
            <!-- Page Table Lookup -->
            - page 120: `7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f cb 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f`
            - Page120[2] = 7f = 0 1111111
                - 0 = invalid
            - Answer: Page table entry not valid 
        **Virtual Address 2592: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 2592 = 0010 0101 1001 0010
                - VPN (page directory bits) = 01001 = 9 (decimal)
                - VPN (page table bits)     = 01100 = 12 (decimal)
                - Offset = 10010
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[9] = 9e = 1 0011110
                - 1 = valid
                - 0011110 = 30 (decimal)
            - So page table starts at page 30
            <!-- Page Table Lookup -->
            - page 30: `7f 7f 7f 7f 7f 84 7f 7f 7f 7f 97 7f bd 7f 7f f4 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 9c 7f 7f`
            - Page30[12] = bd = 1 0111101
                - 1 = valid
                - 0111101 = 61 (decimal)
            <!-- Physical address -->
            - Physical address = 0111101 10010
                - 10010 = 2^4 + 2^1 = 18
            - page 61: `01 05 10 02 0c 0a 0c 03 1c 0e 1a 1e 0a 0e 15 0d 09 16 1b 1c 13 0b 1e 13 02 02 17 01 00 0c 10 0d`
            <!-- Fetch -->
            - Fetched value: page61[18] = 1b = 0001 1011 = 27 (decimal)
            - Answer: Physical address: 0111101 10010; Fetched Value: 27
        **Virtual Address 3e99: Translates To What Physical Address (And Fetches what Value)? Or Fault?**
            <!-- Parse virtual address -->
            - 3e99 = 0011 1110 1001 1001
                - VPN (page directory bits) = 01111 = 15 (decimal)
                - VPN (page table bits)     = 10100 = 20 (decimal)
                - Offset = 11001
            <!-- Page Directory Lookup -->
            - page 108: `83 fe e0 da 7f d4 7f eb be 9e d5 ad e4 ac 90 d6 92 d8 c1 f8 9f e1 ed e9 a1 e8 c7 c2 a9 d1 db ff`
            - Page108[15] = d6 = 1 1010110
                - 1 = valid
                - 1010110 = 86 (decimal)
            - So page table starts at page 86
            <!-- Page Table Lookup -->
            - page  86: `7f 7f 7f 7f 7f 7f 7f c5 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f ca 7f 7f ee 7f 7f 7f 7f 7f 7f 7f 7f`
            - Page54[20] = ca = 1 1001010
                - 1 = valid
                - 1001010 = 74 (decimal)
            <!-- Physical address -->
            - Physical address = 1001010 11001
                - 11001 = 2^4 + 2^3 + 2^0 = 25
            - page 25: `03 03 1c 03 1b 0e 0e 0a 0c 0b 11 0a 19 07 07 0e 1c 00 16 00 0c 17 0d 0d 07 0e 07 08 14 12 1c 1e`
            <!-- Fetch -->
            - Fetched value: page25[20] = 0c = 0000 1100
            - Answer: Physical address: 0111101 10010; Fetched Value: 27
    <!-- SKIPPING NEXT TWO SEEDS FOR THIS QUESTION SINCE THEY ARE VERY SIMILAR TO FIRST SEED -->
---

### Question 3

Given your understanding of how cache memory works, how do you think memory references to the page table will behave in the cache? Will they lead to lots of cache hits (and thus fast accesses?) Or lots of misses (and thus slow accesses)?
    <!-- - Read this question as: "When the CPU needs to walk the page table, will those memory accesses likely be in cache or not? Why?" -->
    - In a multi-level page table, we know that page directories and page tables are spread across many pages in physical memory. Hence, our program's memory references will "jump around" across many different physical pages before a physical address is found. For this reason, when the CPU needs to walk the page table, the memory accesses will likely not be in the cache; i.e., there will many cache misses (and therefore slow accesses).