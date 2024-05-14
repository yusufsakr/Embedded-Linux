#!/bin/bash
sudo insmod /home/pi/device_driver/002-2ndgpio/gpio_driver.ko
sudo insmod /home/pi/device_driver/002-2ndgpio/gpio_devices.ko
sudo chmod 777 /dev/LED0
sudo chmod 777 /dev/BTN0
sudo chmod 777 /dev/LED1
sudo chmod 777 /dev/BTN1
sudo chmod 777 /dev/LED2
sudo chmod 777 /dev/BTN2
sudo chmod 777 /dev/BUZZER
sudo chmod 777 /dev/SMOKESENSOR
sudo chmod 777 /dev/FIRESENSOR
# sudo chmod 777 /dev/DHT11

sudo pip install pigpio-dht --break-system-package
echo "insmod is running"
