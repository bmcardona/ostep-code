<!-- See the ./ostep-homework/vm-segmentation/README.md file for this assignment -->

<!-- 
IMPORTANT NOTE:
**An N-byte address space requires log₂(N) bits to address.**

Because you need exactly enough bits to count N locations, and each bit doubles how many you can count. So:

- 16-byte → 4-bit addresses (2⁴ = 16)
- 32-byte → 5-bit addresses (2⁵ = 32)
- 128-byte → 7-bit addresses (2⁷ = 128)
- 1KB (1024-byte) → 10-bit addresses (2¹⁰ = 1024)

And for segmentation specifically, the **top bit** of that address splits the space in half — segment 0 on the bottom, segment 1 on the top.
 
- A 128-byte address space means:
    - There are 128 addresses
    - Each address points to 1 byte of data 
-->

## FLAGS
-----
<!-- 
  -h, --help                  show this help message and exit
  -s, --seed=SEED             random seed (use same seed to regenerate same problem)
  -a, --asize=ASIZE           address space size (e.g. 16, 64k, 32m)
  -p, --physmem=PSIZE         physical memory size (e.g. 16, 64k, 32m)
  -n, --addresses=NUM         number of virtual addresses to generate
  -b, --b=BASE                value of segment 0 base register
  -l, --l=LIMIT               value of segment 0 limit register
  -B, --B=BASE                value of segment 1 base register
  -L, --L=LIMIT               value of segment 1 limit register
  -A, --addresses=ADDR        comma-separated list of addresses to translate (e.g. 19,20,107,108)
  -c, --compute               compute answers for me 
-->

## QUESTIONS
<!-- For each virtual address, either write down the physical address it translates to
OR write down that it is an out-of-bounds address (a segmentation violation). For
this problem, you should assume a simple address space with two segments: the top
bit of the virtual address can thus be used to check whether the virtual address
is in segment 0 (topbit=0) or segment 1 (topbit=1). Note that the base/limit pairs
given to you grow in different directions, depending on the segment, i.e., segment 0
grows in the positive direction, whereas segment 1 in the negative.  -->

<!-- Question 1 -->
* First, let’s use a tiny address space to translate some addresses. Here’s a simple set of parameters with a few different random seeds; can you translate the addresses?
    * `python3 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0 -c`
    <!-- 
    - See IMPORTANT NOTE at top of file
    - We know that an N-byte address space requires log₂(N) bits to address. Hence, since we have a 128-byte address space, we need a total of 7 bits to address. 
    - The top (7th) bit splits it in half:    
        - 0xxxxxx = 0–63 → Segment 0
        - 1xxxxxx = 64–127 → Segment 1 
    -->
        - VA  0: 0x0000006c (decimal:  108)
            - VA 0 in segment 1
            - offset = 108 - 128 = -20
            - Since |-20| <= Segment 1 limit (20), the physical address of VA 0 is 492.
        - VA  1: 0x00000061 (decimal:   97)
            - VA 1 in segment 1
            - offset = 97 - 128 = -31
            - Since |-31| > Segment 1 limit (20), we have a segmentation violation.
        - VA  2: 0x00000035 (decimal:   53)
            - VA 2 in segment 0
            - offset = 53 - 0 = 53
            - Since 50 > Segment 0 limit (20), we have a segmentation violation.
        - VA  3: 0x00000021 (decimal:   33)
            - VA 3 in segment 0
            - offset = 33 - 0 = 33
            - Since 33 > Segment 0 limit (20), we have a segmentation violation.
        - VA  4: 0x00000041 (decimal:   65)
            - VA 4 in segment 1
            - offset = 65 - 128 = -63
            - Since |-63| > Segment 1 limit (20), we have a segmentation violation.
    * `python3 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1 -c`
        - VA  0: 0x00000011 (decimal:  17)
            - VA 0 in segment 0
            - offset = 17 - 0 = 17
            - Since 17 <= Segment 0 limit (20), the physical address of VA 0 is 17.
        - VA  1: 0x0000006c (decimal:   108)
            - VA 1 in segment 1
            - offset = 108 - 128 = -20
            - Since |-20| <= Segment 1 limit (20), the physical address of VA 1 is 492.
        - VA  2: 0x00000061 (decimal:   97)
            - VA 2 in segment 1
            - offset = 97 - 128 = -31
            - Since |-31| > Segment 1 limit (20), we have a segmentation violation.
        - VA  3: 0x00000020 (decimal:   32) 
            - VA 3 in segment 0
            - offset = 32 - 0 = 32
            - Since 32 > Segment 0 limit (20), we have a segmentation violation.
        - VA  4: 0x0000003f (decimal:   63)
            - VA 4 in segment 0
            - offset = 63 - 0 = 63
            - Since 63 > Segment 0 limit (20), we have a segmentation violation.
    * `python3 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2 -c`
        - VA  0: 0x0000007a (decimal:  122)
            - VA 0 in segment 1
            - offset = 122 - 128 = -6
            - Since |-6| <= Segment 1 limit (20), the physical address of VA 0 is 506.
        - VA  1: 0x00000079 (decimal:  121)
            - VA 1 in segment 1
            - offset = 121 - 128 = -7
            - Since |-7| <= Segment 1 limit (20), the physical address of VA 1 is 505.
        - VA  2: 0x00000007 (decimal:    7)
            - VA 2 in segment 0
            - offset = 7 - 0 = 7
            - Since 7 <= Segment 0 limit (20), the physical address of VA 2 is 7.
        - VA  3: 0x0000000a (decimal:   10)
            - VA 3 in segment 0
            - offset = 10 - 0 = 10
            - Since 10 <= Segment 0 limit (20), the physical address of VA 3 is 10.
        - VA  4: 0x0000006a (decimal:  106)
            - VA 4 in segment 1
            - offset = 106 - 128 = -22
            - Since |-22| > Segment 1 limit (20), we have a segmentation violation. 

<!-- Question 2 -->
* Now, let’s see if we understand this tiny address space we’ve constructed (using the parameters from the question above).
    * What is the highest legal virtual address in segment 0? 
        - 0 + (20 - 1) = 19. (Note: virtual addresses are 0 indexed)
        <!-- Read the question incorrectly the first time around. This answer is wrong -- 63, since the largest 7-bit string that starts with 0 is 0111111, which in decimal form is equal to 2^5 + 2^4 + 2^3 + 2^2 + 2^1 + 2^0. -->
    * What about the lowest legal virtual address in segment 1?
        - 128 - 20 = 108.
        <!-- Read the question incorrectly the first time around. This answer is wrong -- 64, since the lowest 7-bit string that starts with 1 is 1000000, which in decimal form is equal to 2^6.  -->
    * What are the lowest and highest illegal addresses in this entire address space?
        - lowest = 19 + 1 = 20
        - highest = 108 - 1 = 107
    * Finally, how would you run `segmentation.py` with the `-A` flag to test if you are right?
        - `python3 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -A 19,20,107,108 -c`

<!-- Question 3 -->
* Let’s say we have a tiny 16-byte address space in a 128-byte physical memory. What base and bounds would you set up so as to get the simulator to generate the following translation results for the specified address stream: `valid, valid, violation, ..., violation, valid, valid`? Assume the following parameters: `segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 ? --l0 ? --b1 ? --l1 ?`
    - `python3 segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 0 --l0 2 --b1 15 --l1 2 -c`


<!-- Question 4 -->
* Assuming we want to generate a problem where roughly 90% of the randomly-generated virtual addresses are valid (i.e., not segmentation violations). How should you configure the simulator to do so? Which parameters are important?
    - Assuming we ignore the `-s` and `-A` flags to get randomly-generated addresses, we need only make roughly 10% of the virtual addresses invalid. If we specify the virtual address space to be equal to 
    128 bytes, and make is so that only addresses 0 to 57 (inclusive) and 71-128 (inclusive) are
    valid addresses, then roughly 90% -- specifically, (57 + 58) / 128 = 89.84375% -- of the randomly-generated virtual addresses will be valid. The following command does exactly this; here, for the sake of checking our work, we set the "number of virtual addresses to generate" to 1000:
        - `python3 segmentation.py -a 128 -p 512 --b0 0 --l0 57 --b1 128 --l1 57 -n 1000 -c`
        - Using Claude, we find that out of 1000 virtual addresses (VA 0–VA 999), 893 of these addresses are valid, with the other 107 being segmentation violations.

<!-- Question 5 -->
* Can you run the simulator such that no virtual addresses are valid? How?
    - Sure. Using our answer from question 4, we need only set the base registers of segments 0 and 1 to be 0 and 0, respectively. 
    - `python3 segmentation.py -a 128 -p 512 --b0 0 --l0 0 --b1 128 --l1 0 -n 1000 -c`
    - Here, we find that out of 1000 randomly-generated virtual addresses (VA 0–VA 999), 1000 are segmentation violations.

---
<!-- 
  -h, --help                  show this help message and exit
  -s, --seed=SEED             random seed (use same seed to regenerate same problem)
  -a, --asize=ASIZE           address space size (e.g. 16, 64k, 32m)
  -p, --physmem=PSIZE         physical memory size (e.g. 16, 64k, 32m)
  -n, --addresses=NUM         number of virtual addresses to generate
  -b, --b=BASE                value of segment 0 base register
  -l, --l=LIMIT               value of segment 0 limit register
  -B, --B=BASE                value of segment 1 base register
  -L, --L=LIMIT               value of segment 1 limit register
  -A, --addresses=ADDR        comma-separated list of addresses to translate (e.g. 19,20,107,108)
  -c, --compute               compute answers for me 
-->