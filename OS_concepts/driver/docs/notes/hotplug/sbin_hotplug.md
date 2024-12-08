This `/sbin/hotplug` program is used by kernel to notify user space when hotplug event occured. Kernel will call this program. This is a user space program. This program is called when ever kobject is created or deleted.

This program is simple bash script, that passes execution to other programs which paced in `/etc/hotplug.d`. In this folder it looks for `.hotplug` suffix files that interested in events. This program pass the number of environment variables passed by the kernel. Using this env variables the hotplug programs will take specific actions.

The command line arguments passed to `/sbin/hotplug` is the name associated with the hotplug event. It is determined by the Kset's [[Hotplug_events]] name function. If name function is not present its name is Kset's name.

## Default environment variables

**ACTION**

	Type of action. The string add or remove

**DEVPATH**

	Directory path for the kobject that is being created or destroyed

**SEQNUM**

	Seq number of this hotplug event. It is incremented for every hotplug event that is generated

**SUBSYSTEM**

	The same string is passed as command line argument

## Specific environment variables

Each bus type can also add it's specific own environment variables with the help of `hotplug` callback in Kset's hotplug_events struct

For eg:

**Networking**

	Parameter name and SUBSYSTEM is set to `net` and add it's own env's
	
		INTERFACE:  
			The name of the interface that has been registered or unregistered eg: lo, eth0

**PCI**

	Parameter name and SUBSYSTEM is set to `pci`. It's env variables are
	
		PCI_CLASS:  
			PCI class number for the device in hex
	
		PCI_ID:  
			PCI vendor and device ID's for device in hex format of `vendor:device`
	
		PCI_SUBSYS_ID:  
			PCI subsystem's vendor and device ID in the format of `subsys_vendor:subsys_device`
	
		PCI_SLOT_NAME:  
			This name is given by the kernel in the format of `domain:bus:slot:function` eg: `0000:00:0d.0.`


## Using /sbin/hotplug

Linux kernel is calling `/sbin/hotplug` for  every device added and removed from the kernel, user space has number of useful tools to make use of make use of that. In that Two are more important
* Linux hotplug script
* udev
[[Linux_hotplug_script]]
[[udev]]