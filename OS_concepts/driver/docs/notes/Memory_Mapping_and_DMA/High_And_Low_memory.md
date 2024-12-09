On 32bit architecture machine it supports maximum of 4GB memory.
In that 4GB, Kernel space have (1GB) and User space have (3GB). All the kernel data and code must needs to fit in these 1 GB. Most memory of Kernel space is consumed by virtual memory mapping table. Kernel must build it's own virtual memory to work with.
Sooner 4GB of memory is not sufficient for so the processor manufactures have added `address extension` feature, by using this feature 32bit processor can support more than 4GB of physical memory.
Even though system can support more than 4GB but each application is limited to the logical address(1 or 2 GB based on kernel and hardware config), rest of the memory are high memory page.  Before accessing the higher memory page, kernel must set up an explicit virtual mapping to make that page available to the kernel address space.
Thus kernel data structure is placed low memory and high memory is reserved for user space

*Low memory*

	Memory for which logical address exists in kernel address space

*High memory*

	Memory for which logical address do not exists, because it is beyond the address space set for kernel space