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

struct pse_blk {
    char *buffer;
    __u32 end;
    struct cdev cdev;
};

static struct pse_blk *g_dev;
DECLARE_MUTEX(dev_sem);

int my_open(struct inode *inode, struct file *filp)
{
    printk("pid = %d open, inode = %p, file = %p\n", current->pid, inode, filp);
    filp->private_data = container_of(inode->i_cdev, struct pse_blk, cdev);
    return 0;
}

int my_release(struct inode *inode, struct file *filp)
{
    return 0;
}

ssize_t my_read(struct file *filp, char __user *buf, size_t size, loff_t *pos)
{
    __u32 b, len;
    struct pse_blk *dev;

    dev = filp->private_data;

    down(&dev_sem);
    b = *pos;
    if ((size ==0) || (b >= dev->end))
        return 0;

    len = min(size, dev->end - b);

    up(&dev_sem);
    if (copy_to_user(buf, dev->buffer + b, len)) {
        printk(KERN_ERR"copy to user fail!\n");
        return -EFAULT;
    }

    *pos += len;
    return len;
}

ssize_t my_write(struct file *filp, const char __user *buf, size_t size, loff_t *pos)
{
    __u32 b, len;
    struct pse_blk *dev;

    dev = filp->private_data;
    down(&dev_sem);
    if (0 == size)
        return 0;
    if (filp->f_flags & O_APPEND) {
        b = dev->end;
        *pos = b;
    } else 
        b = *pos;
    if (b >= BLKSIZE) {
        printk("exceed file end!\n");
        return -EFBIG;
    }

    len = min(size, BLKSIZE - b);
    if (copy_from_user(dev->buffer + b, buf, len)) {
        printk(KERN_ERR"copy from user fail!\n");
        return -EFAULT;
    }

    dev->end += len;
    *pos += len;
    up(&dev_sem);
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

    g_dev->end = 0;
    devno = MKDEV(major, 0);
    ret = register_chrdev_region(devno, 1, "zhangming");
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

//2011年 08月 25日 星期四 21:39:23 CST
