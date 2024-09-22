/**
 * Author: E.K. Jithendiran
 * Date  : 22 Sep 24
 */
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include "scull.h"
/**
 * Using the cdev inside scull_dev 
 */
dev_t deb_num = MKDEV(27, 0);

struct scull_dev dev;


int open_callback(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "open method\n");

    struct scull_dev *dev; /* device information */
    dev = container_of(inode->i_cdev, struct scull_dev, cdev); 

    printk(KERN_INFO "Address of scull_dev : %p\n", *dev); // Address of scull_dev : ffffffffc2ab9328
    printk(KERN_INFO "Address of cdev : %p\n", dev->cdev); // Address of cdev : 0000000000000004

    return 0;
}
int releas_callback(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "release method\n");
    return 0;
}

ssize_t read_callback(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
    printk(KERN_INFO "read method\n");
    return 1;
}

ssize_t write_callback(struct file *filp, const char *buff, size_t count, loff_t *offp) {
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
    printk(KERN_ALERT "Init dr_06_1\n");
    printk(KERN_INFO "Size of scull_dev : %d\n", sizeof(dev)); //  Size of scull_dev : 232
    printk(KERN_INFO "Address of scull_dev : %p\n", dev); // Address of scull_dev : ffffffffc2ab9328
    printk(KERN_INFO "Address of cdev : %p\n", dev.cdev); // Address of cdev : 0000000000000004

    cdev_init(&(dev.cdev), &fops);

    if (register_chrdev_region(deb_num, 1, "dr06_1 mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num)); // Major : 27, Minor : 0

    if (cdev_add(&(dev.cdev), deb_num, 1)  < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit dr_06_1\n");
    cdev_del(&(dev.cdev));
    unregister_chrdev_region(deb_num, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

/**
 * Run 
 * 1. compile : make
 * 2. install : make install
 * 
 * 3. mknod /dev/dr06_1 c 27 0
 * 4. echo 1 | tee /dev/dr06_1 or echo 1 > /dev/dr06_1
 * 5. cat /dev/dr06_1
 * 6. rm /dev/dr06_1
 * 
 * 7. uninstall: make uninstall
 * 8. clean: make clean
 * 
 * To check Kernel output open new terminal and run
 * $ journalctl -f
 */