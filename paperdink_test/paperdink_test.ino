
#include "config.h"
#include "alberteinstein.h"
#include "Button.h"
#include "mario_tune.h"
#include "Roboto_Regular_12pt7b.h"

// Tests
void test_sd_card();
void test_wifi();
void test_expander();

char PI_string[] = "Paperd.Ink";
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

  Serial.println("**********************************");
  Serial.println("Paperd.Ink revision 4 Test Program");
  Serial.println("**********************************");

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

  // Power up EPD
  digitalWrite(EPD_EN, LOW);
  digitalWrite(EPD_RST, LOW);
  delay(50);
  digitalWrite(EPD_RST, HIGH);
  delay(50);

  // EPD tests
  display.init();
  // first update should be full refresh
  test_text_print();
  delay(2000);
  test_partial_update();
  delay(1000);

  // Init again to use bufferd mode
  display.init();
  // first update should be full refresh
  display.fillScreen(GxEPD_WHITE);
  display.display(false);
  
  // Buttons test
  test_button(&btn1, 1, 35);
  test_button(&btn2, 2, 120);
  test_button(&btn3, 3, 207);
  test_button(&btn4, 4, 290);

  // Buzzer test
  test_buzzer();

  // SD Card test
  test_sd_card();

  // GPIO Expander test
  test_expander();

  // WiFi Test
  test_wifi();
  
  // End message
  print_success_msg();
  
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

void print_success_msg()
{
  int16_t tbx, tby; uint16_t tbw, tbh;
  uint16_t x,y;
  
  display.setRotation(0);
  display.setFont(&Roboto_Regular12pt7b);
  display.setTextColor(GxEPD_BLACK);
  
  display.getTextBounds(success_msg1, 0, 0, &tbx, &tby, &tbw, &tbh);
  x = ((display.width() - tbw) / 2) - tbx;
  y = ((display.height() - tbh) / 2) - tby;
  display.fillScreen(GxEPD_WHITE);
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

void buzz(int targetPin, long frequency, long length) {
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
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  //digitalWrite(LED_PIN, HIGH);
}

void sing() {
    Serial.println("Singing Mario");
    int size = sizeof(melody)/sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
            
      int noteDuration;
      if(tempo[thisNote] == 0){
        noteDuration = 1000 / 1;
      }else{
        noteDuration = 1000 / tempo[thisNote];
      }
      Serial.print(" PLaying note: ");
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

void test_buzzer()
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

void test_button(Button *btn, int8_t btn_num, uint16_t y)
{
  Serial.printf("Testing button %d\n", btn_num);

  display.fillScreen(GxEPD_WHITE);
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  
  display.setCursor(200, y);
  display.println("Press button -->");
  display.display(true);

  bool buttonState = 0;  
  while(1){
    btn->loop();
    buttonState = btn->getState();
    if(buttonState == LOW){
      Serial.printf("Button %d pressed\n", btn_num);
      return;
    }
  }
}

void test_text_print()
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
    display.print(PI_string);
  }
  while (display.nextPage());
}

void test_partial_update()
{
  Serial.println("Testing Partial Update");
  
  // use asymmetric values for test
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  float value = 27.3;
  uint16_t incr = display.epd2.hasFastPartialUpdate ? 1 : 3;
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  // show updates in the update box
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.setPartialWindow(box_x, box_y, box_w, box_h);
    for (uint16_t i = 1; i <= 3; i += incr)
    {
      display.firstPage();
      do
      {
        display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
        display.setCursor(box_x, cursor_y);
        display.print(value * i, 2);
      }
      while (display.nextPage());
      delay(500);
    }
    
    delay(500);
    
    display.firstPage();
    do
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    }
    while (display.nextPage());
    delay(500);
  }
}

void test_image()
{
  Serial.println("Testing Image");
  
  display.setFullWindow();
  display.setRotation(0);
  
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, gImage_alberteinstein, 400, 300, GxEPD_BLACK);
  }
  while (display.nextPage());
}
