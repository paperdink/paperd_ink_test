#include "config.h"
#include "PCF8574.h"

// PCF8574 GPIO extender
PCF8574 pcf8574(PCF_I2C_ADDR, SDA, SCL);

void test_expander(GxEPD2_GFX& display)
{
	char pcf_failed[] = "PCF8574 Failed";
	char pcf_success[] = "PCF8574 Success";
	int16_t tbx, tby; uint16_t tbw, tbh;
	uint16_t x, y;
	char *result;

	display.fillScreen(GxEPD_WHITE);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	display.setRotation(0);

	pcf8574.pinMode(P0, OUTPUT);
	if (pcf8574.begin()) {
		Serial.println(pcf_success);
		pcf8574.digitalWrite(P0, HIGH);
		result = pcf_success;
	} else {
		Serial.println(pcf_failed);
		result = pcf_failed;
	}

	display.getTextBounds(result, 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((display.width() - tbw) / 2) - tbx;
	y = ((display.height() - tbh) / 2) - tby;
	display.setCursor(x, y);
	display.println(result);
	display.display(false);

	if (result == pcf_failed) {
		while (1) {
		}
	}

	delay(1000);
}
