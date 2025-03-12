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

	- This address is used between peripheral buses and memory. Often this is same as physical address, but this is not always necessary. It is architecture specific  
	- Fo example: DMA based hardware uses BUS address, ISA and PCI bus uses physical address
	- Some architecture provided IOMMU it remaps the address between main memory and bus

**Kernel logical address**

	- This is usual address space for kernel
	- This address maps some portion of main memory and It treated as physical address, because logical address and physical address are differ from constant offset. By adding or subracting offset can get physical address (direct mapping)
	- This is Linear address, meaning continous memory locations
	- This is distinct from user space
	- This address are stored in types like `unsigned long` or `void *`
	- When kernel allocate memory with `kmalloc` the address returned is a kernel logical address
	- Usually logical memory is 0 - 896 MB (low mem)

**Kernel virtual address**

	- This is used by kernel
	- Here address are not linear meaning not continous,it is maintaining 1:1 mapping
	- Some time kernel logical memory is not sufficient, so it is accessing user memory with help of this kernel virtual address
	- Memory allocated by `valloc` and `kmap` has virtual address
	- Usually kernel virtual address on 1GB to 4GB any where (high mem)

>[!Note]
>Kernel code is not always very clear on which type of address is using so programmer needs to take care of it

## Conversion

logical address to physical address using macro `__pa()`
physical address to logical address using macro `__va()` only for low-memory pages
