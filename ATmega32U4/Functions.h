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
#define MAX_DELAY 0x0100
#elif (defined xd004 )||(defined staryu )
#define WS2812_PORT		PORTC
#define WS2812_DDR		DDRC
#define WS2812_MASK		(1<<6)
#define WS2812_SAVE		1			/*Power saver, divide light level with this.*/
#define MAX_DELAY 0x0200
#elif (defined CXT64 )
#define WS2812_PORT		PORTD
#define WS2812_DDR		DDRD
#define WS2812_MASK		(1<<7)
#define WS2812_SAVE		1			/*Power saver, divide light level with this.*/
#define MAX_DELAY 0x0010
#elif defined(LILILI84)
#define WS2812_PORT		PORTB
#define WS2812_DDR		DDRB
#define WS2812_MASK		(1<<1)
#define WS2812_SAVE		1			/*Power saver, divide light level with this.*/
#define MAX_DELAY 0x0010
#elif defined(Vem84)
#define WS2812_PORT		PORTC
#define WS2812_DDR		DDRC
#define WS2812_MASK		(1<<7)
#define WS2812_SAVE		1			/*Power saver, divide light level with this.*/
#define MAX_DELAY 0x0010
#endif
//volatile uint16_t rgb_rainbow[WS2812_COUNT];
//uint8_t rgb_pos[WS2812_COUNT];
//volatile uint8_t led_mask[ROWS][COLS];
uint8_t rgb_fixcolor[(WS2812_COUNT * 3)];
uint8_t rgb_type;
uint8_t rgb_state;
//rgb_state和rgb_type定义相同,一个是实时，一个是默认状态。
//bit7->第1组 0 off, 1 on
//bit6->第2组 0 off, 1 on
//bit5->第full组 0 off, 1 on
//bit4->第RGB组 0 off, 1 on
//bit0-3->第1组 0 fix rgb_fixcolor[]，1 Rainbow rgb_rainbow[]，print
//////////////////////matrix/////////////////////////
#define DELAY_AFTER 0x08
#define DELAY_BEFORE 0x04
extern  uint8_t hexa_keys0[ROWS][COLS] ;
extern  uint8_t hexa_keys1[ROWS][COLS];
extern volatile uint8_t key_mask[ROWS][COLS];
extern  uint8_t row_pins[ROWS];
extern  uint8_t col_pins[COLS];
int initMain(void);
void initCols();
void initRows();
void qmkMode();
void initLED();
void resetLED();
void openLED();
void closeLED();
void updateLED();
//void LED_Timer(volatile uint16_t* delay_val_blink);
//void blink_LED(uint16_t delay_val_blink);
//void Rainbow_LED(uint16_t delay_val_blink);
//void Fix_LED(uint16_t delay_val_blink);
void keyPrintWordEEP(uint16_t address);
void keyPrintWordFlash(uint16_t address_t);
//////////////////////eeprom//////////////////////////
#define ADD_INDEX 10
#define ADD_ROW ADD_INDEX+ROWS //15 15
#define ADD_COL ADD_ROW+COLS //30 29
#define ADD_KEYS1 ADD_COL+(ROWS*COLS) //30+15*5=105 99
#define ADD_KEYS2 ADD_KEYS1+(ROWS*COLS) //105+75=180 169
#define ADD_RGB ADD_KEYS2+(ROWS*COLS) //180+75=255 239
#define ADD_RGBTYPE ADD_RGB+(WS2812_COUNT*3)//
#define ADD_EEP ADD_RGBTYPE+6 //
void resetMatrixFormEEP();
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