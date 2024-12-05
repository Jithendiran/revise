This created in intention of easy use. Every member in this class needs to be export at least a minimum attribute containing device number, using this interface is just a matter of function calls, usually a boilerplate codes

## Class Registration

*Create class_simple*

	`struct class_simple *class_simple_create(struct module *owner, char *name);` 
	It creates the class in the given name.

*Destroy class_simple*

	`void class_simple_destroy(struct class_simple *cs);`

## Device operations

*Add device to class*

	`struct class_device *class_simple_device_add(struct class_simple *cs, dev_t devnum, struct device *device, const char *fmt, ...);`   
	cs: ceated class
	devnum: assigned device number
	device: Struct that represent's the device. If it is not null it will create a symbolic link points to divice entry under /sys/devices
	fmt: variable arguments to create name of the device
	
	This will add entry to the class containing one attribute called dev which holds the device number

*To remove device*

	`void class_simple_device_remove(dev_t dev);`

## Other Attributes

*To add other attributes to the device entry*

	`int class_device_create_file(struct class_device *cls, const struct class_device_attribute *attr);`

*Remove attributes*

	`void class_device_remove_file(struct class_device *cls, const struct class_device_attribute *attr);`

## Hotevents

Classes generates hot events when device come and go 

*Add env*

`int class_simple_set_hotplug(struct class_simple *cs, int (*hotplug)(struct class_device *dev, char **envp, int num_envp, char *buffer, int buffer_size))`

	This will setup a callback to add the env to user-space events


Links
[[class_device]]