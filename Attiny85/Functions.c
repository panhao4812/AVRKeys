#include "Functions.h"
uint8_t i1=0;
uint8_t i2=0;
//////////////////////////////////////////////////////////////////////
uint8_t usb_mouse_send_required(){
	uint8_t send_required_t=0;
	if(mouse_report.mouse.buttons!=mouse_buffer.mouse_keys)
	{
		mouse_report.mouse.buttons=mouse_buffer.mouse_keys;
		send_required_t=REPORT_ID_MOUSE;
	}
	if(mouse_report.system_keys.usage!=mouse_buffer.system_keys)
	{
		mouse_report.system_keys.usage=mouse_buffer.system_keys;
		send_required_t=REPORT_ID_SYSTEM;
	}
	if(mouse_report.consumer_keys.usage!=mouse_buffer.consumer_keys)
	{
		mouse_report.consumer_keys.usage=mouse_buffer.consumer_keys;
		send_required_t=REPORT_ID_CONSUMER;
	}
	if(send_required_t){
		mouse_buffer.Send_Required=send_required_t;
	}
	return send_required_t;
}
uint8_t usb_keyboard_send_required(){
	uint8_t send_required_t=0;
	if(keyboard_report.modifier!=keyboard_buffer.keyboard_modifier_keys)
	{keyboard_report.modifier = keyboard_buffer.keyboard_modifier_keys;send_required_t=1;}
	for(uint8_t i=0;i<KEY_CODE_NUMBER;i++){
		if(keyboard_report.keycode[i]!=keyboard_buffer.keyboard_keys[i])
		{keyboard_report.keycode[i]=keyboard_buffer.keyboard_keys[i];send_required_t=1;}
	}
	if(send_required_t)keyboard_buffer.Send_Required=send_required_t;
	return send_required_t;
}
uint8_t usb_mouse_send(){
	if(mouse_buffer.Send_Required==0)return 0;
	i1=0;
	while(++i1<50){
		usbPoll();
		if (usbInterruptIsReady3()){
			if(mouse_buffer.Send_Required==REPORT_ID_MOUSE){
				usbSetInterrupt3((void *)&mouse_report.mouse, sizeof(report_mouse0_t));
				mouse_buffer.Send_Required=0;
			}
			else if(mouse_buffer.Send_Required==REPORT_ID_SYSTEM){
				usbSetInterrupt3((void *)&mouse_report.system_keys, sizeof(report_extra_t));
				mouse_buffer.Send_Required=0;
			}
			else if(mouse_buffer.Send_Required==REPORT_ID_CONSUMER){
				usbSetInterrupt3((void *)&mouse_report.consumer_keys, sizeof(report_extra_t));
				mouse_buffer.Send_Required=0;
			}
			return 1;
		}
	}
	return 0;
}
uint8_t usb_keyboard_send(){
	if(keyboard_buffer.Send_Required==0)return 0;
	i2=0;
	while(++i2<50){
		usbPoll();
		if (usbInterruptIsReady()){
			if(keyboard_buffer.Send_Required){
				usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
				keyboard_buffer.Send_Required=0;
			}
			return 1;
		}
	}
	return 0;
}
void usb_update(){
	usbPoll();
}
////////////////////////////////////////////
uint8_t presskey(uint8_t key){
	uint8_t i;
	for ( i=0; i < KEY_CODE_NUMBER; i++) {
		if (keyboard_buffer.keyboard_keys[i] == key) {
			return 1;
		}
	}
	for ( i=0; i < KEY_CODE_NUMBER; i++) {
		if (keyboard_buffer.keyboard_keys[i] == 0) {
			keyboard_buffer.keyboard_keys[i] = key;
			return 1;
		}
	}
	return 0;
}
void pressModifierKeys(uint8_t key){
	keyboard_buffer.keyboard_modifier_keys|=key;
}
void pressmousekey(uint8_t key){
	mouse_buffer.mouse_keys|=key;
}
void presssystemkey(uint8_t key){
	mouse_buffer.system_keys=(uint16_t)key;
}
void pressconsumerkey(uint8_t key){
	mouse_buffer.consumer_keys=(uint16_t)key;
}
void releaseAllmousekeys(){
	mouse_buffer.mouse_keys=0;
	mouse_buffer.system_keys=0;
	mouse_buffer.consumer_keys=0;
}
void releaseAllkeyboardkeys(){
	uint8_t i;
	for ( i=0; i < KEY_CODE_NUMBER; i++) {
		keyboard_buffer.keyboard_keys[i] = 0;
	}
	keyboard_buffer.keyboard_modifier_keys=0;
}
////////////////////////HID report////////////////////////
void ResetMatrix(uint8_t mask,uint16_t address){
	uint8_t j=0;
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			switch (mask){
				case 0:
				hexaKeys0[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
				case 1:
				hexaKeys1[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
				case 2:
				keymask[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
			}
			j++;
		}
	}
}
void ResetMatrixFormEEP(){	
	uint16_t address_row=eeprom_read_word((uint16_t *)0);
	uint16_t address_col=eeprom_read_word((uint16_t *)2);
	uint16_t address_hexakeys0=eeprom_read_word((uint16_t *)4);
	uint16_t address_hexaKeys1=eeprom_read_word((uint16_t *)6);
	uint16_t address_keymask=eeprom_read_word((uint16_t *)8);	
	if(address_row!=add1){return;}
	if(address_col!=add2){return;}
	if(address_hexakeys0!=add3){return;}
	if(address_hexaKeys1!=add4){return;}
	if(address_keymask!=add5){return;}
	//Confirm type of keyboard 
	uint16_t j;
	for( j=0;j<ROWS;j++){rowPins[j]=eeprom_read_byte((uint8_t *)(j+address_row));}
	for( j=0;j<COLS;j++){colPins[j]=eeprom_read_byte((uint8_t *)(j+address_col));}
	ResetMatrix(0,address_hexakeys0);
	ResetMatrix(1,address_hexaKeys1);
	ResetMatrix(2,address_keymask);
	for( j=0;j<(WS2812_COUNT * 3);j++){WS2812fix[j]=eeprom_read_byte((uint8_t *)(j+addRGB));}
	RGB_Type=eeprom_read_byte((uint8_t *)addRGBType);
}
void usbFunctionWriteOut(uchar *data, uchar len){
	if(len==8){
		if(data[0]==0xFF && data[1]==0xF1 ){
			if( keyboard_buffer.enable_pressing==1 ){
				keyboard_buffer.enable_pressing=0;
				Open_LED();
			}
			return;
		}
		else if(data[0]==0xFF && data[1]==0xF2 ){
			if( keyboard_buffer.enable_pressing==0 ){
				keyboard_buffer.enable_pressing=2;
				Close_LED();
			}
			return;
		}
		/*
		else if(data[0]==0xFF && data[1]==0xFA ){
			Open_LED();
			return;
		}
		*/
		else if(keyboard_buffer.enable_pressing==0){
			uint8_t i=0;
			memset(&raw_report_out, 0,sizeof(raw_report_out));
			for(i=0;i<8;i++)raw_report_out.bytes[i]=data[i];
			uint16_t address=raw_report_out.word[0];
			if(address<(maxEEP-1)){
				eeprom_busy_wait();
				eeprom_write_word ((uint16_t *)address,raw_report_out.word[1]);
			}
			if((address+2)<(maxEEP-1)){
				eeprom_busy_wait();
				eeprom_write_word ((uint16_t *)(address+2),raw_report_out.word[2]);
			}
			if((address+4)<(maxEEP-1)){
				eeprom_busy_wait();
				eeprom_write_word ((uint16_t *)(address+4),raw_report_out.word[3]);
			}
		}
	}
}
//////////////////////////////IO////////////////////////////////////////
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
////////////////////////////print///////////////////////////////
void pressmacrokey(uint8_t key){
	if(key==MACRO2){
		if(keyboard_report.modifier){
			//??????keyboard buffer ???buffer???????????
			//report ?????????
			presskey(KEY_TILDE);
		}
		else{
			presskey(KEY_ESC);
		}
	}
	macrobuffer|=key;
}
uint8_t usb_macro_send_required(){
	if (macroreport!=macrobuffer){
		macroreport=macrobuffer;
	return 1;}
	return 0;
}
void keyPrintCtl(uint8_t data){
	while(1){usbPoll();
		if(usbConfiguration && usbInterruptIsReady()){
			keyboard_report.modifier=KEY_CTRL;
			keyboard_report.keycode[0]=0;keyboard_report.keycode[1]=0;
			usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
			break;
		}
	}usbPoll();
	while(1){usbPoll();
		if(usbConfiguration && usbInterruptIsReady()){
			keyboard_report.modifier=KEY_CTRL;
			keyboard_report.keycode[0]=data;keyboard_report.keycode[1]=0;
			usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
			break;
		}
	}usbPoll();
	while(1){usbPoll();
		if(usbConfiguration && usbInterruptIsReady()){
			keyboard_report.modifier=0;
			keyboard_report.keycode[0]=0;keyboard_report.keycode[1]=0;
			usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
			break;
		}
	}usbPoll();
}
void keyPrintChinese(uint8_t data[5]){
	while(1){usbPoll();
		if(usbConfiguration && usbInterruptIsReady()){
			keyboard_report.modifier = 0x40;
			keyboard_report.keycode[0] =0;
			usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
			break;
		}
	}usbPoll();
	uint8_t i=0;
	for( i=0;i<5;i++){
		while(1){usbPoll();
			if(usbConfiguration && usbInterruptIsReady()){
				keyboard_report.keycode[0]=98;
				if(data[i]>0){keyboard_report.keycode[0]=data[i]+88;}
				usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
				break;
			}
		}usbPoll();
		while(1){usbPoll();
			if(usbConfiguration && usbInterruptIsReady()){
				keyboard_report.keycode[0] =0;
				usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
				break;
			}
		}usbPoll();
	}

	while(1){usbPoll();
		if(usbConfiguration && usbInterruptIsReady()){
			keyboard_report.modifier = 0;
			keyboard_report.keycode[0] =0;
			usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
			break;
		}
	}usbPoll();
}
void keyPrintEnglish(uint8_t data)
{
	if(data==0)return;
	while(1){usbPoll();
		if(usbConfiguration && usbInterruptIsReady()){
			keyboard_report.modifier = (data >> 7) ? 0x20 : 0x00;//shift加了128
			keyboard_report.keycode[0] =data & 0b01111111;//abs删除正负号
			usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
			break;
		}
	}usbPoll();
	while(1){usbPoll();
		if(usbConfiguration && usbInterruptIsReady()){
			keyboard_report.modifier = 0;
			keyboard_report.keycode[0] =0;
			usbSetInterrupt((void *)&keyboard_report, sizeof(report_keyboard_t));
			break;
		}
	}usbPoll();
}
void keyPrintChar(usbWord_t data){
	keyboard_report.modifier = 0;
	memset(keyboard_report.keycode,0,6);
	if(data.bytes[1]==0x00){
		keyPrintEnglish(data.bytes[0]);
		}else{
		uint16_t out=(uint16_t)data.word;
		//out|=0x8080;//汉字内码每个byte最高位为1
		uint8_t datachinese[5];
		datachinese[4]=out%10;out=out/10;
		datachinese[3]=out%10;out=out/10;
		datachinese[2]=out%10;out=out/10;
		datachinese[1]=out%10;out=out/10;
		datachinese[0]=out;
		keyPrintChinese(datachinese);
	}
}
void keyPrintWordEEP(uint16_t address_t){
	keyboard_buffer.enable_pressing=0;
	uint16_t i=0;
	uint16_t len=eeprom_read_word((uint16_t *)address_t);
	for(i=0;i<len;i++){
		uint16_t address=address_t+i*2+2;
		if(address>maxEEP-1)break;
		while(1){
			if(usbConfiguration && usbInterruptIsReady()){
				uint16_t data = eeprom_read_word((uint16_t *)address);
				keyPrintChar((usbWord_t)data);
				break;
			}usbPoll();
		}
	}
	keyboard_buffer.enable_pressing=1;
}
