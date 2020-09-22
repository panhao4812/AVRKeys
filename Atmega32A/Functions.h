/*
 ��������
 ����Ϊ��д�»���
 ����ΪСд�»���
 ����Ϊ����ĸСд���»��� inline ����ĸ��д
 */ 
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <avr/io.h>
#include "usbdevice.h"
#include "Keycode.h"

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
#define MAX_DELAY 0x1000
uint16_t rgb_rainbow[WS2812_COUNT];
uint8_t rgb_fixcolor[(WS2812_COUNT * 3)];
uint8_t rgb_type;
uint8_t rgb_state;
//rgb_state��rgb_type������ͬ
//bit7->��1�� 0 off, 1 on
//bit6->��2�� 0 off, 1 on
//bit5->��full�� 0 off, 1 on
//bit4->��RGB�� 0 off, 1 on
//bit0-3->��1�� 0 fix rgb_fixcolor[]��1 Rainbow rgb_rainbow[]��print
////////////////matrix////////////////////
#define DELAY_AFTER  0x06
#define DELAY_BEFORE  0x03
uint8_t key_mask[ROWS][COLS];
uint8_t hexa_keys1[ROWS][COLS];
uint8_t hexa_keys0[ROWS][COLS];
uint8_t row_pins[ROWS];
uint8_t col_pins[COLS];
int initMain(void);
void initRows();
void initCols();
void openLED();
void closeLED();
void initLED();
void resetLED();
void updateLED();
void bfaceMod();
void keyPrintWordEEP(uint16_t address_t);
///////////////////eeprom//////////////////////////
#define ADD_INDEX 10
#define ADD_ROW ADD_INDEX+ROWS //10+5=15
#define ADD_COL ADD_ROW+COLS //15+15=30
#define ADD_KEYS1 ADD_COL+(ROWS*COLS) //30+15*5=105
#define ADD_KEYS2 ADD_KEYS1+(ROWS*COLS) //105+15*5=180
#define ADD_RGB ADD_KEYS2+(ROWS*COLS) //180+15*5=255
#define ADD_RGBTYPE ADD_RGB+(WS2812_COUNT*3)//255+12*3=291
#define ADD_EEP ADD_RGB+(WS2812_COUNT*3)+6 //291+6=297
void resetMatrix(uint8_t mask,uint16_t address);
void resetMatrixFormEEP();
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