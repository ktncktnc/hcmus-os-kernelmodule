#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/random.h>

static int randevice_open(struct inode *i, struct file *f);
static int randevice_close(struct inode *i, struct file *f);
static ssize_t randevice_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t randevice_write(struct file *f, const char __user *buf,size_t len, loff_t *off);
static int __init randevice_init(void);
static void __exit randevice_exit(void);

#define RANDEVICE_DEV_NAME "randevice"

static dev_t deviceNumber;
static struct cdev c_dev;
static struct class *ranClass;
static int oc = 0;
static int _ret = 0;
static int _ran_MAX = 123456789;

static struct file_operations pugs_fops ={
    .owner = THIS_MODULE,
    .open = randevice_open,
    .release = randevice_close,
    .read = randevice_read,
    .write = randevice_write
};



static int randevice_open(struct inode *i, struct file *f){
    printk(KERN_INFO "Randevice: open()\n");
    return 0;
}

static int randevice_close(struct inode *i, struct file *f){
    printk(KERN_INFO "Randevice: close()\n");
    return 0;
}

static ssize_t randevice_read(struct file  *f,  char __user  *buf,  size_t len,  loff_t *off){
    get_random_bytes(&_ret, sizeof(_ret));

    //remove the sign bit to get absolute value
    _ret = _ret % _ran_MAX;
    printk(KERN_INFO "Randevice: %d", _ret);
    if(copy_to_user(buf, &_ret, sizeof(_ret)) != 0){
        printk(KERN_ALERT "Randevice: Failed to return random number");
        _ret = 0;
        return -EFAULT;
    }
    _ret = 0;
    printk(KERN_INFO "Randevice: Return random number");
    return 0;
}

static ssize_t randevice_write(struct file *f, const char __user *buf,size_t len, loff_t *off){
    printk(KERN_INFO "Randevice: Opening device: %d time(s)\n", oc++);
    return 0;
}

static int __init randevice_init(void){
    
    if(alloc_chrdev_region(&deviceNumber, 0, 1, "rand_chrdev") <     0){
        printk(KERN_ALERT "Randevice: Failed to allocate\n");
        return -1;
    }

    if((ranClass = class_create(THIS_MODULE, "randcdd")) == NULL){
        unregister_chrdev_region(deviceNumber, 1);
        printk(KERN_ALERT "Randevice: Fail to create class\n");
        return- 1;
    }

    if(device_create(ranClass, NULL, deviceNumber, NULL, RANDEVICE_DEV_NAME) == NULL){
       printk(KERN_ALERT "Randevice: Fail to create device\n");
        class_destroy(ranClass);
        unregister_chrdev_region(deviceNumber, 1);
        return -1;
    }
    
    
    cdev_init(&c_dev, &pugs_fops);

    if(cdev_add(&c_dev, deviceNumber, 1)== -1){
       printk(KERN_ALERT "Randevice: Fail to declare device file");
        device_destroy(ranClass, deviceNumber);
        class_destroy(ranClass);
        unregister_chrdev_region(deviceNumber, 1);
        return -1;
    }
    printk(KERN_INFO "Randevice: device registered\n");
    return 0;
}

static void __exit randevice_exit(void){
    cdev_del(&c_dev);
    device_destroy(ranClass, deviceNumber);
    class_destroy(ranClass);
    unregister_chrdev_region(deviceNumber, 1);
    printk(KERN_INFO "Randevice: unregistered");
}



module_init(randevice_init);
module_exit(randevice_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quoc-Bao Tran - 18120..., Trong-Long Pham - 18120000, Minh-Khoa Le - 18120415");
MODULE_DESCRIPTION("Our first linux kernel module which is used to generate random number.");