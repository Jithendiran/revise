Class is a high-level view device that abstract low level implementations. Class allows users to what they want to do rather than how it work or how it connected. Class members are usually handled by high-level code, rather than need of explicit support from driver. There are some times drivers dealing with classes directly.

>[!tip]
>Real purpose of class is to serve as a container of the devices that are member of class


	Eg: Driver may see SCSI disk or ATA disk, bust for class it is simply a disk

*Location*
 Usual class will be under `/sys/class/`. Other than block devices `/sys/block/` due to historical reason.

	Eg: /sys/class/net (network), /sys/class/input (Input), ...

The class subsystem is the best way to export information to user space. When subsystem create class it own entire thing rather than modules own individual parts, So it enables searching for device faster

To manage class driver core exports two interfaces

* `class_simple`: 
	It is designed to add new class to the system as easy as possible. This class expose attributes containing device numbers that enables the automatic creation of the device node

* `class_regular`:
	It is complex one but it offers more features as well


[[class_simple]]
[[Regular_class]]
[[class_interface]]