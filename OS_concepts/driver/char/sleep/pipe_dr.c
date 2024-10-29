/**
 * Author: E.K.Jithendiran
 * Date: 1 Oct 24
 */

#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/cdev.h>

int pipe_open_ji(struct inode *inode, struct file *filp);
int pipe_release_ji(struct inode *inode, struct file *filp);
ssize_t pipe_read_ji(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t pipe_write_ji(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
/**
 * It will have read head, write head
 * When no data to read, read will be in sleep, when no no space to write, write will be in sleep
 */
int BUFF_SIZE = 10; // 10 character long
dev_t deb_num = MKDEV(27, 0);
struct scull_pipe
{
    char *buf, *end;             // buffer space
    char *rh, *wh;               // read & write head
    struct cdev cdev;            // char dev
    wait_queue_head_t inq, outq; // read and write queues
} sc_pipe;

int pipe_open_ji(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "pipe open\n");
    struct scull_pipe *add;
    add = container_of(inode->i_cdev, struct scull_pipe, cdev);
    if (!add)
        return -1;
    filp->private_data = add;
    return 0;
}

int pipe_release_ji(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "pipe release\n");
    return 0;
}

/* How much space is free? */
static int spacefree(struct scull_pipe *dev)
{
    if (dev->rh == dev->wh)
        return BUFF_SIZE - 1;
    return ((dev->rh + (BUFF_SIZE)-dev->wh) % BUFF_SIZE) - 1;
}

static int scull_getwritespace(struct scull_pipe *dev)
{
	if (spacefree(dev) == 0) { /* full */
        printk(KERN_ALERT "No space to write\n");
		DEFINE_WAIT(wait);
        // change processor state and add to wait queue
		prepare_to_wait(&dev->outq, &wait, TASK_INTERRUPTIBLE);
		if (spacefree(dev) == 0)
            // yield processor
			schedule();
        // This is change processor state to runnable, change the state is necessary if above of failed
        // remove from wait queue
		finish_wait(&dev->outq, &wait);
	}
	return 0;
}

ssize_t pipe_write_ji(struct file *filp, const char __user *buf, size_t count,
                   loff_t *f_pos)
{

    printk(KERN_ALERT "pipe write : %d\n",count);

    struct scull_pipe *dev = filp->private_data;
    scull_getwritespace(dev);
    
    // if(spacefree(dev) == 0){
    //     printk(KERN_ALERT "No space to write\n");
    //     // buffer full no space to write
    //     // return count;
    //     if(wait_event_interruptible(dev->outq, (dev->rh != dev->wh)))
    //         return -ERESTARTSYS;
    // }


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
    printk(KERN_DEBUG "write Count : %d\n",count);
    return count;
}

ssize_t pipe_read_ji(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "pipe read: %d\n",count);

    struct scull_pipe *dev = filp->private_data;

    if(dev->rh == dev->wh){
        // if no data wait until write wakeup
        printk(KERN_ALERT "No space to read\n");
        if (wait_event_interruptible(dev->inq, (dev->rh != dev->wh)))
			return -ERESTARTSYS;
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
    wake_up_interruptible(&(dev->outq));
    printk(KERN_DEBUG "Read Count : %d\n",count);
    return count;
}
struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = pipe_open_ji,
    .read = pipe_read_ji,
    .write = pipe_write_ji,
    .release = pipe_release_ji};

static __init int pipe_init(void)
{
    printk(KERN_ALERT "Init pipe\n");

    cdev_init(&(sc_pipe.cdev), &fops);

    // For register major and minor number
    if (register_chrdev_region(deb_num, 1, "pipe mod") < 0)
    {
        printk(KERN_ALERT "problem in register\n");
        return -1;
    }

    printk(KERN_INFO "Major : %d, Minor : %d\n", MAJOR(deb_num), MINOR(deb_num)); // Major : 27, Minor : 0

    // init buffer, place buf head
    sc_pipe.buf = kmalloc(BUFF_SIZE, GFP_KERNEL);
    sc_pipe.end = sc_pipe.buf + BUFF_SIZE;
    memset(sc_pipe.buf, 0, sizeof(char) * BUFF_SIZE);
    sc_pipe.rh = sc_pipe.wh = sc_pipe.buf;

    init_waitqueue_head(&(sc_pipe.inq));
	init_waitqueue_head(&(sc_pipe.outq));

    // Register driver in kernel
    if (cdev_add(&(sc_pipe.cdev), deb_num, 1) < 0)
    {
        printk(KERN_ALERT "problem in cdev_add\n");
        unregister_chrdev_region(deb_num, 1);
        return -1;
    }

    return 0;
}

static __exit void pipe_exit(void)
{
    // free buf space
    printk(KERN_ALERT "exit pipe\n");
    if (sc_pipe.buf)
    {
        printk(KERN_ALERT "Mem pipe unallocate\n");
        kfree(sc_pipe.buf);
        sc_pipe.buf = NULL;
    }
    cdev_del(&(sc_pipe.cdev));
    unregister_chrdev_region(deb_num, 1);
}

module_init(pipe_init);
module_exit(pipe_exit);
MODULE_LICENSE("GPL");

// mknod /dev/sc_pipe c 27 0
// new terminal $ cat /dev/sc_pipe
// new terminal $ echo "Hi Hello Abdfojfsfsfjofvd" > /dev/sc_pipe