/**
 * Author: E.K. Jithendiran
 * Date  : 17 Mar 24
 */

// file operations and Important data structure
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include "scull.h"

dev_t deb_num = MKDEV(27, 0);
struct cdev *my_cdev;

// For scull data structure see scull.h file

// Important data structure
// file_operation: It has the important callback functions like: open, read, write,....
// file    : It is different from user space FILE pointer, 
//           It is passed as single pointer reference for for every file opened in kernel. From 1st open to last close
//           It has pointer like: private_data, file_operation,..
// inode   : inode has many field, for driver only 2 feild is helpful
//           dev_t -> has the device number
//           cdev  -> It is reference to internal character deice 
//----------------------------------
// parameter and return type of read & write
// filp    : file referenece
// count   : request size to transfer
// buff    : It points to the user buffer, where it holds data to be write or this is the buffer where read data to be placed
// offp    : It is long offset type object that indicate the file position
// ssize_t : signed size type

/**
 * buff
 *  Therefore, it cannot be directly dereferenced by kernel code. There are a few reasons for this restriction.
 *    - The user-space pointer may not be valid while running in kernel
 *      mode at all. There may be no mapping for that address, or it could point to some other, random data.
 *      
 *    - Even if the pointer does mean the same thing in kernel space, user-space memory
 *      is paged, and the memory in question might not be resident in RAM when the system call is made.
 *      Attempting to reference the user-space memory directly could generate a page fault,
 *      which is something that kernel code is not allowed to do
 * 
 * This access must always be performed by special, kernel-supplied functions, however, in order to be safe (<asm/uaccess.h>)
 * The code for read and write in scull needs to copy a whole segment of data to or from the user address space.
 * unsigned long copy_to_user(void __user *to, const void *from, unsigned long count); copy data from device to user space
 * unsigned long copy_from_user(void *to, const void __user *from, unsigned long count); copy data from user space to device
 * return value is amount of data to be copy. if 0 every thing is read, A negative value means there was an error
 * Whatever the amount of data the methods transfer, they should generally update the file position at *offp to represent the current file position after successful completion of the system call
 * The error does not get reported until the next time the function is called
 * 
 * __copy_to_user and __copy_from_user is perform the same operation of the above functions, but it will not check valid pointer
 * 
*/

/**
 * open function is responsible for below things
 *  - Check for device specific errord
 *  - Initilize the device if it is opened for 1st time
 *  - update f_op pointer, if necessary
 *  - Allocate and fill any data structure to be put in filp->private_data (it is in scull data structure)
 * 
 */

int open_callback(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "open method\n");
    struct scull_dev *dev; /* device information */
    /**
     * To get scull_dev from inode and cdev, container_of macro function is used
     * scull_dev is stored inside private_data
    */
    // to know working of container_of see container_of.c 
    dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    filp->private_data = dev; /* for other methods */
    
    return 0;
}

/**
 * This function will deallocate anything that allocateed in filp->private_data
 * Shutdown device on last close 
*/
int releas_callback(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "release method\n");
    return 0;
}

ssize_t read_callback(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    printk(KERN_INFO "read method\n");
    return 1;
}

ssize_t write_callback(struct file *filp, const char *buff, size_t count, loff_t *offp)
{
    printk(KERN_INFO "write method\n");
    return count;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = open_callback,
    .read = read_callback,
    .write = write_callback,
    .release = releas_callback
};

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init dr_06\n");
    if (register_chrdev_region(deb_num, 1, "dr06 mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num));


    my_cdev = cdev_alloc(); 
    my_cdev->ops = &fops;

    if (cdev_add(my_cdev, deb_num, 1)  < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit dr_06\n");
    cdev_del(my_cdev);
    unregister_chrdev_region(deb_num, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
