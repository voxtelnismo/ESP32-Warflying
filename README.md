# ESP32-Warflying
This is an airborne version of Alex Lynd's project (can be found here: https://github.com/AlexLynd/ESP8266-Wardriving)

![IMG_1288](https://github.com/voxtelnismo/ESP32-Warflying/assets/47276106/b90b6302-37df-4594-9575-b9c80c77862c)

I needed to build a wardriving kit that can be put on a small FPV drone. Initially I started with Raspberry Pi with Kismet, but that was too big and heavy. While ESP32 doesn't support true monitor mode, it meets my specific needs. 

I chose the AI Thinker ESP32 Cam module because it was very cheap, had built-in SD card reader, and offered an attachment for an external 2.4G antenna. 

How it works:


There are three key components to this project: 
1. ESP32 with its WiFi Station capbility. (Covered by its organic WiFi library)
2. ESP32 SD card and File System libraries, which allow you to write findings on a removable SD Card.
3. MSP library that lets you pull the GPS coordinates from the flight controller, instead of having a dedicated GPS module.

https://github.com/fdivitto/MSP
That's the MSP library I used. It is super simple and reliable. Supports all of the important messages.

![IMG_1289](https://github.com/voxtelnismo/ESP32-Warflying/assets/47276106/3f6dec18-781e-4c67-91c4-976def80e6ec)

Build instructions:

1. Desolder the pin headers.
2. Use FTDI adapter to upload code. Here is a great article on the specific quirks of ESP Cam: https://dronebotworkshop.com/esp32-cam-intro/
3. Attach UART0 of the ESP32 to any hardware UART of the flight controller (GPIO1 -> FC RX; GPIO3 -> FC TX).
4. Wire 5V and ground from ESP32 to 5V and ground on the FC. Do not use 4V5 pads, because ESP could pull more current than your USB port can provide.
5. Switch that port in INAV Configurator to MSP, baud rate 115200.
6. Move the 0 Ohm resistor from PCB antenna to u.FL port, and attach a decent 2.4G antenna. (I'm using an ELRS 2.4G T-type dipole antenna)

I'm using Caddx Vista with DJI controller (both on 5.8G), but you can also use 915MHz, like ELRS or Crossfire. I do not recommend using 2.4G ELRS, because when it's sending telemetry, it could easily fry the internal amp on ESP32.

The program polls the WiFi for all available SSIDs, records them in a table on SD card, then waits 5 seconds. It does not check for duplicates. Later I'm gonna write a Python script that removes duplicates and formats the GPS coordinates for Google Earth.
