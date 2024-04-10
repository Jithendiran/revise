
/**
 * Author: E.K. Jithendiran
 * Date  : 16 Mar 24
 */

#include <linux/fs.h>
#include<linux/cdev.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>

// This is older method of init character device in kernel
// It's limitaion is Major and Minor number cannot exceed 0 - 255
// due to it's limitation cdev init or alloc is used is introduced
// it will internally assign default cdev
// here minor number will auto create
int majno;


int open_callback(struct inode *inode, struct file *filp) {
    printk(KERN_INFO "open method\n");
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

static __init int hello_init(void) {
    // if 0 -> it will auto allocate major number, 
    // if number is greater than 0, try to allocate the major number what got in paramete 

    printk(KERN_ALERT "Init dr_05\n");
    majno = register_chrdev(0, "dr05 mod", &fops);

    if(majno < 0){
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d\n", majno);

    return 0;
}

static __exit void hello_exit(void) {
    printk(KERN_ALERT "exit dr_05\n");
    unregister_chrdev(majno, "dr05 mod");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");


/**
 * Write command :echo 1 | tee /dev/dr04  -> this will call open, write and release function pointers
 * read command : cat /dev/dr04 -> open, read and release function pointers
*/