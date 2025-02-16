# ESP8266 UNAPI Firmware

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/R6R2BRGX6)


Two sample TCP IP UNAPI drivers that work along with this firmware
are available at:

(ROM Version)
https://github.com/ducasp/MSX-Development/tree/master/MSX-SM/WiFi/UNAPI_BIOS_CUSTOM_ESP_FIRMWARE

(RAM Version, requires Memory Mapper and RAMHELPR by Konamiman)
https://github.com/ducasp/MSX-Development/tree/master/MSX-SM/WiFi/UNAPI_DRIVER_CUSTOM_ESP_FIRMWARE

This application is an example of how to set configurations, scan and join networks or update firmware or certificates:
https://github.com/ducasp/MSX-Development/tree/master/MSX-SM/WiFi/CFG8266

# How to build it

You will need Arduino IDE and install ESP8266 Arduino IDE version 2.7.1. After installing it, you
will need to apply some customizations I've made for it, those are available at:

https://github.com/ducasp/ArduinoIDE_ESP8266_Customizations

On Tools menu, choose the "Generic ESP8266 Moudle" board, set the built-in led to the correct GPIO
port of your ESP implementation. Make sure to set CPU frequency to 160 MHz, otherwise BearSSL will
not have its best performance and also certificates indexation that takes place after new
certificates are installed will take much longer.

Adjust flash size accordingly to your device, ESP-01 modules have 1MB, if you use a module with 2MB
or more of flash memory, you won't need to upload the certificates after uploading a new firmware.
Remember to have a FS of at least 256KB to hold the certificates.

To generate certificate files, use the script available at:

https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/examples/BearSSL_CertStore

To get the certificates in a single certs.ar file. That file goes to the /data folder of your 
Arduino IDE project, and you can generate the file system using the following plugin:

https://github.com/esp8266/arduino-esp8266fs-plugin

Just enable logging on your Arduino IDE, and after uploading the filesystem, you can get in the log
where the plugin created the bin file to upload to the ESP8266. Copy that file before closing the
IDE and that is the file used for OTA or LOCAL update of certificates. There are different ways to
do it, but that is the one that worked for me, can't recommend or support other methods. :stuck_out_tongue_closed_eyes:

# Project Design Constraints

This firmware was designed taking into consideration that the other end connected to it either has
a large 2KB reception buffer OR is fast enough to pick-up data at the desired baud rate without the
help of such large buffer.

Copyright (c) 2019 - 2025 Oduvaldo Pavan Junior ( ducasp@ gmail.com )
All rights reserved.

If you integrate this on your hardware, please consider the possibility of sending one piece of it
as a thank you to the author :) Of course this is not mandatory, if you like the idea, contact the
author in the e-mail address above.

This software uses ESP8266 for Arduino IDE library, licensed under
GNU Lesser General Public License 2.1 and available at:

https://github.com/esp8266/Arduino

It depends on some customizations I've made on that library, those
are available at:

https://github.com/ducasp/ArduinoIDE_ESP8266_Customizations

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as 
published by the Free Software Foundation, either version 2.1 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>
