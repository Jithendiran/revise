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

dev_t deb_num = MKDEV(27, 0);
struct cdev *my_cdev;

//Internally, scull represents each device with a structure of type struct scull_dev
struct scull_dev
{
    struct scull_qset *data;    /* Pointer to first quantum set */
    int quantum;                /* the current quantum size */
    int qset;                   /* the current array size */
    unsigned long size;         /* amount of data stored here */
    unsigned int access_key;    /* used by sculluid and scullpriv */
    struct semaphore sem;       /* mutual exclusion semaphore */
    struct cdev cdev;           /* Char device structure*/
};

// Important data structure
// file_operation: It has the important callback functions like: open, read, write,....
// file : It is different from user space FILE pointer, 
//        It is passed as single pointer reference for for every file opened in kernel. From 1st open to last close
//        It has pointer like: private_data, file_operation,..
// inode: inode has many field, for driver only 2 feild is helpful
//        dev_t -> has the device number
//        cdev  -> It is reference to internal character deice 

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
     * container_of(pointer, container_type, container_field);
     * scull_dev is stored inside private_data
    */
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

ssize_t write_callback(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
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
    printk(KERN_ALERT "exit dr_06\n");
    cdev_del(my_cdev);
    unregister_chrdev_region(deb_num, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
