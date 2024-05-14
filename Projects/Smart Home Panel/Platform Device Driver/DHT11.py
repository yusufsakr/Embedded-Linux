from pigpio_dht import DHT11
import time

gpio = 4 # BCM Numbering

sensor = DHT11(gpio)

while True:
	try:
		result = sensor.read()
		if result["valid"] == True:
			value = ":{:02d},{:02d}".format(result["temp_c"], result["humidity"])
			# print("==")
			# print("==")
			# print(value)
			with open('/home/pi/device_driver/DHT11', 'w') as file:
				file.write(value)
			
			# print("==")
			
			with open('/home/pi/device_driver/DHT11', 'r') as file:
				print(file.read().strip())
				# print(file.read().strip())
			
			# print("==")
			# print("==")
	except:
		pass
	time.sleep(1)