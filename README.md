# Basic tests for paperd.ink rev4
Each revision has a branch

## Install Library Dependencies (from arduino library manager)
  - Adafruit GFX library by Adafruit (tested with v1.7.5)
  - GxEPD2 by Jean-Marc Zingg version (tested with v1.3.9)

## Usage from Arduino
1) Update wifi details and time zone in config.h
2) Upload image.bmp onto a FAT32 formatted microSD card
3) Insert microSD card in paperd.ink
4) Upload sketch and follow along on screen instruction

## Usage of provided bin file
1) Set up a wifi AP with internet connectivity having both name and password as 'rgpitest'.
   This can be a router or a mobile hotspot
2) Upload image.bmp onto a FAT32 formatted microSD card
3) Insert microSD card in paperd.ink
4) Change directory to where `paperdink_test.ino.bin` and other bin files are
5) For mac and linux: Upload using following command. Replace ``<port>`` with tty port on mac/linux.
``esptool.py --chip esp32 --port <port> --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 boot_app0.bin 0x1000 bootloader_qio_80m.bin 0x10000 paperdink_test.ino.bin 0x8000 paperdink_test.ino.partitions.bin``.
  
  For Windows: Open powershell window in same directory, and type ``.\upload``. The script will autodetect the COM port and upload the binary.
