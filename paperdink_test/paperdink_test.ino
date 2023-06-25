
#include "config.h"
#include "alberteinstein.h"
#include "Button.h"
#include "mario_tune.h"
#include "Roboto_Regular_12pt7b.h"

#ifdef PI_CLASSIC
	GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=*/ EPD_CS, /*DC=*/ EPD_DC, /*RST=*/ EPD_RST, /*BUSY=*/ EPD_BUSY));
	const char device_str[] = "Classic";
#elif  PI_MERLOT
	GxEPD2_3C<GxEPD2_420c_Z21, GxEPD2_420c_Z21::HEIGHT> display(GxEPD2_420c_Z21(/*CS=*/ EPD_CS, /*DC=*/ EPD_DC, /*RST=*/ EPD_RST, /*BUSY=*/ EPD_BUSY));
	const char device_str[] = "Merlot";
#else
#error "Select one of PI_CLASSIC or PI_device_str"
#endif

// Tests
void test_sd_card(GxEPD2_GFX& display);
void test_wifi(GxEPD2_GFX& display);
void test_expander(GxEPD2_GFX& display);
void test_buzzer(GxEPD2_GFX& display);
void test_partial_update(GxEPD2_GFX& display);
void test_text_print(GxEPD2_GFX& display);
void test_button(GxEPD2_GFX& display, Button *btn, int8_t btn_num, uint16_t y);
void test_battery(GxEPD2_GFX& display);
void print_success_msg(GxEPD2_GFX& display);

char PI_string[] = "paperd.ink";
char success_msg1[] = "Visit docs.paperd.ink";
char success_msg2[] = "to get started";

// Buttons
Button btn1(BUTTON_1_PIN);
Button btn2(BUTTON_2_PIN);
Button btn3(BUTTON_3_PIN);
Button btn4(BUTTON_4_PIN);

void setup()
{
	Serial.begin(115200);
	Serial.println();
	delay(100);

	Serial.println("******************************************");
	Serial.printf("Paperd.Ink revision 4 %s Test Program\r\n", device_str);
	Serial.println("******************************************");

    // Board Init
	pinMode(EPD_EN, OUTPUT);
	pinMode(EPD_RST, OUTPUT);
	pinMode(SD_EN, OUTPUT);
	pinMode(BATT_EN, OUTPUT);
	pinMode(PCF_INT, INPUT); // Required to lower power consumption
	pinMode(CHARGING_PIN, INPUT_PULLUP);

    // Buttons Init, This is done by the button library as well
	pinMode(BUTTON_1_PIN, INPUT_PULLUP);
	pinMode(BUTTON_2_PIN, INPUT_PULLUP);
	pinMode(BUTTON_3_PIN, INPUT_PULLUP);
	pinMode(BUTTON_4_PIN, INPUT_PULLUP);

    digitalWrite(BUTTON_1_PIN, HIGH);
    digitalWrite(BUTTON_2_PIN, HIGH);
    digitalWrite(BUTTON_3_PIN, HIGH);
    digitalWrite(BUTTON_4_PIN, HIGH);

    // Power up EPD
	digitalWrite(EPD_EN, LOW);
	digitalWrite(EPD_RST, LOW);
	delay(50);
	digitalWrite(EPD_RST, HIGH);
	delay(50);

    // EPD tests
	display.init(115200);
    // first update should be full refresh
	test_text_print(display);
	delay(2000);
	test_partial_update(display);
	delay(1000);

    // Buttons test
    test_button(display, &btn1, 1, 35);
	test_button(display, &btn2, 2, 120);
	test_button(display, &btn3, 3, 207);
	test_button(display, &btn4, 4, 290);

    display.setFullWindow();
    // Buzzer test
	test_buzzer(display);

    // GPIO Expander test
	test_expander(display);

    // Battery test
	test_battery(display);
    
    // SD Card test
	test_sd_card(display);

    // WiFi Test
	test_wifi(display);

    // End message
	print_success_msg(display);

    // Disable power to EPD
    //Serial.println("Turning off everything");
	digitalWrite(SD_EN, HIGH);
	digitalWrite(BATT_EN, HIGH);
	digitalWrite(EPD_EN, HIGH);
	digitalWrite(EPD_RST, LOW);
	delay(50);
	digitalWrite(EPD_RST, HIGH);
}

void loop()
{
}

void print_success_msg(GxEPD2_GFX& display)
{
	int16_t tbx, tby; uint16_t tbw, tbh;
	uint16_t x,y;

	display.setRotation(0);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	display.fillScreen(GxEPD_WHITE);

	display.getTextBounds(PI_string, 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((display.width() - tbw) / 2) - tbx;
	y = 50;
	display.setCursor(x, y);
	display.println(PI_string);
	display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
	display.setCursor(x+24, y+tbh+10);
	display.println(device_str);

	display.setTextColor(GxEPD_BLACK);
	display.getTextBounds(success_msg1, 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((display.width() - tbw) / 2) - tbx;
	y = ((display.height() - tbh) / 2) - tby;
	display.setCursor(x, y);
	display.println(success_msg1);

	display.getTextBounds(success_msg2, 0, 0, &tbx, &tby, &tbw, &tbh);
	x = ((display.width() - tbw) / 2) - tbx;
	y = ((display.height() - tbh) / 2) - tby;
	display.setCursor(x, y+tbh+10);
	display.println(success_msg2);

	display.display(false);
	delay(500);

	Serial.println("+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
	Serial.println("     TESTING COMPLETED      ");
	Serial.println("+=+=+=+=+=+=+=+=+=+=+=+=+=+=");
	Serial.println();
	Serial.println();
}

void buzz(int targetPin, long frequency, long length)
{
    //digitalWrite(LED_PIN, LOW);
	long delayValue;
	if(frequency == 0){
		delayValue = 1000000 / 1 / 2;
	}else{
		delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
        //// 1 second's worth of microseconds, divided by the frequency, then split in half since
        //// there are two phases to each cycle
	}

	long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
    //// multiply frequency, which is really cycles per second, by the number of seconds to
    //// get the total number of cycles to produce
	for(long i = 0; i < numCycles; i++){ // for the calculated length of time...
		digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
		delayMicroseconds(delayValue); // wait for the calculated delay value
		digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
		delayMicroseconds(delayValue); // wait again or the calculated delay value
	}
    //digitalWrite(LED_PIN, HIGH);
}

void sing()
{
	Serial.println("Singing Mario");
	int size = sizeof(melody)/sizeof(int);
	for(int thisNote = 0; thisNote < size; thisNote++){

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

		int noteDuration;
		if(tempo[thisNote] == 0){
			noteDuration = 1000 / 1;
		}else{
			noteDuration = 1000 / tempo[thisNote];
		}
		Serial.print(" Playing note: ");
		Serial.println(melody[thisNote]);
		buzz(melodyPin, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
		int pauseBetweenNotes = noteDuration * 1.30;
		delay(pauseBetweenNotes);

        // stop the tone playing:
		buzz(melodyPin, 0, noteDuration);
	}
}

void test_buzzer(GxEPD2_GFX& display)
{
	Serial.println("Testing buzzer");
	char buzr_str[] = "Testing buzzer. Can you hear sound?";

	pinMode(BUZR, OUTPUT);

	display.setRotation(0);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	int16_t tbx, tby; uint16_t tbw, tbh;
	display.getTextBounds(buzr_str, 0, 0, &tbx, &tby, &tbw, &tbh);
    // center bounding box by transposition of origin:
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = ((display.height() - tbh) / 2) - tby;
	display.fillScreen(GxEPD_WHITE);
	display.setCursor(x, y);
	display.print(buzr_str);
	display.display(false);
	sing();
}

void test_button(GxEPD2_GFX& display, Button *btn, int8_t btn_num, uint16_t y)
{
	Serial.printf("Testing button %d\r\n", btn_num);

    //display.setFullWindow();
	display.fillScreen(GxEPD_WHITE);
	display.setRotation(0);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);

	display.setCursor(200, y);
	display.print("Press button -->");
    display.display(true);
    //display.displayWindow(200, y-15, 200, 25);

	bool buttonState = 0;
	while(1){
		btn->loop();
		buttonState = btn->getState();
		if(buttonState == LOW){
			Serial.printf("Button %d pressed\r\n", btn_num);
			return;
		}
	}
}

void test_text_print(GxEPD2_GFX& display)
{
	Serial.println("Simple print test");

	display.setRotation(0);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	int16_t tbx, tby; uint16_t tbw, tbh;
	display.getTextBounds(PI_string, 0, 0, &tbx, &tby, &tbw, &tbh);
    // center bounding box by transposition of origin:
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = ((display.height() - tbh) / 2) - tby;
	display.setFullWindow();
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(x, y);
		display.println(PI_string);
		display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
		display.setCursor(x+24, y+tbh+10);
		display.println(device_str);
	}
	while(display.nextPage());
}

void test_partial_update(GxEPD2_GFX& display)
{
	Serial.println("Testing Partial Update");

    // some useful background
	char text[] = "Partial update demo";
	int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
	display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
                                                               // center bounding box by transposition of origin:
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = ((display.height() - tbh) / 2) - tby;

	display.fillScreen(GxEPD_WHITE);
	display.setCursor(x,y);
	display.print(text);
	display.display();

    // use asymmetric values for test
	uint16_t box_x = 10;
	uint16_t box_y = 15;
	uint16_t box_w = 70;
	uint16_t box_h = 20;
	uint16_t cursor_y = box_y + box_h - 6;
	float value = 13.95;
	uint16_t incr = display.epd2.hasFastPartialUpdate ? 1 : 10;
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);

	for(uint16_t r = 0; r < 2; r++)
	{
		display.setRotation(r);
		for(uint16_t i = 1; i <= 10; i += incr)
		{
			display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
			display.setCursor(box_x, cursor_y);
			display.print(value * i, 2);
			display.displayWindow(box_x, box_y, box_w, box_h);
			delay(500);
		}
		delay(1000);
		display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
		display.displayWindow(box_x, box_y, box_w, box_h);
		delay(1000);
	}
}

void test_battery(GxEPD2_GFX& display)
{
	Serial.println("Testing battery");

	// read charging pin
	uint8_t charging = digitalRead(CHARGING_PIN) ^ 1;
	Serial.printf("Charging: %d\r\n", charging);

	// read adc
	digitalWrite(BATT_EN, LOW);
	delay(10);
	analogReadResolution(12);
	int adc = analogReadMilliVolts(BATTERY_VOLTAGE);
	digitalWrite(BATT_EN, HIGH);

	// calculate battery voltage according to divider resistors
	// Vbat = (R1+R2)/R2 = (470000 + 1600000) / 1600000 = 1.29375
	double vbat = ((double(adc) * 1.29375) / 1000);
	Serial.printf("ADC(%d): %d\r\n", BATTERY_VOLTAGE, adc);
	Serial.printf("Battery VBat: %f\r\n", vbat);

	display.setRotation(0);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_BLACK);
	display.fillScreen(GxEPD_WHITE);
	display.setCursor(0, 0);
	display.printf(" \n");
	display.printf("      ADC: %d\n", adc);
	display.printf("     VBat: %.2f\n", vbat);
	display.printf(" Charging: %d\n", charging);
	display.display(false);
	delay(5000);
}
