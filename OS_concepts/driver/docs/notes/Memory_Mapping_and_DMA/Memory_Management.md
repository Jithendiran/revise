On 32 bit architecture machine it supports maximum of 4 GB memory. In that 4 GB, Kernel space have (1 GB) and User space have (3 GB).

```
32 bit architecture use maximum of 4GB
0 - 896 MB is kernel memory (Low mem)
897 MB - 4 GB is user memory (High mem)
```


Early all the kernel data and code must needs to fit in these 1 GB. Most memory of Kernel space is consumed by virtual memory mapping table. Kernel must build it's own virtual memory to work with.

Sooner 4GB of memory is not sufficient for so the processor manufactures have added `address extension` feature, by using this feature 32bit processor can support more than 4GB of physical memory.

Even though system can support more than 4GB but each application is limited to the logical address(1 or 2 GB based on kernel and hardware config), rest of the memory are high memory page.  Before accessing the higher memory page, kernel must set up an explicit virtual mapping to make that page available to the kernel address space. Kernel can't directly access high memory

Thus kernel data structure must be placed on low memory
High memory is reserved for user space process pages

Kernel is not owning high memory, it is accessing by using kernel virtual address

*Low memory*

	- Memory for which logical address exists in kernel address space
	- `kalloc` is having Low memory
	- Continous memory

*High memory*

	- Memory for which logical address do not exists, because it is beyond the address space set for kernel space
	- `malloc` will allocate in high memory, kernel will be accessing it by using `valloc` or `kmap`
	- Non continous memory

[[Addressing]]
[[page]]
[[Virtual_memory_areas]]