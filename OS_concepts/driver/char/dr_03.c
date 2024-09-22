/**
 * Author: E.K. Jithendiran
 * Date  : 16 Mar 24
 */

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>

// cdev is newer way
// cdev's Major and Minor number can be more than 256

// cdev_add is initilize the driver opertaion to kernel


dev_t deb_num = MKDEV(27, 0);
struct cdev *my_cdev;


int open_callback(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "open method\n");
    return 0;
}
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

// To get file_operation access need to point file_operation struct while register the device

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = open_callback,
    .read = read_callback,
    .write = write_callback,
    .release = releas_callback
};

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init dr_03\n");
    if (register_chrdev_region(deb_num, 1, "dr03 mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num));

    // after this point a device file in /dev
    // mknod /dev/dr03 c 27 0

    // To register the char_dev internally we use cdev_alloc() //method
    // once cdev_is allocated and added to kernel, kernel able to call file operations, so cdev_add method should call after the setup
    // cdev_add can be fail, if failed need to unregister the device

    my_cdev = cdev_alloc(); // allocate empty cdev
    // point the function pointers with cdev
    my_cdev->ops = &fops;

    if (cdev_add(my_cdev, deb_num, 1) < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit dr_03\n");
    cdev_del(my_cdev);
    unregister_chrdev_region(deb_num, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

/**
 * Write command :echo 1 | tee /dev/dr03  -> this will call open, write and release function pointers
 * read command : cat /dev/dr03 -> open, read and release function pointers
 */
