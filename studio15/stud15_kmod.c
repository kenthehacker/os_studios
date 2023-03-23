#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <linux/nsproxy.h>
#include <linux/kthread.h>

static struct task_struct * kthread = NULL;

static int thread_fn(void * data){
    printk(KERN_INFO "fs: %p files: %p nsproxy: %p \n", current->fs, current->files, current->nsproxy);
    while(!kthread_should_stop()){
        //do nothing
    }
    return 0;
}


static int k_mod_init(void){
    printk(KERN_INFO "LOADED studio 15 module \n");
    kthread = kthread_create(thread_fn, NULL, "vfs_kthread");

    return 0;
}

static void k_mod_exit(void){
    kthread_stop(kthread);
    printk(KERN_INFO "UNLOADED studio 15 module \n");
}

module_init(k_mod_init);
module_exit(k_mod_exit);