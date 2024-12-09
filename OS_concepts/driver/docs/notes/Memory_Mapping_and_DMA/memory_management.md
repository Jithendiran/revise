Linux is a virtual memory management system meaning it's user program memory needs to use page table to convert into physical memory.  
By using virtual memory process could use more than physical memory capacity.  
To refer memory linux uses it's memory address. 
## Address Type

	Linux deals with several types of address

**User Virtual address**

	- This is regular user space address
	- Length is 32 or 64 bit
	- Each process has it's  own virtual address space

**Physical address**

	- This is used between processor and system's memory. This is real address where data is stored
	- Length is 32 or 64 bit

**Bus address**

	- This address is used between peripheral buses and memory. Often this is same as physical address, but this is not always necessary
	- Some architecture provided IOMMU it remaps the address between main memory and bus

**Kernel logical address**

	- This is usual address space for kernel, This address maps some portion of main memory and some times treated as physical memory, because it is logical address and physical address are differ from constant offset
		- In some machines needs to use MMU unit to calculate physical address
		- In some machines by adding or subracting offset can get physical address
	- This is distinct from user space
	- This address are stored in types like `unsigned long` or `void *`
	- When kernel allocate memory with `kmalloc` the address returned is a kernel logical address

**Kernel virtual address**

	- This are similar to logical address in that they are mapping from a kernel space address to physical address
	- Here physical address of one-to-one mapped virtual address are not have to linear
	- All logical address are virtual address but not all virtal address are logical address
	- Memory allocated by `valloc` and `kmap` has virtual address

>[!Note]
>Kernel code is not always very clear on which type of address is using so programmer needs to take care of it

## Conversion

logical address to physical address using macro `__pa()`
physical address to logical address using macro `__va()` only for low-memory pages

## Pages

Physical memory is segmented into pages. Internal handling of memory is in the form of pages. Page size will be varied from architecture. Most 32 bit architectures are using 4096 bytes as page size. It is maintained in `PAGE_SIZE`  constant `<asm/page.h>`

if 4096 page size is used then 12 least significant bits are the offset and remaining higher bits are the page number, If the 12 bits are removed and remaining bits are shifted to right then it results in `page frame number`. This operation will done by a macro called `PAGE_SHIFT`.

[[High_And_Low_memory]]