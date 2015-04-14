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
  //  int i;
    page = alloc_page(GFP_KERNEL);
    SYS_INFO("page = %p, page's virtual addr = %P"
            "physical addr = %p, pfn = %p\n",
            page, page_address(page),
            __phys_addr(page_address(page)),
            page_to_pfn(page) << 12);
#if 0
    for (i = 0; i < 13; i++) {
        SYS_INFO("kmalloc size = %d\n", kmalloc_caches[i].objsize);
    }
    ptr = (char *)kmalloc(4096, GFP_KERNEL);
    if (NULL == ptr)
        printk("kmalloc fail!\n");
#endif
    return 0;
}

static void hello_exit(void)
{
    __free_pages(page, 0);
#if 0
    if (!ptr)
        kfree(ptr);
#endif
    SYS_ERROR("Goodbye!");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
