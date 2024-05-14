#include "gpio_driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heba");
MODULE_DESCRIPTION("Your module description");

static bool read_flag = false;

struct private_driver_data prv_driver =
{
    .device_count = NO_OF_DEVICES,
};

const struct platform_device_id mytable[] = {
 {.name ="LED0", .driver_data = LED0},
 {.name ="BTN0", .driver_data = BTN0},
 {.name ="LED1", .driver_data = LED1},
 {.name ="BTN1", .driver_data = BTN1},
 {.name ="LED2", .driver_data = LED2},
 {.name ="BTN2", .driver_data = BTN2},
 {.name ="BUZZER", .driver_data = BUZZER},
 {.name ="FIRESENSOR", .driver_data = FIRESENSOR},
 {.name ="SMOKESENSOR", .driver_data = SMOKESENSOR},
//  {.name ="DHT11", .driver_data = DHT11},
 {}
};

struct file_operations fops =
{
    .open = my_open,
    .read = mydriver_read,
	.write = mydriver_write,
    .release = mydriver_release,
    .owner = THIS_MODULE 
    
};

struct platform_driver platform_driver_data =
{
    .probe = my_probe,
    .remove = my_remove,
    .id_table = mytable,
    .driver = {
        .name = "mydrive"
    }

};

int check_openPermissions(int perm, int access_mode)
{
    //  0       --> allowed
    //  -EPERM  --> operation not permitted
    
    if(perm == RDWR)
    {
        return 0;
    }

    /* Read only */
    if((perm == RDONLY) && ((access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE)))
    {
        return 0;
    }

    /* Write only */
    if((perm == WRONLY) && ((access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ)))
    {
        return 0;
    }

    /* Operation not permitted */
    return -EPERM;
}

int my_open(struct inode *my_inode, struct file *my_file)
{
    int r_err = 0;
    struct private_dvc_data *pcdev = container_of(my_inode->i_cdev,struct private_dvc_data, mycdev);
    my_file->private_data = pcdev;
    r_err = check_openPermissions(pcdev->perm, my_file->f_mode);
    if(!r_err)
    {
        pr_info("file opened successfully\n");  
    }
    else
    {
        pr_info("file didnt open successfully\n");
    }
    return r_err;
}

ssize_t mydriver_read(struct file *my_file, char __user *u, size_t s, loff_t *off) 
{

    struct private_dvc_data *pcdev = (struct private_dvc_data *) my_file->private_data;

    pcdev->buff[0] = 48 + gpio_get_value(START_PIN_OFFSET + pcdev->id); 
    pcdev->buff[1] = '\n';

    if (read_flag == false) {
        if(copy_to_user(u, pcdev->buff, 2))
        {
            pr_info("Error Read pin no = %d and pin value is = %d\n", pcdev->id, gpio_get_value(START_PIN_OFFSET + pcdev->id));         
            return -EFAULT;
        }
        read_flag = true;            // Set flag to indicate read has occurred
        pr_info("Success Read pin no = %d and pin value is = %d\n", pcdev->id, gpio_get_value(START_PIN_OFFSET + pcdev->id));
        return 2;                    // Return the number of bytes written
    }
	read_flag = false;      
    pr_info("Read pin no = %d and pin value is = %d\n", pcdev->id, gpio_get_value(START_PIN_OFFSET + pcdev->id));         
    return 0;                      
}

ssize_t mydriver_write(struct file *f, const char __user *u, size_t s, loff_t *off)
{
    struct private_dvc_data *pcdev = f->private_data;
    memset(pcdev->buff, 0, pcdev->size); // Clear the device buffer
    if(copy_from_user(pcdev->buff, u, 1))
    {
        pr_info("Error write value = %c in pin no = %d and pin value become = \n", pcdev->buff[0], pcdev->id, gpio_get_value(START_PIN_OFFSET + pcdev->id));
        return -EFAULT;
    }

    switch(pcdev->buff[0])
    {
        case '0':
            gpio_set_value(START_PIN_OFFSET + pcdev->id, 0);
            break;
        case '1':
            gpio_set_value(START_PIN_OFFSET + pcdev->id, 1);
            break;
        default:
            pr_info("Invalid data\n");
            return -EINVAL;
    }

    pr_info("write value = %c in pin no = %d and pin value become = \n", pcdev->buff[0], pcdev->id, gpio_get_value(START_PIN_OFFSET + pcdev->id));
	return s;
}

int mydriver_release(struct inode *inode, struct file *filp) 
{
    pr_info("file Released successful\n");
    return 0;
}

int my_remove(struct platform_device * p)
{
    struct private_dvc_data* pcdev;
    pcdev = dev_get_platdata(&p->dev);
    if(pcdev->direction == OUT)
    {
    gpio_set_value(START_PIN_OFFSET + pcdev->id, 0);
    }
    gpio_free(START_PIN_OFFSET + pcdev->id);
    device_destroy(prv_driver.myclass,prv_driver.device_number +pcdev->id);
    cdev_del(&pcdev->mycdev);

    pr_info("pin no = %d is removed\n", pcdev->id);
    return 0;
}


int my_probe(struct platform_device *mytab)
{
        struct private_dvc_data* pcdev;
        pcdev = dev_get_platdata(&mytab->dev);
        cdev_init(&pcdev->mycdev,&fops);
        cdev_add(&pcdev->mycdev,prv_driver.device_number + mytab->id,1);
        prv_driver.mydevice = device_create(prv_driver.myclass,NULL,prv_driver.device_number + mytab->id,NULL,mytab->name); 
        pr_info("device created successfully\n");
        
        /*
         * Attempt to request GPIO pin.
         */
        if (gpio_request(START_PIN_OFFSET + pcdev->id, mytab->name))
        {
            pr_info("Cannot allocate GPIO pin %d\n",pcdev->id);
        }
        else
        {
            pr_info("GPIO pin %d allocated successfully\n", pcdev->id);
        }
        
        /*
         * Set GPIO pin direction.
         */
        if(pcdev->direction == IN)
        {
            if (gpio_direction_input(START_PIN_OFFSET + pcdev->id))
            {
                pr_info("Cannot set the GPIO pin no %d to be input\n",pcdev->id);
                gpio_free(START_PIN_OFFSET + pcdev->id);
            }
            else
            {
                pr_info("GPIO pin %d set to be input and its value = %d \n", pcdev->id, gpio_get_value(START_PIN_OFFSET + pcdev->id));
            }
        }
        else
        {
            if (gpio_direction_output(START_PIN_OFFSET + pcdev->id,pcdev->buff[0]))
            {
                pr_info("Cannot set the GPIO pin no %d to be output\n",pcdev->id);
                gpio_free(START_PIN_OFFSET + pcdev->id);
            }
            else
            {
                pr_info("GPIO pin %d set to be output and its value = %d \n", pcdev->id, gpio_get_value(START_PIN_OFFSET + pcdev->id));
            }
        }

        return 0;

}


static int __init mydriver_init(void)
{
	pr_info("GPIO Driver initialization\n");
    int r_err = 0;    
	r_err = alloc_chrdev_region(&prv_driver.device_number,0,prv_driver.device_count,"mydev");

    if(r_err<0)
    {
        pr_info("failed to allocate\n");
        return 0;
    }

    prv_driver.myclass=class_create("mydevclass");
    platform_driver_register(&platform_driver_data);
    return 0;	

}



static void __exit mydriver_exit(void)
{
	pr_info("Exit GPIO\n");
    platform_driver_unregister(&platform_driver_data); //Optional
    class_destroy(prv_driver.myclass);
    unregister_chrdev_region(prv_driver.device_number,prv_driver.device_count);
}

module_init(mydriver_init);
module_exit(mydriver_exit);
