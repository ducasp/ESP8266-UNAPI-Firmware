# ESP8266 UNAPI Firmware

Two sample TCP IP UNAPI drivers that work along with this firmware
are available at:

(ROM Version)
https://github.com/ducasp/MSX-Development/tree/master/MSX-SM/WiFi/UNAPI_BIOS_CUSTOM_ESP_FIRMWARE

(RAM Version, requires Memory Mapper and RAMHELPR by Konamiman)
https://github.com/ducasp/MSX-Development/tree/master/MSX-SM/WiFi/UNAPI_DRIVER_CUSTOM_ESP_FIRMWARE

This application is an example of how to set configurations, scan and join networks or update firmware or certificates:
https://github.com/ducasp/MSX-Development/tree/master/MSX-SM/WiFi/CFG8266

Project Design Constraints:

This firmware was designed taking into consideration that the other end connected to it either has
a large 2KB reception buffer OR is fast enough to pick-up data at the desired baud rate without the
help of such large buffer.

Copyright (c) 2019 - 2021 Oduvaldo Pavan Junior ( ducasp@ gmail.com )
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
