# Kobjects

```c
struct kobject {
	const char		                *name;
	struct list_head	            entry;
	struct kobject		            *parent;
	struct kset		                *kset;
	const struct kobj_type	      *ktype;
	struct kernfs_node	          *sd; /* sysfs directory entry */
	struct kref		                kref;
	...
};
```

It is the very basic fundamental data structure. Initially it was developed for reference counting and by years of requirements it’s functionality also increased. Kobject exists only to tie high-level objects into the device model. If view in Object oriented terms, Kobjects is the top-level class (Parent of all) which is used to derive other classes. C is lack in OOPS, so it uses struct

```c
struct cdev {
	struct kobject kobj;
	struct module *owner;
	struct file_operations *ops;
	struct list_head list;
	dev_t dev;
	unsigned int count;
};
```

Tasks handled by Kobjects

- Reference counting of objects
    
    It will count the object references from the time of creation, when a object is used some where it’s count is increased. When no code holds the reference of object it can be delete able.
    
    - Reference Count
        
        As long as the reference count exists, Kobject continue to exists
        
        Function to manipulate Reference count
        
        - **Increment**
            
            `struct kobject *kobject_get(struct kobject *kobj);` Increment the reference count to 1 and return the kobject, if kobject already destroyed it will return NULL
            
        - **Decrement**
            
            `void kobject_put(struct kobject *kobj);` It will decrement the reference count to 1 and possibly free the object. Need to make sure call this function for kobject_init
            
        - **Module**
            
            The reference count of kobject only protect the kobject, not it’s context structure, In our example the cdev’s kobject only protects the kobject of cdev, it won’t protect cdev to be unload when using it. So to protect the cdev need to increment the reference count of the it’s module
            
            ```c
            struct kobject *cdev_get(struct cdev *p)
            {
            	struct module *owner = p->owner;
            	struct kobject *kobj;
            	if (owner && !try_module_get(owner)/*Increment the ref of it's module*/)
            		return NULL;
            	kobj = kobject_get(&p->kobj);
            	if (!kobj)
            		module_put(owner);
            	return kobj;
            }
            ```
            
- Sysfs representation
    
    In sysfs virtual file system Kobject is interact with kernel to create a visual representation.
    
- Data structure glue
    
    Device model made up of many data structure, Kobject is holding the different data structure together
    
- Hotplug event handling
    
    Hot plug events notification of user space is generated by Kobject subsystem
    
- Hierarchies
    
    Kobjects are linked by two ways
    
    - Parent Pointer
        
        The parent field in struct kobject is point to another kobject. For example USB Kobject’s parent is connected to USB HUB’s Kobject. Use of this parent pointer is used in sysfs to position the object in a hierarchies
        
    - Kset
        
        It is a separate structure [kset](Kset.md)
        

*struct kobject; <linux/kobject.h>*

## Lifecycle

## Initialization

Initialization of Kobject is more complicated especially when all it’s functions are used,instead of how Kobject are used

### Steps

1. Set is entire Kobject to zero, failure in this step can cause problem
2. Set the internal fields with  `void kobject_init(struct kobject *kobj);` .This function set’s reference count to 1
3. Set name by `int kobject_set_name(struct kobject *kobj, const char *format, ...);` . This is the name used in sysfs entries

## Release

The reference count is not under the direct control of code which it created, reference can go to 0 at any point of time, so the code must be notified by asynchronous way when the reference count is goes 0

The notification is done through release call back, so every kobject must have a release function.

Kobject’s release function is not stored on kobject itself, it is stored in [Kobject_type](Kobject_type.md)

kobject_type concerns with type of the object, kset concerns with the collection and aggregation.

[Kobject_type](Kobject_type.md)

[Kset](Kset.md)

[Attributes](Attributes.md)