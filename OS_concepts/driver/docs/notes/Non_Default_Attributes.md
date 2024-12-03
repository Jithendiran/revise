Attributes can be add at any will, it is not restricted to used default attributes

**Add new attribute to kobject's sys directory**  

`int sysfs_create_file(struct kobject *kobj, struct attribute *attr);`   

	*kobj -> New attribute will be added to this kobject  
	*attr -> This the attribute to be added  

`kobj_type->sysfs_ops`'s same show and store are used 

`int sysfs_remove_file(struct kobject *kobj, struct attribute *attr);`

	Will remove the attr from the given attributes

>[!caution]
>User-space application could have open descriptor for the removed attribute. Show and store call is possible
