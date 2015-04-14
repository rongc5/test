#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/slub_def.h>
#define SYS_INFO(fmt, args...) \
        printk(KERN_INFO"Information:"fmt, ##args)
#define SYS_ERROR(fmt, args...) \
        printk(KERN_ERR"Error:"fmt, ##args)

struct page *page;
char *ptr;
static int hello_init(void)
{
    int i;
    page = alloc_page(GFP_KERNEL);
    SYS_INFO("page = %p, page's virtual addr = %p"
            "physical addr = %p, pfn = %p\n",
            page, page_address(page), __phys_addr)
}
