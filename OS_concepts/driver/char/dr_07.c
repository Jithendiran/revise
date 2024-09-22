/**
 * Author: E.K. Jithendiran
 * Date  : 1 Apr 24
 */

// using proc file system
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

dev_t deb_num = MKDEV(27, 0);
struct cdev *my_cdev;
static struct proc_dir_entry *ent;

// proc file system is like dev 
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

ssize_t write_callback(struct file *filp, const char  *buff, size_t count, loff_t *offp)
{
    printk(KERN_INFO "write method\n");
    return count;
}


struct proc_ops fops = {
    .proc_open = open_callback,
    .proc_read = read_callback,
    .proc_write = write_callback,
    .proc_release = releas_callback
};

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init dr_07\n");
    if (register_chrdev_region(deb_num, 1, "dr03 mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num));


    my_cdev = cdev_alloc(); 

    if (cdev_add(my_cdev, deb_num, 1) < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    /**
    * 0th name of the file
    * 1st mode
    * 2nd is parent file structure
    * 3rd is file operation structure 
    */
    ent = proc_create("ch_proc", 0666, NULL, &fops);

    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit dr_07\n");
    // remove_proc_entry("ch_proc", NULL); // or below (neither works)
    proc_remove(ent);
    cdev_del(my_cdev);
    unregister_chrdev_region(deb_num, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");