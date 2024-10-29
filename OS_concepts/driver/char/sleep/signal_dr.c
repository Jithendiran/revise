/**
 * Author: E.K.Jithendiran
 * Date: 13 Oct 24
 */

#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/kdev_t.h>
#include <linux/poll.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/cdev.h>

int signal_open_ji(struct inode *inode, struct file *filp);
int signal_release_ji(struct inode *inode, struct file *filp);
static int signal_fasync(int fd, struct file *filp, int mode);
ssize_t signal_read_ji(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t signal_write_ji(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
/**
 * It will have read head, write head
 * When no data to read, read will be in sleep, when no no space to write, write will be in sleep
 */
int BUFF_SIZE = 10; // 10 character long
dev_t deb_num = MKDEV(27, 0);
struct scull_signal
{
    char *buf, *end;             // buffer space
    char *rh, *wh;               // read & write head
    struct cdev cdev;            // char dev
    struct fasync_struct *async_queue; /* asynchronous readers */
} sc_signal;

static int signal_fasync(int fd, struct file *filp, int mode)
{
    printk(KERN_DEBUG "IN signal: Mode = %d, fd = %d\n", mode, fd);
	struct scull_signal *dev = filp->private_data;

    // add or remove entry in kernel
	return fasync_helper(fd, filp, mode, &dev->async_queue);
}

int signal_open_ji(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "signal open\n");
    struct scull_signal *add;
    add = container_of(inode->i_cdev, struct scull_signal, cdev);
    if (!add)
        return -1;
    filp->private_data = add;
    return 0;
}

int signal_release_ji(struct inode *inode, struct file *filp)
{
    // remove
    signal_fasync(-1, filp, 0);
    printk(KERN_ALERT "signal release\n");
    return 0;
}

/* How much space is free? */
static int spacefree(struct scull_signal *dev)
{
    if (dev->rh == dev->wh)
        return BUFF_SIZE - 1;
    return ((dev->rh + (BUFF_SIZE)-dev->wh) % BUFF_SIZE) - 1;
}

ssize_t signal_write_ji(struct file *filp, const char __user *buf, size_t count,
                      loff_t *f_pos)
{

    printk(KERN_ALERT "signal write : %d\n", count);

    struct scull_signal *dev = filp->private_data;

    if (spacefree(dev) == 0)
    {
        if (filp->f_flags & O_NONBLOCK)
        {
            printk(KERN_DEBUG "No space to write\n");
            return -EAGAIN;
        }
    }

    count = min(count, (size_t)spacefree(dev));
    if (dev->wh >= dev->rh)
        count = min(count, (size_t)(dev->end - dev->wh));
    else
        count = min(count, (size_t)(dev->rh - dev->wh - 1));

    if (copy_from_user(dev->wh, buf, count))
    {
        return -1;
    }
    dev->wh += count;
    if (dev->wh == dev->end)
        dev->wh = dev->buf;

    if (dev->async_queue) // to signal the interested processes
		kill_fasync(&dev->async_queue, SIGIO, POLL_OUT);

    printk(KERN_DEBUG "write Count : %d\n", count);
    return count;
}

ssize_t signal_read_ji(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "signal read: %d\n", count);

    struct scull_signal *dev = filp->private_data;

    if (dev->rh == dev->wh)
    {
        if (filp->f_flags & O_NONBLOCK)
        {
            printk(KERN_DEBUG "No data to read\n");
            return -EAGAIN;
        }
    }

    if (dev->wh > dev->rh)
        count = min(count, (size_t)(dev->wh - dev->rh));
    else
        count = min(count, (size_t)(dev->end - dev->rh));

    if (copy_to_user(buf, dev->rh, count))
    {
        return -EFAULT;
    }

    dev->rh += count;
    if (dev->rh == dev->end)
        dev->rh = dev->buf; /* wrapped */

    if (dev->async_queue) // to signal the interested processes
		kill_fasync(&dev->async_queue, SIGIO, POLL_IN);

    printk(KERN_DEBUG "Read Count : %d\n", count);
    return count;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = signal_open_ji,
    .read = signal_read_ji,
    .fasync = signal_fasync, 
    .write = signal_write_ji,
    .release = signal_release_ji
};

static __init int signal_init(void)
{
    printk(KERN_ALERT "Init signal\n");

    cdev_init(&(sc_signal.cdev), &fops);

    // For register major and minor number
    if (register_chrdev_region(deb_num, 1, "signal mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num)); // Major : 27, Minor : 0

    // init buffer, place buf head
    sc_signal.buf = kmalloc(BUFF_SIZE, GFP_KERNEL);
    sc_signal.end = sc_signal.buf + BUFF_SIZE;
    memset(sc_signal.buf, 0, sizeof(char) * BUFF_SIZE);
    sc_signal.rh = sc_signal.wh = sc_signal.buf;

    // Register driver in kernel
    if (cdev_add(&(sc_signal.cdev), deb_num, 1) < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    return 0;
}

static __exit void signal_exit(void)
{
    // free buf space
    printk(KERN_ALERT "exit signal\n");
    if (sc_signal.buf)
    {
        printk(KERN_ALERT "Mem signal unallocate\n");
        kfree(sc_signal.buf);
        sc_signal.buf = NULL;
    }
    cdev_del(&(sc_signal.cdev));
    unregister_chrdev_region(deb_num, 1);
}

module_init(signal_init);
module_exit(signal_exit);
MODULE_LICENSE("GPL");

// mknod /dev/sc_signal c 27 0

// when we trigger IO, before read or write signal method is called