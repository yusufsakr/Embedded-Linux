#include "gpio_driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heba");
MODULE_DESCRIPTION("Your module description");

/* Create six platform device */

struct private_dvc_data prvDevData[] = {
    [0] = {.id = LED0, .perm = RDWR, .size = BUFFER_SIZE, .direction = OUT, .buff[0]=0 },
    [1] = {.id = BUZZER, .perm = RDWR, .size = BUFFER_SIZE, .direction = OUT, .buff[0]=0 },
    [2] = {.id = BTN0, .perm = RDWR, .size = BUFFER_SIZE, .direction = IN, .buff[0]=0 },
    [3] = {.id = SMOKESENSOR, .perm = RDWR, .size = BUFFER_SIZE, .direction = IN, .buff[0]=0 },
    [4] = {.id = FIRESENSOR, .perm = RDWR, .size = BUFFER_SIZE, .direction = IN, .buff[0]=0 },
    [5] = {.id = LED1, .perm = RDWR, .size = BUFFER_SIZE, .direction = OUT, .buff[0]=0 },
    [6] = {.id = LED2, .perm = RDWR, .size = BUFFER_SIZE, .direction = OUT, .buff[0]=0 },
    [7] = {.id = BTN1, .perm = RDWR, .size = BUFFER_SIZE, .direction = IN, .buff[0]=0 },
    [8] = {.id = BTN2, .perm = RDWR, .size = BUFFER_SIZE, .direction = IN, .buff[0]=0 }
    // [5] = {.id = DHT11, .perm = RDWR, .size = BUFFER_SIZE, .direction = IN, .buff[0]=0 }
};


struct platform_device mydev_1= 
{
    .name ="LED0", 
    .id = LED0,
    .dev = 
    {
        .platform_data = &prvDevData[0],
        .release = mydevice_release 
    }
};

struct platform_device mydev_2= 
{
    .name ="BUZZER", 
    .id = BUZZER,
    .dev = 
    {
        .platform_data = &prvDevData[1],
        .release = mydevice_release 
    }
};

struct platform_device mydev_3= {
    .name ="BTN0", 
    .id = BTN0,
    .dev = 
    {
        .platform_data = &prvDevData[2], 
        .release = mydevice_release 
    }
};

struct platform_device mydev_4= {
    .name ="SMOKESENSOR", 
    .id = SMOKESENSOR,
    .dev = 
    {
        .platform_data = &prvDevData[3], 
        .release = mydevice_release 
    }
};

struct platform_device mydev_5= {
    .name ="FIRESENSOR", 
    .id = FIRESENSOR,
    .dev = 
    {
        .platform_data = &prvDevData[4], 
        .release = mydevice_release 
    }
};

struct platform_device mydev_6= 
{
    .name ="LED1", 
    .id = LED1,
    .dev = 
    {
        .platform_data = &prvDevData[5],
        .release = mydevice_release 
    }
};

struct platform_device mydev_7= 
{
    .name ="LED2", 
    .id = LED2,
    .dev = 
    {
        .platform_data = &prvDevData[6],
        .release = mydevice_release 
    }
};

struct platform_device mydev_8= {
    .name ="BTN1", 
    .id = BTN1,
    .dev = 
    {
        .platform_data = &prvDevData[7], 
        .release = mydevice_release 
    }
};

struct platform_device mydev_9= {
    .name ="BTN2", 
    .id = BTN2,
    .dev = 
    {
        .platform_data = &prvDevData[8], 
        .release = mydevice_release 
    }
};
// struct platform_device mydev_6= {
//     .name ="DHT11", 
//     .id = DHT11,
//     .dev = 
//     {
//         .platform_data = &prvDevData[5], 
//         .release = mydevice_release 
//     }
// };


struct platform_device *platform_devices[] = {
    &mydev_1,
    &mydev_2,
    &mydev_3,
    &mydev_4,
    &mydev_5,
    &mydev_6,
    &mydev_7,
    &mydev_8,
    &mydev_9
};

/* Callback to free the device after all references have gone away */
void mydevice_release(struct device *dev) 
{

    pr_info("Device released successfully\n");
}

static int __init mydriver_init(void)
{

    platform_device_register(&mydev_1);
    platform_device_register(&mydev_2);
    platform_device_register(&mydev_3);
    platform_device_register(&mydev_4);
    platform_device_register(&mydev_5);
    platform_device_register(&mydev_6);
    platform_device_register(&mydev_7);
    platform_device_register(&mydev_8);
    platform_device_register(&mydev_9);
    pr_info("Platform device setup module loaded\n");
	return 0;	

}

static void __exit mydriver_exit(void)
{
    /* Unregister platform-level device */
    platform_device_unregister(&mydev_1);
    platform_device_unregister(&mydev_2);
    platform_device_unregister(&mydev_3);
    platform_device_unregister(&mydev_4);
    platform_device_unregister(&mydev_5);
    platform_device_unregister(&mydev_6);
    platform_device_unregister(&mydev_7);
    platform_device_unregister(&mydev_8);
    platform_device_unregister(&mydev_9);
    pr_info("Platform device setup module unloaded\n");
}

module_init(mydriver_init);
module_exit(mydriver_exit);
