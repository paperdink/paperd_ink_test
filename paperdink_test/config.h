
#ifndef CONFIG_H
#define CONFIG_H

#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// DETAILS TO EDIT
// Uncomment only one of the below #define statements
// based on the paperd.ink device you have
#define PI_CLASSIC (true)
//#define PI_MERLOT (true)

#define SSID     "rgpitest" // your network SSID (name of wifi network)
#define PASSWORD "rgpitest" // your network password
#define TIME_ZONE "+07:00" // Your time zone. Daylight saving not handled yet.
#define START_DAY_OFFSET 0

// PIN ASSIGNMENT

// I2C pins
#define SDA 16
#define SCL 17

// SPI pins

// SD card pins
#define SS 21
#define SD_CS SS
#define SD_EN 5

// E-paper pins
#define EPD_CS 22
#define EPD_DC 15
#define EPD_BUSY 34
#define EPD_EN 12
#define EPD_RST 13

// PCF8574 pins
#define PCF_INT 35
#define SD_CD P4 // input
#define EXT_GPIO1 P5
#define EXT_GPIO2 P6
#define EXT_GPIO3 P7
#define PCF_I2C_ADDR 0x38

// LiPo
#define CHARGING_PIN 36
#define BATT_EN 25
#define BATTERY_VOLTAGE 39

// Buzzer
#define BUZR 26

// Buttons
// Top to bottom
#define BUTTON_1_PIN 14
#define BUTTON_1_RTC_GPIO 16

#define BUTTON_2_PIN 27
#define BUTTON_2_RTC_GPIO 17

#define BUTTON_3_PIN 4
#define BUTTON_3_RTC_GPIO 10

#define BUTTON_4_PIN 2
#define BUTTON_4_RTC_GPIO 12

#define DEBOUNCE_TIME 10

#endif /* CONFIG_H */
