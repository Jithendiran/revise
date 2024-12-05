```c
struct class_interface {
	struct class *class;
	int (*add) (struct class_device *cd);
	void (*remove) (struct class_device *cd);
};
```

This is used to send notification when a device enter or leave the class.

## Fields

**class**

	Class to which it points to

**add**

	When ever a device is added to a class, this call back will be called

**remove**

	When ever a device is removed from class, this call back will be called

## Registration

```c
int class_interface_register(struct class_interface *intf);
void class_interface_unregister(struct class_interface *intf);
```

>[!Note]
>Multiple `class_interface`  can be register to a class
