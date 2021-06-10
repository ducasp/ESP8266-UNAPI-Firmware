# ESP8266 UNAPI Firmware Documentation

Copyright (c) 2019 - 2021 Oduvaldo Pavan Junior ( ducasp@ gmail.com ) All rights reserved.

## Table of Contents

1. [Introduction / Design Choices - Goals](#goals)
   1. [Introduction](#intro)
   2. [Design Choices - Goals](#dcgoals)    
2. [Protocol](#protocol)
3. [Configurations / Behavior Modifiers](#config)
4. [Commands](#commands)
   1. [Custom Commands](#ccommands)
      1. [Reset](#creset)
      2. [Retry Transmission](#cretry)
      3. [Scan Available Access Points](#cscans)
      4. [Scan Available Access Points Results](#cscanr)
   2. [UNAPI Commands](#ucommands)    

## <a name="goals"></a> Introduction / Design Choices - Goals

### <a name="intro"></a> Introduction

ESP8266 and modules using it, like ESP-01, are a quite affordable way to connect to Wi-Fi. Usually they have quite a few GPIO pins that can be used in many different interfaces, but the most common ESP-01 module do not expose a lot of those pins. That ESP-01 module is what provides network capabilities to the SM-X MSX system designed by Victor Trucco ([Get in touch with the new SM-X | MSX Resource Center](https://www.msx.org/news/hardware/en/get-in-touch-with-the-new-smx)). Victor designed the hardware of SM-X and I was one of a few fortunate fellows that were able to get an early prototype version of it. As he has plenty of projects on his table, and still was pretty much busy debugging possible hardware / interface issues with SM-X, he did not have time to create an interface to use the ESP8266 / ESP-01 module. For some reason, after helping him to understand an issue with the prototype firmware where I/O only interfaces were not working fine, I just had the idea of getting the Network interface working. 

As a concept, Victor told me about Luis Fernando Luca Zanoto Wi-Fi cartridge that also used an ESP-01 ([wifi card | MSX Resource Center ](https://www.msx.org/forum/msx-talk/hardware/wifi-card?page=1)) module, and I've first designed the VHDL module that would bridge the ESP8266 serial TX/RX lines to the MSX, it would be kind of similar to the way that Wi-Fi cartridge worked, but I've made a few changes to it as I've progressed through the development. If you are curious, that VHDL interface code can be found at [src/SM-X · master · Victor Trucco / SM-X · GitLab](https://gitlab.com/victor.trucco/sm-x/-/tree/master/src/SM-X), wifi.vhd describes the "cartridge" that will connect to the MSX bus and react to I/O commands, it also uses a UART and a FIFO, customized for this usage ( UART.vhd and fifo.vhd ), those were provided to me by Victor and I've changed the original fifo.vhd to work like a real fifo (it's original behavior was a little strange, it was not really a circular buffer).

The original idea was to, basically, use ESP8266 AT Firmware and perhaps software made for that Wi-Fi Cartridge... Can't say how bad that idea was... The AT firmware has a protocol that is not tokenized / machine efficient, so, parsing it's responses and assembling commands is a nightmare for a z80, let's say it is not really efficient for those tasks, more specifically given that the AT Firmware that is officially available for ESP-01 due to its small size of flash memory doesn't allow you to say it to stop receiving data until you clear your buffer, it is all unsolicited, non-stop, which in turn required an "artificial" way of slowing down it through the serial port speed. This is not optimal, at all, as even though an MSX using a z80 at 3.58MHz can sometimes get data really fast, some other time it would take some time on an proccess like writing to disk, which would cause data to be overrun due to the unsolicited nature of the AT firmware... Making the matter worse, there was not much software available for the Zanoto Wi-Fi cartridge, so, after spending quite some time on a telnet client using the AT firmware directly and on an UNAPI driver that was talking with the ESP using AT commands and getting less than optimal results (had to limit speed to 33600bps to avoid losing characters, resulting in up to 3.3KB/s transfer, the current design will allow you to achieve 15KB/s at 3.58MHz and much faster speeds at turbo speeds).

So, that was a turning point... One of many... Just to enumerate a few:

1. First I've said that having UNAPI on that thing would not be done by me... I knew from the very start that Konamiman (Nestor Soriano) [MSX-UNAPI-specification/TCP-IP UNAPI specification](https://github.com/Konamiman/MSX-UNAPI-specification/blob/master/docs/TCP-IP%20UNAPI%20specification.md) was the best way to do it... There is plenty of software available for UNAPI compatible adapters... But that would require z80 assembly knowledge (which I didn't have much by that time, most stuff I did was using SDCC / C Language) and to be honest, the first time I've seen the source code of the UNAPI driver for Denyonet it was totally out of the world for me, working through Hooks, slots, etc... Didn't thought I would be able to get something working, even using the Denyonet code as reference...

2. Then I've just tried and after a few months I had a working UNAPI driver (not perfect, but working to a good extent) using ESPRESSIF AT Firmware. At that point I've said that going beyond 3KBps wouldn't be doable, AT firmware and z80 having to parse the unsolicited responses wouldn't allow any better (and probably that is true, unless you use the ESP directly instead of using an API like UNAPI, then you probably can hit something like 5 to 6KBps)... Again, ESPRESSIF documentation at that time on doing ESP8266 custom firmwares was not the best, but I've figured out that the excellent [esp8266/Arduino: ESP8266 core for Arduino](https://github.com/esp8266/Arduino) was perfect for what I've wanted... 

3. And then I've just done a quite stable solution with a custom ESP8266 firmware, and Nestor Soriano (Konamiman) extended the UNAPI specification to allow for TLS/SSL (https uses it). You can guess that it is impossible to do TLS/SSL on a puny 8 bit processor running at 3.58MHz, but ESP8266 has a powerful cpu core that can handle that with ease (just limited to a single connection due to RAM constraints). At first, working a little with BearSSL I thought that it would be troublesome to have it working, but could get it working fine and even added https capability to Konamiman HGET utility, which he merged back into his original version!

4. And then it came to have a ROM version of the driver... Making a RAM version was a lot easier, it is sitting on a 16KB segment of RAM memory that is owned by the driver alone, so I can use RAM to make a lot of stuff... When your driver is running in a ROM, it will boot automatically and you can do cool stuff like setting the MSX clock automatically on boot and not needing to load anything using MSX-DOS. But you are left with 16 bytes of RAM memory that is allocated for your slot (as long as it is not expanded or if it is, the other slots do not have devices using their 4 bytes of RAM) which surely was not enough... But hey, that is not firmware related... 

So, this is basically the story behind this development. And for sure I would like to express my gratitude towards really nice people that helped make this true:

- First, Victor Trucco, who helped me whenever I had questions about VHDL, as well gave me a great insight on how to proceed quite a few times, and also for finally trusting in what I've done to use as the official solution for all SM-X devices he produced!

- Now, Nestor Soriano (Konamiman), this guy is really, really busy and did a lot of relevant stuff for the MSX scene... Not only the UNAPI specification and quite a few of the implementations for it, but also NEXTOR that is the evolution of MSX-DOS among many other relevant stuff. You could imagine that someone that has a vast knowledge would not like to help answering basic questions (as I've never done anything either Network related or that used MSX low level stuff), but he was always really kind and helped me whenever I had a question about UNAPI or Network stuff.

- Last, but not least (so, list is in order of when each one jumped in to assist) KdL, that is best know for the great OCM / Zemmix Neo firmware that expanded those machines to an extent the original project never dreamt of... There is no good adjective to his attention to detail and testing capabilities... His input (some times reporting problems, suggesting features, other times helping with code as well) is really valuable and without him, it wouldn't be nearly as good as it is today (i.e.: automatic clock setting was something he suggested me quite a few times, he has helped with some parts of the UNAPI rom Driver as well)

### <a name="dcgoals"></a> Design Choices - Goals

Well, this custom ESP8266 firmware was designed upon a few baselines:

- Implement most features that are part of TCP-IP Unapi specification. SEND ECHO and RCV ECHO was left out as ESP8266 API's do not implement ping in a way that would work with those commands and I personally don't think it is worth the trouble of implementing the code to have it... The only software that uses those calls, as far as I know, is ping, and that is something of limited use. All RAW connection functionality also was left out, as Espressif and Arduino IDE API's are geared towards UDP and TCP protocols, not for raw use of the network. Currently, the only TCP-IP UNAPI application that doesn't work with this implementation, to my knowledge, is PING.
- Move all TCP-IP UNAPI functionality, including parameters parsing and checking, to the firmware. So, basically the firmware is mostly a TCP-IP UNAPI implementation over a different interface, causing the driver to use extremely low CPU resources just to move data to the ESP and receive the response that is alligned so it can be copied directly to the registers as response.
- No unsolicited events/messages: firmware is slave to command requests and only send data in response to those. This makes a lot, lot easier to work with it on the driver.
- As simple as possible protocol: again, z80 at 3.58MHz can limit quite a few possibilities... I.e.: CRC calculation, even using tables, is not very performatic. Checksum is not really reliable even though it is a lot faster... But in the end, when you are trying to squeeze out as much performance as possible, it still adds some overhead. Since the serial communication between ESP-01 and the FPGA is a short trace route, even at 859372bps communications are really, really reliable, occasionally a byte or two might get lost, but the driver is able to detect this discrepancy and request data to be re-sent.
- A little bit over 2KB of serial input buffer. This is probably overkill as I doubt a MSX can cause a buffer overrun on an ESP, but hey, better be safe than sorry.
- Up to 4 simultaneous connections, TCP or UDP.
- Allow 1 active TCP connection only when using SSL, as it nearly sucks all RAM available for a program running on an ESP8266. Two connections could be possible depending upon the other end configuring less usage of buffers, but this is not reliable.
- It should be possible to update the custom firmware or SSL certificates either through the serial connection (computer reads file contents and send it to the device to update it) or OTA (over the air, connects to a server hosting the update file and update directly from the server).
- As some SM-X devices are prone to get audio noise as a result of interference from the ESP-01 lines (most likely the power line, as interference occurs even when there is no data being sent to/from the ESP), have means to disable radio fully (so power spikes don't occur and thus noise is not generated) or to have it disabled automatically after a pre-set period of time occurs without any connection being active.
- Certificates are installed on a SPIFFS partition to work with Bear SSL. Currently a 256KB partition is enough for this.
- Due to the limited size of flash memory available on ESP-01 modules, customized update routines allow the firmware size to be greater than half the size of (flash memory size - spiff partition). If not for those routines, it wouldn't be possible to implement SSL, as the firmware goes over 400KB in size when BearSSL is included. As a result, you can update the SPIFFS / update certificates at any time, but, if you update the firmware itself, you will need to update certificates as well, as the SPIFFS is corrupted during the firmware update. This minor inconvenience still allows for safe firmware updates that will not kill the device if failing, being possible to resume the update again using the computer or the OTA server.
- Firmware exposed commands are always SYNCHRONOUS, that means, for every command you send you need to wait for the command response before sending a new command. This is not an issue as all TCP-IP UNAPI commands are synchronous, and the ones that are not have a second command to check status and get results, this simplifies the design of both the driver on the computer as well as of the firmware.
- Extra commands that are not part of the TCP-IP UNAPI specification should be implemented to allow for firmware updates, easy detection of device being plugged in, easy time and date retrieval from network, configuration of the device behavior, scanning and connecting to Access Points.

## <a name="protocol"></a> Protocol

The idea is mostly to not have a protocol... Basically, the first byte is an indication of which command to be executed, where TCP-IP UNAPI commands have the values ranging from 1 to 29 matching the values defined by TCP-IP UNAPI specification for each command. Custom commands can use any character starting from '?' that has a value of 63. This is the only thing that is standardized for the protocol. Each command then will treat the remaining bytes as needed, to avoid overhead of unnecessary data being sent for the command execution or the overhead of unnecessary data being replied as result of the command execution. This leads to having a larger driver code, but that will execute a lot faster for simples commands, and having the hability to better suit the command format and response format so it can align better with what the driver needs to put in each z80 register as result.  Commands sent to ESP will mostly use one of those formats:

- Quick Commands: Those commands do not need any parameters, so they are composed of just one byte the command byte. Examples are RESET, RETRY_TX, GET VER.

- Regular Commands: Those commands need to send parameters. So they are composed of a three bytes header and the parameter data:

| CMD_BYTE | DATA_SIZE_MSB | DATA_SIZE_LSB | DATA BYTES |
| -------- | ------------- | ------------- | ---------- |

A command reception is expected that no more than 250ms occur between bytes. After 250ms occur while receiving a command since the last byte was received, firmware will discard the data received so far and get back to idle state waiting a new command.

Command responses format vary wildly, an as such, it is better to refer to each command documentation. Most commands will have a response like this if response data is expected for that command:

| CMD_BYTE | ERROR_CODE | RESP_SIZE_MSB | RESP_SIZE_LSB | RESPONSE BYTES |
| -------- | ---------- | ------------- | ------------- | -------------- |

If no response data is expected for that command:

| CMD_BYTE | ERROR_CODE |
| -------- | ---------- |

## <a name="config"></a> Configurations / Behavior Modifiers

This firmware uses ESP8266 Arduino IDE EEPROM functionality, that uses part of the flash memory that is not affected during firmware or file system updates to store a few bytes of data. 32 bytes of flash memory are reserved for this firmware, that are used as follow:

| EEPROM Address(es) | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                | Default Value                    |
| ------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------- |
| 0 to 7             | Configuration File Name: this is mostly to indicate whether the EEPROM has already been initialized or not, if not containing the expected string (that is not zero terminated), firmware will consider it will need to initialize EEPROM and device configuration to default values.                                                                                                                                                                                                      | "ESPUNAPI"                       |
| 8                  | Structure Version: this identifies which structure of data has been saved to the EEPROM, if an older version if found, a newer one with the extra configurations is generated, using the default values.                                                                                                                                                                                                                                                                                   | 2                                |
| 9                  | Nagle Algorithm: tells whether Nagle Algorithm is off (0) or on (1).                                                                                                                                                                                                                                                                                                                                                                                                                       | 0                                |
| 10                 | Always On: tells whether Wi-Fi radio is always on (1) or if it will be automatically disabled after a given period (0).                                                                                                                                                                                                                                                                                                                                                                    | 0                                |
| 11-14              | Radio Off Timer: 32 bits unsigned integer containing the time in seconds that need to elapse with the device idle and without open connections before turning off the Wi-Fi radio automatically. Only taken into consideration if Always On is set to 0.                                                                                                                                                                                                                                   | 120                              |
| 15                 | Auto Clock: this is a byte that the ROM version of the driver uses to determine a few of its behavior. Current possible values are: (0) Normal Operation but do not set clock automatically, (1) Will wait up to 10s for SNTP server response during computer boot and if a response is received will to update the system time and date with SNTP information, (2) Same as 1 but will turn off the Wi-Fi radio as soon as clock is set and (3) Will disable Wi-Fi radio and UNAPI Driver. | 0                                |
| 16-19              | GMT Setting: 32 bits integer that allows to add a GMT offset from -12 to +12 hours to the time and date response from the SNTP server. Only full hour adjustment is supported.                                                                                                                                                                                                                                                                                                             | -3 (Brazil - Brasilia time zone) |
| 20-31              | Unused                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | ?                                |

**Note:** IP Configuration (whether it is set to use DHCP or manual IP assignment) and DNS configuration (whether it is automatic from DHCP or force a different DNS server to be used) are saved within ESPRESSIF nonos SDK automatically, not needing to be saved in the configuration structure.

## <a name="commands"></a> Commands

### <a name="ccommands"></a> Custom Commands

#### <a name="creset"></a> Reset

This command will cause a full reset to the device, as the device was being powered off.

*Input Parameters:* none

*Command Structure:*

| Position | Function | Value |
|:--------:|:-------- |:----- |
| 0        | CMD_BYTE | 'R'   |

*Response Structure:*

| Position | Function    | Value |
|:--------:| ----------- | ----- |
| 0        | CMD_BYTE    | 'R'   |
| 1        | Fixed Value | '0'   |

#### <a name="cretry"></a> Retry Transmission

This command will cause the latest command response to be re-sent.

*Input Parameters:* none

*Command Structure:*

| Position | Function | Value |
| -------- | -------- | ----- |
| 0        | CMD_BYTE | 'r'   |

*Response Structure:* depends on the latest command executed

#### <a name="cscans"></a> Scan Available Access Points

This command will cause the device to start a scan for available Access Points in the range of the device radio. This is a function that takes quite some time to finish, so you need to query for Scan Results after starting the Scan with this command.

*Input Parameters:* none

*Command Structure:*

| Position | Function | Value |
|:--------:| -------- | ----- |
| 0        | CMD_BYTE | 'S'   |

*Response Structure:*

| Position | Function   | Value         |
|:--------:| ---------- | ------------- |
| 0        | CMD_BYTE   | 'S'           |
| 1        | Error Code | 0 - always Ok |

#### <a name="cscanr"></a> Scan Available Access Points Results

This command will return the results of the latest scan for available Access Points, or, the progress of the current scan if it did not finish.

*Input Parameters:* none

*Command Structure:*

| Position | Function | Value |
| -------- | -------- | ----- |
| 0        | CMD_BYTE | 's'   |

*Response Structure when not finished or no Access Point has been found:*

| Position | Function   | Value                                                                                       |
| -------- | ---------- | ------------------------------------------------------------------------------------------- |
| 0        | CMD_BYTE   | 's'                                                                                         |
| 1        | Error Code | 2 - Scan finished and no Network in range<br/>3 - Scan still in progress, check again later |

*Response Structure when finished:*

| Position | Function                      | Value                 |
|:--------:| ----------------------------- | --------------------- |
| 0        | CMD_BYTE                      | 's'                   |
| 1        | Error Code                    | 0 - Ok                |
| 2        | Number of Access Points Found | 1 to 255              |
| 3 - XXX  | Access Point Information List | See description below |

*Access Point Information List*:

The list has a variable size and it is always comprised of:

| Position | Function   | Value                                                                                             |
| -------- | ---------- | ------------------------------------------------------------------------------------------------- |
| 0 - X    | AP Name    | A variable number of characters describing the Access Point Name, NULL character (0) is not valid |
| X+1      | Separator  | 0 - Null character, indicating the end of the AP Name                                             |
| X+2      | Encryption | 'O' - Means open, no password needed<br/>'E' - Means encrypted, password needed                   |
| X+3 - Y  | AP Name    | ....                                                                                              |

### <a name="ucommands"></a> UNAPI Commands
