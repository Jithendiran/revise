#include<linux/fs.h>
#include <linux/init.h>
#include<linux/kdev_t.h>
#include <linux/module.h>

// dynanmic register
// In this method device major and minor number are allocated by kernel
dev_t dev;

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init dr_02");
    if(alloc_chrdev_region(&dev, 0, 1, "dr02 mod") < 0){
        printk(KERN_ALERT "problem in register");
        return -1;
    }
    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(dev),MINOR(dev));
    // check in cat /proc/devices
    // in character device with the name of dr02 mod u will be able to see this
    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit dr_02");
    unregister_chrdev_region(dev, 1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");