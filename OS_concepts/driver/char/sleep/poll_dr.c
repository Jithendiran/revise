/**
 * Author: E.K.Jithendiran
 * Date: 12 Oct 24
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

int poll_open_ji(struct inode *inode, struct file *filp);
int poll_release_ji(struct inode *inode, struct file *filp);
static unsigned int poll_ji(struct file *filp, poll_table *wait);
ssize_t poll_read_ji(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t poll_write_ji(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
/**
 * It will have read head, write head
 * When no data to read, read will be in sleep, when no no space to write, write will be in sleep
 */
int BUFF_SIZE = 10; // 10 character long
dev_t deb_num = MKDEV(27, 0);
struct scull_poll
{
    char *buf, *end;             // buffer space
    char *rh, *wh;               // read & write head
    struct cdev cdev;            // char dev
    wait_queue_head_t inq, outq; // read and write queues
} sc_poll;

int poll_open_ji(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "poll open\n");
    struct scull_poll *add;
    add = container_of(inode->i_cdev, struct scull_poll, cdev);
    if (!add)
        return -1;
    filp->private_data = add;
    return 0;
}

int poll_release_ji(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "poll release\n");
    return 0;
}

/* How much space is free? */
static int spacefree(struct scull_poll *dev)
{
    if (dev->rh == dev->wh)
        return BUFF_SIZE - 1;
    return ((dev->rh + (BUFF_SIZE)-dev->wh) % BUFF_SIZE) - 1;
}

ssize_t poll_write_ji(struct file *filp, const char __user *buf, size_t count,
                      loff_t *f_pos)
{

    printk(KERN_ALERT "poll write : %d\n", count);

    struct scull_poll *dev = filp->private_data;

    if (spacefree(dev) == 0)
    {
        // poll
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

    // awake reader
    wake_up_interruptible(&dev->inq);
    printk(KERN_DEBUG "write Count : %d\n", count);
    return count;
}

ssize_t poll_read_ji(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "poll read: %d\n", count);

    struct scull_poll *dev = filp->private_data;

    if (dev->rh == dev->wh)
    {
        // poll
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
    // read has emptied some blocks of buffer, now writer might has some blocks to write
    wake_up_interruptible(&(dev->outq)); // This will change the state of fd and poll will check for IO possible
    printk(KERN_DEBUG "Read Count : %d\n", count);
    return count;
}

static unsigned int poll_ji(struct file *filp, poll_table *wait)
{
    printk(KERN_DEBUG "In Poll\n");
    
    struct scull_poll *dev = filp->private_data;
    unsigned int mask = 0;

    poll_wait(filp, &dev->inq, wait);
    poll_wait(filp, &dev->outq, wait);
    if (dev->rh != dev->wh)
        mask |= POLLIN | POLLRDNORM; /* readable */
    if (spacefree(dev))
        mask |= POLLOUT | POLLWRNORM; /* writable */
    return mask;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .poll = poll_ji,
    .open = poll_open_ji,
    .read = poll_read_ji,
    .write = poll_write_ji,
    .release = poll_release_ji
};

static __init int poll_init(void)
{
    printk(KERN_ALERT "Init poll\n");

    cdev_init(&(sc_poll.cdev), &fops);

    // For register major and minor number
    if (register_chrdev_region(deb_num, 1, "poll mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num)); // Major : 27, Minor : 0

    // init buffer, place buf head
    sc_poll.buf = kmalloc(BUFF_SIZE, GFP_KERNEL);
    sc_poll.end = sc_poll.buf + BUFF_SIZE;
    memset(sc_poll.buf, 0, sizeof(char) * BUFF_SIZE);
    sc_poll.rh = sc_poll.wh = sc_poll.buf;

    init_waitqueue_head(&(sc_poll.inq));
    init_waitqueue_head(&(sc_poll.outq));

    // Register driver in kernel
    if (cdev_add(&(sc_poll.cdev), deb_num, 1) < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    return 0;
}

static __exit void poll_exit(void)
{
    // free buf space
    printk(KERN_ALERT "exit poll\n");
    if (sc_poll.buf)
    {
        printk(KERN_ALERT "Mem poll unallocate\n");
        kfree(sc_poll.buf);
        sc_poll.buf = NULL;
    }
    cdev_del(&(sc_poll.cdev));
    unregister_chrdev_region(deb_num, 1);
}

module_init(poll_init);
module_exit(poll_exit);
MODULE_LICENSE("GPL");

// mknod /dev/sc_poll c 27 0
// run poll_c_r in new terminal
// run poll_c_w in new terminal

// when we trigger IO, before read or write poll method is called