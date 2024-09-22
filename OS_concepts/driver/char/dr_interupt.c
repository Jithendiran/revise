/**
 * Author: E.K. Jithendiran
 * Date  : 2 Apr 24
 */
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/interrupt.h>

#define IRQ_NO_K 1

struct cdev *my_cdev;
dev_t deb_num = MKDEV(27, 0);

static irqreturn_t irq_handler(int irq, void *dev_id) {
    printk(KERN_ALERT "Keyboard Interrupt");
    return IRQ_HANDLED;
}

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init dr_08\n");
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

    if(request_irq(IRQ_NO_K, irq_handler, IRQF_SHARED, "chr_dev", (void *)(irq_handler))) {
        printk(KERN_ALERT "IRQ reg failed");
    }

    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit dr_08\n");
    free_irq(IRQ_NO_K,(void *)(irq_handler));
    cdev_del(my_cdev);
    unregister_chrdev_region(deb_num, 1);
}


module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

/**
* cat /proc/interrupts
* check in 1st interrupt
* eg
           CPU0       CPU1       
  1:          9          0   IO-APIC   1-edge      i8042, chr_dev
*/