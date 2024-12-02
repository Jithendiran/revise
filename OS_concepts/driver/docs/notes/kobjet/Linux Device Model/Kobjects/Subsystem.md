# Subsystem

```c
struct subsystem {
	struct kset kset;
	struct rw_semaphore rwsem;
};
```

Subsystem is a high-level portion of the kernel, subsystem usually shown in the top of the sysfs hierarchy. It is a wrapper around [kset](Kset.md). Every kset must belongs to subsystem

<aside>
💡

Ex:  block_subsys (block subsystem) on `/sys/block` for block devices, devices_subsys (device subsystem) on  `/sys/devices` for the core device hierarchy,..

</aside>

kset can belongs to multiple subsystem

From kset it is possible to find it’s all subsystem, but it not possible to find all the kset’s that belongs to subsystem 

One can find kset’s sub-system from kset→kobj→bus_type→kset (The kset representing the subsystem)

To declare subsystem `decl_subsys(name, struct kobj_type *type,
struct kset_hotplug_ops *hotplug_ops);` 

The above macro will create a subsystem with name given to the macro and appending *_subsys to it.* This macro also create the internam kset with given type and hotplugops

```c
oid subsystem_init(struct subsystem *subsys);
int subsystem_register(struct subsystem *subsys);
void subsystem_unregister(struct subsystem *subsys);
struct subsystem *subsys_get(struct subsystem *subsys)
void subsys_put(struct subsystem *subsys);
Most of these operations just act upon the subsystem’s kset.
```