Sysfs follows hierarchy of kobject representation. Some files in different hierarchy needs to deal with some files in other hierarchy. 
For example `/sys/devices` represents all the devices and `/sys/bus` represents the device drivers. With the usual tree structure it is not possible to relate those files, to relate those files need to create a symbolic link

*Create a symbolic link*
`int sysfs_create_link(struct kobject *kobj, struct kobject *target, char *name);`

	kobj:  where the link will create. It follows kobj's sysfs hierarchy
	target: The link will point to which kobject
	name: symbolic link will created on this name

*Remove symbolic link*
`void sysfs_remove_link(struct kobject *kobj, char *name);`


[[Kobjects]]