# Basic tests for paperd.ink rev3

## Install Library Dependencies (from arduino library manager)
  - Adafruit GFX library by Adafruit (tested with v1.7.5)
  - GxEPD2 by Jean-Marc Zingg version (tested with v1.3.9)

## Usage from Arduino
1) Update wifi details and time zone in config.h
2) Upload image.bmp onto a FAT32 formatted microSD card
3) Insert microSD card in paperd.ink
4) Upload sketch and follow along on screen instruction

## Usage of provided bin file
1) Set up a wifi AP with internet connectivity having both name and password as 'pitest'.
   This can be a router or a mobile hotspot
2) Download esptool from here ``https://github.com/espressif/esptool/releases``
3) Upload image.bmp onto a FAT32 formatted microSD card
4) Insert microSD card in paperd.ink
5) Upload using following command. Replace ``<port>`` with COM port on windows or tty port on max/linux.
``python3 esptool.py --chip esp32 --port <port> --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0x00 ./paperd.ink.bin``
