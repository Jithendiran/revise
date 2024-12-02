# Kobject_type

```c
struct kobj_type {
	void (*release)(struct kobject *);
	struct sysfs_ops *sysfs_ops;
	struct attribute **default_attrs;
};
```

This kobject_type reference is found in two ways

- Kobject itself contains the kobj_type reference
- If Kobject is a member of Kset, then kobj_type reference is provided by kset instead of Kobject