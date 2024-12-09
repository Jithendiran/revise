```c
struct firmware {
	size_t size;
	u8 *data;
};
```
**size**

	size of the firmware

**data**

	firmware data


For some devices need to download firmware before it functions.
Kernel should obtain from the user space. Should not open firmware file directly into kernel, Instead needs to use firmware interface

## request firmware

```c
int request_firmware(const struct firmware **fw, char *name,
struct device *device);
```

A call to request_firmware will request user space locate and provide firmware image into kernel
Name of the firmware is normally the firmware file eg: `my_firmware.bin`

### Async request firmware
```c
int request_firmware_nowait(struct module *module,
	char *name, struct device *device, void *context,
	void (*cont)(const struct firmware *fw, void *context));
```
**module**

	module fields, almost THIS_MODULE

**context**

	private data.

**cont**

	callback 

## Release firmware

```c
void release_firmware(struct firmware *fw);
```
After firmware is sent to device, needs to release the firmware into the kernel


## How it works

Firmware works with sysfs subsystem and hotplug mechanism.
When request_firmware is called new directory (`/sys/class/{firmware_by_device_name}`) is created with three attributes

### Attributes

**loading**

	Attribute should only set by user space that loading the firmware. When loading process is complete it set to 0, -1 will abort the loading process

**data**

	Is a binary attribute that receives the firmware data, after setting loading userspace will write to this attribute

**device**

	sysbolic link to entry under /sys/devices

Once sysfs entries have been created, kernel will generate a hotplug events for your device. env of the hotplug handler will have variable called `FIRMWARE` which is set to the name provided to request_firmware, Handler will locate firmware and copy to kernel by using the attributes. If handler cannot find the firmware handler should set -1 to the loading attribute.

By default timeout for request_firmware is 10 sec. If firmware request is not server with in the timeout, kernel will give up and sent failure status to the driver. To change timeout period use attribute `/sys/class/{firmware_by_device_name}/timeout`