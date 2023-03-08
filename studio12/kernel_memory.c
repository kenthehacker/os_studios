#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/kthread.h>
#include <linux/mm.h>
#include <asm/uaccess.h>


#define ARR_SIZE 8
typedef struct datatype_t {
    unsigned int array[ARR_SIZE];
} datatype;


static uint nr_structs = 2000;
module_param(nr_structs, uint, 0644); 

static struct task_struct * kthread = NULL;

static unsigned int
my_get_order(unsigned int value)
{
    unsigned int shifts = 0;

    if (!value)
        return 0;

    if (!(value & (value - 1)))
        value--;

    while (value > 0) {
        value >>= 1;
        shifts++;
    }

    return shifts;
}

static int
thread_fn(void * data)
{
    unsigned int order;
    int nr_structs_per_page;
    int nr_pages;
    struct page *pages;

    printk("Hello from thread %s. nr_structs=%u\n", current->comm, nr_structs);
    printk("kernel page size %lu bytes \n",PAGE_SIZE);
    printk("datatype struct is %zu bytes \n", sizeof(datatype));
    printk("%ld fits in a page\n",PAGE_SIZE/sizeof(datatype));
    
    nr_structs_per_page = (int)(PAGE_SIZE/sizeof(datatype));
    nr_pages = nr_structs/nr_structs_per_page;
    if (nr_structs % nr_structs_per_page !=0){
        nr_pages++;
    }
    printk("found we need %u pages\n",nr_pages);
    order = my_get_order((unsigned int)nr_pages);
    printk("order is %u \n",order );
    pages = alloc_pages(GFP_KERNEL, (unsigned int) nr_pages);
    printk("alloced pages \n");
    if (pages == NULL){
        printk("FAILED TO MAKE PAGE");
        return 0;
    }
    printk("SUCCESS: pages made\n");
    printk("nr_struct_per_page: %u nr_pages: %u order: %u \n",nr_structs_per_page,nr_pages,order);

    while (!kthread_should_stop()) {
        schedule();
    }

    return 0;
}

static int
kernel_memory_init(void)
{
    printk(KERN_INFO "Loaded kernel_memory module\n");

    kthread = kthread_create(thread_fn, NULL, "k_memory");
    if (IS_ERR(kthread)) {
        printk(KERN_ERR "Failed to create kernel thread\n");
        return PTR_ERR(kthread);
    }
    
    wake_up_process(kthread);

    return 0;
}

static void 
kernel_memory_exit(void)
{
    kthread_stop(kthread);
    printk(KERN_INFO "Unloaded kernel_memory module\n");
}

module_init(kernel_memory_init);
module_exit(kernel_memory_exit);

MODULE_LICENSE ("GPL");



