# padding, alignment

If a memory is arranged as single bank of one-byte width, for int(4bytes) processor needs 4 cycles to read int.
To reduce the cycle. Memory will be arranged as group of 4 bytes

It is faster to access data that is located at a memory address that is divisible by a certain number. Putting your data on such “even” addresses is called memory alignment.

On modern platforms, all basic C types have to be self-aligned. This means that each basic type has to be located at a memory address that is a multiple of its size

--------------------------------------------------------------------
|Data Type |Size in Bytes  |Self-Alignment                         |
|----------|---------------|---------------------------------------|
|char	   |1	           | None (any address is fine)            |
|short	   |2	           | The address has to be a multiple of 2 |
|int	   |4	           | The address has to be a multiple of 4 |
|long	   |8	           | The address has to be a multiple of 8 |
|Pointer   |8	           | The address has to be a multiple of 8 |
--------------------------------------------------------------------

if the address of var is % of it's size should be 0

CPU prefers to read 1, 2, 4, 8, .. bytes in it's multiplied address

for example to read 8 bytes of data, 

if aligned
    address is 1000 - 1007, it is multiplies of 4
    so 1000- 1003 is one cycle and 1004 - 1007 is one cycle

if not aligned
    if address start address is 1001
    1st cycle 0x1000-0x1003 - needed 3 byte
    2nd cycle 0x1004-0x1007 - needed 4 byte
    3rd cycle 0x1008-0x100B - needed 1 byte

    if address start address is 1003
    1st cycle 0x1000-0x1003 - needed 1 byte
    2nd cycle 0x1004-0x1007 - needed 4 byte
    3rd cycle 0x1008-0x100B - needed 3 byte

for char if start address is 1001, it wont read from 1000-1003, it directly read 1001
