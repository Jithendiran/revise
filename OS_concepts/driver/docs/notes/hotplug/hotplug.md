The term hotplug means ability to handle device appearance and disappear while the system is running

Hotplugs are viewed as two types
* *Kernel view* - It view as interaction between kernel, hardware and the kernel driver.
* *User view* - It viewed as interaction between kernel and user space, through `/sbin/hotplug`

Each different bus handles it's loss of device differently  

	For example: When PCI device is removed, before the driver is notified through it's `remove` function, all reads from the PCI bus return all bits set. So every read operation needs to be verified


```c
result = readl(ptr);
if (result = = ~(u32)0)
	return -ENODEV;
```

	When USB device is removed, It's pending submitted urbs will start failing with error -ENODEV. USB Driver needs to handle this

[[sbin_hotplug]]