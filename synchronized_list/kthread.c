#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h> // for kmalloc
#include <linux/delay.h>

#include "linked_list_impl.c"

#define T_NUM 4
#define RANGE 2500
struct task_struct* MyThread[T_NUM];

void set_iter_range(int thread_id, int *range_bound) {
	range_bound[0] = thread_id * RANGE;
	range_bound[1] = range_bound[0] + RANGE - 1;
}

static int work_fn(void* data) {
	int range_bound[2];
	int thread_id = *(int*) data;
	set_iter_range(thread_id, range_bound);
	void *ret = add_to_list(thread_id, range_bound);
 	search_list(thread_id, ret, range_bound);
	delete_from_list(thread_id, range_bound);
	
	while(!kthread_should_stop()) {
		msleep(500);
	}
	printk(KERN_INFO "thread #%d stopped!\n", thread_id);

	return 0;
}


static int __init init_kthread(void) { 
	printk("Entering Spinlock  Module\n");
	int i;
	for (i=0; i<T_NUM; i++) {
		int *t = (int *)kmalloc(sizeof(int),GFP_KERNEL);
		*t = i;
		MyThread[i] = kthread_run(work_fn, t, "mythread");
	}

	return 0;
}

extern unsigned long long list_op_time[3], list_op_count[3];

static void __exit exit_kthread(void) {
	int i;
	for (i=0; i<T_NUM; i++) {
		if (MyThread[i]) {
			kthread_stop(MyThread[i]);
		}
		// printk("stop Thread %d\n", i);
	}
	printk("linked list insert time: %llu ns, counts: %llu\n",
			list_op_time[0], list_op_count[0]);
	printk("linked list search time: %llu ns, counts: %llu\n",
                        list_op_time[1], list_op_count[1]);
	printk("linked list delete time: %llu ns, counts: %llu\n",
                        list_op_time[2], list_op_count[2]);
	printk("Exiting Spinlock Module\n");
}

module_init(init_kthread);
module_exit(exit_kthread);
