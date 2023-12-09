#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h> // for kmalloc
#include <linux/delay.h>

#define T_NUM 4

static struct task_struct* MyThread[T_NUM];
int i = 0;
int counter = 0;

static int work_fn(void* data) {
        // int original = 0;
        while(!kthread_should_stop()) {
                // critical section
		while(__sync_fetch_and_add(&i, 1));
		counter++;
		printk(KERN_INFO "pid[%u] %s: counter: %d\n", current->pid, __func__, counter);
		__sync_lock_test_and_set(&i, 0);
                // end of the critical section
		msleep(500);
        }
        // do_exit(0);
	return 0;
}


static int __init init_kthread(void) { 
	printk("Entering Compare and Swap Module\n");
	int i;
	for (i=0; i<T_NUM; i++) {
		MyThread[i] = kthread_run(work_fn, NULL, "mythread");
	}

	return 0;
}

static void __exit exit_kthread(void) {
	int i;
	for (i=0; i<T_NUM; i++) {
		if (MyThread[i]) {
			kthread_stop(MyThread[i]);
                	// printk("stop MyThread %d\n", i);
		}
	}
	printk("Exiting Compare and Swap Module\n");
}

module_init(init_kthread);
module_exit(exit_kthread);
