Hotplug events are notifications sent by the kernel to the user space, when something got changed in system's configuration, like when [[Kobjects]] created or destroyed. When kobject passed to `kobject_add` or `kobject_del` event will generate 

	For example: When a digital camera is plugged into USB cable, when user switches console modes,..

Hotplug events are turned to `/sbin/hotplug` invocation, which is respond to each and every events by loading drivers, creating device node or taking appropriate actions

Before events are handed over to user space, code associated with kobject particularly [[Kset]] has the opportunity to add information or to disable the event for user space 

# Hotplug Operations

```c
struct kset_hotplug_ops {
	int (*filter)(struct kset *kset, struct kobject *kobj);
	char *(*name)(struct kset *kset, struct kobject *kobj);
	int (*hotplug)(struct kset *kset, struct kobject *kobj, char **envp, int num_envp, char *buffer, int buffer_size);
};
```
>[!note]
This struct is defined in kset, If given kobject is not contained in kset, it will search in parent hierarchy until it finds a kobject that does have kset, that kset's kset_hotplug_ops will use

**filter**  

	This callback will be called when kernel is considered to generate a event for given kobject, This method will decide to send event to user space or not. If return 0 event won't generate

 **name**  
 
	 It provides the name of the relevent subsystem to user space

**hotplug**  

	This will pass the additional information needed by the user space script. envp is mentioned in this format `NAME=value`, in end need to add `NULL` . If returns 0 call is success, any non-zero call is aborted


 > [!IMPORTANT]  
> Order of the flow is `filter` -> `name`->`hotplug`