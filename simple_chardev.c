#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "simple_chardev"
#define CLASS_NAME "simplechrdev"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sysprog Semangat Team");
MODULE_DESCRIPTION("Simple Linux char driver to read and write");

 // Function prototypes usually used by the driver 

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

int majorNumber;
char msg[256] = {0};
char *msg_ptr;
struct class* cls;
struct device* dev;
int open_count;
short size_of_message;

// Pointers to the function used to implement read, write, open, release syscall

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

// Function called when module loaded

int __init init_mod(void){
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops); //Register driver
	if (majorNumber < 0) {
	  printk(KERN_ALERT "%s: Registering char device failed with %d\n", CLASS_NAME, majorNumber);
	  return majorNumber;
	}
	
	// Create device class and create device to add it to device node in /dev
	// Add to device node to be able to access from user space
	cls = class_create(THIS_MODULE, CLASS_NAME);
	dev = device_create(cls, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	printk(KERN_INFO "%s: Device created successfully\n", CLASS_NAME);
	return 0;
}

// Function called whne module unloaded
void __exit exit_mod(void){
	device_destroy(cls, MKDEV(majorNumber, 0));
	class_unregister(cls);
	class_destroy(cls);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_INFO "%s: Device removed from Loadable Kernel Module\n", CLASS_NAME);
}

// Function called when process try to open device file
static int device_open(struct inode *inode, struct file *file){
	open_count++;
	printk(KERN_INFO "%s: Device file opened %d times\n", CLASS_NAME, open_count);
	return 0;
}

// Function called when process closes the device file
static int device_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "%s: Device file closed", CLASS_NAME);
	return 0;
}

// Function called when process attempt to read device while device file is open
static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset){
	size_t bytesToCopy = length >= size_of_message ? size_of_message: length;
    size_t bytesNotCopied = 0;
    if(!bytesToCopy) return 0;

	bytesNotCopied = raw_copy_to_user(buffer, msg, bytesToCopy);
    size_of_message = 0;
    return bytesToCopy;
}

static ssize_t device_write(struct file *file, const char *buffer, size_t length, loff_t *offset){
	const size_t maxLen = 256 - 1;
    size_t bytesToCopy = length >= maxLen ? maxLen: length;
    size_t bytesNotCopied = 0;

    bytesNotCopied = raw_copy_from_user(msg, buffer,bytesToCopy);
    size_of_message = bytesToCopy - bytesNotCopied;

    return bytesToCopy;
}

module_init(init_mod);
module_exit(exit_mod);
