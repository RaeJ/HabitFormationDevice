# Multisensory Book

This is an iot device that can be controlled using Alexa. Routines can be set up using the Alexa app so that at the designated
time the book will open up and use a variety of modalities to get the users' attention. The code used to make the sound work is 
from [SFX-I2S-web-trigger](https://github.com/bbx10/SFX-I2S-web-trigger) whilst the code used for being able to connect to
Alexa is from [arduino-esp8266-alexa-wemo-switch](https://github.com/kakopappa/arduino-esp8266-alexa-wemo-switch).


## Internet Access
The Rapberry Pi is simply used for internet access. In order for the device to be able to work both the ESP8266 and the Alexa need internet access. If you're using a secure network (such as eduroam) it may be difficult to connect up the devices. I'm using a Raspberry Pi 3 B+ and a USB wifi dongle. I connect the wifi dongle up to eduroam by following [these](https://www.willprice.org/2014/03/17/eduroam-on-the-raspberry-pi.html) by the wonderful Will Price, simply replacing wlan0 with wlan1. I then set the wifi chip on the Raspberry Pi board up as a hotspot by following [this guide](https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md).

![alt text](https://github.com/RaeJ/HabitFormationDevice/blob/master/photos/eduroam_access.jpg "Eduroam access kit")
