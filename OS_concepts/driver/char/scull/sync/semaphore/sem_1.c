/**
 * Author: E.K. Jithendiran
 * Date  : 27 Sep 24
 */

#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
/**
 * semaphore
 */

dev_t deb_num = MKDEV(27, 0);

struct cdev my_cdev;

/**
 * This below code will create the semaphore and lock the semaphore
 */
DECLARE_MUTEX(sem1); // init to 1
// DECLARE_MUTEX_LOCKED(sem1); // init to 0

// // create by manual
// struct semaphore sem;


ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,
                    loff_t *f_pos)
{
    if (down(sem1))
        return -ERESTARTSYS;
    
    printk(KERN_ALERT "write Sem lock");

    up(sem1);
    printk(KERN_ALERT "write free\n");
    return 0;
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count /*6*/,
                   loff_t *f_pos /*5*/)
{
    if (down(sem1))
        return -ERESTARTSYS;
    
    printk(KERN_ALERT "read Sem lock");

    up(sem1);
    printk(KERN_ALERT "read free\n");
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = scull_read,
    .write = scull_write,
};

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init sem_1\n");
    
    cdev_init(&my_cdev, &fops);

    if (register_chrdev_region(deb_num, 1, "sem_1 mod") < 0)
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
    printk(KERN_ALERT "exit sem_1\n");
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
 * 3. mknod /dev/sc_1 c 27 0
 * 4. echo "Hi hello" | tee /dev/sc_1 or echo "Hi hello" > /dev/sc_1
 * 5. cat /dev/sc_1
 * 6. rm /dev/sc_1
 * 
 * 7. uninstall: make uninstall
 * 8. clean: make clean
 * 
 * To check Kernel output open new terminal and run
 * $ journalctl -f
 */