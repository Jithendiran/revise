```c
struct class {
	char *name;
	struct class_attribute *class_attrs;
	struct class_device_attribute *class_dev_attrs;
	int (*hotplug)(struct class_device *dev, char **envp,
	int num_envp, char *buffer, int buffer_size);
	void (*release)(struct class_device *dev);
	void (*class_release)(struct class *class);
	...
};
```

This class is offer more flexibility.

## Fields
**name**

	Unique name of the class, shows in `/sys/class`

**class_attrs**

	This a list of default class attributes. When class is registered, these are created. This is null termnated array.

**class_dev_attrs**

	This is the list of default device attributes. When a device is added to this class, it will be created.

**hotplug**

	This call back is used to add env variabled when a hot event is generated

**release**

	This method is called when a device is removed from class.

**class_release**

	This is called when class is released.

## Registration

```c
int class_register(struct class *cls);
void class_unregister(struct class *cls);
```

## Attributes

```c
struct class_attribute {
	struct attribute attr;
	ssize_t (*show)(struct class *cls, char *buf);
	ssize_t (*store)(struct class *cls, const char *buf, size_t count);
};
```

*Compile time macro*

	CLASS_ATTR(name, mode, show, store);

Attributes file operations

```c
int class_create_file(struct class *cls, const struct class_attribute *attr);
void class_remove_file(struct class *cls, const struct class_attribute *attr);
```

Links
[[class_device]]