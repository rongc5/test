#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/slub_def.h>
#include <linux/slab.h>

#define SYS_INFO(fmt, args...)\
    printk(KERN_INFO"Information:"fmt, ##args);
#define SYS_ERROR(fmt, args...) \
    printk(KERN_ERR"Error:"fmt, ##args)

struct stu {
    char name[32];
    int id;
    int avg;
};

struct kmem_cache *stu_cache;
struct stu *a[3];

static int hello_init(void)
{
    int i;
    stu_cache = kmem_cache_create("stu_info", sizeof(struct stu), 0, 0, NULL);
    if (stu_cache == NULL) {
        SYS_ERROR("create cache fail!\n");
        return -ENOMEM;
    }

    for (i = 0 ; i < 3; i++) {
        a[i] = kmem_cache_alloc(stu_cache, GFP_KERNEL);
        if (a[i] == NULL) {
            SYS_ERROR("get object from cache fail!\n");
            return -ENOMEM;
        }
        strcpy(a[i]->name, "abc");
        a[i]->id = i;
        a[i]->avg = 100;
    }
    return 0;
}

static void hello_exit(void)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (stu_cache && a[i]) {
            SYS_INFO("name = %s, id = %d, avg = %d\n", a[i]->name, a[i]->id, a[i]->avg);
            kmem_cache_free(stu_cache, a[i]);
        }
    }
    if (stu_cache)
        kmem_cache_destroy(stu_cache);
    SYS_ERROR("good bye");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
