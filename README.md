# VMM
Virtual Memory Manager - COEN 346 Programming Project 4
Done in collaboration with Aida Bumbu

Goal:

Simulate a virtual memory manager:
A program that translates logical to physical addresses for a virtual address space of size 65,536 bytes.
Input file contains several logical addresses. The translation will be done through the use of a Translation Look-aside Buffer (TLB) and a page table.
This simulation displays the steps involved in translating logical to physical addresses.

Specification:

Read 32-bit integer numbers representing logical addresses.
Masking of the rightmost 16 bits is required as only they are required.
Those 16 bits are divided in 2 parts: an 8-bit page number and an 8-bit page offset.
VMM includes following:
- 256 entries in page table.
- -page size of 256 bytes.
- 16 entries in the TLB.
- frame size of 256 bytes.
- 256 frames.
- physical memory of 65,536 bytes (256 frames x 256-byte frame size).
