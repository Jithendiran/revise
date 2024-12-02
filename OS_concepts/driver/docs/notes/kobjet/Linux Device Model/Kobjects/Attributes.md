# Attributes

[[Kobjects]] are the mechanism behind sysfs virtual file system. For every directory in sysfs there is a kobject in kernel. Every kobject have zero or more attributes. Attributes are the files that are interact with user, these files are kernel generated information(give info’s on fly)

*<linux/sysfs.h>*

[`kobject_add`](Kset.md) will add kobject to kset and also create entries in sysfs 

- kobjects are always a directory, it can be a empty directory with out attributes
- Name of the kobject directory is name of the kobject that set with  [`kobject_set_name`](../Kobjects) . Name of the kobject in same level must be unique

### Directory Location

- If parent kobject and Kset is null, this kobject is the top-level
- Kobject directory hierarchy is corresponding to parent kobjects.
- If parent is null when added to kset. It is set to the new kset’s embedded kobject, so hierarchy is matches kset’s hierarchy.