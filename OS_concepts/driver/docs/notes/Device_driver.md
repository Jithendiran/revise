```c
struct device_driver {
	char *name;
	struct bus_type *bus;
	struct kobject kobj;
	struct list_head devices;
	int (*probe)(struct device *dev);
	int (*remove)(struct device *dev);
	void (*shutdown) (struct device *dev);
};
```
The device model tracks all the driver know to the system, this helps in matching up the driver for new devices

*<linux/device.h>*
## Fields
**name**

	name of the driver that shows in sysfs.

**bus**

	Type of the bus this driver works with

**kobj**

	Kobject of the driver

**devices**

	List of all the devices currently bound to this driver

**probe**

	This call back is to check whether particular device will work with this device or not

**remove**

	This call back will called when the driver is removed from system

**shutdown**

	This callback will called when driver becomes quiet

## Driver Registration

Registration functions
```c
int driver_register(struct device_driver *drv);
void driver_unregister(struct device_driver *drv)
```

## Driver Attributes

```c
struct driver_attribute {
	struct attribute attr;
	ssize_t (*show)(struct device_driver *drv, char *buf);
	ssize_t (*store)(struct device_driver *drv, const char *buf,
	size_t count);
};
```

*Compile time macro*  
	`DRIVER_ATTR(name, mode, show, store);`

*Attribute file operations*

```c
int driver_create_file(struct device_driver *drv, struct driver_attribute *attr);
void driver_remove_file(struct device_driver *drv, struct driver_attribute *attr);
```

After registration drivers found `/sys/bus/<bus_name>/drivers/<driver_name>` 

>[!note]
>`struct bus_type` contain a field called `drv_attrs`, that points to the set of default attributes that is created for all drivers for associated bus
>

Links
[[Attributes]]
[[Buses]]
[[Kobjects]]