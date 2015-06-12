#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

void dump_information(void);

static int hello_init(void)
{
    dump_information();

    return 0;
}

static void hello_exit(void)
{
        printk("bye!");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("richard.zhang");

