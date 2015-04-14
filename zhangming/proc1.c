#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

//extern struct proc_dir_entry *create_proc_entry(const char *name, mode_t mode, struct proc_dir_entry *parent);

//typedef int (read_proc_t)(char *page, char **start, off_t off, int count, int *eof, void *data);

struct proc_dir_entry *base, *info;

//typedef int (write_proc_t)(struct file *file, const char __user*buffer, unsigned long count, void *dat);

char displaybuf[100] = "hello, just test!\n";

int write_infomation(struct file *file, const char *buf, unsigned long count, void *data)
{
    char buffer[100];
    int size;

    size = min(100, count);

    if (copy_from_user(buffer, buf, count)) {
        printk(KERN_ERR"copy memory fail!\n");
        return -EFAULT;
    }

    memcpy(displaybuf, buffer, size);
    return size;
}

int display_info(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int ret = 0;

    ret += sprintf(page, displaybuf);
    *eof = 1;
    return ret;
}

static int proc_init(void)
{
    base = proc_mkdir("zhang", NULL);
    if (!base) {
        printk(KERN_ERR"create proc directory fail!\n");
        return -ENOMEM;
    }
    info = create_proc_entry("info", 0666, base);
    if (!info) {
        printk(KERN_ERR"create proc file fail!\n");
        return -ENOMEM;
    }
    info->read_proc = display_info;
    info->write_proc = write_infomation;
    return 0;
}

static void proc_exit(void)
{
    if (info)
        remove_proc_entry("info", base);
    if (base)
        remove_proc_entry("ming", NULL);
    printk("bye bye!\n");
}

module_init(proc_init);
module_exit(proc_exit);
//2011年 08月 18日 星期四 16:40:45 CST
