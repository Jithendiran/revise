[[Kobjects]] have set of default attributes defined in [[Kobject_type]] 
`kobj_type->default_attrs`
default attributes are list of attributes 

> [!IMPORTANT]
> Last entry of default_attrs list must be a zero-filled 

Implementation of attributes are defined in `kobj_type->sysfs_ops` 

[[sysfs_ops]]