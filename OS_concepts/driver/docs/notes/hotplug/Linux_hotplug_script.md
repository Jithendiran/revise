This Linux hotplug script is the very first user of the `/sbin/hotplug` call. This script look at different env variable set by kernel to load the suitable kernel module for the driver.

when a driver uses `MODULE_DEVICE_TABLE`, macro `depmod` takes that information and create map files in `/lib/module/{kernel_version}/modules.*map`.

The hotplug script uses these map files to load the modules that is suitable for new device discovered by the kernel. They load all modules and do not stop at first match to find best suitable module. This script does not unload module when the device removed from kernel, that module may use by other device that could shutdown that device.

Modern **`modprobe`** no longer relies on the map files (`modules.*map`) because it can directly extract the information from the module binaries. `modprobe` program can read the `MODULE_DEVICE_TABLE` information directly from the module without the need of the map files, This script might be reduced to a small wrapper around the `modprobe` program.