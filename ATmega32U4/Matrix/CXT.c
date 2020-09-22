#include "../Functions.h"
#include "../ws2812.h"
#if (defined CXT64 )||  (defined CXT84 )
#if defined CXT64
/*
#define MATRIX_ROW_PINS { C7, C6, B6, B5, B4 }5
#define MATRIX_COL_PINS { F7, F6, F5, F4, F1, F0, E6, B0, B1, B2, B3, D0, D1, D2, D3 }15
uint8_t LED_caps=D5;
#define BACKLIGHT_PIN B7
#define RGB_DI_PIN D7
#define RGBLED_NUM 64
*/
#define LED_COUNT 1
uint8_t row_pins[ROWS]={10,9,15,14,13};
uint8_t col_pins[COLS]={16,17,18,19,20,21,24,0,1,2,3,5,6,7,8};
//                     1  2  3  4  5  6  7 8 9 10 11 12 13 14
uint8_t led_pins[LED_COUNT]={23};
uint8_t rgb_rainbow[WS2812_COUNT]={
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,       0x00,             0x00,                   0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t rgb_fixcolor[(WS2812_COUNT*3)]={
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,       0x00,             0x00,                   0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,       0x00,             0x00,                   0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,       0x00,             0x00,                   0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t hexa_keys0[ROWS][COLS] = {
	{MACRO2,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,0x00,KEY_BACKSPACE},
	{KEY_TAB,0x00,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK,0x00,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,KEY_ENTER,0x00},
	{0x00,KEY_LEFT_SHIFT,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,KEY_RIGHT_SHIFT,KEY_UP,KEY_RIGHT_CTRL},
	{KEY_LEFT_CTRL,KEY_FN,0x00,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,KEY_FN,KEY_FN,KEY_LEFT,KEY_DOWN,KEY_RIGHT}
};
uint8_t hexa_keys1[ROWS][COLS] = {
	{KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,0x00, KEY_DELETE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,KEY_LEFT_SHIFT,KEY_NUM_LOCK,MACRO1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_RIGHT_SHIFT,KEY_UP,KEY_RIGHT_CTRL},
	{KEY_LEFT_CTRL,KEY_FN,0x00,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,KEY_FN,KEY_FN,KEY_LEFT,KEY_DOWN,KEY_RIGHT}
};
//keyMask_bits:7-press 654-hexatype0 3-press 210-hexatype1
//type: 1-key 2-modifykey 3-mousekey 4-systemkey 5-consumerkey 6-FN 7-macro
uint8_t key_mask[ROWS][COLS] = {
	{0x71,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x11},
	{0x10,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00},
	{0x00,0x22,0x11,0x17,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x22,0x11,0x22},
	{0x22,0x66,0x00,0x22,0x00,0x00,0x11,0x00,0x00,0x00,0x66,0x66,0x11,0x11,0x11}
};
uint8_t ledMask[ROWS][COLS] = {
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x80},
	{0x80,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},
	{0x80,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},
	{0x80,0x80,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80}
};
///////////////////////////////////////////////////////////////////////////
#elif defined CXT84
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
	delay_val=MAX_DELAY*8;
}
void resetLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(led_pins[i],LOW);
	}
	rgb_state=rgb_type;//默认开关状态
	ws2812Clear();
	ws2812Send2();
}
uint8_t r,g,b;
void updateLED(){
	if((keyboard_buffer.keyboard_leds&(1<<1))==(1<<1)){
		digitalWrite(led_pins[0],HIGH);
		}else{
		digitalWrite(led_pins[0],LOW);
	}
	//////////////full led//////////////////
	if(rgb_state & (1<<5)){}//full led on
	else{}//full led off
	////////////////rgb////////////////
	if(rgb_state & (1<<4)){
		/////////////////rianbow/////////////////////
		if((rgb_state&0x0F)==0x01){
			for(uint8_t i=0;i<WS2812_COUNT;i++){
				if(rgb_rainbow[i]>=WS2812_COLOR_COUNT) rgb_rainbow[i]=0;
				if(delay_val==MAX_DELAY*1) r=pgm_read_byte(Rcolors+rgb_rainbow[i]);
				if(delay_val==MAX_DELAY*2) g=pgm_read_byte(Gcolors+rgb_rainbow[i]);
				if(delay_val==MAX_DELAY*3) b=pgm_read_byte(Bcolors+rgb_rainbow[i]);
				if(delay_val==MAX_DELAY*4){
					ws2812SetRGB(i,r,g,b);
					rgb_rainbow[i]++;
				}
			}
		}
		/////////////////fix/////////////////////
		else if((rgb_state&0x0F)==0x00){
			if(delay_val==MAX_DELAY*5){
				for(uint8_t i=0;i<WS2812_COUNT;i++){
					ws2812SetRGB(i,rgb_fixcolor[i*3],rgb_fixcolor[i*3+1],rgb_fixcolor[i*3+2]);//default
				}
			}
		}
		/////////////////print led/////////////////////
		else if((rgb_state&0x0F)==0x02){
			uint8_t wcount=WS2812_COUNT-1;
			//ws2812，1.2us一个bit，一个灯28.8us，100灯2.88ms 6灯17.28us。
			for (r = 0; r < ROWS; r++) {
				for (c = 0; c < COLS; c++) {
					if(wcount>=0 ){
						if(ledMask[r][c]){
							ws2812SetRGB(wcount,ledMask[r][c],ledMask[r][c],ledMask[r][c]);
						}
						else{
							ws2812SetRGB(wcount,0,0,0);
						}
						if((key_mask[r][c]&(~0x88))!=0)wcount--;
					}
				}
			}
			if(delay_val==MAX_DELAY*6){
				for (r = 0; r < ROWS; r++) {
					for (c = 0; c < COLS; c++) {
						if(ledMask[r][c]){ledMask[r][c]=ledMask[r][c]>>1;}
					}
				}
			}
		}
		}else{
		/////////////////////closed///////////////////
		ws2812Clear();
	}
	///////////////////clock /////////////////
	//尽可能减少每次循环的时间，将任务错开。
	if(delay_val>=MAX_DELAY*8){
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
			else {key_mask[r][c]|= 0x88;delay_after=DELAY_AFTER;ledMask[r][c]=0xFF;}
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
		rgb_type=0x02;//set default rgb
		releaseAllKeyboardKeys();
		releaseAllMousekeys();
		//	resetMatrixFormEEP();
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