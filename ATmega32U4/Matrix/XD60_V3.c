#include "../Functions.h"
#include "../ws2812.h"
#if (defined xd60 )||  (defined xd75 )
#if defined xd60
/*
//row D0 D1 D2 D3 D5
//col F0,F1,E6,C7,C6,B6,D4,B1,B7,B5,B4,D7,D6,B3
uint8_t LED_caps=2;
uint8_t rgb=17;
uint8_t full led=18;
*/
#define LED_COUNT 1
#define FULL_LED 18
uint8_t row_pins[ROWS]={5,6,7,8,23};
uint8_t col_pins[COLS]={21,20,24,10,9,15,22,1,4,14,13,12,11,3};
//                     1  2  3  4  5  6  7 8 9 10 11 12 13 14
uint8_t led_pins[LED_COUNT]={0xFF,2};//缺numlock灯
uint8_t rgb_rainbow[WS2812_COUNT]={0,34,68,102,136,170,170,136,102,68,34,0};
uint8_t hexa_keys0[ROWS][COLS] = {
	{MACRO2,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE},
	{KEY_TAB,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,0x00,KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,0x00, KEY_SLASH,KEY_UP},
	{KEY_LEFT_CTRL,KEY_FN,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,KEY_SLASH,KEY_LEFT,0x00,KEY_FN,KEY_RIGHT_CTRL,KEY_DOWN, KEY_RIGHT}
};
uint8_t hexa_keys1[ROWS][COLS] = {
	{KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12, KEY_DELETE},
	{KEY_TAB,KEYPAD_1,KEYPAD_2,KEYPAD_3,KEYPAD_4,KEYPAD_5,KEYPAD_6,KEYPAD_7,KEYPAD_8,KEYPAD_9,KEYPAD_0,KEYPAD_MINUS,KEYPAD_PLUS,KEY_BACKSLASH},
	{KEY_CAPS_LOCK, MOUSE_LEFT,MOUSE_MID,MOUSE_RIGHT,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00,KEY_NUM_LOCK ,KEY_SCROLL_LOCK,KEY_INSERT,KEY_PRINTSCREEN,0x00,0x00,0x00,AUDIO_VOL_DOWN,AUDIO_VOL_UP,0x00,0x00,KEY_UP},
	{KEY_LEFT_CTRL,KEY_FN,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,KEY_LEFT,0x00,KEY_FN,KEY_RIGHT_CTRL,KEY_DOWN,KEY_RIGHT}
};
//keyMask_bits:7-press 654-hexatype0 3-press 210-hexatype1
//type: 1-key 2-modifykey 3-mousekey 4-systemkey 5-consumerkey 6-FN 7-macro
uint8_t key_mask[ROWS][COLS] = {
	{0x71,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11},
	{0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11},
	{0x11,0x13,0x13,0x13,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x11},
	{0x22,0x00,0x11,0x11,0x11,0x11,0x10,0x10,0x10,0x15,0x15,0x00,0x10,0x11},
	{0x22,0x66,0x22,0x00,0x00,0x11,0x00,0x10,0x11,0x00,0x66,0x22,0x11,0x11}
};
///////////////////////////////////////////////////////////////////////////
#elif defined xd75
//#define MATRIX_ROW_PINS { D0, D1, D2, D3, D5 }
//#define MATRIX_COL_PINS { F0, F1, E6, C7, C6, B6, D4, B1, B7, B5, B4, D7, D6, B3, B0 }
//CAPSLOCK_LED    B2 2
//GP103_LED       F4 19
//KEYCAPS_LED     F5 18
//GP100_LED       F7 16
#define LED_COUNT 2
#define FULL_LED 18
uint8_t row_pins[ROWS]={5,6,7,8,23};
uint8_t col_pins[COLS]={21,20,24,10,9,15,22,1,4,14,13,12,11,3,0};
uint8_t led_pins[LED_COUNT]={2,18};uint8_t ledPins2[LED_COUNT]={19,16};
uint8_t rgb_rainbow[WS2812_COUNT]={0,34,68,102,136,170};
uint8_t hexa_keys0[ROWS][COLS] = {
	{KEY_TILDE,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE,KEY_DELETE},
	{KEY_TILDE,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE,MACRO0},
	{KEY_TILDE,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE,MACRO1},
	{KEY_TILDE,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE,MACRO2},
	{KEY_TILDE,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE,MACRO3},
};
uint8_t hexa_keys1[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
uint8_t key_mask[ROWS][COLS] = {
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x70},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x70},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x70},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x70}
};
#endif
//////////////////////////////////////////////////////////////////////
uint16_t delay_val;
uint8_t r,c,i,FN;
uint8_t delay_after=0;//backswing 后摇
uint8_t delay_before=0;//windup 前摇
void initCols(){
	for ( i=0; i<COLS; i++){
		pinMode(col_pins[i],INPUT);
		digitalWrite(col_pins[i],HIGH);
	}
}
void initRows(){
	for ( i=0; i<ROWS; i++){
		pinMode(row_pins[i],INPUT);
		digitalWrite(row_pins[i],HIGH);
	}
}
void openLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(led_pins[i],HIGH);
	}
}
void closeLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(led_pins[i],LOW);
	}
}
void initLED(){
	ws2812Setup();
	ws2812Clear();
	ws2812Send2();
	for ( i=0; i<LED_COUNT; i++){
		pinMode(led_pins[i],OUTPUT);
		digitalWrite(led_pins[i],LOW);
	}
	pinMode(FULL_LED,OUTPUT);
	digitalWrite(FULL_LED,LOW);
	delay_val=MAX_DELAY;
}
void resetLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(led_pins[i],LOW);
	}
	digitalWrite(FULL_LED,LOW);
	rgb_state=rgb_type;
	ws2812Clear();
	ws2812Send2();
}
void updateLED(){
	for ( i=0; i<LED_COUNT; i++){
		if((keyboard_buffer.keyboard_leds&(1<<i))==(1<<i)){
		digitalWrite(led_pins[i],HIGH);}
		else{
		digitalWrite(led_pins[i],LOW);}
	}
	#ifdef xd75
	for ( i=0; i<LED_COUNT; i++){
		if((keyboard_buffer.keyboard_leds&(1<<i))==(1<<i)){
		digitalWrite(ledPins2[i],HIGH);}
		else{
		digitalWrite(ledPins2[i],LOW);}
	}
	#endif
	/////////////full led///////////////////
	if(rgb_state & (1<<5)){
	digitalWrite(FULL_LED,HIGH);}
	else{
	digitalWrite(FULL_LED,LOW);}
	/////////////RGB///////////////////
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
/////////////////////////////////////////////////////////////////////
void qmkMode(){
	for (r = 0; r < ROWS; r++) {
		pinMode(row_pins[r],OUTPUT);
		digitalWrite(row_pins[r],LOW);
		for (c = 0; c < COLS; c++) {
			if (digitalRead(col_pins[c])) {key_mask[r][c]&= ~0x88;}
			else {key_mask[r][c]|= 0x88;delay_after=DELAY_AFTER;}
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
	if(usbMacroSendRequired())delay_before=DELAY_BEFORE;
	if(usbKeyboardSendRequired())delay_before=DELAY_BEFORE;
	if(usbMouseSendRequired())delay_before=DELAY_BEFORE;
	if(delay_after==DELAY_AFTER && delay_before==1)
	{usbMacroSend();usbKeyboardSend();usbMouseSend();}
	if(delay_after==1)
	{usbMacroSend();usbKeyboardSend();usbMouseSend();}
	if(delay_after>0)delay_after-=1;
	if(delay_before>0)delay_before-=1;
}
int initMain(void) {
	CPU_PRESCALE(CPU_16MHz);//16M晶振分频设置
	closeJtag();
	initLED();//插电亮灯会掉电，导致hub掉电不识别。所以要提前关灯。
	_delay_ms(500);
	usbInit();
	while (!usbConfigured()){_delay_ms(300);}
	//  TCCR0A = 0x00;
	//	TCCR0B =(1<<CS00);
	//	TIMSK0 = (1<<TOIE0);
	////////////////////////////////////////////////
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
/*
ISR(TIMER0_OVF_vect)
{

}
*/