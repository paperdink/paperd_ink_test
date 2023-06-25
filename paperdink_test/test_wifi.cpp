#include <WiFiClientSecure.h>

#include <Arduino.h>
#include <sys/time.h>
#include <esp_attr.h>

#include "config.h"
#include "date_time.h"

RTC_DATA_ATTR struct time_struct now; // keep track of time
String time_zone_base = "UTC";

// Time APIs
int8_t get_date_dtls(String time_zone)
{
	String time_zone_string = time_zone_base + time_zone;
	setenv("TZ", time_zone_string.c_str(), 1);

	struct tm timeinfo;
	if (!getLocalTime(&timeinfo)) {
		Serial.println("Failed to obtain time");
		return -1;
	}
	time_t epoch = mktime(&timeinfo);

	sscanf(ctime(&epoch), "%s %s %hhd %hhd:%hhd:%hhd %d", now.wday, now.month, &now.mday, &now.mil_hour, &now.min, &now.sec, &now.year);

	now.month_num = timeinfo.tm_mon + 1;
	// gives offset of first day of the month with respect to Monday
	//https://www.tondering.dk/claus/cal/chrweek.php#calcdow
	// 0=Sunday, 1=Monday ... 6=Saturday
	uint8_t a = (14 - now.month_num) / 12;
	uint16_t y = now.year - a;
	uint16_t m = now.month_num + (12 * a) - 2;
	now.day_offset = (now.mday + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7;
	now.day_offset = (now.day_offset + START_DAY_OFFSET) % 7;
	// convert to 12 hour
	if (now.mil_hour > 12)
		now.hour = now.mil_hour - 12;
	else
		now.hour = now.mil_hour;

	Serial.printf("Time is %d %d:%d:%d on %s on the %d/%d/%d . It is the month of %s. day_offset: %d\n", now.mil_hour, now.hour, now.min, now.sec, now.wday, now.mday, now.month_num, now.year, now.month, now.day_offset);
	return 0;
}

int8_t connect_wifi()
{
	uint8_t connAttempts = 0;

	WiFi.begin(SSID, PASSWORD);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		if (connAttempts > 40)
			return -1;
		connAttempts++;
	}
	Serial.println("Connected");
	return 0;
}

void test_wifi(GxEPD2_GFX& display)
{
	Serial.println("Testing Wifi");

	char date_str[20];
	char wifi_failed[] = "Wifi Failed";
	char wifi_success[] = "Wifi Success";
	int16_t tbx, tby; uint16_t tbw, tbh;
	uint16_t x, y;

	display.fillScreen(GxEPD_WHITE);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	display.setRotation(0);

	if (connect_wifi() == 0) {
		configTime(0, 0, "pool.ntp.org");
		if (get_date_dtls(TIME_ZONE) < 0)
			configTime(0, 0, "pool.ntp.org");
		sprintf(date_str, "Today is %d %s", now.mday, now.month);
		display.getTextBounds(date_str, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((display.width() - tbw) / 2) - tbx;
		y = ((display.height() - tbh) / 2) - tby;
		display.setCursor(x, y);
		display.printf("%s\n", date_str);

		display.getTextBounds(wifi_success, 0, 0, &tbx, &tby, &tbw, &tbh);
		x = ((display.width() - tbw) / 2) - tbx;
		y = ((display.height() - tbh) / 2) - tby;
		display.setCursor(x, y + tbh + 10);
		display.printf("%s\n", wifi_success);

		display.display(false);
		Serial.println("Wifi Success");
		delay(2000);
	} else {
		Serial.println("=====> Wifi Failed");

		display.getTextBounds(wifi_failed, 0, 0, &tbx, &tby, &tbw, &tbh);
		uint16_t x = ((display.width() - tbw) / 2) - tbx;
		uint16_t y = ((display.height() - tbh) / 2) - tby;
		display.setCursor(x, y);
		display.println(wifi_failed);
		display.display(false);
		while (1) {
		}
	}
}
