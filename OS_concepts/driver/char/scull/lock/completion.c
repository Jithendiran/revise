/**
 * Author: E.K. Jithendiran
 * Date  : 1 Oct 24
 */
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/completion.h>

struct cdev my_cdev;
dev_t deb_num = MKDEV(27, 0);

// DECLARE_COMPLETION(comp);

struct completion comp;

ssize_t complete_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
    printk(KERN_DEBUG "process %i (%s) going to sleep\n",
           current->pid, current->comm);
    wait_for_completion(&comp);
    printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);
    return 0; /* EOF */
}

ssize_t complete_write(struct file *filp, const char __user *buf, size_t count,
                       loff_t *pos)
{
    printk(KERN_DEBUG "process %i (%s) awakening the readers...\n",
           current->pid, current->comm);
    complete(&comp);
    return count; /* succeed, to avoid retrial */
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = complete_read,
    .write = complete_write,
};

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init complete lock\n");
    
    init_completion(&comp); // if declare & use; incase DECLARE_COMPLETION is used comment this

    cdev_init(&my_cdev, &fops);

    if (register_chrdev_region(deb_num, 1, "complete lock mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num)); 

    if (cdev_add(&my_cdev, deb_num, 1) < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit complete lock\n");
    cdev_del(&(my_cdev));
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
 * 3. mknod /dev/dr c 27 0
 *New terminal (Read terminal will block untill some thing write )
 * 4. cat /dev/dr
 *New terminal
 * 5. echo "Hi hello" | tee /dev/dr or echo "Hi hello" > /dev/dr
 * 6. rm /dev/dr
 * 
 * 7. uninstall: make uninstall
 * 8. clean: make clean
 * 
 * To check Kernel output open new terminal and run
 * $ journalctl -f
 */