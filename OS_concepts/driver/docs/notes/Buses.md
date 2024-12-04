```c
struct bus_type {
	char *name;
	struct subsystem subsys;
	struct kset drivers;
	struct kset devices;
	int (*match)(struct device *dev, struct device_driver *drv);
	struct device *(*add)(struct device * parent, char * bus_id);
	int (*hotplug) (struct device *dev, char **envp, int num_envp, char *buffer, int buffer_size);
...
};
```
Bus is a channel between processor and one or device.  In device model all the devices are connected with bus, even if it is a internal virtual bus.
Buses can plug into each others

**name**

	name of the bus, eg: pci

**subsys**

	This subsystem is bus subsystem, not the top level. It is usually found underneth the bus.

**driver**

	Known drivers for this bus

**device**

	Connected deviced into this bus

## Bus Registration

New bus must be register with the system by calling `bus_register(&ldd_bus_type);` This function can fail. If success the new function will be added to system and visible under `/sys/bus`. Then it is ready for adding devices, divers.

It is necessary to remove the bus
`void bus_unregister(struct bus_type *bus);`

## Bus Methods

These methods are serve between device core and individual driver

**match**

	This methods is called each time when ever new device or driver added to this bus. If driver can handle the device it should return non-zero value. 
	Match function usually compare between hardware ID of device and ID's supported by drivers

**hotplug**

	This method allows the bus to add environment values prior to the generation of the hotplug vents in user space.

### Iteration over device and driver

To perform the operations to all the device and driver connected to the bus

*To operate on every device*

	`int bus_for_each_dev(struct bus_type *bus, struct device *start,void *data, int (*fn)(struct device *, void *));`
	This function iterate over every device on the bus, pass the associated device structure to the fn callback. If start is NULL it will start from begining, else iterations begins from next to the start. Id fn return's non-zero value iteration will stop and bus_for_each_dev will return that value

*To operate over every drivers*

	This works like function for device.

> [!caution]
> Both function are holding the subsystem semaphore when doing the operations



## Bus Attributes
```c
struct bus_attribute {
	struct attribute attr;
	ssize_t (*show)(struct bus_type *bus, char *buf);
	ssize_t (*store)(struct bus_type *bus, const char *buf,
	size_t count);
};
```
*<linux/device.h>*

Refer [[Attributes]].

Macro for compile time creation and initialization of bus_attribute: `BUS_ATTR(name, mode, show, store);`

### Sysfs files

To create attribute file 
`int bus_create_file(struct bus_type *bus, struct bus_attribute *attr);`

To remove attribute file
`void bus_remove_file(struct bus_type *bus, struct bus_attribute *attr);` 

`/sys/bus/<name_of_the_bus>/<name_of_the_attr_file>`

links
[[Kset]]
[[Subsystem]]
[[Attributes]]