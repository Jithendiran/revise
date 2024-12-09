The **`/dev`** folder in Linux is a special directory that contains device nodes (or special files) representing hardware and virtual devices. Before this directory is handled by `devfs`.

## Flaws of devfs
* It requires every device driver needs to be modified to support it.
* It requires that device driver needs to specify name and location with in `/dev`
* It not properly handled dynamic major and minor number
* It lacked in naming policy, It not allowed the user space to do with naming, It forces kernel to do it.

	 For Example: when 2 printers are connected in system 
	 - **First boot**: Printer A → `/dev/lpt0`, Printer B → `/dev/lpt1`
	 - **Second boot**: Printer B → `/dev/lpt0`, Printer A → `/dev/lpt1`
	User don't know which printer is lpt0... and other

## Rise of udev

So unified driver model is developed (udev), it allows user space to manage `/dev` tree in dynamic fashion. udev replies on the information exported to user space through sysfs and being notified by `/sbin/hotplug` when device is added or removed, naming policy is decided by user space 

To work with udev all the device driver needs to is ensure any major and minor number are assigned to a device controlled by driver are exported through sysfs to user space.
If  a driver is uses subsystem to assign major and minor number, this is already handled by subsystem
If  driver is handling getting major and minor number on it's own through `cdev_init` or `register_chrdev` it needs to be modify in order to work with udev

udev looks for a files called dev in `/class/` tree of sysfs to determine major and minor number of the device when it is called by `/sbin/hotplug`. A device driver needs to create that file for every device it controls

udev creates the device node with the help of dev file that is created by driver