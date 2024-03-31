/**
 * Author: E.K. Jithendiran
 * Date  : 31 Mar 24
 */
// make 
// insmod ioctl_dr.ko
// mknod /dev/ioctl_dr c 27 0
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/uaccess.h>

// IOCTL define
#define IOCTL_MAGIC  'k'

#define IOCTL_R _IOR(IOCTL_MAGIC, 1, int32_t*)
#define IOCTL_W _IOW(IOCTL_MAGIC, 2, int32_t*)

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

ssize_t write_callback(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
    printk(KERN_INFO "write method\n");
    return count;
}

int32_t val = -1;

static long chr_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
     printk(KERN_INFO "IOCTL method %d\n",((int32_t*)arg));
     switch(cmd){
        case IOCTL_W:
            // read from user and write to driver
            copy_from_user(&val, (int32_t*)arg, sizeof(val)); // dest, src, size (arg of the function)
            printk(KERN_INFO "IOCTL User Read method : %d\n",val);
            break;
        case IOCTL_R:
            copy_to_user((int32_t*)arg, &val, sizeof(val)); // dest, src, size (arg of the function)
            printk(KERN_INFO "IOCTL User Write method \n");
            break;
        default:
         printk(KERN_INFO "IOCTL method : cmd invalid\n");
         return -ENOTTY;
     }
     return 0;
}

// To get file_operation access need to point file_operation struct while register the device

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = open_callback,
    .read = read_callback,
    .write = write_callback,
    .release = releas_callback,
    .unlocked_ioctl = chr_ioctl
};


static __init int hello_init(void)
{
    printk(KERN_ALERT "Init IOCTL\n");
    if (register_chrdev_region(deb_num, 1, "dr03 mod") < 0)
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
    printk(KERN_ALERT "exit IOCTL\n");
    cdev_del(my_cdev);
    unregister_chrdev_region(deb_num, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");