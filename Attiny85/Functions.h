/*
 命名规则
 常数为大写下划线
 变量为小写下划线
 函数为首字母小写无下划线
 */ 
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <avr/io.h>
#include "usbdevice.h"

#define ROWS  1
#define COLS  2
#define WS2812_COUNT	2
//ws2812
#define WS2812_PORT		PORTB
#define WS2812_DDR		DDRB
#define WS2812_MASK		(1<<2)
#define WS2812_SAVE		1
#define MAX_DELAY 0x1000
////////
#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

void pinMode(uint8_t IO,uint8_t value);
void digitalWrite(uint8_t IO,uint8_t value);
uint8_t digitalRead(uint8_t IO);
void initCols();
void initRows();
void initLED();
void openLED();
void closeLED();
void updateLED();
int initMain();
void resetMatrix(uint8_t mask,uint16_t address);
void resetMatrixFormEEP();

uint8_t key_mask[ROWS][COLS];
uint8_t hexa_keys1[ROWS][COLS];
uint8_t hexa_keys0[ROWS][COLS];
uint8_t row_pins[ROWS];
uint8_t col_pins[COLS];
uint8_t rgb_fixcolor[(WS2812_COUNT * 3)];
uint8_t rgb_type;// bit1-> 0 off 1 on ;bit0-> 0 fix，1 Rainbow
#define DELAY_AFTER 0x90
#define DELAY_BEFORE 0x40
#define ADD_INDEX 10
#define ADD_ROW ADD_INDEX+ROWS //11
#define ADD_COL ADD_ROW+COLS //13
#define ADD_KEYS1 ADD_COL+(ROWS*COLS) //15
#define ADD_KEYS2 ADD_KEYS1+(ROWS*COLS) //17
#define ADD_RGB ADD_KEYS2+(ROWS*COLS) //19
#define ADD_RGBTYPE ADD_RGB+(WS2812_COUNT*3)//19+2*3=25
#define ADD_EEP ADD_RGB+(WS2812_COUNT*3)+6 //25+6=31

#endif /* FUNCTIONS_H_ */