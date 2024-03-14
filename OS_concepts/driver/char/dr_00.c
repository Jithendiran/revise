/**
 * Author: E.K. Jithendiran
 * Date  : 14 Mar 24
 */

/**
 * Dynamic allocation
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

// this will available in RAM untill initialization complete the it will remove
int __initdata count = 2;
// __exit & __exitdata is available only in exit time
int __exitdata exitcount = 5;

static int hello_init(void);
static void hello_exit(void);

static int howmany = 1;
static char *text = "Hi";

module_param(howmany, int, S_IRUGO);
module_param(text, charp, S_IRUGO);
// module param will modify by
// (init modification) insmod module_name.ko howmany=2 text="hello"

// (runtime modification) [it need's driver file in /dev]
// echo 2 < | sudo tee /dev/filename

// init function will removed from RAM after initilization
static __init int hello_init(void)
{
    printk(KERN_ALERT "Init.....%d\n",count); // to view dmesg or  cat /var/log/syslog 
    for (int i = 0; i < count; i++)
    {
        printk(KERN_INFO"count in: %d\n",i);
    }
    printk(KERN_INFO "Module Param -----------\n");
    printk(KERN_INFO "howmany : %d\n",howmany);
    printk(KERN_INFO "text  : %s\n",text);
    printk(KERN_INFO "Module Param -----------\n");

    printk(KERN_INFO "Exit data : %d\n",exitcount);
    return 0;
}


static __exit void hello_exit(void)
{
    printk(KERN_ALERT "exit............\n");
    for (int i = 0; i < exitcount; i++)
    {
        printk(KERN_INFO"count out: %d\n",i);
    }
    printk(KERN_INFO "Module Param -----------\n");
    printk(KERN_INFO "howmany : %d\n",howmany);
    printk(KERN_INFO "text  : %s\n",text);
    printk(KERN_INFO "Module Param -----------\n");
}

// execute when loading kernel
module_init(hello_init);
// execute when unloading kernel, without this kernel is not allow to unload the module  
module_exit(hello_exit);

MODULE_LICENSE("GPL");
// lsmod -> view loaded modules

/**
 * Module                  Size  Used by
    zfs                  6680576  6
    spl                   184320  1 zfs

module column is Name, our case it is dr_oo
size is size of the module
used by is this module's exported symbol is used in some other modules, name of the modules that are using 
*/