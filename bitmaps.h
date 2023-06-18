/*
  出于隐私考虑，bitmap.cpp不会公开
*/

#ifndef bitmaps_h
#define bitmaps_h

#include <stdint.h>
#include <avr/pgmspace.h>

#define NUM_OF_STUDENTS 52
#if NUM_OF_STUDENTS > 63
  #error
#endif


extern const uint8_t digits_bitmap[10][4] PROGMEM;
extern const uint8_t upper_bitmap[256] PROGMEM; // 自信自强追求卓越
extern const uint8_t lower_bitmap[208] PROGMEM; // Powered by LG
extern const uint8_t names_bitmap[NUM_OF_STUDENTS][384] PROGMEM;

#endif
