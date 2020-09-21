#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <avr/io.h>
#include "usbdevice.h"
#include "Keycode.h"
//#define ps2avrU
//#define bface60_v2_1
#define minila

#if defined(ps2avrU)
#define ROWS  8
#define COLS  10
#define WS2812_COUNT	0
#elif defined(bface60_v2_1)
#define ROWS  5
#define COLS  15
#define WS2812_COUNT	12
#elif defined(minila)
#define ROWS  8
#define COLS  15
#define WS2812_COUNT	20
#else
#define ROWS  8
#define COLS  15
#define WS2812_COUNT	0
#endif
//////////////ws2812////////////////////
#define WS2812_PORT		PORTC
#define WS2812_DDR		DDRC
#define WS2812_MASK		(1<<1)
#define WS2812_SAVE		1
#define Maxdelay 0x1000
uint8_t WS2812fix[(WS2812_COUNT * 3)];
uint8_t RGB_Type;
uint8_t ledmacro;
//ledmacro和RGB_Type定义相同
//bit7->第1组 0 off, 1 on
//bit6->第2组 0 off, 1 on
//bit5->第full组 0 off, 1 on
//bit4->第RGB组 0 off, 1 on
//bit0-3->第1组 0 fix WS2812fix[]，1 Rainbow cindex[]，print
////////////////matrix////////////////////
#define _delay_after 0x06
#define _delay_before 0x03
uint8_t keymask[ROWS][COLS];
uint8_t hexaKeys1[ROWS][COLS];
uint8_t hexaKeys0[ROWS][COLS];
uint8_t rowPins[ROWS];
uint8_t colPins[COLS];
int init_main(void);
void init_rows();
void init_cols();
void Open_LED();
void Close_LED();
void init_LED();
void Reset_LED();
void LED();
void BfaceMod();
void keyPrintWordEEP(uint16_t address_t);
///////////////////eeprom//////////////////////////
#define add1 10
#define add2 add1+ROWS //10+5=15
#define add3 add2+COLS //15+15=30
#define add4 add3+(ROWS*COLS) //30+15*5=105
#define add5 add4+(ROWS*COLS) //105+15*5=180
#define addRGB add5+(ROWS*COLS) //180+15*5=255
#define addRGBType addRGB+(WS2812_COUNT*3)//255+12*3=291
#define addPrint addRGB+(WS2812_COUNT*3)+6 //291+6=297
void ResetMatrix(uint8_t mask,uint16_t address);
void ResetMatrixFormEEP();
//////////////////////////IO////////////////////////////
#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)
void pinMode(uint8_t IO,uint8_t value);
void digitalWrite(uint8_t IO,uint8_t value);
uint8_t digitalRead(uint8_t IO);
//////////////////////////////////////
#endif /* FUNCTIONS_H_ */