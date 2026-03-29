


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