/**
 * Author: E.K. Jithendiran
 * Date  : 15 Mar 24
 */

// static register

#include<linux/fs.h>
#include<linux/init.h>
#include<linux/kdev_t.h>
#include <linux/module.h>

// In this method device Major & Minor number are known
dev_t deb_num = MKDEV(27,0);

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init dr_01");
    if(register_chrdev_region(deb_num, 1,"dr01 mod") < 0){
        printk(KERN_ALERT "problem in register");
        return -1;
    }

    // check in cat /proc/devices
    // in character device with the name of dr01 mod u will be able to see this
    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit dr_01");
    unregister_chrdev_region(deb_num, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");