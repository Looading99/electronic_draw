#include "Wire.h"
#include "EEPROM.h"
#include "LedControl.h" // http://wayoda.github.io/LedControl/
#include "ssd1306.h" // https://github.com/lexus2k/ssd1306
#include "Enerlib.h" // https://playground.arduino.cc/Code/Enerlib/

#include "bitmaps.h"

#define MAX7219_DIN 4
#define MAX7219_CS 7
#define MAX7219_CLK 8
#define TOUCH_IO 2

#define LEN_CALLED 31
#define LEN_BLACKLIST sizeof(blacklist)

uint8_t blacklist[] = {46}, use = 0, called_last_modified = LEN_CALLED-1;
uint32_t last_use = 0;

Energy energy;
LedControl max7219 = LedControl(MAX7219_DIN, MAX7219_CLK, MAX7219_CS);

void touch_interrupt(void);
void show_id(uint8_t id);
uint8_t get_random_id(void);

void setup() {
  // Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  attachInterrupt(digitalPinToInterrupt(2), touch_interrupt, RISING);
  ssd1306_128x64_i2c_init();
  ssd1306_clearScreen();
  
  randomSeed(analogRead(A0)*analogRead(A2));
  
  uint8_t mask = EEPROM.read(0) >> 6, ui;
  for (ui = 0; ui < LEN_CALLED; ui++) {
    if ((EEPROM.read(ui) >> 6) != mask) {
      called_last_modified = ui;
      break;
    }
  }
}

void loop() {
  if ((millis()-last_use)>30*1000) {
    max7219.shutdown(0, true);
    ssd1306_clearScreen();
    energy.PowerDown();
  }
  if (!(use)) return;

  max7219.shutdown(0, false);
  max7219.setIntensity(0, 1);
  ssd1306_drawBitmap(0, 0, 128, 16, upper_bitmap);
  ssd1306_drawBitmap(0, 6, 13*8, 16, lower_bitmap);

  uint8_t ui, id;

  for (ui = 0; ui < 10; ui++) {
    show_id(get_random_id());
    delay(150);
  }
  called_last_modified = (called_last_modified + 1) % LEN_CALLED;
  id = get_random_id();
  EEPROM.write(called_last_modified, id | ((EEPROM.read(called_last_modified)>>6)+1)%4<<6);
  for (ui = 0; ui < 2; ui++) {
    show_id(id);
    delay(200);
    max7219.clearDisplay(0);
    ssd1306_clearBlock(0, 2, 32*3, 32);
    delay(200);
  }
  show_id(id);

  last_use = millis();
  use = 0;
}

void touch_interrupt(void) {
  use = 1;
}

void show_id(uint8_t id) {
  uint8_t a = id/10, b = id%10, r = 0;
  for (; r<4; r++) max7219.setRow(0, r, pgm_read_byte(&digits_bitmap[a][r]));
  for (; r<8; r++) max7219.setRow(0, r, pgm_read_byte(&digits_bitmap[b][r-4]));
  ssd1306_drawBitmap(0, 2, 32*3, 32, names_bitmap[id-1]);
}

uint8_t get_random_id(void) {
  uint8_t id, ui;
  gen_id:
  id = random(1, NUM_OF_STUDENTS+1);
  for (ui = 0; ui < LEN_CALLED; ui++) if (id == (EEPROM.read(ui)&0b00111111)) goto gen_id;
  for (ui = 0; ui < LEN_BLACKLIST; ui++) if (id == blacklist[ui]) goto gen_id;
  return id;
}
