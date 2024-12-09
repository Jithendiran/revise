```c
struct class_device {
	struct kobject kobj;
	struct class *class;
	struct device *dev;
	void *class_data;
	char class_id[BUS_ID_SIZE];
};
```

Device members of a class is represented in `class_device` structure.

## Fields

**class_id**

	Name of the device appear in sysfs.

**class**

	Points to the class holding this device.

**dev**

	Point to the associated device structure. It is a optional. If non-NULL it is used to created symbolic link from the class enry to the corresponding entry under /sys/devices, it will makes easy for searching

**class_data**

	Private pointer data


## Registration

```c
int class_device_register(struct class_device *cd);
void class_device_unregister(struct class_device *cd);
```

## rename existing class  

`int class_device_rename(struct class_device *cd, char *new_name);`

## Attributes

```c
struct class_device_attribute {
	struct attribute attr;
	ssize_t (*show)(struct class_device *cls, char *buf);
	ssize_t (*store)(struct class_device *cls, const char *buf,
	size_t count);
};
```

*Compile macro*

`CLASS_DEVICE_ATTR(name, mode, show, store);`

*File operations*

```c
int class_device_create_file(struct class_device *cls,
	const struct class_device_attribute *attr);
void class_device_remove_file(struct class_device *cls,
	const struct class_device_attribute *attr);
```

[[Kobjects]]