When a user space calls `mmap` to map device memory into it's address space the following this will happen

	- System create new VMA to represent mapping
	- Driver support `mmap` should initialize the process on created VMA

To initialize the VMA, driver writer should know about the minimal info about VMA

kernel maintain lists and trees of `struct vm_area_struct` to optimize lookup and to maintain organization, therefore VMA's cannot created at will of driver or break the structure

*<linux/mm.h>*

```c
struct vm_area_struct {
	 unsigned long vm_star;
	 unsigned long vm_end;
	 struct file *vm_file;
	 unsigned long vm_pgoff;
	 unsigned long vm_flags;
	 struct vm_operations_struct *vm_ops;
	 void *vm_private_data;
}

```
## Fields
**vm_start/vm_end**

	Virtual address covered by VMA, this is the 1st two fields in /proc/<pid>/maps

**vm_file**

	pointer to strcut file if any associated with this

**vm_pgoff**

	Offset of the area in the file

**vm_flag**

	This describe this area. Flags for driver interest are
	VM_IO marks VMA as memory mapped IO region, this flag won't included in core dump
	VM_RESERVED marks VMS as reserved, meaning this VMA is not swapped out

**vm_ops**

	Set of function to operate on this memory region

**vm_private_data**

	Driver's own data

## vm_operations_struct

*<linux/mm.h>*

```c
struct vm_operations_struct {
	void (*open)(struct vm_area_struct *vma);
	void (*close)(struct vm_area_struct *vma);
	struct page *(*nopage)(struct vm_area_struct *vma, unsigned long address, int* type);
	int (*populate)(struct vm_area_struct *vm, unsigned long address, unsigned long len, pgprot_t prot, unsigned long pgoff, int nonblock);
}
```
These call back are needs to handle the process memory need

**open**

	- Kernel call this method to allow subsystem to initialize the VMA area
	- This method is called every time new reference to VMA is made
	- When the VMA is first created instead of calling this function, driver's mmap is called

**close**

	- Kernel call's this method when area is destroyed, there is no ref count in VMA

**nopage**

	- This method is called when a process tries to access this page in valid VMA, but the page is not available in memory
	- This method struct page pointer for the physical page after reading the secondary storage
	- If this method is not defined for area, empty page is allocated by kernel and return it

**populate**

	- This method allows kernel to prefault the page into memory