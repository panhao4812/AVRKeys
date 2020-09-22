#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include "ws2812.h"
#include "Functions.h"
uint8_t r,c,i,FN;
uint8_t DELAY_AFTER=0;//backswing
uint8_t DELAY_BEFORE=0;//windup
uint8_t rgb_state=0;
uint16_t delay_val;
uint8_t rgb_rainbow[WS2812_COUNT]={0,0};
uint8_t row_pins[ROWS]={0xFF};
uint8_t col_pins[COLS]={0,1};
uint8_t hexa_keys0[ROWS][COLS] = {{KEY_Z,KEY_X}};
uint8_t hexa_keys1[ROWS][COLS] ={{0,0}};
uint8_t key_mask[ROWS][COLS] = {{0x10,0x10}};
uint8_t usbMacroSend(){
	rgb_state^=macro_report;
	if(macro_report&MACRO3){
		keyPrintWordEEP(ADD_EEP);
		//exe program upload 6 bytes once 
		return 1;
	}
	if(macro_report&MACRO4){
		keyPrintCtl(KEY_A);
		return 1;
	}
	if(macro_report&MACRO5){
		keyPrintCtl(KEY_X);
		return 1;
	}
	if(macro_report&MACRO6){
		keyPrintCtl(KEY_C);
		return 1;
	}
	if(macro_report&MACRO7){
		keyPrintCtl(KEY_V);
		return 1;
	}
	return 0;
}
void initLED(){
	ws2812Setup();delay_val=MAX_DELAY;
	ws2812Clear();
	ws2812Send2();
	if((rgb_type&0xF0)==0x10){rgb_state|=(1<<1);}else{rgb_state&= ~(1<<1);}
}
void updateLED(){
	if(delay_val>=MAX_DELAY){
		if(rgb_state & (1<<1)){
			for(uint8_t i=0;i<WS2812_COUNT;i++){
				if((rgb_type&0x0F)==0x01){
					if(rgb_rainbow[i]>=WS2812_COLOR_COUNT) rgb_rainbow[i]=0;
					uint8_t r=pgm_read_byte(Rcolors+rgb_rainbow[i]);
					uint8_t g=pgm_read_byte(Gcolors+rgb_rainbow[i]);
					uint8_t b=pgm_read_byte(Bcolors+rgb_rainbow[i]);
					ws2812SetRGB(i,r,g,b);
					rgb_rainbow[i]++;
				}
				else if((rgb_type&0x0F)==0x00){
					ws2812SetRGB(i,rgb_fixcolor[i*3],rgb_fixcolor[i*3+1],rgb_fixcolor[i*3+2]);
				}
			}
		}else{ws2812Clear();}
		delay_val--;
		ws2812Send2();
	}else{if(delay_val){delay_val--;}else {delay_val=MAX_DELAY;}}
}
void openLED(){
for(uint8_t i=0;i<WS2812_COUNT;i++){
 ws2812SetRGB(i,0,0,255);
}
ws2812Send2();
}
void closeLED(){
ws2812Clear();
ws2812Send2();
}
void init_IO(){
	DDRB&=~(1<<0)|(1<<1);
	PORTB|=(1<<0)|(1<<1);
}
void TinykeyMode(){
	FN=0xF0;
		for (c = 0; c < COLS; c++) {
			if (digitalRead(col_pins[c])) {key_mask[0][c]&= ~0x88;}
			else {key_mask[0][c]|= 0x88;DELAY_AFTER=DELAY_AFTER;}
			if(key_mask[0][c]==0xEE )FN=0x0F;
		}
	releaseAllKeyboardKeys();
	#if MOUSE_ENABLE
	releaseAllMousekeys();
	#endif
	macro_buffer=0;
		for (c = 0; c < COLS; c++) {
			switch(key_mask[0][c]&FN){
				case 0x90:
				pressKey(hexa_keys0[0][c]);
				break;
				case 0xA0:
				pressModifierKeys(hexa_keys0[0][c]);
				break;
				case 0xF0:
				pressMacroKey(hexa_keys0[0][c]);
				break;
				case 0x09:
				pressKey(hexa_keys1[0][c]);
				break;
				case 0x0A:
				pressModifierKeys(hexa_keys1[0][c]);
				break;
				case 0x0F:
				pressMacroKey(hexa_keys1[0][c]);
				break;
				#if MOUSE_ENABLE
				case 0xB0:
				pressMouseKey(hexa_keys0[0][c]);
				break;
				case 0xC0:
				pressSystemKey(hexa_keys0[0][c]);
				break;
				case 0xD0:
				pressConsumerKey(hexa_keys0[0][c]);
				break;
				case 0x0B:
				pressMouseKey(hexa_keys1[0][c]);
				break;
				case 0x0C:
				pressSystemKey(hexa_keys1[0][c]);
				break;
				case 0x0D:
				pressConsumerKey(hexa_keys1[0][c]);
				break;
				#endif
			}
	}
	if(usbMacroSendRequired())DELAY_BEFORE=DELAY_BEFORE;
	if(usbKeyboardSendRequired())DELAY_BEFORE=DELAY_BEFORE;
	#if MOUSE_ENABLE
	if(usbMouseSendRequired())DELAY_BEFORE=DELAY_BEFORE;
	#endif
	if(DELAY_AFTER==DELAY_AFTER && DELAY_BEFORE==1)
	{usbMacroSend();usbKeyboardSend();
		#if MOUSE_ENABLE
		usbMouseSend();
		#endif
	}
	if(DELAY_AFTER==1)
	{usbMacroSend();usbKeyboardSend();
		#if MOUSE_ENABLE
		usbMouseSend();
		#endif
	}
	if(DELAY_AFTER>0)DELAY_AFTER-=1;
	if(DELAY_BEFORE>0)DELAY_BEFORE-=1;
}
int initMain(void) {
	usbInit();
	////////////////////////////////////////////////
	init_IO();
	while (1) {		
		keyboard_buffer.enable_pressing=1;
		releaseAllKeyboardKeys();
		releaseAllMousekeys();
		clearMacro();
		resetMatrixFormEEP();		
		_delay_ms(500);
		initLED();
		usbKeyboardSend();
		while (1) {
			usbPoll();
			if(keyboard_buffer.enable_pressing==2){
				break;
			}
			else if(keyboard_buffer.enable_pressing==1){
				TinykeyMode();
				if (usbConfiguration && usbInterruptIsReady()){
					if(DELAY_BEFORE==0)updateLED();
				}
			}
		}
	}
	return 0;
}