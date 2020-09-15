#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "usb_keyboard.h"
#include "Keycode.h"

///////////////////////ws2812//////////////////////////
#if (defined xd60 )||(defined xd75 )
#define WS2812_PORT		PORTF
#define WS2812_DDR		DDRF
#define WS2812_MASK		(1<<6)
#define WS2812_SAVE		1			/*Power saver, divide light level with this.*/
#define Maxdelay 0x0100
#elif (defined xd004 )||(defined staryu )
#define WS2812_PORT		PORTC
#define WS2812_DDR		DDRC
#define WS2812_MASK		(1<<6)
#define WS2812_SAVE		1			/*Power saver, divide light level with this.*/
#define Maxdelay 0x0200
#endif
uint8_t WS2812fix[(WS2812_COUNT * 3)];
uint8_t RGB_Type;// bit1-> 0 off 1 on ;bit0-> 0 fix£¬1 Rainbow
//////////////////////matrix/////////////////////////
#define _delay_after 0x08
#define _delay_before 0x04
extern  uint8_t hexaKeys0[ROWS][COLS] ;
extern  uint8_t hexaKeys1[ROWS][COLS];
extern  uint8_t keymask[ROWS][COLS];
extern  uint8_t rowPins[ROWS];
extern  uint8_t colPins[COLS];
int init_main(void);
void init_cols();
void init_rows();
void QMKMode();
void init_LED();
void Open_LED();
void Close_LED();
void LED();
void keyPrintWordEEP(uint16_t address);
void keyPrintWordFlash(uint16_t address_t);
//////////////////////eeprom//////////////////////////
#define add1 10
#define add2 add1+ROWS //15 15
#define add3 add2+COLS //30 29
#define add4 add3+(ROWS*COLS) //30+15*5=105 99
#define add5 add4+(ROWS*COLS) //105+75=180 169
#define addRGB add5+(ROWS*COLS) //180+75=255 239
#define addRGBType addRGB+(WS2812_COUNT*3)//
#define addPrint addRGB+(WS2812_COUNT*3)+6 //
void ResetMatrixFormEEP();
//////////////////////IO////////////////////////////////
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08
#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
void pinMode(uint8_t IO,uint8_t value);
void digitalWrite(uint8_t IO,uint8_t value);
uint8_t digitalRead(uint8_t IO);
void closeJtag();
///////////////////////////////////////////////////////
#endif /* */