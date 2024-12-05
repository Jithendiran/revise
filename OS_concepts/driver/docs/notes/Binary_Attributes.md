```c
struct bin_attribute {
	struct attribute attr;
	size_t size;
	ssize_t (*read)(struct kobject *kobj, char *buffer,
	loff_t pos, size_t size);
	ssize_t (*write)(struct kobject *kobj, char *buffer,
	loff_t pos, size_t size);
};
```
Usual Attributes are to contain single human readable values, In some rare cases we need to handle large chunks of binary data in this case bin_attributes will help. For example some devices need to send firmware code from server, in that time server will make use of this attribute and send the firmware binary data

* [[Attributes]] is usual attribute field
* Size is the maximum size of binary attribute, 0 means no maximum size.
* Read and write call backs work's similar like char driver's

>[!help]
> Since there is no way for sysfs to signal the last set of write operations, our code need to determine that
> 

Binary attributes must be created explicitly

To create binary attribute file
`int sysfs_create_bin_file(struct kobject *kobj, struct bin_attribute *attr);`

To remove binary attribute file
`int sysfs_remove_bin_file(struct kobject *kobj, struct bin_attribute *attr);` 

[[Kobjects]]