#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/gpio.h>

#define BUFFER_SIZE 2
#define RDONLY 0x01
#define WRONLY 0x10
#define RDWR   0x11

#define START_PIN_OFFSET 512
#define NO_OF_DEVICES 9

enum device_names
{
    LED0=2,LED1=16,LED2=20,BTN0=10,BTN1=25,BTN2=18,BUZZER=3,FIRESENSOR=17,SMOKESENSOR=27//,DHT11=4
};

typedef enum pin_direction
{
    IN,OUT
}pin_direction;

struct private_driver_data
{
    int     device_count;
    dev_t   device_number;
    struct class*   myclass;
    struct device*  mydevice;  
};

struct private_dvc_data
{
    int id;
    ssize_t size;
    int perm;
    char buff[BUFFER_SIZE];
    pin_direction direction;
	struct cdev mycdev;
};


/* The prototype functions for the file operations of character driver */
int check_openPermissions(int perm, int access_mode);
int my_open(struct inode *my_inode, struct file *my_file);
ssize_t mydriver_read(struct file *my_file, char __user *u, size_t s, loff_t *off);
ssize_t mydriver_write(struct file *f, const char __user *u, size_t s, loff_t *off);
int mydriver_release(struct inode *inode, struct file *filp);

/* The prototype functions for the platform driver */
int my_remove(struct platform_device * p);
int my_probe(struct platform_device *mytab);


/* The prototype functions for platform devices */
void mydevice_release(struct device *dev);