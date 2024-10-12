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

/**
 * What
 * IOCTL is used to do extend the ability of drive only from read and write
 *
 * Why
 * For eg: Let's take USB
 *       USB can read, write it is obvious, Other than it can mount, eject, ...
 */

/**
 * _IO(type,nr) (for a command that has no argument)
 * _IOR(type,nr,datatype) (for reading data from the driver)
 * _IOW(type,nr,datatype) (for writing data)
 * _IOWR(type,nr,datatype) (for bidirectional transfers)
 * 
 * The type and nr(number) fields are passed as arguments, and the
    size field is derived by applying sizeof to the datatype argument.
    _IOC_DIR(nr), _IOC_TYPE(nr), _IOC_NR(nr), and _IOC_SIZE(nr)
 * 
 * type
 *  unique key+sequence number
 * 
 * nr
 *  direction
 * 
 * datatype (size)
 *The size of user data involved. The width of this field is architecture dependent,
    but is usually 13 or 14 bits. You can find its value for your specific architecture
    in the macro _IOC_SIZEBITS. It’s not mandatory that you use the size field—the
    kernel does not check it—but it is a good idea. Proper use of this field can help
    detect user-space programming errors and enable you to implement backward
    compatibility if you ever need to change the size of the relevant data item. If you
    need larger data structures, however, you can just ignore the size field
 * 
 * W means write to driver, R means Read from driver
 * 
 * For get by value and set by value _IO is enough, Only for pointer _IOW, _IOR or _IOWR is required
 * 
 * From kernel it can read & write on any data even though need to use _IOR for read, _IOW for write and _IOWR for both it is standard
 */

// IOCTL define
#define IOCTL_MAGIC 'k' // unique key

#define SCULL_DEFINE _IO(IOCTL_MAGIC, 0)

#define SCULL_SETINT _IOW(IOCTL_MAGIC, 1, int) //  ptr K <- U
#define SCULL_SETFLOAT _IOW(IOCTL_MAGIC, 1, float) // Dup of SCULL_SETINT: So DIRECTION + number is matter not type

#define SCULL_GETINT _IOR(IOCTL_MAGIC, 1, int) // ptr K -> U

//  get user
#define SCULL_SETINTPTEGET _IOW(IOCTL_MAGIC, 2, int) // it is handling pointers
#define SCULL_GETINTPTEPUT _IOR(IOCTL_MAGIC, 2, int) // it is handling pointer

#define SCULL_DUAL _IOWR(IOCTL_MAGIC, 1, int)

dev_t deb_num = MKDEV(27, 0);
struct cdev *my_cdev;


int val = -1;
int *ptr = &val;
float f = 23.0;

// long can be casted to int, float, char,.. so return type is long
static long chr_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int err = 0;
    printk(KERN_INFO "TYPE : %c, Number: %d", _IOC_TYPE(cmd), _IOC_NR(cmd)); // will print TYPE 'k' or other character and Number as integer to select command
    printk(KERN_INFO "DIRECTION : %c : %d", _IOC_DIR(cmd), _IOC_DIR(cmd));   // \x02 : 2 for read, \x01 : 1 for write

    switch (cmd)
    {
        case SCULL_DEFINE:
            printk(KERN_DEBUG "NO ARGT %d\n",arg); // get, give by value
            return 4; // give
            break;


        case SCULL_SETINT:
            printk(KERN_DEBUG "SET INT PTR\n");
            // check for valid user address space and retrive value
            copy_from_user(&val, (int __user *)arg, sizeof(val)); // dest, src, size (arg of the function)            
            printk(KERN_DEBUG "Value from user: %d\n", val); 

            val = 13;
            copy_to_user((int*)arg, &val, sizeof(val)); // it is writing
            // from kernel point of view it can read or write any data
            // return 11; // It can return
            break;

        case SCULL_GETINT:
            printk(KERN_DEBUG "GET INT PTR \n");
            val = 521;
            copy_to_user((int*)arg, &val, sizeof(val));

            val = 0;
            // try to read
            copy_from_user(&val, (int __user *)arg, sizeof(val));
            printk(KERN_DEBUG "Value from user: %d\n", val); 
            break;

        case SCULL_SETINTPTEGET:

            printk(KERN_DEBUG "GET INT PTR by GET USER\n");
            // get_user(val, ((int *)arg)); // it is fast compare to copy_from_user, but it is capable of copying simple data structure only like int,char,.. not a complex structure like struct

            // check for valid user space address
            err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
            if (err)
                return -EFAULT;
            __get_user(val, ((int *)arg));

            // get_user = access_ok + __get_user

            printk(KERN_DEBUG "Value from user: %d\n", val);
            break;

        case SCULL_GETINTPTEPUT:
             printk(KERN_DEBUG "SET INT PTR by PUT USER\n");
            // same like get_user

            // check for valid user space address
            err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
            if (err)
                return -EFAULT;
            __put_user(123, ((int *)arg));

            // put_user(321, ((int *)arg))
            break;
        
        case SCULL_DUAL:
            printk(KERN_DEBUG "GET & SET \n");

            copy_from_user(&val, (int __user *)arg, sizeof(val));         
            printk(KERN_DEBUG "Value from user: %d\n", val); 

            val = 69;
            copy_to_user((int*)arg, &val, sizeof(val));
            break;
        

        // case SCULL_SETFLOAT:
        //     printk(KERN_DEBUG "SET FLOAT\n");
        //     f = arg;
        //     printk(KERN_DEBUG "Value from user: %f\n", f); // Value from user: 27
        //     break;

        /**
         * Error of duplicate
         * 
         * /home/jidesh/Project/revise/OS_concepts/driver/char/ioctl/ioctl_dr.c:129:9: error: duplicate case value
            129 |         case SCULL_SETFLOAT:
                |         ^~~~
            /home/jidesh/Project/revise/OS_concepts/driver/char/ioctl/ioctl_dr.c:77:9: note: previously used here
            77 |         case SCULL_SETINT:
                |         ^~~~
         */

        default:
            printk(KERN_ALERT "No such cmd \n");
            break;
    }
    return 0;
}

// To get file_operation access need to point file_operation struct while register the device

struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = chr_ioctl};

static __init int hello_init(void)
{
    printk(KERN_ALERT "Init IOCTL\n");
    if (register_chrdev_region(deb_num, 1, "ioctl mod") < 0)
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
