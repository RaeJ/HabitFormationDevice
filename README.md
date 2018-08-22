# Multisensory Book

This is an IOT device that can be controlled using Alexa. Routines can be set up using the Alexa app so that at the designated
time the book will open up and use a variety of modalities to get the users' attention. The code used to make the sound work is 
from [SFX-I2S-web-trigger](https://github.com/bbx10/SFX-I2S-web-trigger) whilst the code used for being able to connect to
Alexa is from [arduino-esp8266-alexa-wemo-switch](https://github.com/kakopappa/arduino-esp8266-alexa-wemo-switch).

![alt text](https://github.com/RaeJ/HabitFormationDevice/blob/master/photos/open_book1.jpg "Multisensory Book")


## How to run the device
### Initial Setup
Based off the [Arduino IDE for ESP8266](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide).
1. Install the the [SiLabs CP2104 Driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers). This will allow you to upload code to the ESP8266 using a micro USB cable to the Feather HUZZAH with the other side in your computer's USB port.
2. Install the [Arduino IDE 1.6.8 or greater](https://www.arduino.cc/en/Main/Software) and open it up.
3. Install the ESP8266 Board Package. To do this navigate to *Arduino* *Preferences* and enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into the *Additional Board Manager URLs* field. Then navigate to the *Boards Manager* (found under *Tools*->*Board*) and install the ESP8266 Board Package. Close the Boards Manager window once the install process has completed. [This guide](https://learn.adafruit.com/add-boards-arduino-v164) is a good resource to use if you have trouble adding 3rd party boards.
4. Restart the Arduino Editor.
5. Select Adafruit Feather HUZZAH ESP8266  from the *Tools*->*Board* dropdown. Whilst still in *Tools* set the *CPU frequency* as 80MHz. The *Flash Size* can be kept at 4M (3M SPIFFS). For *Upload Speed*, select 115200 baud. You will also need to set the port as whichever port you are using.

## Internet Access
The Raspberry Pi is simply used for internet access. In order for the device to be able to work both the ESP8266 and the Alexa need internet access. If you're using a secure network (such as eduroam) it may be difficult to connect up the devices. I'm using a Raspberry Pi 3 B+ and a USB wifi dongle. I connect the wifi dongle up to eduroam by following [these instructions](https://www.willprice.org/2014/03/17/eduroam-on-the-raspberry-pi.html) by the wonderful Will Price, simply replacing wlan0 with wlan1. I then set the wifi chip on the Raspberry Pi board up as a hotspot by following [this guide](https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md). From here you can connect to the hotspot which should then give you access to eduroam and the internet.

When setting up eduroam on the Raspberry Pi I initially made the mistake of running the wpa_supplicant more than once. If you type `pgrep wpa_supplicant` into your terminal there should be only one process running. If there is more then one you can type `pkill wpa_supplicant` to kill them and start over.

Further to this, following Will Price's instructions did give me internet access, however, I initially had to repeat the process every time I restarted the Raspberry Pi. The easy fix for this was to move the terminal commands into the /etc/rc.local file.

![alt text](https://github.com/RaeJ/HabitFormationDevice/blob/master/photos/eduroam_access.jpg "Eduroam access kit")
