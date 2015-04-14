#include <linux/init.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

#define BLKSIZE 4096

int major = 88;

typedef enum {
    EMPTY,
    NORMAL,
    FULL
}Que_status_t;

struct pse_blk {
    char *buffer;
    int head;
    int tail;
    struct cdev cdev;
    Que_status_t status;
};

static struct pse_blk *g_dev;
DECLARE_MUTEX(dev_sem);

int my_open(struct inode *inode, struct file *filp)
{
    printk("pid=%d open, inode=%p, filp=%p\n",
            current->pid, inode, filp);
    filp->private_data = container_of(inode->i_cdev, 
            struct pse_blk, cdev);
    return 0;
}

int my_release(struct inode * inode, struct file *filp)
{
    return 0;
}

int read_one_elm(struct pse_blk *dev, char *ep)
{
    if (dev->status == EMPTY)
        return -1;

    if (copy_to_user(ep, dev->buffer + dev->tail, sizeof(char))) {
        printk(KERN_ERR"copy to user fail!\n");
        return -EFAULT;
    }
    dev->tail++;
    if (dev->tail == BLKSIZE)
        dev->tail = 0;
    if (dev->tail == dev->head)
        dev->status = EMPTY;
    else
        dev->status = NORMAL;

    return 0;
}

ssize_t my_read(struct file *filp, 
        char __user *buf, size_t size, loff_t *pos)
{
    __u32 len = 0;
    struct pse_blk *dev;
    int i, ret; 

    dev = filp->private_data;
    //char *base = (char *)buf;
   
    if (size == 0)
        return 0;

    for (i = 0; i < size; i++) {
         down(&dev_sem);
        ret = read_one_elm(dev, buf + i);
        if (-1 == ret)
            break;
	len++;
        up(&dev_sem);
    }

    return len;
}

int write_one_elm(struct pse_blk *dev, const char *ep)
{
    if (dev->status == FULL)
        return -1;

    if (copy_from_user(dev->buffer + dev->head, ep, sizeof(char))) {
        printk(KERN_ERR"copy from user fail!\n");
        return -EFAULT;
    }
    dev->head++;
    if (dev->head == BLKSIZE)
        dev->head = 0;
    if (dev->head == dev->tail)
        dev->status = FULL;
    else
        dev->status = NORMAL;
    return 0;
}

ssize_t my_write(struct file *filp, 
        const char __user *buf, size_t size, loff_t *pos)
{
    __u32 len = 0;
    struct pse_blk *dev;

    int i, ret;
    dev = filp->private_data;
   
    //char *base = (char *)buf;
    if (size == 0)
        return 0;

    for (i = 0; i < size; i++) {
	 down(&dev_sem);
        ret = write_one_elm(dev, buf+i);
        if (-1 == ret)
            break;
	len++;
	up(&dev_sem);
    }
    
    return len;
}

static struct file_operations my_ops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write
};

int char_init(void)
{
    int ret;
    dev_t devno;

    g_dev = kmalloc(sizeof(*g_dev), GFP_KERNEL);
    if (!g_dev) {
        printk("kmalloc dev buffer fail!\n");
        return -EFAULT;
    }
    memset(g_dev, 0, sizeof(*g_dev));
    g_dev->buffer = kmalloc(BLKSIZE, GFP_KERNEL);
    if (!g_dev->buffer) {
        printk("kmalloc buffer fail!\n");
        return -EFAULT;
    }

    g_dev->head = g_dev->tail = 0;
    g_dev->status = EMPTY;

    //g_dev->end = 0;
    devno = MKDEV(major, 0);
    ret = register_chrdev_region(devno, 1, "zhang");
    if (ret) {
        printk(KERN_ERR"register dev number fail!\n");
        return -EFAULT;
    }
    cdev_init(&(g_dev->cdev), &my_ops);
    g_dev->cdev.owner = THIS_MODULE;
    cdev_add(&(g_dev->cdev), devno, 1);

    return 0;
}

void char_exit(void)
{
    dev_t devno;
    if (g_dev) {
        if (g_dev->buffer)
            kfree(g_dev->buffer);
        devno = g_dev->cdev.dev;
        unregister_chrdev_region(devno, 1);
        cdev_del(&(g_dev->cdev));
        kfree(g_dev);
        printk("module char exit!\n");
    }
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("Dual BSD/GPL");
