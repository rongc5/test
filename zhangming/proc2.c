#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/slab.h>

struct stu {
    char name[32];
    int id;
    int avg;
    struct list_head_entry;
};

LIST_HEAD(g_head);

struct proc_dir_entry *base, *info;
char displaybuf[100] = "hello, just test!\n";

int write_info(struct file *file, const char *buf, unsigned long count, void *data)
{
    char buffer[100];
    int size;

    size = (int)min((unsigned long)100, count);

    if (copy_from_user(buffer, buf, size)) {
        printk(KERN_ERR"copy memory fail!\n");
        return -EFAULT;
    }
    memcpy(displaybuf, buffer, size);
    return size;
}

int display_info(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    struct stu *pos;
    int ret = 0;
    memset(page, 0, PAGE_SIZE);
    ret += sprintf(page+ret, "name  id    score\n");

    list_for_each_entry(pos, &g_head, entry) {
        ret += sprintf(page+ret, "%s    %d  %d\n", pos->name, pos->id, pos->avg);

    }
    *eof = 1;
    return ret;
}

static int proc_init(void)
{
    int i;
    struct stu *p;
    char buf[32];
    for (i = 0; i < 3; i++) {
        p = kmalloc(sizeof(*p), GFP_KERNEL);
        if (!p) {
            printk(KERN_ERR"kmalloc fail!\n");
            return -ENOMEM;
        }
        sprintf(buf, "a%d", i);
        strcpy(p->name, buf);
        p->id = i;
        p->avg = 80;
        list_add(&(p->entry), &g_head);
    }

    base = proc_mkdir("zhang ming", NULL);
    if (!base) {
        printk(KERN_ERR"Create proc directory fail!\n");
        return -ENOMEN;
    }
    info = create_proc_entry("info", 0666, base);
    if (!info) {
        printk(KERN_ERR"Create proc file fail!\n");
        return -ENOMEN;
    }
    info->read_proc = display_info;
    info->write_info = write_info;
    return 0;
}

static void proc_exit(void)
{
    struct stu *pos, *n;
    if (info)
        remove_proc_entry("info", base);
    list_for_each_entry_safe(pos, n, &g_head, entry) {
        if (pos)
            kfree(pos);
    }
    printk("bye bye!\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("Dual BSD/GPL");
