#include "mq.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>/*alloc_chrdev_region()
						struct inode*/
#include <linux/cdev.h>/*struct cdev*/
#include <linux/device.h>/*class_create()
							device_create()*/
#include <linux/slab.h> /*kmalloc()*/
#include <linux/list.h>
#include <linux/uaccess.h>//copy_from_user
#define NUM_OF_MINORS 8
#define MAX_MSG_CAPACITY 100

/*STRUCTS*/
static struct cdev cdev_msgq;
static struct class *class_msgq;
static dev_t dev_msgq;

struct q_message
{
	struct list_head node;
	char* data;
	int size;
};

struct ipc_queue
{	
	struct device* msgq_dev;
	unsigned long msg_counter;
	wait_queue_head_t wait_write_queue;
	wait_queue_head_t wait_read_queue;
	struct mutex lock;
	struct list_head list;

};

static struct ipc_queue* msgq_list;
/***END OF STRUCTS***/

static int isNotEmpty(struct ipc_queue* queue)
{
	mutex_lock(&queue->lock);
	if(queue->msg_counter==0)
	{
		mutex_unlock(&queue->lock);
		return 0;
	}
	return 1;
}

static int isNotFull(struct ipc_queue* queue)
{
	mutex_lock(&queue->lock);
	if(queue->msg_counter==MAX_MSG_CAPACITY) {
		mutex_unlock(&queue->lock);
		return 0;
	}
	return 1;
}

static void ipc_msgq_ctor(struct ipc_queue* queue, struct device* dev)
{
	queue->msg_counter = 0;
	queue->msgq_dev = dev;
	init_waitqueue_head(&queue->wait_write_queue);
	init_waitqueue_head(&queue->wait_read_queue);
	mutex_init(&queue->lock);
	INIT_LIST_HEAD(&queue->list);
}

static void q_message_dtor(struct q_message* message)
{
	kfree(message->data);
	kfree(message);
}


/*FILE OPERATIONS*/
static int open_ipc_msgq_dev(struct inode* inode, struct file* f)
{
	pr_info("ipc_msgq_dev: inside OPNE function\n");
	f->private_data = msgq_list+(iminor(inode)-MINOR(dev_msgq));
	return 0;
}

static int release_ipc_msgq_dev(struct inode* ind, struct file* f)
{
	pr_info("ipc_msgq_dev: TEST inside RELEASE function\n");
	return 0;
}

static long ipc_msgq_dev_ioctl(struct file* f, unsigned int mode, unsigned long message_addr)
{
	struct mq_reg new_message;
	struct ipc_queue* curr_ipc_queue = f->private_data;
	struct q_message* new_q_message=NULL;
	int retval = 0;
	
	pr_info("ipc_msgq_dev: inside IOCTL function\n");
	switch(mode)
	{
		case MSGQ_WRITER:

			pr_info("ipc_msgq_dev: inside WRITER mode\n");
			retval = copy_from_user(&new_message, (struct mq_reg*)message_addr, sizeof(struct mq_reg));
			if(retval)
			{
				pr_err("ipc_msgq_dev: error on copy_from_user(mq_reg)\n");
				pr_err("ipc_msgq_dev: message - %s\n", new_q_message->data);
				pr_err("ipc_msgq_dev: size - %d\n", new_q_message->size);

				return retval;
			}
			
			new_q_message = kmalloc(sizeof(struct q_message), GFP_KERNEL);
			if (IS_ERR(new_q_message))
			{
				pr_err("ipc_msgq_dev: kmalloc q_message error\n");
				return PTR_ERR(new_q_message);
			}

			new_q_message->data = kmalloc(new_message.size, GFP_KERNEL);
			if (IS_ERR(new_q_message->data))
			{
				pr_err("ipc_msgq_dev: kmalloc q_message->data error\n");
				kfree(new_q_message);
				return PTR_ERR(new_q_message->data);
			}

			retval = copy_from_user(new_q_message->data, new_message.data, new_message.size);
			new_q_message->size = new_message.size;
			if(retval)
			{
				pr_err("ipc_msgq_dev: error on message from recieved mq_reg.data\n");
				kfree(new_q_message->data);
				kfree(new_q_message);
				return retval;
			}

			wait_event_interruptible(curr_ipc_queue->wait_write_queue, isNotFull(curr_ipc_queue));
			list_add(&new_q_message->node, &curr_ipc_queue->list);
			pr_info("ipc_msgq_dev: recieved message %s\n", new_q_message->data);
			pr_info("ipc_msgq_dev: recieved message size = %d\n", new_q_message->size);
			++curr_ipc_queue->msg_counter;
			mutex_unlock(&curr_ipc_queue->lock);
			wake_up_interruptible(&curr_ipc_queue->wait_read_queue);
			return 0;

		case MSGQ_READER:

			pr_info("ipc_msgq_dev: inside READER mode\n");
			wait_event_interruptible(curr_ipc_queue->wait_read_queue, isNotEmpty(curr_ipc_queue));
			new_q_message = list_entry((&curr_ipc_queue->list)->prev, struct q_message, node);
			list_del((&curr_ipc_queue->list)->prev);
			curr_ipc_queue->msg_counter--;
			mutex_unlock(&curr_ipc_queue->lock);
			wake_up_interruptible(&curr_ipc_queue->wait_write_queue);			

			pr_info("ipc_msgq_dev: message size - %d\n", new_q_message->size);
			pr_info("ipc_msgq_dev: message - %s\n", new_q_message->data);
			retval = copy_to_user((char*)message_addr, new_q_message->data, new_q_message->size);
			if (retval)
			{
				pr_err("ipc_msgq_dev: copy_to_user error\n");
				return retval;
			}
			q_message_dtor(new_q_message);
			
			return new_q_message->size;

		default:
			return -ENOTTY;
	}
	return -ENOTTY;
}

/***END OF FILE OPERATIONS***/

static struct file_operations file_ops = 
{
	.owner = THIS_MODULE,
	.open = open_ipc_msgq_dev,
	.release = release_ipc_msgq_dev,
	.unlocked_ioctl = ipc_msgq_dev_ioctl
};


static int __init msgq_init(void)
{
	int retval = 0;
	int i;
	pr_info("ipc_msgq_dev: Inserting ipc_msgq_dev module\n");

	msgq_list = kmalloc(sizeof(struct ipc_queue)*NUM_OF_MINORS, GFP_KERNEL);
	if(!msgq_list)
	{
		pr_err("ipc_msgq_dev: ***FAIL*** kmalloc(msgq_list)\n");
		goto ret_val_exit;
	}

	retval = alloc_chrdev_region(&dev_msgq, 0, NUM_OF_MINORS, "ipc_msgq_dev");
	if (retval != 0)
	{
		pr_err("ipc_msgq_dev: ***FAIL*** alloc_chrdev_region()\n");
		goto alloc_chrdev_region_err;
	}

	pr_info("ipc_msgq_dev: major = %d\n", MAJOR(dev_msgq));

	cdev_init(&cdev_msgq, &file_ops);

	retval = cdev_add(&cdev_msgq, dev_msgq, NUM_OF_MINORS);
	if (retval != 0)
	{
		pr_err("ipc_msgq_dev: ***FAIL*** cdev_add()\n");
		goto cdev_add_err;
	}

	class_msgq = class_create(THIS_MODULE, THIS_MODULE->name);

	if (IS_ERR(class_msgq)) 
	{
		pr_err("ipc_msgq_dev: ***FAIL*** class_create()\n");
		retval = PTR_ERR(class_msgq);
		goto class_create_err;
	}

	for (i = 0; i < NUM_OF_MINORS; ++i)
	{
		struct device* dev = device_create(	class_msgq, 
											NULL, 
											MKDEV(MAJOR(dev_msgq), MINOR(dev_msgq)+i), 
											NULL, 
											"%s%d", 
											THIS_MODULE->name,
											i);

		if (IS_ERR(dev))
		{
			pr_err("ipc_msgq_dev: ***FAIL*** device_create() iteration %d\n", i);
			goto device_create_err;
		}

		ipc_msgq_ctor(&msgq_list[i], dev);	
	}

	goto ret_val_exit;

device_create_err:
	class_destroy(class_msgq);

class_create_err:
	cdev_del(&cdev_msgq);

cdev_add_err:
	unregister_chrdev_region(dev_msgq, NUM_OF_MINORS);

alloc_chrdev_region_err:
	kfree(msgq_list);

ret_val_exit:
	return retval;
}

static void __exit msqq_exit(void)
{
	int i;
	pr_info("ipc_msgq_dev: Removing msg queue module\n");
	
	pr_info("ipc_msgq_dev: Devices destroy\n");
	for(i=0;i < NUM_OF_MINORS; ++i)
	{
		device_destroy(class_msgq, 
						MKDEV(	MAJOR(dev_msgq), 
								MINOR(dev_msgq)+i));
	}
	
	pr_info("ipc_msgq_dev: class_destroy\n");
	class_destroy(class_msgq);

	pr_info("ipc_msgq_dev: cdev_del\n");
	cdev_del(&cdev_msgq);

	pr_info("ipc_msgq_dev: unregister_chrdev_region\n");
	unregister_chrdev_region(dev_msgq, NUM_OF_MINORS);

	pr_info("ipc_msgq_dev: kfree\n");
	kfree(msgq_list);
}

module_init(msgq_init);
module_exit(msqq_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IPC message queue");
MODULE_AUTHOR("Alex");
