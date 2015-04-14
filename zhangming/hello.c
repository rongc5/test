#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int a;
static char *who = "zhang";
static int array[3];
int num = 3;

void dump_information(void)
{
    printk("This module just for test, author:zhang\n");
}

EXPORT_SYMBOL(dump_information);

static int hello_init(void)
{
    printk("i am %s, a = %d\n", who, a);
    printk("%d, %d, %d\n", array[0], array[1], array[2]);
    return 0;
}

static void hello_exit(void)
{
    printk("good bye!");
}

module_param(a, int, 0644);//声明外部传参
module_param(who, charp, 0644);
module_param_array(array, int, &num, 0644);

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("richard.zhang");




