# Homework: Paging Introduction

<!-- Run the program with the -h flag to see what options there are: -->

<!-- Options:
-h, --help              show this help message and exit
-s SEED, --seed=SEED    the random seed
-a ASIZE, --asize=ASIZE 
                        address space size (e.g., 16, 64k, ...)
-p PSIZE, --physmem=PSIZE
                        physical memory size (e.g., 16, 64k, ...)
-P PAGESIZE, --pagesize=PAGESIZE
                        page size (e.g., 4k, 8k, ...)
-n NUM, --addresses=NUM number of virtual addresses to generate
-u USED, --used=USED    percent of address space that is used
-v                      verbose mode
-c                      compute answers for me -->

---

## Questions

### Question 1

Before doing any translations, let's use the simulator to study how linear page tables change size given different parameters. Compute the size of linear page tables as different parameters change. Some suggested inputs are below; by using the `-v` flag, you can see how many page-table entries are filled.

- **First**, to understand how linear page table size changes as the address space grows:
  - `python3 paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0`
    - Size of linear page table: 1024
  - `python3 paging-linear-translate.py -P 1k -a 2m -p 512m -v -n 0`
    - Size of linear page table: 2048
  - `python3 paging-linear-translate.py -P 1k -a 4m -p 512m -v -n 0`
    - Size of linear page table: 4096

- **Then**, to understand how linear page table size changes as page size grows:
  - `python3 paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0`
    - Size of linear page table: 1024
  - `python3 paging-linear-translate.py -P 2k -a 1m -p 512m -v -n 0`
    - Size of linear page table: 512
  - `python3 paging-linear-translate.py -P 4k -a 1m -p 512m -v -n 0`
    - Size of linear page table: 256

Before running any of these, try to think about the expected trends:
- How should page-table size change as the address space grows?
    - The page-table size should grow (i.e., it should be directly proportional to the address space size)
- How should page-table size change as the page size grows?
    - The page-table size should shrink (i.e., it should be indirectly proportional to the page size)
- Why shouldn't we just use really big pages in general?
    - Because then small allocations would be allocated more memory than necessary 
        - I think I should have just said "to reduce internal fragmentation"
---

### Question 2

Now let's do some translations. Start with some small examples, and change the number of pages that are allocated to the address space with the `-u` flag. For example:

- `python3 paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 0 -c`
    <!-- Page Table (from entry 0 down to the max size)
    [       0]  0x00000000
    [       1]  0x00000000
    [       2]  0x00000000
    [       3]  0x00000000
    [       4]  0x00000000
    [       5]  0x00000000
    [       6]  0x00000000
    [       7]  0x00000000
    [       8]  0x00000000
    [       9]  0x00000000
    [      10]  0x00000000
    [      11]  0x00000000
    [      12]  0x00000000
    [      13]  0x00000000
    [      14]  0x00000000
    [      15]  0x00000000 -->
    - VA 0x00003a39 (decimal:    14905) --> Invalid address (these should all be invalid since `u` is set to 0)
    - VA 0x00003ee5 (decimal:    16101) --> Invalid address
    - VA 0x000033da (decimal:    13274) --> Invalid address
    - VA 0x000039bd (decimal:    14781) --> Invalid address 
    - VA 0x000013d9 (decimal:     5081) --> Invalid address
- `python3 paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 25 -c`
    <!-- Page Table (from entry 0 down to the max size)
    [       0]  0x80000018 - Valid
    [       1]  0x00000000
    [       2]  0x00000000
    [       3]  0x00000000
    [       4]  0x00000000
    [       5]  0x80000009 - Valid
    [       6]  0x00000000
    [       7]  0x00000000
    [       8]  0x80000010 - Valid
    [       9]  0x00000000
    [      10]  0x80000013 - Valid
    [      11]  0x00000000
    [      12]  0x8000001f - Valid
    [      13]  0x8000001c - Valid
    [      14]  0x00000000
    [      15]  0x00000000 
    -->
    <!-- 
    - address space size = 16k = 16384 bytes
    - page size = 1k = 1024 bytes
    - log2(1024) = 10 bits
    - log2(16384) = 14 bits
    - Hence, 4 bits are for the Virtual Page Number (VPN), and 10 bits are for the offset 
    -->
    - VA 0x00003986 (decimal:    14726) --> Invalid
        - 0x00003986 (hex) = 0000 0000 0000 0000 0011 1001 1000 1010 (binary)
        - We only care about the rightmost 14 bits, so our virtual address is 1110 0110001010 
        - This tells us that our VA has VPN 14 and offset 0110001010
        - From the page table, we see that VPN 14 is invalid.

    - VA 0x00002bc6 (decimal:    11206) --> PA (0x73c6)
        - 0x00002bc6 (hex) = 0000 0000 0000 0000 0010 1011 1100 0110 (binary)
        - We only care about the rightmost 14 bits, so our virtual address is 1010 1111000110
        - Hence, 
            - VPN = 10; and
            - Offset = 1111000110
        - From the page table, we see that VPN 10 is valid and maps to 13 = 0001 0011.
        - So
            - PFN    = 0001 0011
            - Offset =          11 1100 0110 
            - PA     = 00 0100 1111 1100 0110 = 0x4fc6

    - VA 0x00001e37 (decimal:     7735) --> Invalid
        - 0x00001e37 (hex) = 0000 0000 0000 0000 0001 1110 0011 0111 (binary)
        - We only care about the rightmost 14 bits, so our virtual address is 0111 1000110111
        - Hence, 
            - VPN = 7; and
            - Offset = 1000110111
        - From the page table, we see that VPN 7 is invalid.

    - VA 0x00000671 (decimal:     1649) --> Invalid
        - 0x00000671 (hex) = 0000 0000 0000 0000 0000 0110 0111 0001 (binary)
        - We only care about the rightmost 14 bits, so our virtual address is 0001 1001110001
        - Hence, 
            - VPN = 1; and
            - Offset = 1001110001
        - From the page table, we see that VPN 1 is invalid.

    - VA 0x00001bc9 (decimal:     7113) --> Invalid
        - 0x00001bc9 (hex) = 0000 0000 0000 0000 0001 1011 1100 1001 (binary)
        - We only care about the rightmost 14 bits, so our virtual address is 0110 1111001001 
        - Hence, 
            - VPN = 6; and
            - Offset = 1111001001
        - From the page table, we see that VPN 6 is invalid.
- `python3 paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 50 -c`
    <!-- 
    Page Table (from entry 0 down to the max size)
    [       0]  0x80000018 - Valid
    [       1]  0x00000000
    [       2]  0x00000000
    [       3]  0x8000000c - Valid
    [       4]  0x80000009 - Valid
    [       5]  0x00000000
    [       6]  0x8000001d - Valid
    [       7]  0x80000013 - Valid
    [       8]  0x00000000
    [       9]  0x8000001f - Valid
    [      10]  0x8000001c - Valid
    [      11]  0x00000000
    [      12]  0x8000000f - Valid
    [      13]  0x00000000
    [      14]  0x00000000
    [      15]  0x80000008 - Valid
    -->
    - VA 0x00003385 (decimal:    13189) --> 0x3f85
        - VA: 0000 0000 0000 0000 0011 0011 1000 0101 (binary)
        - So our virtual address is 1100 1110000101
        - Hence, 
            - VPN = 12; and
            - Offset = 1110000101
        - From the page table, we see that VPN 12 is valid.
        - PFN    = 1111
        - Offset =      11 1000 0101
        - PA     = 11 1111 1000 0101 = 0x3f85

    - VA 0x0000231d (decimal:     8989) --> Invalid
        - VA: 0000 0000 0000 0000 0010 0011 0001 1101 (binary)
        - So our virtual address is 1000 1100011101
        - VPN 8 is invalid.

    - VA 0x000000e6 (decimal:      230) --> 0x60e6
        - VA: 0000 0000 0000 0000 0000 0000 1110 0110 (binary)
        - So our virtual address is 0000 0011100110
        - Hence, 
            - VPN = 0; and
            - Offset = 0011100110
        - From the page table, we see that VPN 0 is valid.
        - PFN    = 0001 1000
        - Offset =           0011100110
        - PA     = 00 0110 0000 1110 0110 = 0x60e6

    - VA 0x00002e0f (decimal:    11791) --> Invalid
        - VA: 0000 0000 0000 0000 0010 1110 0000 1111 (binary)
        - So our virtual address is 1011 1000001111
        - Hence, 
            - VPN = 11; and
            - Offset = 1000001111
        - From the page table, we see that VPN 11 is invalid.

    - VA 0x00001986 (decimal:     6534) --> PA or invalid address?
        - VA: 0000 0000 0000 0000 0001 1001 1000 0110 (binary)
        - So our virtual address is 0110 0110000110
        - Hence, 
            - VPN = 6; and
            - Offset = 0110000110
        - From the page table, we see that VPN 6 is valid.
        - PFN    = 0001 1101
        - Offset =           0001000110
        - PA     = 00 0111 0100 0100 0110 = 0x7446

<!-- Skipping the last two exercises in this section for now since they're basically just repeats of the last three exercises -->

- `python3 paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 75`
    <!-- Page Table (from entry 0 down to the max size)
    [       0]  0x80000018 - All entries valid
    [       1]  0x80000008
    [       2]  0x8000000c
    [       3]  0x80000009
    [       4]  0x80000012
    [       5]  0x80000010
    [       6]  0x8000001f
    [       7]  0x8000001c
    [       8]  0x80000017
    [       9]  0x80000015
    [      10]  0x80000003
    [      11]  0x80000013
    [      12]  0x8000001e
    [      13]  0x8000001b
    [      14]  0x80000019
    [      15]  0x80000000 -->
    - VA 0x00002e0f (decimal:    11791) --> PA or invalid address?
    - VA 0x00001986 (decimal:     6534) --> PA or invalid address?
    - VA 0x000034ca (decimal:    13514) --> PA or invalid address?
    - VA 0x00002ac3 (decimal:    10947) --> PA or invalid address?
    - VA 0x00000012 (decimal:       18) --> PA or invalid address?

- `python3 paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 100`
    <!-- Page Table (from entry 0 down to the max size)
    [       0]  0x80000018 - All entries valid
    [       1]  0x80000008
    [       2]  0x8000000c
    [       3]  0x80000009
    [       4]  0x80000012
    [       5]  0x80000010
    [       6]  0x8000001f
    [       7]  0x8000001c
    [       8]  0x80000017
    [       9]  0x80000015
    [      10]  0x80000003
    [      11]  0x80000013
    [      12]  0x8000001e
    [      13]  0x8000001b
    [      14]  0x80000019
    [      15]  0x80000000 -->
    - VA 0x00002e0f (decimal:    11791) --> PA or invalid address?
    - VA 0x00001986 (decimal:     6534) --> PA or invalid address?
    - VA 0x000034ca (decimal:    13514) --> PA or invalid address?
    - VA 0x00002ac3 (decimal:    10947) --> PA or invalid address?
    - VA 0x00000012 (decimal:       18) --> PA or invalid address?

What happens as you increase the percentage of pages that are allocated in each address space?
    - The number of valid entries in the page table increases

---

### Question 3

Now let's try some different random seeds, and some different (and sometimes quite crazy) address-space parameters, for variety:

- `python3 paging-linear-translate.py -P 8 -a 32 -p 1024 -v -s 1`
    <!-- 
    - page size: 8
    - address space size: 32
    - phys mem size: 1024 -->
    <!-- 
     - log2(32) = 5, our VA will thus consist of 5 bits 
    -->
    <!-- Page Table (from entry 0 down to the max size)
    [       0]  0x00000000
    [       1]  0x80000061
    [       2]  0x00000000
    [       3]  0x00000000 -->
    VA 0x0000000e (decimal:       14) --> 0x30e
        - VA: 0000 0000 0000 0000 0000 0000 0000 1110 (binary)
        - So our virtual address is 01 110
        - Hence, 
            - VPN = 1; and
            - Offset = 110
        - From the page table, we see that VPN 1 is valid.
        - PFN    = 0110 0001
        - Offset =           110
        - PA     = 011 0000 1110 = 0x30e
    VA 0x00000014 (decimal:       20) --> invalid
        - VA: 0000 0000 0000 0000 0000 0000 0001 0100 (binary)
        - So our virtual address is 10 100
        - Hence, 
            - VPN = 2; and
            - Offset = 100
        - From the page table, we see that VPN 2 is invalid.
    VA 0x00000019 (decimal:       25) --> invalid
        - VA: 0000 0000 0000 0000 0000 0000 0001 1001 (binary)
        - So our virtual address is 11 001
        - Hence, 
            - VPN = 3; and
            - Offset = 001
        - From the page table, we see that VPN 3 is invalid.
        - PFN    = 0110 0001
        - Offset =           110
        - PA     = 011 0000 1110 = 0x30e
    VA 0x00000003 (decimal:        3) --> invalid
        - VA: 0000 0000 0000 0000 0000 0000 0000 0011 (binary)
        - So our virtual address is 00 011
        - Hence, 
            - VPN = 0; and
            - Offset = 011
        - From the page table, we see that VPN 3 is invalid.
        - PFN    = 0110 0001
        - Offset =           110
        - PA     = 011 0000 1110 = 0x30e
    VA 0x00000000 (decimal:        0) --> invalid
        - VA: 0000 0000 0000 0000 0000 0000 0000 0000 (binary)
        - So our virtual address is 00 000
        - Hence, 
            - VPN = 0; and
            - Offset = 000
        - From the page table, we see that VPN 0 is invalid.

- `python3 paging-linear-translate.py -P 8k -a 32k -p 1m -v -s 2 -c`
    <!-- 
    address space size: 32 KB
    phys mem size: 1 MB
    page size: 8 KB -->
    <!-- 
     - 32 KB = 32,768 bytes, and log2(32,768) = 15
     - Our VA will thus consist of 15 bits 
    -->
    <!-- Page Table (from entry 0 down to the max size)
    [       0]  0x80000079
    [       1]  0x00000000
    [       2]  0x00000000
    [       3]  0x8000005e -->
    VA 0x000055b9 (decimal:    21945) --> invalid
        - VA: 0000 0000 0000 0000 0101 0101 1011 1001 (binary)
        - So our virtual address is 10 1010110111001
        - Hence, 
            - VPN = 2; and
            - Offset = 1010110111001
        - From the page table, we see that VPN 2 is not valid.
    VA 0x00002771 (decimal:    10097) --> invalid
        - VA: 0000 0000 0000 0000 0010 0111 0111 0001 (binary)
        - So our virtual address is 01 0011101110001
        - Hence, 
            - VPN = 1; and
            - Offset = 0011101110001
        - From the page table, we see that VPN 1 is not valid.
    VA 0x00004d8f (decimal:    19855) --> PA or invalid address?
        - VA: 0000 0000 0000 0000 0100 1101 0111 1111 (binary)
        - So our virtual address is 10 0110101111111
        - Hence, 
            - VPN = 2; and
            - Offset = 0110101111111
        - From the page table, we see that VPN 2 is not valid.
    VA 0x00004dab (decimal:    19883) --> PA or invalid address?
        - VA: 0000 0000 0000 0000 0100 1101 1010 1011 (binary)
        - So our virtual address is 10 0110110101011
        - Hence, 
            - VPN = 2; and
            - Offset = 0110110101011
        - From the page table, we see that VPN 2 is not valid.
    VA 0x00004a64 (decimal:    19044) --> PA or invalid address?
            - VA: 0000 0000 0000 0000 0100 1010 0110 0100 (binary)
        - So our virtual address is 10 0101001100100
        - Hence, 
            - VPN = 2; and
            - Offset = 0101001100100
        - From the page table, we see that VPN 2 is not valid.

- `python3 paging-linear-translate.py -P 1m -a 256m -p 512m -v -s 3 -c`
    <!-- address space size: 256 MB
    phys mem size: 512 MB
    page size: 1 MB -->
    <!-- We thus have 256 MB / 1 MB = 256 pages, and require log2(256) = 8 bits for each VPN
    Moreover, we know that the number of bytes in 256 MB is 268435456 bytes.
    It therefore follows that the number of bits needed for each VA is log2(268435456) 28.
    So we need 8 bits for the VPN and 28 - 8 = 20 bits for each offset -->
<!-- Page Table (from entry 0 down to the max size)
  [       0]  0x00000000
  [       1]  0x800000bd
  [       2]  0x80000140
  [       3]  0x00000000
  [       4]  0x00000000
  [       5]  0x80000084
  [       6]  0x00000000
  [       7]  0x800000f0
  [       8]  0x800000f3
  [       9]  0x8000004d
  [      10]  0x800001bc
  [      11]  0x8000017b
  [      12]  0x80000020
  [      13]  0x8000012e
  [      14]  0x00000000
  [      15]  0x00000000
  [      16]  0x800000f2
  [      17]  0x800001c1
  [      18]  0x800001d7
  [      19]  0x00000000
  [      20]  0x800000e3
  [      21]  0x00000000
  [      22]  0x00000000
  [      23]  0x00000000
  [      24]  0x800000df
  [      25]  0x8000009a
  [      26]  0x800000c5
  [      27]  0x00000000
  [      28]  0x8000012b
  [      29]  0x8000015d
  [      30]  0x800001b6
  [      31]  0x80000157
  [      32]  0x00000000
  [      33]  0x800001ed
  [      34]  0x80000123
  [      35]  0x8000006c
  [      36]  0x80000125
  [      37]  0x00000000
  [      38]  0x00000000
  [      39]  0x800001fa
  [      40]  0x00000000
  [      41]  0x800000d2
  [      42]  0x00000000
  [      43]  0x00000000
  [      44]  0x800001be
  [      45]  0x00000000
  [      46]  0x80000017
  [      47]  0x800000a9
  [      48]  0x800001f6
  [      49]  0x800001ff
  [      50]  0x00000000
  [      51]  0x00000000
  [      52]  0x80000010
  [      53]  0x00000000
  [      54]  0x00000000
  [      55]  0x8000004f
  [      56]  0x00000000
  [      57]  0x800000a0
  [      58]  0x800001cb
  [      59]  0x00000000
  [      60]  0x00000000
  [      61]  0x80000149
  [      62]  0x8000011e
  [      63]  0x800001e1
  [      64]  0x800000dc
  [      65]  0x80000079
  [      66]  0x00000000
  [      67]  0x8000010a
  [      68]  0x80000005
  [      69]  0x00000000
  [      70]  0x8000000a
  [      71]  0x80000143
  [      72]  0x00000000
  [      73]  0x800000ee
  [      74]  0x800000b4
  [      75]  0x80000179
  [      76]  0x00000000
  [      77]  0x00000000
  [      78]  0x00000000
  [      79]  0x00000000
  [      80]  0x800000a3
  [      81]  0x00000000
  [      82]  0x00000000
  [      83]  0x00000000
  [      84]  0x80000099
  [      85]  0x00000000
  [      86]  0x8000000d
  [      87]  0x80000178
  [      88]  0x00000000
  [      89]  0x00000000
  [      90]  0x8000007a
  [      91]  0x00000000
  [      92]  0x00000000
  [      93]  0x80000034
  [      94]  0x00000000
  [      95]  0x00000000
  [      96]  0x800000e0
  [      97]  0x80000056
  [      98]  0x00000000
  [      99]  0x800001c5
  [     100]  0x00000000
  [     101]  0x00000000
  [     102]  0x00000000
  [     103]  0x80000061
  [     104]  0x800001ad
  [     105]  0x00000000
  [     106]  0x00000000
  [     107]  0x80000148
  [     108]  0x800000b0
  [     109]  0x00000000
  [     110]  0x00000000
  [     111]  0x8000008a
  [     112]  0x00000000
  [     113]  0x00000000
  [     114]  0x00000000
  [     115]  0x00000000
  [     116]  0x00000000
  [     117]  0x800001c2
  [     118]  0x800000de
  [     119]  0x800001da
  [     120]  0x00000000
  [     121]  0x800001ac
  [     122]  0x80000109
  [     123]  0x00000000
  [     124]  0x00000000
  [     125]  0x00000000
  [     126]  0x00000000
  [     127]  0x80000092
  [     128]  0x80000163
  [     129]  0x80000127
  [     130]  0x00000000
  [     131]  0x80000057
  [     132]  0x00000000
  [     133]  0x8000010c
  [     134]  0x00000000
  [     135]  0x80000139
  [     136]  0x00000000
  [     137]  0x00000000
  [     138]  0x800000f4
  [     139]  0x00000000
  [     140]  0x800001a2
  [     141]  0x00000000
  [     142]  0x800001a5
  [     143]  0x00000000
  [     144]  0x800001b9
  [     145]  0x00000000
  [     146]  0x00000000
  [     147]  0x00000000
  [     148]  0x800000d8
  [     149]  0x00000000
  [     150]  0x80000000
  [     151]  0x00000000
  [     152]  0x00000000
  [     153]  0x00000000
  [     154]  0x00000000
  [     155]  0x800001b5
  [     156]  0x00000000
  [     157]  0x800000a1
  [     158]  0x00000000
  [     159]  0x00000000
  [     160]  0x8000012c
  [     161]  0x00000000
  [     162]  0x00000000
  [     163]  0x00000000
  [     164]  0x00000000
  [     165]  0x8000016e
  [     166]  0x00000000
  [     167]  0x00000000
  [     168]  0x00000000
  [     169]  0x00000000
  [     170]  0x80000190
  [     171]  0x00000000
  [     172]  0x8000013e
  [     173]  0x00000000
  [     174]  0x00000000
  [     175]  0x00000000
  [     176]  0x800000d7
  [     177]  0x800000eb
  [     178]  0x00000000
  [     179]  0x00000000
  [     180]  0x00000000
  [     181]  0x00000000
  [     182]  0x800001df
  [     183]  0x00000000
  [     184]  0x80000194
  [     185]  0x00000000
  [     186]  0x00000000
  [     187]  0x00000000
  [     188]  0x80000153
  [     189]  0x80000195
  [     190]  0x80000177
  [     191]  0x80000002
  [     192]  0x00000000
  [     193]  0x80000016
  [     194]  0x00000000
  [     195]  0x00000000
  [     196]  0x8000005b
  [     197]  0x00000000
  [     198]  0x8000003e
  [     199]  0x80000158
  [     200]  0x800001e7
  [     201]  0x80000198
  [     202]  0x00000000
  [     203]  0x80000105
  [     204]  0x80000036
  [     205]  0x800001de
  [     206]  0x00000000
  [     207]  0x00000000
  [     208]  0x800001a7
  [     209]  0x00000000
  [     210]  0x00000000
  [     211]  0x00000000
  [     212]  0x80000181
  [     213]  0x00000000
  [     214]  0x00000000
  [     215]  0x00000000
  [     216]  0x00000000
  [     217]  0x00000000
  [     218]  0x00000000
  [     219]  0x800001f2
  [     220]  0x00000000
  [     221]  0x80000052
  [     222]  0x80000183
  [     223]  0x80000108
  [     224]  0x00000000
  [     225]  0x00000000
  [     226]  0x00000000
  [     227]  0x800001d5
  [     228]  0x800000e2
  [     229]  0x8000005f
  [     230]  0x00000000
  [     231]  0x00000000
  [     232]  0x00000000
  [     233]  0x800001e8
  [     234]  0x00000000
  [     235]  0x800000d3
  [     236]  0x00000000
  [     237]  0x00000000
  [     238]  0x00000000
  [     239]  0x00000000
  [     240]  0x00000000
  [     241]  0x00000000
  [     242]  0x00000000
  [     243]  0x00000000
  [     244]  0x80000049
  [     245]  0x800000f5
  [     246]  0x800000ef
  [     247]  0x800001a4
  [     248]  0x800000f6
  [     249]  0x00000000
  [     250]  0x800001eb
  [     251]  0x00000000
  [     252]  0x00000000
  [     253]  0x00000000
  [     254]  0x80000159
  [     255]  0x00000000 -->
    VA 0x0308b24d (decimal: 50901581) --> 0x1f68b24d
        - VA: 0000 0011 0000 1000 1011 0010 0100 1101 (binary)
        - So our virtual address is 00110000 10001011001001001101
        - Hence, 
            - VPN = 48; and
            - Offset = 10001011001001001101
        - From the page table, we see that VPN 48 is valid.
        - PFN    = 1f6 = 0001 1111 0110
        - Offset =                      10001011001001001101
        - PA     = 0001 1111 0110 1000 1011 0010 0100 1101 = 0x1f68b24d
    VA 0x042351e6 (decimal: 69423590) --> invalid
        - VA: 0000 0100 0010 0011 0101 0001 1110 0110 (binary)
        - So our virtual address is 01000010 00110101000111100110
        - Hence, 
            - VPN = 66; and
            - Offset = 00110101000111100110
        - From the page table, we see that VPN 66 is invalid.
    VA 0x02feb67b (decimal: 50247291) --> 0xa9db67b
        - VA: 0000 0010 1111 1110 1011 0110 0111 1011 (binary)
        - So our virtual address is 00101111 11101011011001111011
        - Hence, 
            - VPN = 47; and
            - Offset = 11101011011001111011
        - From the page table, we see that VPN 48 is valid.
        - PFN    = a9 = 1010 1001
        - Offset =                11101011011001111011
        - PA     = 1010 1001 1110 1011 0110 0111 1011 = 0xa9db67b
<!-- Skipping the last two exercises in this section for now since they're basically just repeats of the last three exercises -->
    VA 0x0b46977d (decimal: 189175677) --> PA or invalid address?
    VA 0x0dbcceb4 (decimal: 230477492) --> PA or invalid address?

Which of these parameter combinations are unrealistic? Why?
<!-- paging-linear-translate.py -P 8 -a 32 -p 1024 -v -s 1  -->
- The `-P 8` and `-a 32` combo seems unrealistic, in that it implies we would only have 4 pages.
- Having a `p` memory size  of 1024 is also extremely low.  
<!-- paging-linear-translate.py -P 8k -a 32k -p 1m -v -s 2  -->
- The combo `-P 8k` and `-a 32k` also seems unrealistic, in that it implies we would only have 4 pages
<!-- paging-linear-translate.py -P 1m -a 256m -p 512m -v -s 3  -->
- In the third command, the combo `-a 256m` and `-p 512m` seems unrealistic, in that it implies one process would take up half the physical memory. 
---

### Question 4

Use the program to try out some other problems. Can you find the limits of where the program doesn't work anymore? For example, what happens if the address-space size is bigger than physical memory?
    - Let's try out the following command
        - `python3 paging-linear-translate.py -P 8k -a 1m -p 32k -v -s 2`
        - Since the address space size is bigger than physical memory, we get the following error:
            - Error: physical memory size must be GREATER than address space size (for this simulation)
            - Note the "for this simulation"; I'm guessing this is because the authors haven't yet touched on paging. 



<!-- Run the program with the -h flag to see what options there are: -->

<!-- Options:
-h, --help              show this help message and exit
-s SEED, --seed=SEED    the random seed
-a ASIZE, --asize=ASIZE 
                        address space size (e.g., 16, 64k, ...)
-p PSIZE, --physmem=PSIZE
                        physical memory size (e.g., 16, 64k, ...)
-P PAGESIZE, --pagesize=PAGESIZE
                        page size (e.g., 4k, 8k, ...)
-n NUM, --addresses=NUM number of virtual addresses to generate
-u USED, --used=USED    percent of address space that is used
-v                      verbose mode
-c                      compute answers for me -->