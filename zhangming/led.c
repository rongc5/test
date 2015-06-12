#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/io.h>

#include "led.h"

#define GPM_CONF ()
volatile unsigned int *gpm_conf
volatile unsigned int *gpm_data









void led1_off(void)
{
    int a = *gpm_conf;
    int b = *gpm_data;
    a = (a & ~0x0F) | 0x01;
    *gpm_conf = a;
    *gpm_data = (b & ~0x1) | 0x1;
}

static int led_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
{
    if (cmd == LED1) {
        if (cmd == LED1) {
            
        }
    }
}

static struct file_operations led = {
    .open = led_open,
    .ioctl = led_ioctl,
    .release = led_release
};

static int led_driver_init(void)
{
    printk("Func %s is calling\n", __func__);
}

static 
    
