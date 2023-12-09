#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

// #include "calclock.h" 
#define BILLION 1000000000UL
unsigned long long calclock(struct timespec *myclock,
		unsigned long long *total_time, unsigned long long *total_count)
{
	unsigned long long timedelay = 0, temp = 0, temp_n = 0;
	if (myclock[1].tv_nsec >= myclock[0].tv_nsec) {
		temp = myclock[1].tv_sec - myclock[0].tv_sec;
		temp_n = myclock[1].tv_nsec - myclock[0].tv_nsec;
		timedelay = BILLION * temp + temp_n;
	} else {
		temp = myclock[1].tv_sec - myclock[0].tv_sec-1;
		temp_n = BILLION + myclock[1].tv_nsec - myclock[0].tv_nsec;
		timedelay = BILLION * temp + temp_n;
	}

	__sync_fetch_and_add(total_time, timedelay);
	__sync_fetch_and_add(total_count, 1);

	return timedelay;
}


// define your lock here
spinlock_t list_lock;
struct rw_semaphore counter_rwse;
struct mutex my_mutex;

// initialize your list here
struct my_node {
	int num;
	struct list_head list;
};

LIST_HEAD(my_list);

unsigned long long list_op_time[3], list_op_count[3];


void* add_to_list(int thread_id, int range_bound[]) {
	printk(KERN_INFO "thread #%d range: %d~ %d\n", thread_id, range_bound[0], range_bound[1]);
	
	int i;
	struct list_head *first;
	for (i=range_bound[0]; i<= range_bound[1]; i++) {
		struct timespec localclock[2];
        	getrawmonotonic(&localclock[0]);
		
		struct my_node *new = kmalloc(sizeof(struct my_node), GFP_KERNEL);
		new->num = i;
		spin_lock(&list_lock); // spinlock
		// down_write(&counter_rwse); // rw_semaphore
		// mutex_lock(&my_mutex); // mutex
		list_add(&new->list, &my_list);
		// up_write(&counter_rwse); // rw_semaphore
		// mutex_unlock(&my_mutex); // mutex
		spin_unlock(&list_lock); // splinlock
		if (i == range_bound[0]) {
			first = &new->list;
		}

		getrawmonotonic(&localclock[1]);
        	calclock(localclock, &list_op_time[0], &list_op_count[0]);
	}
	msleep(500);
	return first;
}

int search_list(int thread_id, void* data, int range_bound[]) {
	printk(KERN_INFO "thread #%d searched range: %d~ %d\n", thread_id, range_bound[0], range_bound[1]);
	
	int i;
	struct my_node *cur = (struct my_node*) data, *tmp;
	for (i=range_bound[0]; i<= range_bound[1]; i++) {
		struct timespec localclock[2];
                getrawmonotonic(&localclock[0]);

		spin_lock(&list_lock); // spinlock
		// down_read(&counter_rwse); // rw_semaphore
		// mutex_lock(&my_mutex); // mutex
		list_for_each_entry_safe(cur, tmp, &my_list, list) {
			if (cur->num == i) {
				// printk("search current node value : %d\n", cur->num);
			}
        	}
		// up_read(&counter_rwse); // rw_semaphore
		// mutex_unlock(&my_mutex); // mutex
		spin_unlock(&list_lock); // splinloc

		getrawmonotonic(&localclock[1]);
                calclock(localclock, &list_op_time[1], &list_op_count[1]);
	}
	return 0;
}

int delete_from_list(int thread_id, int range_bound[]) {
	printk(KERN_INFO "thread #%d deleted range: %d~ %d\n", thread_id, range_bound[0], range_bound[1]);
	
	struct my_node *cur, *tmp;
	int i;
	for (i=range_bound[0]; i<= range_bound[1]; i++) {
		struct timespec localclock[2];
                getrawmonotonic(&localclock[0]);

		spin_lock(&list_lock); // spinlock	
		// down_write(&counter_rwse); // rw_semaphore
		// mutex_lock(&my_mutex); // mutex
		list_for_each_entry_safe(cur, tmp, &my_list, list) {
			if (cur->num == i) {
				// printk("delete current node value : %d\n", cur->num);
				list_del(&cur->list);
				kfree(cur);
			}
		}
		// up_write(&counter_rwse); // rw_semaphore
		// mutex_unlock(&my_mutex); // mutex
		spin_unlock(&list_lock); // splinlock

		getrawmonotonic(&localclock[1]);
                calclock(localclock, &list_op_time[2], &list_op_count[2]);
	}
	
	return 0;
}
