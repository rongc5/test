#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/slub_def.h>

static int proc_init(void)
{
    struct *page;
    printk("%p", page = alloc_pages(GFP_ATOMIC, 0));
    __free_pages((page), 0);
    return 0;
}

static void proc_exit(void)
{
    printk("bye bye!\n");
}

module_init(proc_init);
module_exit(proc_exit);
//2011年 08月 19日 星期五 11:21:29 CST
