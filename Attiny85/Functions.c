
#include "Functions.h"
void resetMatrix(uint8_t mask,uint16_t address){
	uint8_t j=0;
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			switch (mask){
				case 0:
				hexa_keys0[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
				case 1:
				hexa_keys1[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
				case 2:
				key_mask[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
			}
			j++;
		}
	}
}
void resetMatrixFormEEP(){	
	uint16_t address_row=eeprom_read_word((uint16_t *)0);
	uint16_t address_col=eeprom_read_word((uint16_t *)2);
	uint16_t address_hexakeys0=eeprom_read_word((uint16_t *)4);
	uint16_t address_hexaKeys1=eeprom_read_word((uint16_t *)6);
	uint16_t address_keyMask=eeprom_read_word((uint16_t *)8);	
	if(address_row!=ADD_INDEX){return;}
	if(address_col!=ADD_ROW){return;}
	if(address_hexakeys0!=ADD_COL){return;}
	if(address_hexaKeys1!=ADD_KEYS1){return;}
	if(address_keyMask!=ADD_KEYS2){return;}
	//Confirm type of keyboard 
	uint16_t j;
	for( j=0;j<ROWS;j++){row_pins[j]=eeprom_read_byte((uint8_t *)(j+address_row));}
	for( j=0;j<COLS;j++){col_pins[j]=eeprom_read_byte((uint8_t *)(j+address_col));}
	resetMatrix(0,address_hexakeys0);
	resetMatrix(1,address_hexaKeys1);
	resetMatrix(2,address_keyMask);
	for( j=0;j<(WS2812_COUNT * 3);j++){rgb_fixcolor[j]=eeprom_read_byte((uint8_t *)(j+ADD_RGB));}
	rgb_type=eeprom_read_byte((uint8_t *)ADD_RGBTYPE);
}
void usbFunctionWriteOut(uchar *data, uchar len){
	if(len==8){
		if(data[0]==0xFF && data[1]==0xF1 ){
			if( keyboard_buffer.enable_pressing==1 ){
				keyboard_buffer.enable_pressing=0;
				openLED();
			}
			return;
		}
		else if(data[0]==0xFF && data[1]==0xF2 ){
			if( keyboard_buffer.enable_pressing==0 ){
				keyboard_buffer.enable_pressing=2;
				closeLED();
			}
			return;
		}
		/*
		else if(data[0]==0xFF && data[1]==0xFA ){
			openLED();
			return;
		}
		*/
		else if(keyboard_buffer.enable_pressing==0){
			uint8_t i=0;
			memset(&raw_report_out, 0,sizeof(raw_report_out));
			for(i=0;i<8;i++)raw_report_out.bytes[i]=data[i];
			uint16_t address=raw_report_out.word[0];
			if(address<(MAX_EEP-1)){
				eeprom_busy_wait();
				eeprom_write_word ((uint16_t *)address,raw_report_out.word[1]);
			}
			if((address+2)<(MAX_EEP-1)){
				eeprom_busy_wait();
				eeprom_write_word ((uint16_t *)(address+2),raw_report_out.word[2]);
			}
			if((address+4)<(MAX_EEP-1)){
				eeprom_busy_wait();
				eeprom_write_word ((uint16_t *)(address+4),raw_report_out.word[3]);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////
#if defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)
void pinMode(uint8_t IO,uint8_t value){
	if(value){DDRB|=(1<<IO);}else{DDRB &= ~(1<<IO);}
}
void digitalWrite(uint8_t IO,uint8_t value){
if(value){PORTB|= (1<<IO);}else{PORTB &= ~(1<<IO);}	
}
uint8_t digitalRead(uint8_t IO){
	uint8_t value=0;
    value= PINB& (1<<IO) ;
	return value;
}
#endif

