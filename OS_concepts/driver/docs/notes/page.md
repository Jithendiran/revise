```c
struct page{
	atomic_t count;
	void *virtual;
	unsigned long falgs;
}
```
Physical memory is segmented into pages. Internal handling of memory is in the form of pages. Page size will be varied from architecture. Most 32 bit architectures are using 4096 bytes as page size. It is maintained in `PAGE_SIZE`  constant `<asm/page.h>`

if 4096 page size is used then 12 least significant bits are the offset and remaining higher bits are the page number, If the 12 bits are removed and remaining bits are shifted to right then it results in `page frame number`. This operation will done by a macro called `PAGE_SHIFT`.

Kernel has `logical address` to refer pages in physical memory.
High memory not having logical address, So kernel function deals with memory are using `pointers` to `struct page`.

## Fields

**count**

	This has the reference count for page. when counts downs to 0, page returned to free list

**virtual**

	This is kernel virtual address of the page, if mapped else NULL.
	This field will available only when compiled with. Some architecture doesn't supported

**flag**

	This describe the status of the page.
	PG_locked -> page has been locked in memory
	PG_reserved -> prevent MMU working with it

Kernel maintains one or more array of struct page

**mem_map**

	This is a single array that keeps track of all struct page in system

**Nonuniform memory access (NUMA)**

	This is having multiple array's to store discontiguous physical memory pages.


## Functions and Macro

### Logical address to page structure

*<asm/page.h>*
```c
struct page *virt_to_page(void *kaddr);
```
it won't work with `valloc` or `high memory`

### Page frame number to page structure
```c
struct page *pfn_to_page(int pfn);
```

### Struct page to kernel virtual address

*<linux/mm.h>*
```c
void *page_address(struct page *page);
```
For high memory address exists only if the page has been mapped

### Create mapping for virtual address

*<linux/highmem.h>*
```c
void *kmap(struct page *page);
void kunmap(struct page *page);
```

`kmap` returns a kernel virtual address

	- For low mem it return's logical address of the page
	- For High mem it create mapping in dedicated kernel address space
	Only limited number mapping is available 
Mapping created with `kmap` should free with `kunmap`

**Atomic**
```c
void *kmap_atomic(struct page *page, enum km_type type);
void kunmap_atomic(void *addr, enum km_type type);
```
It is a high performance.
Each architectures maintains set of slots for atomic kmaps.
For drivers only some slots make sense
 -  `KM_USER0 and KM_USER1` code running directly form user space
 -  `KM_IRQ0 and KM_IRQ1` for interrupt handling
 Code cannot sleep while holding

## Page Tables

The on modern processor must have a mechanism to translate virtual memory into corresponding physical memory. This mechanism is page table

It is multilevel tree structured array containing virtual to physical mapping with associate flags

 
[[Addressing]]