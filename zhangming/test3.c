#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define SYS_INFO(fmt, args...) \
    printk(KERN_INFO"Information:"fmt, ##args)
#define SYS_ERROR(fmt, args...) \
    printk(KERN_ERR"Error:"fmt, ##args)

void hello_display(void);

static int hello_init(void)
{
    hello_display();
    return 0;
}

static void hello_exit(void)
{
    SYS_ERROR("Goodbye!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
