#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define SYS_INFO(fmt, args...) \
    printk(KERN_INFO"Information:"fmt, ##args)
#define SYS_ERROR(fmt, args...) \
    printk(KERN_ERR"Error:"fmt, ##args);

int k = 0;
void hello_display(void)
{
    SYS_INFO("I am happy!\n");
}

EXPORT_SYMBOL(hello_display);

static int hello_init(void)
{
    int i = 50;
    while (i--) {
        if (printk_ratelimit())
            SYS_INFO("number %d, k = %d!\n", i, k);
        else
            k++;
    }
    return 0;
}

static void hello_exit(void)
{
    SYS_ERROR("Goodbye!k = %d\n", k);
    dump_stack();
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
