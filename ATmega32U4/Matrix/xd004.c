#include "../functions.h"
#include "../ws2812.h"

#if (defined xd004 )||(defined staryu)
#if defined xd004
//SW d3 d0 c4 b4
//LED d5 d2 b7 b5
//RGB C6
#define LED_COUNT 4
uint8_t col_pins[COLS]={19,16,12,4};
uint8_t row_pins[ROWS]={0xFF};
uint8_t ledPins[LED_COUNT]={21,18,7,5};
uint8_t hexa_keys0[ROWS][COLS]={
	{KEY_1,KEY_2,KEY_3,KEY_FN}
};
uint8_t hexa_keys1[ROWS][COLS]={
	{MACRO0,MACRO1,MACRO3,KEY_FN}
};
uint8_t key_mask[ROWS][COLS]={
	{0x17,0x17,0x17,0x66}
};
uint8_t rgb_rainbow[WS2812_COUNT]={0,170};
////////////////////////////////////////////////////
#elif defined staryu
//SW D0 D1 D2 D3 D4
//LED C2 C7 D5 D6 B0
//RGB C6
#define LED_COUNT 5
uint8_t col_pins[COLS]={16,17,18,19,20};
uint8_t row_pins[ROWS]={0xFF};
uint8_t ledPins[LED_COUNT]={10,15,21,22,0};
uint8_t hexa_keys0[ROWS][COLS]={
	{KEY_UP,KEY_FN,KEY_RIGHT,KEY_DOWN,KEY_LEFT}
};
uint8_t hexa_keys1[ROWS][COLS]={
	{MACRO0,KEY_FN,MACRO4,MACRO5,MACRO1}
};
uint8_t key_mask[ROWS][COLS]={
	{0x17,0x66,0x17,0x17,0x17}
};
uint8_t rgb_rainbow[WS2812_COUNT]={0};
#endif
/////////////////////////////////////////////////////////////////////////continue
uint16_t delay_val;//rgb速率
uint8_t r,c,i,FN;
uint8_t DELAY_AFTER=0;//backswing
uint8_t DELAY_BEFORE=0;//windup
void initCols(){
	for ( i=0; i<COLS; i++){
		pinMode(col_pins[i],INPUT);
		digitalWrite(col_pins[i],HIGH);
	}
}
void initRows(){
}
void openLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(ledPins[i],LOW);
	}
}
void closeLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(ledPins[i],HIGH);
	}
}
void initLED(){
	ws2812Setup();
	ws2812Clear();
	ws2812Send2();
	for ( i=0; i<LED_COUNT; i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],HIGH);
	}
	delay_val=MAX_DELAY;
}
void resetLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(ledPins[i],HIGH);
	}
	rgb_state=rgb_type;
	ws2812Clear();
	ws2812Send2();
}
void updateLED(){
	//////////////////////////////full led/////////////////////
	if(rgb_state & (1<<5)){
		openLED();
	}
	else{
		closeLED();
	}
	////////////////////////////RGB////////////////////////
	if(delay_val>=MAX_DELAY){
		if(rgb_state & (1<<4)){
			for(uint8_t i=0;i<WS2812_COUNT;i++){
				if((rgb_state&0x0F)==0x01){
					if(rgb_rainbow[i]>=WS2812_COLOR_COUNT) rgb_rainbow[i]=0;
					uint8_t r=pgm_read_byte(Rcolors+rgb_rainbow[i]);
					uint8_t g=pgm_read_byte(Gcolors+rgb_rainbow[i]);
					uint8_t b=pgm_read_byte(Bcolors+rgb_rainbow[i]);
					ws2812SetRGB(i,r,g,b);
					rgb_rainbow[i]++;
				}
				else if((rgb_state&0x0F)==0x00){
					ws2812SetRGB(i,rgb_fixcolor[i*3],rgb_fixcolor[i*3+1],rgb_fixcolor[i*3+2]);
				}
			}
			}else{
		ws2812Clear();}
		delay_val--;
		ws2812Send2();
		}else{
		if(delay_val){
			delay_val--;
			}else {
			delay_val=MAX_DELAY;
		}
	}
}
void qmkMode(){
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			if (digitalRead(col_pins[c])) {key_mask[r][c]&= ~0x88;}
			else {key_mask[r][c]|= 0x88;DELAY_AFTER=DELAY_AFTER;}
			if(key_mask[r][c]==0xEE )FN=0x0F;
		}
		initRows();
	}
	releaseAllKeyboardKeys();
	releaseAllMousekeys();
	macro_buffer=0;
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			switch(key_mask[r][c]&FN){
				case 0x90:
				pressKey(hexa_keys0[r][c]);
				break;
				case 0xA0:
				pressModifierKeys(hexa_keys0[r][c]);
				break;
				case 0xB0:
				pressMouseKey(hexa_keys0[r][c]);
				break;
				case 0xC0:
				pressSystemKey(hexa_keys0[r][c]);
				break;
				case 0xD0:
				pressConsumerKey(hexa_keys0[r][c]);
				break;
				case 0xF0:
				pressMacroKey(hexa_keys0[r][c]);
				break;
				case 0x09:
				pressKey(hexa_keys1[r][c]);
				break;
				case 0x0A:
				pressModifierKeys(hexa_keys1[r][c]);
				break;
				case 0x0B:
				pressMouseKey(hexa_keys1[r][c]);
				break;
				case 0x0C:
				pressSystemKey(hexa_keys1[r][c]);
				break;
				case 0x0D:
				pressConsumerKey(hexa_keys1[r][c]);
				break;
				case 0x0F:
				pressMacroKey(hexa_keys1[r][c]);
				break;
			}
		}
	}
	if(!isBufferClear())FN=0xF0;//Fix FN key state error
	if(usbMacroSendRequired())DELAY_BEFORE=DELAY_BEFORE;
	if(usbKeyboardSendRequired())DELAY_BEFORE=DELAY_BEFORE;
	if(usbMouseSendRequired())DELAY_BEFORE=DELAY_BEFORE;
	if(DELAY_AFTER==DELAY_AFTER && DELAY_BEFORE==1)
	{usbMacroSend();usbKeyboardSend();usbMouseSend();}
	if(DELAY_AFTER==1)
	{usbMacroSend();usbKeyboardSend();usbMouseSend();}
	if(DELAY_AFTER>0)DELAY_AFTER-=1;
	if(DELAY_BEFORE>0)DELAY_BEFORE-=1;
}
int initMain(void)
{
	CPU_PRESCALE(CPU_16MHz);//16M晶振分频设置
	#if defined __AVR_ATmega32U4__
	void closeJtag();
	#endif
	initLED();//插电亮灯会掉电，导致hub掉电不识别。所以要提前关灯。
	_delay_ms(500);
	usbInit();
	while (!usbConfigured()){_delay_ms(300);}
	initCols();
	initRows();
	while (1) {//重启
		EnableRecv=1;
		keyboard_buffer.enable_pressing=1;
		rgb_type=0x01;///set default on & rainbow
		releaseAllKeyboardKeys();
		releaseAllMousekeys();
		resetMatrixFormEEP();
		resetLED();
		FN=0xF0;
		_delay_ms(500);
		usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
		while (1) {
			eepWrite();
			if(keyboard_buffer.enable_pressing==2){
				break;
			}
			else if(keyboard_buffer.enable_pressing==1){
				qmkMode();
				if(DELAY_BEFORE==0) updateLED();
			}
		}
	}
	return 0;
}
#endif