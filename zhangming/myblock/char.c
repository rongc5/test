#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/slub_def.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

int major = 88;
dev_t devno;
struct cdev mydev;
char *ptr;
char *ptr_read;
char *ptr_write;

int my_open(struct inode *inode, struct file *filp)
{
    ptr = (char *)kmalloc(4096, GFP_KERNEL);
    ptr_read = ptr;
    ptr_write = ptr;
    if (NULL == ptr) {
          printk(KERN_ERR"kmalloc fail!\n");
    }
   
    return 0;
}

int my_realease(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO"goodbye!\n");

    if (!ptr)
        kfree(ptr);
    ptr = NULL;
    return 0;
}

ssize_t my_read(struct file *filp, char __user *buf, size_t size, loff_t *pos)
{
    if (copy_to_user(buf, ptr_read, size)) {
        printk(KERN_ERR"copy to user fail!\n");
        return -EFAULT;
    }

    ptr_read = ptr_read + size;
    return size;
}

ssize_t my_write(struct file *filp, const char __user *buf, size_t size, loff_t *pos)
{   
    if (copy_from_user(ptr_write, buf, sizeof(buf))) {
        printk(KERN_ERR"copy memory fail!\n");
        return -EFAULT;
    }

    ptr_write = ptr_write + size;
    printk("write success!\n");
    return size;
}

static struct file_operations my_ops = {
    .open = my_open,
    .release = my_realease,
    .read = my_read,
    .write = my_write
};

int char_init(void)
{
    int ret;
    devno = MKDEV(major, 0);
    ret = register_chrdev_region(devno, 1, "zhang");
    if (ret) {
        printk(KERN_ERR"register dev number fail !\n");
        return -EFAULT;
    }

    cdev_init(&mydev, &my_ops);
    cdev_add(&mydev, devno, 1);
    return 0;
}

void char_exit(void)
{
    unregister_chrdev_region(devno, 1);
    cdev_del(&mydev);
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("Dual BSD/GPL");


