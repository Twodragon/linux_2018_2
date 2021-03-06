#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/timer.h>
#include <linux/input.h>


#define DEVICE_NAME "keylogger"
#define START_MSG "Hello!\nYou have to wait for a minute to expire to see the keylogger statistics!\n"
#define MSG_BUFFER_LEN 150
#define TIMER_INTERVAL 60000
MODULE_LICENSE("GPL v2");


static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);

static int major_num;
static int device_open_count = 0;
static char msg_buffer[500];


static struct file_operations file_ops = {
	.read = device_read,
	.open = device_open,
	.release = device_release
};

static int keylogger_cb(struct notifier_block *nblock,
		unsigned long code,
		void *_param);

static int touches_num = 0;


static struct notifier_block keylogger_blk = {
	.notifier_call = keylogger_cb,
};

static struct timer_list touches_timer;


void touches_timer_callback(struct timer_list *t)
{
	memset(msg_buffer, '\0', MSG_BUFFER_LEN); 
	sprintf(msg_buffer, "You pressed %d keys in the last minute\n", touches_num);
	touches_num = 0;
	mod_timer(&touches_timer, jiffies + msecs_to_jiffies(TIMER_INTERVAL));
}


int keylogger_cb(struct notifier_block *nblock,
		  unsigned long code,
		  void *_param)
{

	struct keyboard_notifier_param *param = _param;


	/* Trace only when a key is pressed down */
	if (!(param->down))
		return NOTIFY_OK;

	int keycode = param->value;
	if (keycode > KEY_RESERVED && keycode <= KEY_PAUSE) 
		touches_num++;

	return NOTIFY_OK;
}

static ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset) {
	return simple_read_from_buffer(buffer, len, offset, msg_buffer, MSG_BUFFER_LEN);
}

static int device_open(struct inode *inode, struct file *file) {

 	if (device_open_count) {
 	return -EBUSY;
 	}

 	device_open_count++;
 	try_module_get(THIS_MODULE);
 	return 0;
}


static int device_release(struct inode *inode, struct file *file) {
 	device_open_count--;
 	module_put(THIS_MODULE);
 	return 0;
}

static int __init keylogger_init(void)
{
 	major_num = register_chrdev(0, "keylogger", &file_ops);

 	if (major_num < 0) {
 		printk(KERN_ALERT "Could not register device: %d\n", major_num);
 		return major_num;
 	} else {
	 	strncpy(msg_buffer, START_MSG, MSG_BUFFER_LEN);
		printk(KERN_INFO "keylogger module loaded with device major number %d\n", major_num);
	}


	timer_setup(&touches_timer, touches_timer_callback, 0);
 	mod_timer(&touches_timer, jiffies + msecs_to_jiffies(TIMER_INTERVAL));

	register_keyboard_notifier(&keylogger_blk);
	return 0;
}

static void __exit keylogger_exit(void)
{
	unregister_keyboard_notifier(&keylogger_blk);
 	unregister_chrdev(major_num, DEVICE_NAME);
	del_timer(&touches_timer);

 	printk(KERN_INFO "Goodbye, World!\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);
