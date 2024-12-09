```c
struct device {
	struct device *parent;
	struct kobject kobj;
	char bus_id[BUS_ID_SIZE];
	struct bus_type *bus;
	struct device_driver *driver;
	void *driver_data;
	void (*release)(struct device *dev);
	struct list_head  devres_head;
	...
};
```
At lowest level ever devices in system is represented by `struct device`. This struct contains the information that device model core needs to model the system.

## Fields
**parent**

	parent device is where the current device is attached to, in most cased parent device will be bus or host controller. If parent device is NULL then this device is the top level

**kobj**

	Kobject that represent's the device and it's hierarchy.
>[!note]
>`device -> kobj -> parent == &device -> parent -> kobj`

**bus_id**

	Unique identifier of the device on bus

**bus**

	Device's bus type

**driver**

	Driver that manages this device, `struct device_driver`

**driver_data**

	Private data used by device driver

**release**

	This method is called when last reference of the device is removed

**devres_head**

	The resources list of device


>[!important]
>Minimum of parent, bus_id, bus and release fields are must to set, before registration.
>For top level bus device parent and bus field are NULL


****

## Device registration
To register device  
	`int device_register(struct device *dev);`
	If bus is registered it will be visible like `/sys/devices/<bus_id>`
	if device is registered it will visible under `/sys/devices/<bus_id>/<device>`  

To un register device  
	`void device_unregister(struct device *dev);`


> [!Tip] 
> Actual bus `struct bus_type` is a device, that must be registered separately.


## Device attributes

```c
struct device_attribute {
	struct attribute attr;
	ssize_t (*show)(struct device *dev, char *buf);
	ssize_t (*store)(struct device *dev, const char *buf,
	size_t count);
};
```

*Compile time macro*  
`DEVICE_ATTR(name, mode, show, store);`  

*Attribute files*   

`int device_create_file(struct device *device,struct device_attribute *entry);`   

`void device_remove_file(struct device *dev, struct device_attribute *attr);`

>[!note]
>The `dev_attrs` of `struct bus_type` points to list of default attributes that created for every device added to that bus
>


links
[[Attributes]]
[[Buses]]
[[Device_driver]]
[[Kobjects]]