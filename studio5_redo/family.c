#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/sched.h>

static pid_t pid;
struct pid *pid_struct;
struct task_struct *task_of_pid;


static ssize_t pid_show(struct kobject *kobj, struct kobj_attribute *attr,char *buf) {
	return sprintf(buf, "%d\n", pid);
}


static ssize_t pid_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
	int ret;
	ret = kstrtoint(buf, 10, &pid);
	if (ret < 0){
		return ret;
	}
	printk(KERN_INFO "pid value: %d\n",pid);
		
	pid_struct = find_vpid(pid);


	if (pid_struct == NULL){
		printk(KERN_INFO "find_vpid failed \n");
	}
	task_of_pid  = get_pid_task(pid_struct,PIDTYPE_PID);
		
	int counter = 100;
	while(task_of_pid->real_parent!=NULL && pid>=1 && counter >0){
		printk(KERN_INFO "comm: %s PID: %d\n",task_of_pid->real_parent->comm ,pid );
		task_of_pid = task_of_pid->real_parent;
		pid = task_of_pid->pid;
		counter--;
	}
	
	return count;
}


static struct kobj_attribute pid_attribute =
	__ATTR(pid, 0664, pid_show, pid_store);



static struct attribute *attrs[] = {
	&pid_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;


static int __init example_init(void){
	int retval;
	
	example_kobj = kobject_create_and_add("kobject_pid_example", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(example_kobj, &attr_group);
	if (retval)
		kobject_put(example_kobj);
	printk(KERN_ALERT "pid module initialized\n");
	return retval;

}


static void __exit example_exit(void)
{
	kobject_put(example_kobj);
}


module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("KEN");




