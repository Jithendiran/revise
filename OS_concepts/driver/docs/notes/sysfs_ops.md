```c
struct sysfs_ops {
	ssize_t (*show)(struct kobject *kobj, struct attribute *attr,
char *buffer);
	ssize_t (*store)(struct kobject *kobj, struct attribute *attr,
const char *buffer, size_t size);
};
```

Same show and store method for all the attributes, operations for each attributes must be determined by `*attr` parameter

**show**

	- When a read operation is performed from user space, show method is called with the pointers
	- To send data to user need to use buffer, size of the buffer must not exceed `PAGE_SIZE` byte
	- Return the actual length of the buffer

**store**

	- Rquired Write permission  
	- It is to write to kernel or modify parameter, buffer contains user space data
	- size is the size of buffer, It should not exceed `PAGE_SIZE`


[[Kobjects]]
