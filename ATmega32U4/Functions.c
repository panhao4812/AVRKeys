#include "Functions.h"
////////////////////keyprint////////////////////
void keyPrintChinese(uint8_t data[5]){
	memset(print_keyboard_report.keycode,0,6);
	print_keyboard_report.modifier = 0x40;
	print_keyboard_report.keycode[0] =0;
	usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
	for(uint8_t i=0;i<5;i++){
		print_keyboard_report.keycode[0]=98;
		if(data[i]>0){print_keyboard_report.keycode[0]=data[i]+88;}
		usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
		print_keyboard_report.keycode[0] =0;
		usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
	}
	print_keyboard_report.modifier = 0;
	print_keyboard_report.keycode[0] =0;
	usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
}
void keyPrintEnglish(uint8_t data)
{
	if(data==0)return;
	memset(print_keyboard_report.keycode,0,6);
	print_keyboard_report.modifier = (data >> 7) ? 0x20 : 0x00;//shift加了128
	print_keyboard_report.keycode[0] =data & 0b01111111;//abs删除正负号
	usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
	print_keyboard_report.modifier = 0;
	print_keyboard_report.keycode[0] =0;
	usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
}
void keyPrintChar(uint16_t wrapdata){
	usbWord_t data=(usbWord_t)wrapdata;
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
	uint16_t len=eeprom_read_word((uint16_t *)address_t);
	for(uint16_t i=0;i<len;i++){
		uint16_t address=address_t+i*2+2;
		if(address>MAX_EEP-1)break;
		uint16_t data = eeprom_read_word((uint16_t *)address);
		keyPrintChar(data);
	}
}
////////////////usb repport///////////////
uint8_t usbKeyboardSendRequired(){
	uint8_t send_required_t=0;
	if(keyboard_report.modifier!=keyboard_buffer.keyboard_modifier_keys)
	{keyboard_report.modifier = keyboard_buffer.keyboard_modifier_keys;send_required_t=1;}
	else if(keyboard_report.keycode[0]!=keyboard_buffer.keyboard_keys[0])
	{keyboard_report.keycode[0]=keyboard_buffer.keyboard_keys[0];send_required_t=1;}
	else if(keyboard_report.keycode[1]!=keyboard_buffer.keyboard_keys[1])
	{keyboard_report.keycode[1]=keyboard_buffer.keyboard_keys[1];send_required_t=1;}
	else if(keyboard_report.keycode[2]!=keyboard_buffer.keyboard_keys[2])
	{keyboard_report.keycode[2]=keyboard_buffer.keyboard_keys[2];send_required_t=1;}
	else if(keyboard_report.keycode[3]!=keyboard_buffer.keyboard_keys[3])
	{keyboard_report.keycode[3]=keyboard_buffer.keyboard_keys[3];send_required_t=1;}
	else if(keyboard_report.keycode[4]!=keyboard_buffer.keyboard_keys[4])
	{keyboard_report.keycode[4]=keyboard_buffer.keyboard_keys[4];send_required_t=1;}
	else if(keyboard_report.keycode[5]!=keyboard_buffer.keyboard_keys[5])
	{keyboard_report.keycode[5]=keyboard_buffer.keyboard_keys[5];send_required_t=1;}
	if(send_required_t)keyboard_buffer.Send_Required=send_required_t;
	return send_required_t;
}
uint8_t usbKeyboardSend()
{
	if(keyboard_buffer.Send_Required){
		keyboard_buffer.Send_Required=0;
		uint8_t send_required_t=usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
		return send_required_t;
	}return 1;
}
uint8_t usbMouseSendRequired(){
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
	if(send_required_t)mouse_buffer.Send_Required=send_required_t;
	return send_required_t;
}
uint8_t usbMouseSend(){
	uint8_t intr_state,timeout;
	mouse_buffer.Send_Required&=0x03;
	if(mouse_buffer.Send_Required==0)return 1;
	if (!usbConfigured()) return 1;
	intr_state = SREG;
	cli();
	timeout = FrameNumber() + 50;
	SetEP(MOUSE_ENDPOINT);
	while (1) {
		if (ReadWriteAllowed()) break;
		SREG = intr_state;
		if (!usbConfigured()) return 1;
		if (FrameNumber() == timeout) return 1;
		intr_state = SREG;
		cli();
		SetEP(MOUSE_ENDPOINT);
	}
	switch(mouse_buffer.Send_Required){
		case REPORT_ID_MOUSE:
		mouse_buffer.Send_Required=0;
		UEDATX=mouse_report.mouse.report_id;
		UEDATX=mouse_report.mouse.buttons;
		UEDATX=mouse_report.mouse.x;
		UEDATX=mouse_report.mouse.y;
		UEDATX=mouse_report.mouse.v;
		UEDATX=mouse_report.mouse.h;
		break;
		case REPORT_ID_SYSTEM:
		mouse_buffer.Send_Required=0;
		UEDATX=mouse_report.system_keys.report_id;
		UEDATX=LSB(mouse_report.system_keys.usage);
		UEDATX=MSB(mouse_report.system_keys.usage);
		break;
		case REPORT_ID_CONSUMER:
		mouse_buffer.Send_Required=0;
		UEDATX=mouse_report.consumer_keys.report_id;
		UEDATX=LSB(mouse_report.consumer_keys.usage);
		UEDATX=MSB(mouse_report.consumer_keys.usage);
		break;
	}
	ReleaseTX();
	SREG = intr_state;
	return 0;
}
uint8_t isBufferClear(){
	if(mouse_buffer.mouse_keys!=0)return 1;
	if(mouse_buffer.system_keys!=0)return 1;
	if(mouse_buffer.consumer_keys!=0)return 1;
	for (uint8_t i=0; i < 6; i++) {
		if(keyboard_buffer.keyboard_keys[i] != 0)return 1;
	}
	if(keyboard_buffer.keyboard_modifier_keys!=0)return 1;
	if(macro_buffer!=0)return 1;
	return 0;
}
uint8_t usbMacroSendRequired(){
	if (macro_report!=macro_buffer){
		macro_report=macro_buffer;
	return 1;}
	return 0;
}
void clearMouse(){
	memset(&print_mouse_report, 0, sizeof(mouse_report));
	memset(&mouse_report, 0, sizeof(mouse_report));
	memset(&mouse_buffer,0,sizeof(mouse_buffer));
	mouse_report.mouse.report_id= REPORT_ID_MOUSE;
	mouse_report.system_keys.report_id= REPORT_ID_SYSTEM;
	mouse_report.consumer_keys.report_id= REPORT_ID_CONSUMER;
	mouse_buffer.mouse_protocol=1;
}
void clearKeyboard(){
	memset( &print_keyboard_report, 0,sizeof(keyboard_report));
	memset( &keyboard_report, 0,sizeof(keyboard_report));
	memset( &keyboard_buffer, 0,sizeof(keyboard_buffer));
	keyboard_buffer.enable_pressing=1;
	// protocol setting from the host.  We use exactly the same reportMOUSE_ENABLE
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use.
	keyboard_buffer.keyboard_protocol=1;
	// the idle configuration, how often we send the report to the
	// host (ms * 4) even when it hasn't changed
	keyboard_buffer.keyboard_idle_config=125;
	// count until idle timeout
	keyboard_buffer.keyboard_idle_count=0;
}
void clearRaw(){
	memset( &raw_report_in, 0,sizeof(raw_report_in));
	memset(&raw_report_out, 0,sizeof(raw_report_out));
}
uint8_t usbMacroSend(){
	// MACRO0 0x01//full led
	// MACRO1 0x02//rgb led
	// MACRO2 0x04//esc ~
	// MACRO3 0x08//print eep
	//rgb_state和rgb_type定义相同
	//bit7->第1组 0 off, 1 on
	//bit6->第2组 0 off, 1 on
	//bit5->第full组 0 off, 1 on
	//bit4->第RGB组 0 off, 1 on
	//bit0-3->第1组 0 fix rgb_fixcolor[]，1 Rainbow rgb_rainbow[]，print led_mask[ROWS][COLS]
	//RGB_EFFECT_COUNT<=0x0F
	if(macro_report&MACRO0){
		rgb_state^=(1<<5);
	}
	if(macro_report&MACRO1){
		rgb_state^=(1<<4);
	}
	if(macro_report&MACRO3){
		keyPrintWordEEP(ADD_EEP);
		return 1;
	}
	#ifdef FLASH_END_ADDRESS
	if(macro_report&MACRO4){
	//调试用途
		keyPrintWordFlash(FLASH_END_ADDRESS);
		return 1;
	}
	#endif
	if(macro_report&MACRO5){
		rgb_state++;
		if((rgb_state&0x0F)>=RGB_EFFECT_COUNT)rgb_state&=0xF0;
		return 1;
	}
	return 0;
}
///////////////keys action///////////////////
void pressMacroKey(uint8_t key){
	if(key==MACRO2){
		if(keyboard_report.modifier){
			//不能用keyboard buffer 因为buffer是记录不稳定状态
			//report 则记录稳定状态
			pressKey(KEY_TILDE);
		}
		else{
			pressKey(KEY_ESC);
		}
	}
	macro_buffer|=key;
}
uint8_t pressKey(uint8_t key)
{
	for (uint8_t i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == key) {
			return 1;
		}
	}
	for (uint8_t i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == 0) {
			keyboard_buffer.keyboard_keys[i] = key;
			return 1;
		}
	}
	return 0;
}
void pressModifierKeys(uint8_t key)
{
	keyboard_buffer.keyboard_modifier_keys|=key;
}
void pressMouseKey(uint8_t key){
	mouse_buffer.mouse_keys|=key;
}
void pressSystemKey(uint8_t key){
	mouse_buffer.system_keys=0x0000|key;
}
void pressConsumerKey(uint8_t key){
	uint8_t mask_t=key&0xF0;
	switch(mask_t){
		case 0xB0:
		mouse_buffer.consumer_keys=0x0000|key;
		break;
		case 0xC0:
		mouse_buffer.consumer_keys=0x0000|key;
		break;
		case 0xE0:
		mouse_buffer.consumer_keys=0x0000|key;
		break;
		case 0x80:
		mouse_buffer.consumer_keys=0x0100|key;
		break;
		case 0x90:
		mouse_buffer.consumer_keys=0x0100|key;
		break;
		case 0x20:
		mouse_buffer.consumer_keys=0x0200|key;
		break;
		case 0x00:
		mouse_buffer.consumer_keys=0x0200|key;
		break;
	}
}
uint8_t releaseKey(uint8_t key)
{
	uint8_t send_required=0;
	for (uint8_t i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == key) {
			keyboard_buffer.keyboard_keys[i] = 0;
			send_required=1;
			break;
		}
	}
	return send_required;
}
void releaseModifierKeys(uint8_t key)
{
	keyboard_buffer.keyboard_modifier_keys&=~key;
}
void releaseMouseKey(uint8_t key){
	mouse_buffer.mouse_keys&=~key;
}
void releaseAllMousekeys(){
	mouse_buffer.mouse_keys=0x00;
	mouse_buffer.system_keys=0x0000;
	mouse_buffer.consumer_keys=0x0000;
}
void releaseAllKeyboardKeys()
{
	for (uint8_t i=0; i < 6; i++) {
		keyboard_buffer.keyboard_keys[i] = 0;
	}
	keyboard_buffer.keyboard_modifier_keys=0;
}
////////////////////HID report///////////////////
void resetMatrix(uint8_t mask,uint16_t address){
	uint8_t j=0;
	for (uint8_t r = 0; r < ROWS; r++) {
		for (uint8_t c = 0; c < COLS; c++) {
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
	///////////////////////////////////
	if(address_row!=ADD_INDEX){return;}
	if(address_col!=ADD_ROW){return;}
	if(address_hexakeys0!=ADD_COL){return;}
	if(address_hexaKeys1!=ADD_KEYS1){return;}
	if(address_keyMask!=ADD_KEYS2){return;}
	for(uint16_t j=0;j<ROWS;j++){row_pins[j]=eeprom_read_byte((uint8_t *)(j+address_row));}
	for(uint16_t j=0;j<COLS;j++){col_pins[j]=eeprom_read_byte((uint8_t *)(j+address_col));}
	resetMatrix(0,address_hexakeys0);
	resetMatrix(1,address_hexaKeys1);
	resetMatrix(2,address_keyMask);
	for(uint16_t j=0;j<(WS2812_COUNT * 3);j++){rgb_fixcolor[j]=eeprom_read_byte((uint8_t *)(j+ADD_RGB));}
	rgb_type=eeprom_read_byte((uint8_t *)ADD_RGBTYPE);
	//rgb_type&=0x11;
}
void eepWrite(){
	//	address,word1,word2,word3
	if (enableReset==0){
		uint16_t address=raw_report_out.word[0];
		if(address==0xF1FF && keyboard_buffer.enable_pressing==1 ){
			keyboard_buffer.enable_pressing=0;
		}
		else if(address==0xF2FF && keyboard_buffer.enable_pressing==0 ){
			closeLED();
			keyboard_buffer.enable_pressing=2;
		}
		else{
			if(keyboard_buffer.enable_pressing==0){
				openLED();
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
				closeLED();
			}
		}
		memset(&raw_report_out, 0,sizeof(raw_report_out));
		enableReset=1;
	}
}
#ifdef FLASH_END_ADDRESS
void keyPrintWordFlash(uint16_t address_t){
	uint16_t len=pgm_read_word_near((uint16_t *)address_t);
	if(len<1)return;
	for(uint16_t i=0;i<len;i++){
		uint16_t address=address_t+i*2+2;
		if(address>FLASH_END_ADDRESS)break;
		uint16_t data = pgm_read_word_near((uint16_t*)address);
		keyPrintChar(data);
	}
}
#endif
//////////////////IO///////////////////////
#if defined(__AVR_ATmega32U4__)
void closeJtag(){
	MCUCR = (1<<JTD);
	MCUCR = (1<<JTD);
}
void pinMode(uint8_t IO,uint8_t value){
	switch(IO){
		case 0: if(value){DDRB|= (1<<0);}else{DDRB &= ~(1<<0);}break;
		case 1: if(value){DDRB|= (1<<1);}else{DDRB &= ~(1<<1);}break;
		case 2: if(value){DDRB|= (1<<2);}else{DDRB &= ~(1<<2);}break;
		case 3: if(value){DDRB|= (1<<3);}else{DDRB &= ~(1<<3);}break;
		case 4: if(value){DDRB|= (1<<7);}else{DDRB &= ~(1<<7);}break;
		case 5: if(value){DDRD|= (1<<0);}else{DDRD &= ~(1<<0);}break;
		case 6: if(value){DDRD|= (1<<1);}else{DDRD &= ~(1<<1);}break;
		case 7: if(value){DDRD|= (1<<2);}else{DDRD &= ~(1<<2);}break;
		case 8: if(value){DDRD|= (1<<3);}else{DDRD &= ~(1<<3);}break;
		case 9: if(value){DDRC|= (1<<6);}else{DDRC &= ~(1<<6);}break;
		case 10: if(value){DDRC|= (1<<7);}else{DDRC &= ~(1<<7);}break;
		case 11: if(value){DDRD|= (1<<6);}else{DDRD &= ~(1<<6);}break;
		case 12: if(value){DDRD|= (1<<7);}else{DDRD &= ~(1<<7);}break;
		case 13: if(value){DDRB|= (1<<4);}else{DDRB &= ~(1<<4);}break;
		case 14: if(value){DDRB|= (1<<5);}else{DDRB &= ~(1<<5);}break;
		case 15: if(value){DDRB|= (1<<6);}else{DDRB &= ~(1<<6);}break;
		case 16: if(value){DDRF|= (1<<7);}else{DDRF &= ~(1<<7);}break;
		case 17: if(value){DDRF|= (1<<6);}else{DDRF &= ~(1<<6);}break;
		case 18: if(value){DDRF|= (1<<5);}else{DDRF &= ~(1<<5);}break;
		case 19: if(value){DDRF|= (1<<4);}else{DDRF &= ~(1<<4);}break;
		case 20: if(value){DDRF|= (1<<1);}else{DDRF &= ~(1<<1);}break;
		case 21: if(value){DDRF|= (1<<0);}else{DDRF &= ~(1<<0);}break;
		case 22: if(value){DDRD|= (1<<4);}else{DDRD &= ~(1<<4);}break;
		case 23: if(value){DDRD|= (1<<5);}else{DDRD &= ~(1<<5);}break;
		case 24: if(value){DDRE|= (1<<6);}else{DDRE &= ~(1<<6);}break;
	}
}
void digitalWrite(uint8_t IO,uint8_t value){
	switch(IO){
		case 0: if(value){PORTB |= (1<<0);}else{PORTB &= ~(1<<0);}break;
		case 1: if(value){PORTB|= (1<<1);}else{PORTB &= ~(1<<1);}break;
		case 2: if(value){PORTB|= (1<<2);}else{PORTB &= ~(1<<2);}break;
		case 3: if(value){PORTB|= (1<<3);}else{PORTB &= ~(1<<3);}break;
		case 4: if(value){PORTB|= (1<<7);}else{PORTB &= ~(1<<7);}break;
		case 5: if(value){PORTD|= (1<<0);}else{PORTD &= ~(1<<0);}break;
		case 6: if(value){PORTD|= (1<<1);}else{PORTD &= ~(1<<1);}break;
		case 7: if(value){PORTD|= (1<<2);}else{PORTD &= ~(1<<2);}break;
		case 8: if(value){PORTD|= (1<<3);}else{PORTD &= ~(1<<3);}break;
		case 9: if(value){PORTC|= (1<<6);}else{PORTC &= ~(1<<6);}break;
		case 10: if(value){PORTC|= (1<<7);}else{PORTC &= ~(1<<7);}break;
		case 11: if(value){PORTD|= (1<<6);}else{PORTD &= ~(1<<6);}break;
		case 12: if(value){PORTD|= (1<<7);}else{PORTD &= ~(1<<7);}break;
		case 13: if(value){PORTB|= (1<<4);}else{PORTB &= ~(1<<4);}break;
		case 14: if(value){PORTB|= (1<<5);}else{PORTB &= ~(1<<5);}break;
		case 15: if(value){PORTB|= (1<<6);}else{PORTB &= ~(1<<6);}break;
		case 16: if(value){PORTF|= (1<<7);}else{PORTF &= ~(1<<7);}break;
		case 17: if(value){PORTF|= (1<<6);}else{PORTF &= ~(1<<6);}break;
		case 18: if(value){PORTF|= (1<<5);}else{PORTF &= ~(1<<5);}break;
		case 19: if(value){PORTF|= (1<<4);}else{PORTF &= ~(1<<4);}break;
		case 20: if(value){PORTF|= (1<<1);}else{PORTF &= ~(1<<1);}break;
		case 21: if(value){PORTF|= (1<<0);}else{PORTF &= ~(1<<0);}break;
		case 22: if(value){PORTD|= (1<<4);}else{PORTD &= ~(1<<4);}break;
		case 23: if(value){PORTD|= (1<<5);}else{PORTD &= ~(1<<5);}break;
		case 24: if(value){PORTE|= (1<<6);}else{PORTE &= ~(1<<6);}break;
	}
}
uint8_t digitalRead(uint8_t IO){
	uint8_t value=0;
	switch(IO){
		case 0:  value= PINB & (1<<PINB0);break;
		case 1:  value= PINB& (1<<PINB1) ;break;
		case 2:  value= PINB& (1<<PINB2) ;break;
		case 3:  value= PINB& (1<<PINB3) ;break;
		case 4:  value= PINB& (1<<PINB7) ;break;
		case 5:  value= PIND& (1<<PIND0) ;break;
		case 6:  value= PIND& (1<<PIND1) ;break;
		case 7:  value= PIND& (1<<PIND2) ;break;
		case 8:  value= PIND& (1<<PIND3) ;break;
		case 9:  value= PINC& (1<<PINC6) ;break;
		case 10:  value= PINC& (1<<PINC7);break;
		case 11:  value= PIND& (1<<PIND6);break;
		case 12:  value= PIND& (1<<PIND7);break;
		case 13:  value= PINB& (1<<PINB4);break;
		case 14:  value= PINB& (1<<PINB5);break;
		case 15:  value= PINB& (1<<PINB6);break;
		case 16:  value= PINF& (1<<PINF7);break;
		case 17:  value= PINF& (1<<PINF6);break;
		case 18:  value= PINF& (1<<PINF5);break;
		case 19:  value= PINF& (1<<PINF4);break;
		case 20:  value= PINF& (1<<PINF1);break;
		case 21:  value= PINF& (1<<PINF0);break;
		case 22:  value= PIND& (1<<PIND4);break;
		case 23:  value= PIND& (1<<PIND5);break;
		case 24:  value= PINE& (1<<PINE6);break;
	}
	return value;
}
#elif (defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || \
defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega16U2__) || \
defined(__AVR_ATmega32U2__))
void pinMode(uint8_t IO,uint8_t value){
	switch(IO){
		case 0: if(value){DDRB|= (1<<0);}else{DDRB &= ~(1<<0);}break;
		case 1: if(value){DDRB|= (1<<1);}else{DDRB &= ~(1<<1);}break;
		case 2: if(value){DDRB|= (1<<2);}else{DDRB &= ~(1<<2);}break;
		case 3: if(value){DDRB|= (1<<3);}else{DDRB &= ~(1<<3);}break;
		case 4: if(value){DDRB|= (1<<4);}else{DDRB &= ~(1<<4);}break;
		case 5: if(value){DDRB|= (1<<5);}else{DDRB &= ~(1<<5);}break;
		case 6: if(value){DDRB|= (1<<6);}else{DDRB &= ~(1<<6);}break;
		case 7: if(value){DDRB|= (1<<7);}else{DDRB &= ~(1<<7);}break;
		case 8: if(value){DDRC|= (1<<0);}else{DDRC &= ~(1<<0);}break;
		case 9: if(value){DDRC|= (1<<1);}else{DDRC &= ~(1<<1);}break;
		case 10: if(value){DDRC|= (1<<2);}else{DDRC &= ~(1<<2);}break;
		case 11: if(value){DDRC|= (1<<3);}else{DDRC &= ~(1<<3);}break;
		case 12: if(value){DDRC|= (1<<4);}else{DDRC &= ~(1<<4);}break;
		case 13: if(value){DDRC|= (1<<5);}else{DDRC &= ~(1<<5);}break;
		case 14: if(value){DDRC|= (1<<6);}else{DDRC &= ~(1<<6);}break;
		case 15: if(value){DDRC|= (1<<7);}else{DDRC &= ~(1<<7);}break;
		case 16: if(value){DDRD|= (1<<0);}else{DDRD &= ~(1<<0);}break;
		case 17: if(value){DDRD|= (1<<1);}else{DDRD &= ~(1<<1);}break;
		case 18: if(value){DDRD|= (1<<2);}else{DDRD &= ~(1<<2);}break;
		case 19: if(value){DDRD|= (1<<3);}else{DDRD &= ~(1<<3);}break;
		case 20: if(value){DDRD|= (1<<4);}else{DDRD &= ~(1<<4);}break;
		case 21: if(value){DDRD|= (1<<5);}else{DDRD &= ~(1<<5);}break;
		case 22: if(value){DDRD|= (1<<6);}else{DDRD &= ~(1<<6);}break;
		case 23: if(value){DDRD|= (1<<7);}else{DDRD &= ~(1<<7);}break;
	}
}
void digitalWrite(uint8_t IO,uint8_t value){
	switch(IO){
		case 0: if(value){PORTB |= (1<<0);}else{PORTB &= ~(1<<0);}break;
		case 1: if(value){PORTB |= (1<<1);}else{PORTB &= ~(1<<1);}break;
		case 2: if(value){PORTB |= (1<<2);}else{PORTB &= ~(1<<2);}break;
		case 3: if(value){PORTB |= (1<<3);}else{PORTB &= ~(1<<3);}break;
		case 4: if(value){PORTB |= (1<<4);}else{PORTB &= ~(1<<4);}break;
		case 5: if(value){PORTB |= (1<<5);}else{PORTB &= ~(1<<5);}break;
		case 6: if(value){PORTB |= (1<<6);}else{PORTB &= ~(1<<6);}break;
		case 7: if(value){PORTB |= (1<<7);}else{PORTB &= ~(1<<7);}break;
		case 8: if(value){PORTC |= (1<<0);}else{PORTC &= ~(1<<0);}break;
		case 9: if(value){PORTC |= (1<<1);}else{PORTC &= ~(1<<1);}break;
		case 10: if(value){PORTC |= (1<<2);}else{PORTC &= ~(1<<2);}break;
		case 11: if(value){PORTC |= (1<<3);}else{PORTC &= ~(1<<3);}break;
		case 12: if(value){PORTC |= (1<<4);}else{PORTC &= ~(1<<4);}break;
		case 13: if(value){PORTC |= (1<<5);}else{PORTC &= ~(1<<5);}break;
		case 14: if(value){PORTC |= (1<<6);}else{PORTC &= ~(1<<6);}break;
		case 15: if(value){PORTC |= (1<<7);}else{PORTC &= ~(1<<7);}break;
		case 16: if(value){PORTD |= (1<<0);}else{PORTD &= ~(1<<0);}break;
		case 17: if(value){PORTD |= (1<<1);}else{PORTD &= ~(1<<1);}break;
		case 18: if(value){PORTD |= (1<<2);}else{PORTD &= ~(1<<2);}break;
		case 19: if(value){PORTD |= (1<<3);}else{PORTD &= ~(1<<3);}break;
		case 20: if(value){PORTD |= (1<<4);}else{PORTD &= ~(1<<4);}break;
		case 21: if(value){PORTD |= (1<<5);}else{PORTD &= ~(1<<5);}break;
		case 22: if(value){PORTD |= (1<<6);}else{PORTD &= ~(1<<6);}break;
		case 23: if(value){PORTD |= (1<<7);}else{PORTD &= ~(1<<7);}break;
	}
}
uint8_t digitalRead(uint8_t IO){
	uint8_t value=0;
	switch(IO){
		case 0:  value= PINB & (1<<0) ;break;
		case 1:  value= PINB & (1<<1) ;break;
		case 2:  value= PINB & (1<<2) ;break;
		case 3:  value= PINB & (1<<3) ;break;
		case 4:  value= PINB & (1<<4) ;break;
		case 5:  value= PINB & (1<<5) ;break;
		case 6:  value= PINB & (1<<6) ;break;
		case 7:  value= PINB & (1<<7) ;break;
		case 8:  value= PINC & (1<<0) ;break;
		case 9:  value= PINC & (1<<1) ;break;
		case 10:  value= PINC & (1<<2) ;break;
		case 11:  value= PINC & (1<<3) ;break;
		case 12:  value= PINC & (1<<4) ;break;
		case 13:  value= PINC & (1<<5) ;break;
		case 14:  value= PINC & (1<<6) ;break;
		case 15:  value= PINC & (1<<7) ;break;
		case 16:  value= PIND & (1<<0) ;break;
		case 17:  value= PIND & (1<<1) ;break;
		case 18:  value= PIND & (1<<2) ;break;
		case 19:  value= PIND & (1<<3) ;break;
		case 20:  value= PIND & (1<<4) ;break;
		case 21:  value= PIND & (1<<5) ;break;
		case 22:  value= PIND & (1<<6) ;break;
		case 23:  value= PIND & (1<<7) ;break;
	}
	return value;
}
#endif
