Process memory map is made up of atleast the following:
	- program executable code `text` space
	- Data area's initialized data, uninitialized data (BSS) and the stack
	- One area for each active memory mapping

Process memory mapping is seen in `/proc/<pid>/maps`. For current process `/proc/self/maps`

The fields in each line are:

	start-end | perm | offset | major:minor | inode | image

eg: `# cat /proc/1/maps`
```
08048000-0804e000 r-xp 00000000 03:01 64652 /sbin/init text
....
```

These are stored in VMA

Virtual memory area (VMA) is the kernel data structure used to manage distinct region of process's address space.

VMA is a homogeneous region in virtual memory of process
	- contiguous range of memory that have same permission flag
	- Backed up by same process (file or swap space)

eg:
 all memory in 08048000-0804e000 having same permission, taken from same file,.. 

Each  VMA can be segments or files any thing.. 
simply `memory with it's own property` 

## Fields

	file can be elf, device file, any file,...

**start, end**

	Beginning and ending virtual address of this memory area.

**prem**

	Memory area premission. What the page allowed to do with the pages
	r - read, w - write, x - execute, p - private and s - shared

**offset**

	Memory area corresponds to file
	0 means memory beginning in beginning of file

**major, minor**

	The major and minor number of the device holding the file that is mapped. Refers to major and minor number of disk

**inode**

	inode number of mapped file

**image**

	Name of the file that has beem mapped

[[vm_area_struct]]