# ESP32-Warflying
This project uses Ai Thinker ESP32 Cam module to wardrive.

I needed to build a wardriving kit that can be put on a small FPV drone. Initially I started with Raspberry Pi with Kismet, but that was too big and heavy. While ESP32 doesn't support true monitor mode, it meets my specific needs. 

I chose the AI Thinker ESP32 Cam module because it was very cheap, had built-in SD card reader, and offered an attachment for an external 2.4G antenna. 

How it works:

There are three key components to this project: 
1. ESP32 with its WiFi Station capbility. (Covered by its organic WiFi library)
2. ESP32 SD card and File System libraries, which allow you to write findings on a removable SD Card.
3. MSP library that lets you pull the GPS coordinates from the flight controller, instead of having a dedicated GPS module.

https://github.com/fdivitto/MSP
That's the MSP library I used. It is super simple and reliable. Supports all of the important messages.
