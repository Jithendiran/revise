/**
 * Author: E.K. Jithendiran
 * Date  : 23 Sep 24
 */

#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/cdev.h>

#include "../scull.h"

/**
 * To userstand the structure of scull.png
 */

dev_t deb_num = MKDEV(27, 0);

struct scull_dev dev;

int scull_trim(struct scull_dev *dev)
{
    printk(KERN_ALERT "trim sc_1\n");
    struct scull_qset *next, *dptr;
    int qset = dev->qset;
    int i;
    for (dptr = dev->data; dptr; dptr = next)
    { /* all the list items */
        if (dptr->data)
        {
            for (i = 0; i < qset; i++)
                kfree(dptr->data[i]);
            kfree(dptr->data);
            dptr->data = NULL;
        }
        next = dptr->next;
        kfree(dptr);
    }
    dev->size = 0;
    dev->quantum = scull_quantum;
    dev->qset = scull_qset;
    dev->data = NULL;
    return 0;
}

int scull_open(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "Open sc_1\n");
    struct scull_dev *dev; /* device information */
    dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    filp->private_data = dev; /* for other methods */
    /* now trim to 0 the length of the device if open was write-only */
    if ((filp->f_flags & O_ACCMODE) == O_WRONLY)
    {
        scull_trim(dev); /* ignore errors */
    }
    return 0;
    /* success */
}

int scull_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/**
 * For creating scull_qset, qset is inside scull_qset.
 * data part of scull_qset is qset
 */
struct scull_qset *scull_follow(struct scull_dev *dev, int n)
{
    printk(KERN_ALERT "follow sc_1\n");
    struct scull_qset *qs = dev->data;

    // 1st quantum
    if (!qs)
    {
        qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
        if (qs == NULL)
            return NULL;
        memset(qs, 0, sizeof(struct scull_qset));
        /**
         * memset will place 0 in qs for the length of struct scull_qset
         */
    }

    /* remaining in the list */
    while (n--)
    {
        if (!qs->next)
        {
            qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
            if (qs->next == NULL)
                return NULL;
            memset(qs->next, 0, sizeof(struct scull_qset));
        }
        qs = qs->next;
        continue;
    }
    return qs;
}

/**
 * Able to write only single quantum for a single call of write
 * If content is more, only fill which will match remaining are discarded
 */
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count,
                    loff_t *f_pos)
{
    printk(KERN_ALERT "write sc_1\n");
    struct scull_dev *dev = filp->private_data;
    struct scull_qset *dptr;
    int quantum = dev->quantum, qset = dev->qset;
    int itemsize = quantum * qset;
    int item, s_pos, q_pos, rest;
    ssize_t retval = -ENOMEM; /* value used in "goto out" statements */

    /**
     * Consider quantum_size is 3 byte, quantum_set have 2 quantum, so total size of a quantum_Set is 6 byte
     * If *f_pos has value of 8 means, 1st 6 bytes are stored in 1st quantum remaining 2 bytes are stored in 2nd quantum
     * At 2nd quantum from byte 3 (index 2) to byte 6 (index 5) is free
     *
     * How to find which location to start?
     * Here clue is f_pos is to find which quantum_set and which quantum in q_set
     * f_pos /  size(quantum_Set) = 8 / 6 = 1.
     * Here 1 means from 2nd (index 1) quantum_set we can start filling data
     * Still need to find which quantum and which index of quantum of quantum to start
     * to find which quantum, need to know which byte the cursor is pointing in quantum
     * f_pos %  size(quantum_Set) = 8 % 6 = 2.
     * Here 2 means from 2nd of quantum_set, we can start filling the data. To find which quantum
     * (f_pos %  size(quantum_Set)) / size(quantum) = 2 / 3 = 0
     * 0 is giving which quantum to start in particular quantum_set, but that particular quantum may alreday have some data. So need to find where to start in quantum
     *  (f_pos %  size(quantum_Set)) % size(quantum) = 2 % 3 = 2
     * 2 is given the index of quantum to start
     */

    item = (long)*f_pos / itemsize; // which qset that f_pos is pointing
    rest = (long)*f_pos % itemsize; // from where to start in the q_set (It will give index of where to start), but here we don't know which quantum
    // also quantum's from which byte is free to start store is don't know

    s_pos = rest / quantum; // tells you which quantum in a scull_qset will start
    q_pos = rest % quantum; // where to start within the quantum

    // scull_follow it will get which q_set to start
    dptr = scull_follow(dev, item);
    if (dptr == NULL)
        goto out;

    if (!dptr->data)
    {
        // Creating qset of scull_qset
        dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
        if (!dptr->data)
            goto out;
        memset(dptr->data, 0, qset * sizeof(char *));
    }
    if (!dptr->data[s_pos])
    {
        // creating single quantum of qset
        dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
        if (!dptr->data[s_pos])
            goto out;
    }
    /* write only up to the end of this quantum */
    // 3 - 2 = 1
    // Only one byte of quantum is free so modify the count accordingly
    if (count > quantum - q_pos)
        count = quantum - q_pos;

    // dptr->data[s_pos] + q_pos;
    // precedence is dptr->data => address
    // address[some_point] => move the pointer to some address
    // add the q_pos address to address moved to s_pos

    // Copy content from particular position of quantum
    // dptr->data[s_pos] get quantum in q_set, + q_pos will get which index to start
    if (copy_from_user(dptr->data[s_pos] + q_pos, buf, count))
    {
        retval = -EFAULT;
        goto out;
    }
    *f_pos += count;
    retval = count;
    /* update the size */
    if (dev->size < *f_pos)
        dev->size = *f_pos;
out:
    return retval;
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count /*6*/,
                   loff_t *f_pos /*5*/)
{
    printk(KERN_ALERT "Read sc_1\n");
    struct scull_dev *dev = filp->private_data;
    struct scull_qset *dptr;

    int quantum = dev->quantum, qset = dev->qset;
    int itemsize = quantum * qset; // 100
    int item, s_pos, q_pos, rest;
    ssize_t retval = 0;

    if (*f_pos >= dev->size)
        goto out; // read operation completed

    if (*f_pos + count > dev->size)
        // if count is higher, store actually remaining left length to read from device
        // eg: *f_pos = 5, count = 6, size = 10
        // 5 + 6 > 10 is 11 > 10 so here device should only return data from 6 to 10, 6 to 10 length is 5
        // 10 - 5 = 5
        // overall size update
        count = dev->size - *f_pos;

    /* find listitem, qset index, and offset in the quantum */
    item = (long)*f_pos / itemsize; //  5 / 100 = 0 // 0th quantum
    rest = (long)*f_pos % itemsize; //  5 % 100 = 5 //

    s_pos = rest / quantum; // 5 / 20 = 0
    q_pos = rest % quantum; // 5 % 20 = 5

    /* follow the list up to the right position (defined elsewhere) */
    dptr = scull_follow(dev, item);

    if (dptr == NULL || !dptr->data || !dptr->data[s_pos])
        goto out;

    /* read only up to the end of this quantum */
    if (count > quantum - q_pos) // 5 > (20 - 5)
        count = quantum - q_pos;
    if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count))
    {
        retval = -EFAULT;
        goto out;
    }

    *f_pos += count;
    retval = count;
out:
    return retval;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = scull_open,
    .read = scull_read,
    .write = scull_write,
    .release = scull_release
};

__init int hello_init(void)
{
    printk(KERN_ALERT "Init sc_1\n");
    printk(KERN_INFO "Size of scull_dev : %d\n", sizeof(dev)); //  Size of scull_dev : 232
    printk(KERN_INFO "Address of scull_dev : %p\n", dev); // Address of scull_dev : ffffffffc2ab9328
    printk(KERN_INFO "Address of cdev : %p\n", dev.cdev); // Address of cdev : 0000000000000004

    cdev_init(&(dev.cdev), &fops);

    if (register_chrdev_region(deb_num, 1, "sc_1 mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num)); // Major : 27, Minor : 0

    if (cdev_add(&(dev.cdev), deb_num, 1)  < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    return 0;
}

__exit void hello_exit(void)
{
    printk(KERN_ALERT "exit sc_1\n");
    cdev_del(&(dev.cdev));
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
