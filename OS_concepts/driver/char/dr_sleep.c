/**
 * Author: E.K. Jithendiran
 * Date  : 10 Apr 24
 */

// sleep & wakeup

#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>

struct cdev *my_cdev;
dev_t deb_num = MKDEV(27, 0);

// static DECLARE_WAIT_QUEUE_HEAD(wq);
// or
wait_queue_head_t wq;
init_waitqueue_head(&wq);


static int flag = 0;

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
    printk(KERN_DEBUG "process %i (%s) going to sleep\n", current->pid, current->comm);
    wait_event_interruptible(wq, flag != 0);
    flag = 0;
    printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);
    return 1;
}

ssize_t write_callback(struct file *filp, const char *buff, size_t count, loff_t *offp)
{
    printk(KERN_INFO "write method\n");
    printk(KERN_DEBUG "process %i (%s) awakening the readers...\n",
    current->pid, current->comm);
    flag = 1;
    wake_up_interruptible(&wq);
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
    printk(KERN_ALERT "Init dr_09\n");
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
    printk(KERN_ALERT "exit dr_09\n");
    cdev_del(my_cdev);
    unregister_chrdev_region(deb_num, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

/**
* To execute
* mknod /dev/ch c 27 0

* Terminal 1 
        cat /dev/ch
* Terminal 2
        echo 1 | tee /dev/ch

* then manually kill terminal 1
* * Terminal 3
        dmesg
* OP
*---
[   82.446607] Init dr_06
[   82.446677] Major : 27, Minor : 0
[  139.584622] open method
[  139.584640] read method
[  139.584641] process 1581 (cat) going to sleep
[  175.149737] open method
[  175.149857] write method
[  175.149858] process 1808 (tee) awakening the readers...
[  175.149863] awoken 1581 (cat)
[  175.149882] read method
[  175.149893] process 1581 (cat) going to sleep
[  175.149941] release method
[  223.002074] awoken 1581 (cat)
[  223.002143] release method
*/