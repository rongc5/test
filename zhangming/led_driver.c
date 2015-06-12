#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/platform_device.h>

#include "led.h"

#define GPM_CONF (0x7F008820)
#define GPM_DATA (0x7F008824)
volatile unsigned int *gpm_conf;
volatile unsigned int *gpm_data;

static int led1_open(struct inode *i, struct file *f)
{
    printk("Func %s is calling\n", __func__);
    return 0;
}

static int led1_release(struct inode *i, struct file *f)
{
    printk("Func %s is calling\n", __func__);
    return 0;
}

static int led2_open(struct inode *i, struct file *f)
{
    printk("Func %s is calling\n", __func__);
    return 0;
}

static int led2_release(struct inode *i, struct file *f)
{
    printk("Func %s is calling\n", __func__);
    return 0;
}

static int led3_open(struct inode *i, struct file *f)
{
    printk("Func %s is calling\n", __func__);
    return 0;
}

static int led3_release(struct inode *i, struct file *f)
{
    printk("Func %s is calling\n", __func__);
    return 0;
}

static int led4_open(struct inode *i, struct file *f)
{
    printk("Func %s is calling\n", __func__);
    return 0;
}

static int led4_release(struct inode *i, struct file *f)
{
    printk("Func %s is calling\n", __func__);
    return 0;
}

void led1_on(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;

    a = (a & ~0x0F) | 0x01;
    *gpm_conf = a;
    *gpm_data = (b & ~0x1) | 0x0;
}

void led1_off(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;

    a = (a & ~0x0F) | 0x01;
    *gpm_conf = a;
    *gpm_data = (b & ~0x1) | 0x1;
}

void led2_on(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;

    a = (a & ~0xF0) | 0x10;
    *gpm_conf = a;
    *gpm_data = (b & ~0x2) | 0x0;
}

void led2_off(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;

    a = (a & ~0xF0) | 0x10;
    *gpm_conf = a;
    *gpm_data = (b & ~0x2) | 0x2;
}

void led3_on(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;

    a = (a & ~0xF00) | 0x100;
    *gpm_conf = a;
    *gpm_data = (b & ~0x4) | 0x0;
}

void led3_off(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;

    a = (a & ~0xF00) | 0x100;
    *gpm_conf = a;
    *gpm_data = (b & ~0x4) | 0x4;
}

void led4_on(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;

    a = (a & ~0xF000) | 0x1000;
    *gpm_conf = a;
    *gpm_data = (b & ~0x8) | 0x0;
}

void led4_off(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;

    a = (a & ~0xF000) | 0x1000;
    *gpm_conf = a;
    *gpm_data = (b & ~0x8) | 0x8;
}

static int led_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
{
    printk("FUN %s is calling\n", __func__);

    if (cmd == LED1) {
        if (arg == ON) {
            led1_on();
        } else if(arg == OFF) {
            led1_off();   
        }
    }

    if (cmd == LED2) {
        if (arg == ON) {
            led2_on();
        } else if(arg == OFF) {
            led2_off();   
        }
    }

    if (cmd == LED3) {
        if (arg == ON) {
            led3_on();
        } else if(arg == OFF) {
            led3_off();   
        }
    }

    if (cmd == LED4) {
        if (arg == ON) {
            led4_on();
        } else if(arg == OFF) {
            led4_off();   
        }
    }

    return 0;
}

static struct file_operations led1 = {
    .open = led1_open,
    .ioctl = led_ioctl,
    .release = led1_release
};

static struct file_operations led2 = {
    .open = led2_open,
    .ioctl = led_ioctl,
    .release = led2_release
};

static struct file_operations led3 = {
    .open = led3_open,
    .ioctl = led_ioctl,
    .release = led3_release
};

static struct file_operations led4 = {
    .open = led4_open,
    .ioctl = led_ioctl,
    .release = led4_release
};

static void led_device_release(struct device *dev)
{
}

static struct platform_device led_device = {
    .name = "led",
    .id = -1,
    .num_resources = 2,
    .resource = &led_resource,
    .dev = {
        .release = &led_device_release,
    }
};

static int led_prob(struct platform_device *pdev)
{
    struct resource *res;
    int size;
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    size = res->end - res->start + 1;
    gpm_conf = ioremap(res->start, size);

    res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
    size = res->end - res->start + 1;
    gpm_data = ioremap(res->start, szie);

    register_chrdev(201, "led", &led);
    register_chrdev(202, "led", &led);
    register_chrdev(203, "led", &led);
    register_chrdev(204, "led", &led);

    return 0;
}

static int led_remove(struct platform_device *pdev)
{
    unregister_chrdev(201, "led");
    unregister_chrdev(202, "led");
    unregister_chrdev(203, "led");
    unregister_chrdev(204, "led");
    iounmap(gpm_conf);
    iounmap(gpm_data);
    return 0;
}

static struct platform_driver led_driver = {
    .driver = {
        .name = "led";
        .owner = THIS_MODULE,
    }
    .prob = led_prob,
    .remove = led_remove,
}

static int led_driver_init(void)
{
    printk("FUN %s is calling\n", __func__);

    platform_device_register(&led_device1);
    platform_device_register(&led_device2);
    platform_device_register(&led_device3);
    platform_device_register(&led_device4);

    platform_driver_register(&led_driver);
    return 0;
}

static void led_driver_exit(void)
{
    printk("FUN %s is calling\n", __func__);

    platform_device_unregister(&led_device1);
    platform_device_unregister(&led_device2);
    platform_device_unregister(&led_device3);
    platform_device_unregister(&led_device4);

    platform_driver_unregister(&led_driver);
}
module_init(led_driver_init);
module_exit(led_driver_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("gurdjieff");
MODULE_DESCRIPTION("LED drivers");
