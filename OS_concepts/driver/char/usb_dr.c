/**
 * Author: E.K. Jithendiran
 * Date  : 20 Nov 24
 */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/usb.h>
#include <asm/uaccess.h>

// To view list of devices cat this file /usr/share/hwdata/usb.ids
// 0xfff0 is used for debugger
#define USB_SKEL_VENDOR_ID	0xfff0 
#define USB_SKEL_PRODUCT_ID	0xfff0

/* table of devices that work with this driver */
static struct usb_device_id skel_table [] = {
	{ USB_DEVICE(USB_SKEL_VENDOR_ID, USB_SKEL_PRODUCT_ID) },
	{ }					/* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, skel_table); // expose to user space and used for hot plug and play

#define USB_SKEL_MINOR_BASE	192

/**
 * use device // struct usb_device
 *      usb config 1  // at a time only one config will be active, rarly linux can handle multiple config at a time
 *                    // struct usb_host_config
 *              interface 1 // any number of device can active
 *                          // struct usb_interface
 *                  End point 1 // any number of endpoint can actice
 *                              // struct usb_host_endpoint
 *                  End point 2
 *                  End point 3
 *              interface 2
 *                  End point 1
 *                  End point 2
 *      usb config 2
 *              interface 1
 *                  End point 1
 *                  End point 2
 *              interface 2
 *                  End point 1
 * 
 */

/**
 * struct usb_device {
 *  struct  usb_host_endpoint control; // every usb have one endpoint, that is control endpoint, that is ep0 
 *  struct usb_host_config config, actconfig{ // config
 *      struct usb_interface interface { // interface 
 *              struct usb_host_interface altsetting, cur_altsetting { 
 *                  struct  usb_host_endpoint endpoint;// endpoint
 *              }
 *      }
 *  }
 * }
 * 
 */

struct usb_skel {
	struct usb_device *	udev;			/* the usb device for this device */
	struct usb_interface *	interface;		/* the interface for this device */
	unsigned char *		bulk_in_buffer;		/* the buffer to receive data */
	size_t			bulk_in_size;		/* the size of the receive buffer */
	__u8			bulk_in_endpointAddr;	/* the address of the bulk in endpoint */
	__u8			bulk_out_endpointAddr;	/* the address of the bulk out endpoint */
	struct kref		kref;					/* atomic counting reference*/
};

static struct usb_driver skel_driver; // probe, disconnect,.. callbacks

// -------------------------------------------------------------------

static int skel_open(struct inode *inode, struct file *file)
{
	info("SKL Open...");
	struct usb_skel *dev;
	struct usb_interface *interface;
	int subminor;
	int retval = 0;

	subminor = iminor(inode);

	interface = usb_find_interface(&skel_driver, subminor);
	if (!interface) {
		err ("%s - error, can't find device for minor %d",
		     __FUNCTION__, subminor);
		retval = -ENODEV;
		goto exit;
	}

	dev = usb_get_intfdata(interface);
	if (!dev) {
		retval = -ENODEV;
		goto exit;
	}
	
	/* increment our usage count for the device */
	kref_get(&dev->kref);

	/* save our object in the file's private structure */
	file->private_data = dev;

	info("SKL OPEN done...");

exit:
	return retval;
}

static ssize_t skel_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
	info("SKL READ Start...");
	struct usb_skel *dev;
	int retval = 0;

	dev = (struct usb_skel *)file->private_data;
	
	/* do a blocking bulk read to get data from the device */
	retval = usb_bulk_msg(dev->udev, /*device*/
			      usb_rcvbulkpipe(dev->udev /*device*/, dev->bulk_in_endpointAddr /* Endpoint */), /*pipe*/
			      dev->bulk_in_buffer, /* data */
			      min(dev->bulk_in_size, count), /*size of data to copy*/
			      &count, /* data have been copied*/ 
				  HZ*10 /* wait time out*/
				  );

	/* if the read was successful, copy the data to userspace */
	if (!retval) {
		if (copy_to_user(buffer, dev->bulk_in_buffer, count))
			retval = -EFAULT;
		else
			retval = count;
	}
	info("SKL READ End...");
	return retval;
}

/*
The completion handler of the urb is called exactly once when the urb is completed. When this
function is called, the USB core is finished with the URB, and control of it is now
returned to the device driver.
*/
static void skel_write_bulk_callback(struct urb *urb, struct pt_regs *regs)
{
	info("SKL WRITE Call back start...");
	/* sync/async unlink faults aren't errors */
	if (urb->status &&  /*status 0 if transfer success*/
	    !(urb->status == -ENOENT || 
	      urb->status == -ECONNRESET ||
	      urb->status == -ESHUTDOWN)) {
		dbg("%s - nonzero write bulk status received: %d",
		    __FUNCTION__, urb->status);
	}

	/* free up our allocated buffer */
	usb_buffer_free(urb->dev, urb->transfer_buffer_length, 
			urb->transfer_buffer, urb->transfer_dma);
	
	info("SKL Write Call back end...");
}

static ssize_t skel_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos)
{
	info("SKL write start...");
	struct usb_skel *dev;
	int retval = 0;
	struct urb *urb = NULL;
	char *buf = NULL;

	dev = (struct usb_skel *)file->private_data;

	/* verify that we actually have some data to write */
	if (count == 0)
		goto exit;

	/* create a urb, and a buffer for it, and copy the data to the urb */
	urb = usb_alloc_urb(0 /*iso_packet*/, GFP_KERNEL /*mem flag*/);
	if (!urb) {
		retval = -ENOMEM;
		goto error;
	}

	buf = usb_buffer_alloc(dev->udev, count, GFP_KERNEL, &urb->transfer_dma);
	if (!buf) {
		retval = -ENOMEM;
		goto error;
	}
	if (copy_from_user(buf, user_buffer, count)) {
		retval = -EFAULT;
		goto error;
	}

	/* initialize the urb properly */
	usb_fill_bulk_urb(urb,/*urb*/ 
					dev->udev,/*device*/
			  		usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr), /*pipe*/
			  		buf, /*transfer buffer*/ 
			  		count, /*needs to copy length*/ 
			  		skel_write_bulk_callback, /*complete callback*/
			  		dev /*context*/);
	urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	/* send the data out the bulk port */
	retval = usb_submit_urb(urb, GFP_KERNEL);
	if (retval) {
		err("%s - failed submitting write urb, error %d", __FUNCTION__, retval);
		goto error;
	}

	/* release our reference to this urb, the USB core will eventually free it entirely */
	usb_free_urb(urb);

	info("SKL write end...");

exit:
	return count;

error:
	info("SKL write error...");
	usb_buffer_free(dev->udev, count, buf, urb->transfer_dma);
	usb_free_urb(urb);
	kfree(buf);
	return retval;
}


static int skel_release(struct inode *inode, struct file *file)
{
	struct usb_skel *dev;

	dev = (struct usb_skel *)file->private_data;
	if (dev == NULL)
		return -ENODEV;

	/* decrement the count on our device */
	kref_put(&dev->kref, skel_delete);
	return 0;
}


// ---------------------------------------------------------------------

#define to_skel_dev(d) container_of(d, struct usb_skel, kref)


static struct file_operations skel_fops = {
	.owner =	THIS_MODULE,
	.read =		skel_read,
	.write =	skel_write,
	.open =		skel_open,
	.release =	skel_release,
};

/* 
 * usb class driver info in order to get a minor number from the usb core,
 * and to have the device registered with devfs and the driver core
 */
static struct usb_class_driver skel_class = {
	.name = "usb/skel%d", // name of the file in sysfs
	.fops = &skel_fops, // file operations
	.mode = S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH, // sysfs's file permission crw-rw-r--
	/**
	 * CHR -> char, USR -> USER, GRP -> GROUP and OTH -> OTHERS, S -> SET, I -> INODE
	 * S_IFCHR -> char based device
	 * S_IRUSR -> Read perm for owner
	 * S_IWUSR -> write for owner
	 * ...
	 */
	.minor_base = USB_SKEL_MINOR_BASE, // minor number of device
};

static void skel_delete(struct kref *kref)
{	
	struct usb_skel *dev = to_skel_dev(kref);

	usb_put_dev(dev->udev); // decrement the reference count of a USB device, part of  USB Core API 
	kfree (dev->bulk_in_buffer);
	kfree (dev);
}

static int skel_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	info("SKL PROB...");
	struct usb_skel *dev = NULL;
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	size_t buffer_size;
	int i;
	int retval = -ENOMEM;

	/* allocate memory for our device state and initialize it */
	dev = kmalloc(sizeof(struct usb_skel), GFP_KERNEL);
	if (dev == NULL) {
		err("Out of memory");
		goto error;
	}

	memset(dev, 0x00, sizeof (*dev));

	// init atomic ref
	kref_init(&dev->kref);

	// usb_get_dev ->  Increment the reference count of a USB device, part of  USB Core API 
	dev->udev = usb_get_dev(interface_to_usbdev(interface));	// interface will have usb device
	dev->interface = interface;

	/* set up the endpoint information */
	iface_desc = interface->cur_altsetting; // active interface
	for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
		endpoint = &iface_desc->endpoint[i].desc;

		/* use only the first bulk-in and bulk-out endpoints */
		if (!dev->bulk_in_endpointAddr  &&  /* Empty ref*/
 		    (endpoint->bEndpointAddress & USB_DIR_IN)  &&  /* In */
		    ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) /* BULK */
					== USB_ENDPOINT_XFER_BULK)) {
			/* we found a bulk in endpoint */
			buffer_size = endpoint->wMaxPacketSize;
			dev->bulk_in_size = buffer_size;
			dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
			dev->bulk_in_buffer = kmalloc(buffer_size, GFP_KERNEL);
			if (!dev->bulk_in_buffer) {
				err("Could not allocate bulk_in_buffer");
				goto error;
			}
		}

		if (!dev->bulk_out_endpointAddr && /* Empty ref*/
		    !(endpoint->bEndpointAddress & USB_DIR_IN) && /* Out */
		    ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) /* BULK */ 
					== USB_ENDPOINT_XFER_BULK)) {
			/* we found a bulk out endpoint */
			dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;
		}
	}
	if (!(dev->bulk_in_endpointAddr && dev->bulk_out_endpointAddr)) {
		err("Could not find both bulk-in and bulk-out endpoints");
		goto error;
	}

	/* save our data pointer in this interface device */
	usb_set_intfdata(interface, dev);

	/* we can register the device now, as it is ready */
	retval = usb_register_dev(interface, &skel_class);
	if (retval) {
		/* something prevented us from registering this driver */
		err("Not able to get a minor for this device.");
		usb_set_intfdata(interface, NULL);
		goto error;
	}

	/* let the user know what node this device is now attached to */
	info("USB Skeleton device now attached to USBSkel-%d", interface->minor);
	return 0;

error:
	if (dev)
		// decrement ref and delete
		kref_put(&dev->kref, skel_delete);
	return retval;
}

static void skel_disconnect(struct usb_interface *interface)
{
	info("SKL disconnect ...");
	struct usb_skel *dev;
	int minor = interface->minor;

	/* prevent skel_open() from racing skel_disconnect() */
	// lock_kernel();

	dev = usb_get_intfdata(interface);
	usb_set_intfdata(interface, NULL);

	/* give back our minor */
	usb_deregister_dev(interface, &skel_class);

	// unlock_kernel();

	/* decrement our usage count */
	kref_put(&dev->kref, skel_delete);

	info("USB Skeleton #%d now disconnected", minor);
}

static struct usb_driver skel_driver = {
	.owner = THIS_MODULE,
	.name = "skeleton",
	.id_table = skel_table,
	.probe = skel_probe,
	.disconnect = skel_disconnect,
};





// -----------------------------------------------------------------
static int __init usb_skel_init(void)
{
	int result;

	/* register this driver with the USB subsystem */
	result = usb_register(&skel_driver);
	if (result)
		err("usb_register failed. Error number %d", result);

	return result;
}

static void __exit usb_skel_exit(void)
{
	/* deregister this driver with the USB subsystem */
	usb_deregister(&skel_driver);
}

module_init (usb_skel_init);
module_exit (usb_skel_exit);

MODULE_LICENSE("GPL");