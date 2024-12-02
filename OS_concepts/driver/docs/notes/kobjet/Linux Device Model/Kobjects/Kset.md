# Kset

![Big grey box is kset](image.png)

Big grey box is kset

```c
struct kset {
    struct kobject kobj;           // Embedded kobject, represents the kset in the kobject hierarchy.
    const struct kset_uevent_ops *uevent_ops; // Optional, defines user-space uevent behavior for this kset.
    struct list_head list;         // List of kobjects belonging to this kset.
};
```

Kset looked like an extension of [`kobj_type`](../Kobjects), kset is a collection of kobject embedded within structure of the same type. Every kobject represented in sysfs is a member of kset and it is not necessary every kobject show up in sysfs

kset is treated as container of kobject’s, kset have it’s own kobject

kset representation of sysfs is a directory

kset keeps it’s children in a kernel linked list

Contained kobjects also have pointer to kset in it’s parent’s field 

## Add kobject to kset

Adding of [[Kobjects]] to kset is done when the object is created. kobject’s kset is pointed to kset of interest and passed to add function `int kobject_add(struct kobject *kobj);` 

- kobject_add
    
    It will add directory in the sysfs, usually directories contains one or more [attributes](Attributes.md).
    

`extern int kobject_register(struct kobject *kobj); = kobject_init + kobject_add` 

kobject_add will increase the kobject’s reference count

## Delete kobject from kset

At some point [[Kobjects]] needs to be removed from kset to clear the reference, to do that  `void kobject_del(struct kobject *kobj);` 

`kobject_unregister = kobject_del + kobject_put`

kset also have pointer to kobj_type in it’s kobjects describing the kobject it contains,  as a result of this kobj_type is left NULL in contained kobjet because same field with in the kset is used

## Operations

```c
// set up operations
void kset_init(struct kset *kset);
int kset_add(struct kset *kset);
int kset_register(struct kset *kset);
void kset_unregister(struct kset *kset);

// reference count operations
struct kset *kset_get(struct kset *kset);
void kset_put(struct kset *kset);

// To set kset's internel kobject name
kobject_set_name(&my_set->kobj, "The name");
```

[subsystem](Subsystem.md)