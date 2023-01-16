# Basic tests for paperd.ink rev4

## Install Library Dependencies (from arduino library manager)
  - Adafruit GFX library by Adafruit (tested with v1.7.5)
  - GxEPD2 by Jean-Marc Zingg version (tested with v1.3.9)

## Note:
- This seems to work only on ESP32 Arduino core v1.0.4 at the moment.

## Usage from Arduino
1) Update wifi details and time zone in config.h
2) Upload image.bmp onto a FAT32 formatted microSD card
3) Insert microSD card in paperd.ink
4) Upload sketch and follow along on screen instruction

## Usage of provided bin files
1) Set up a wifi AP with internet connectivity having both name and password as 'rgpitest'.
   This can be a router or a mobile hotspot
2) Upload image.bmp onto a FAT32 formatted microSD card
3) Insert microSD card in paperd.ink
4) Change directory to 'mfg' folder
5) Run appropriate script based on OS and device.
