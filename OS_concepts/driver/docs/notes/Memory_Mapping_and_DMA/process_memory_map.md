Process memory map is the final piece data structure, that holds various data structure together. Each process in system contains this data structure in `struct mm_struct`. Pointer to this found in `current->mm`. It holds data structure like 

	- List of VMA's
	- page table
	- other memory related data structs
	- spinlock
	- semaphore

Memory map in driver is useful for providing direct access to device memory

Mapping device address means associating user space address to device register
when ever read or write happens on that mapped memory, it executes in the device memory

Memory is allocated in `PAGE_SIZE` . So device memory should align with `PAGE_SIZE`. If a device memory is small, then kernel make's it slightly bigger 

If a memory is scatter like 8-bit, 16-bit,.. with no direct mapping, in this case memory map is not possible

let's take `X-org`

`$ sudo lsof /tmp/.X11-unix/X0` (note down PID) 

`$ sudo cat /proc/{x-org-pid}/maps`

The above command will list all the xorg's memory mapping in this `/dev/*` is the direct memory access

* `mmap` is the method for associating user space address with device memory 
* It is a part of `file_operatios`, these are invoked when system's `mmap` is issued
* **System mmap is different from this mmap file_operatios** [CH15, pg:424], system will do some good work in system's mmap and then control transfer to file_operation mmap, so driver only want to build suitable page table for the address rage, if necessary replace new set of operations (`vma->vm_ops`)

	System mmap is called by user space when a new memory map is creating. This system mmap will allocate the virtual memory, then mmap system call will call  `file_operation->mmap` 

`mremap` used to change the mapped region, is used to change the mapping, when a mapping size is reduced, kernel flush out the mapped pages without notifying`

There are two ways to build the page table (pg:)

* Doing all at once (`remap_pfn_range`)

	For this VMA, virtual address are already allocated, here remapping device address with virtual address space is the job. It will do simple linear mapping of device memory into user space

	Adding VMA operations
		`open` - called when a new process forked and creates new reference for VMA
		like open `close` method is there, these are called in addition to the processing performed by the kernel	
		To make sure operations are active for specific VMA, it is necessary to store these vm_operation struct in vm->vm_ops [[vm_area_struct]].
		 
* Doing at on demand (`nopage` in VMA)

	`nopage` function will be called when a `PAGE_FAULT` occur's,  when page fault will occur and `nopage` function must locate and return the page, this `nopage` functions needs to take care of incrementing page reference `get_page(struct page *pageptr);` 

	`mremap` is used to modify bound region, Kernel don't notify about mremap modification. In this case `nopage` will help

	When memory is reduced kernel will flush out unwanted pages without telling driver, When a memory is expanded then driver will call `nopage` to map new pages so here no need of seperate notifications
	
	`nopage` works well for ISA memory regions, it won't work for PCI memory, because it mapped above high mem, there no entries in the system memory map for those address, if there is no struct page to return to `nopage`. in this case it must use `remap_pfn_range`

	if `nopage` is not implemented, kernel allocate new page as zero page with copy on write 
	 
>[!warning]
> Memory mapped area should not use caching



Chapter 15, page: 422
