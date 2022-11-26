#include "../Functions.h"
#include "../ws2812.h"
#if defined PG60
/*
#define MATRIX_ROW_PINS { C7, C6, B6, B5, B4 }5
#define MATRIX_COL_PINS { F7, F6, F5, F4, F1, F0, E6, B0, B1, B2, B3, D0, D1, D2, D3 }15
#define RGB_DI_PIN D7
*/
#define LED_COUNT 0
uint8_t row_pins[ROWS]={10,9,15,14,13};
uint8_t col_pins[COLS]={16,17,18,19,20,21,24,0,1,2,3,5,6,7,8};
uint8_t led_pins[LED_COUNT]={};
uint8_t hexa_keys0[ROWS][COLS] = {
	{MACRO2,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,0x00,KEY_BACKSPACE},
	{KEY_TAB,0x00,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK,0x00,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,KEY_ENTER,0x00},
	{0x00,KEY_LEFT_SHIFT,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,0x00,KEY_RIGHT_SHIFT,0x00},
	{KEY_LEFT_CTRL,KEY_FN,0x00,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_LEFT,0x00,KEY_RIGHT,KEY_RIGHT_CTRL}
};
uint8_t hexa_keys1[ROWS][COLS] = {
	{KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,0x00, KEY_DELETE},
	{KEY_TAB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{KEY_CAPS_LOCK,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_ENTER,0x00},
	{0x00,KEY_LEFT_SHIFT,KEY_NUM_LOCK,MACRO1,MACRO5,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_RIGHT_SHIFT,KEY_UP,KEY_DELETE},
	{KEY_LEFT_CTRL,KEY_FN,0x00,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_UP,0x00,KEY_DOWN,KEY_RIGHT_CTRL}
};
//keyMask_bits:7-press 654-hexatype0 3-press 210-hexatype1
//type: 1-key 2-modifykey 3-mousekey 4-systemkey 5-consumerkey 6-FN 7-macro
volatile uint8_t key_mask[ROWS][COLS] = {
	{0x71,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x00,0x11},
	{0x11,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x11,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x11,0x00},
	{0x00,0x22,0x11,0x17,0x17,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x22,0x00},
	{0x22,0x66,0x00,0x22,0x00,0x00,0x11,0x00,0x00,0x00,0x22,0x11,0x00,0x11,0x22}
};
volatile uint8_t led_mask[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};
volatile uint16_t rgb_rainbow[WS2812_COUNT]={
	14,31,47,64,81,98,115,132,149,166,183,200,217,234,
	14,39,56,73,90,107,124,141,158,174,191,208,225,242,
	14,43,60,77,94,111,128,145,162,179,196,213,230,
	14,47,64,81,98,115,132,149,166,183,200,234,
	14,35,56,77,183,200,217,234
};
uint8_t rgb_fixcolor[(WS2812_COUNT*3)]={
	237,32,50,222,13,91,198,0,118,103,0,120,77,29,136,41,70,160,19,134,196,2,191,219,17,179,98,49,182,48,90,190,10,212,234,31,234,197,26,247,151,19,
	237,32,50,211,1,116,138,0,119,96,8,125,54,55,151,36,83,167,3,187,226,10,184,153,24,179,82,74,185,14,147,211,20,222,232,31,245,165,21,245,110,10,
	237,32,50,210,0,118,114,0,120,87,18,130,46,64,157,28,105,180,1,192,229,14,181,121,35,180,67,79,186,8,188,226,27,227,215,29,247,159,20,
	237,32,50,198,0,118,103,0,120,77,29,136,41,70,160,19,134,196,2,191,219,17,179,98,49,182,48,90,190,10,212,234,31,247,151,19,
	237,32,50,216,6,106,138,0,119,87,18,130,90,190,10,212,234,31,234,197,26,247,151,19
};
uint8_t rgb_pos[WS2812_COUNT]={
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
	31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
	46,47,48,49,50,51,52,53,54,55,56,57,58,59,60
};
uint16_t delay_val;
uint8_t FN;
uint8_t delay_after=0;//backswing 后摇
uint8_t delay_before=0;//windup 前摇
uint8_t color_r,color_g,color_b;
#define MAX_DELAY_1 MAX_DELAY*1
#define MAX_DELAY_2 MAX_DELAY*2
#define MAX_DELAY_3 MAX_DELAY*3
#define MAX_DELAY_4 MAX_DELAY*4
#define MAX_DELAY_5 MAX_DELAY*5
#define MAX_DELAY_6 MAX_DELAY*6
#define MAX_DELAY_7 MAX_DELAY*7
#define MAX_DELAY_8 MAX_DELAY*8
void initCols(){
	for (uint8_t i=0; i<COLS; i++){
		pinMode(col_pins[i],INPUT);
		digitalWrite(col_pins[i],HIGH);
	}
}
void initRows(){
	for (uint8_t i=0; i<ROWS; i++){
		pinMode(row_pins[i],INPUT);
		digitalWrite(row_pins[i],HIGH);
	}
}
void openLED(){}
void closeLED(){}
void initLED(){
	ws2812Setup();
	ws2812Clear();
	ws2812Send2();
	delay_val=MAX_DELAY_8;
}
void LED_Timer(volatile uint16_t* delay_val_blink){
	if((*delay_val_blink)==MAX_DELAY_7){ws2812Send2();}
	if(*delay_val_blink){(*delay_val_blink)--;}else{(*delay_val_blink)=MAX_DELAY_8;}
}
void Fix_LED(uint16_t delay_val_blink){
	if(delay_val_blink==MAX_DELAY_5){
		for(uint8_t i=0;i<WS2812_COUNT;i++){
			ws2812SetRGB(rgb_pos[i],rgb_fixcolor[i*3],rgb_fixcolor[i*3+1],rgb_fixcolor[i*3+2]);//default
		}
	}
}
void Rainbow_LED(uint16_t delay_val_blink){
	for(uint8_t i=0;i<WS2812_COUNT;i++){
		if(rgb_rainbow[i]>=WS2812_COLOR_COUNT) rgb_rainbow[i]=0;
		if(delay_val_blink==MAX_DELAY_4){color_r=pgm_read_byte(Rcolors+rgb_rainbow[i]);ws2812SetR(rgb_pos[i],color_r);}
		if(delay_val_blink==MAX_DELAY_3){color_g=pgm_read_byte(Gcolors+rgb_rainbow[i]);ws2812SetG(rgb_pos[i],color_g);}
		if(delay_val_blink==MAX_DELAY_2){color_b=pgm_read_byte(Bcolors+rgb_rainbow[i]);ws2812SetB(rgb_pos[i],color_b);}
		if(delay_val_blink==MAX_DELAY_1){rgb_rainbow[i]++;}
	}
}
void blink_LED(uint16_t delay_val_blink){
	if(delay_val_blink==MAX_DELAY_6){
		uint8_t wcount=0;
		//ws2812，1.2us一个bit，一个灯28.8us，100灯2.88ms 6灯17.28us。
		for (uint8_t r = 0; r < ROWS; r++) {
			for (uint8_t c = 0; c < COLS; c++) {
				if(wcount<WS2812_COUNT ){
					if(led_mask[r][c]){
						ws2812SetRGB(rgb_pos[wcount],led_mask[r][c],led_mask[r][c],led_mask[r][c]);
					}
					else{
						ws2812SetRGB(rgb_pos[wcount],0,0,0);
					}
					if((key_mask[r][c]&(~0x88))!=0)wcount++;//靠key_mask识别并跳过空键位
				}
			}
		}
		//shading
		for (uint8_t r = 0; r < ROWS; r++) {
			for (uint8_t c = 0; c < COLS; c++) {
				if(led_mask[r][c]){led_mask[r][c]=led_mask[r][c]>>1;}
			}
		}
	}
}
void resetLED(){
	RGB_STATE=RGB_TYPE;//默认开关状态
	ws2812Clear();
	ws2812Send2();
	///*
	//灯自检程序
	for (uint8_t kk = 0; kk < ROWS; kk++) {
		for (uint8_t jj = 0; jj < COLS; jj++) {
			if ((key_mask[ROWS-kk-1][jj]&(~0x88))==0)continue;
			led_mask[ROWS-kk-1][jj]=0xFF;
			for(uint16_t ii=0;ii<=(0x40+jj+kk);ii++){
				//MAX_DELAY_8==0x80	
				//ii jj kk 需要单独定义 避免和函数内部的i j k r c混用导致冲突
				_delay_us(300);		
				blink_LED(delay_val); 
				LED_Timer(&delay_val);	
			}
		}
	}
	/////等所有灯光效果结束
	for(uint16_t ii=0;ii<=(0xF0);ii++){
		_delay_us(300);
		blink_LED(delay_val);
		LED_Timer(&delay_val);
	}	
	delay_val=MAX_DELAY_8;	
	ws2812Clear();
	ws2812Send2();
	//*/
}
void updateLED(){
	////Keyboard Functional LED////////////////////////////
	
	////full led//////////////////////////////////////////
	
	///rgb////////////////////////////////////////////////
	if(RGB_STATE & (1<<4)){
		if((RGB_STATE&0x0F)==0x01){
			Rainbow_LED(delay_val);
		}
		else if((RGB_STATE&0x0F)==0x00){
			Fix_LED(delay_val);
		}
		else if((RGB_STATE&0x0F)==0x02){
			blink_LED(delay_val);
		}
		}else{
	//closed/////////////////////////////////////////////
		ws2812Clear();
	}
	////clock ///////////////////////////////////////////
	//尽可能减少每次循环的时间，将任务错开。
	LED_Timer(&delay_val);
}
/////////////////////////////////////////////////////////////////////
void qmkMode(){
	for (uint8_t r = 0; r < ROWS; r++) {
		//	initCols();
		pinMode(row_pins[r],OUTPUT);
		digitalWrite(row_pins[r],LOW);
		//串键问题，如果没有delay_us会导致col1或者col2串键，不一定每个板子都会串键，不串键可以取消掉delay_us
		//_delay_us(1);
		for (uint8_t c = 0; c < COLS; c++) {
			if (digitalRead(col_pins[c])) {key_mask[r][c]&= ~0x88;}
			else {key_mask[r][c]|= 0x88;delay_after=DELAY_AFTER;led_mask[r][c]=0xFF;}
			if(key_mask[r][c]==0xEE )FN=0x0F;
		}
		initRows();
	}
	releaseAllKeyboardKeys();
	releaseAllMousekeys();
	macro_buffer=0;
	for (uint8_t r = 0; r < ROWS; r++) {
		for (uint8_t c = 0; c < COLS; c++) {
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
		enableReset=1;
		keyboard_buffer.enable_pressing=1;
		RGB_TYPE=0x02;//set default rgb
		releaseAllKeyboardKeys();
		releaseAllMousekeys();
		resetMatrixFormEEP();
		resetLED();
		FN=0xF0;
		_delay_ms(300);
		usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
		while (1) {
			if(delay_before==0&&delay_after==0)eepWrite();
			if(keyboard_buffer.enable_pressing==2){
				break;
			}
			else if(keyboard_buffer.enable_pressing==1){
				qmkMode();
				if(delay_before==0) updateLED();
			}
		}
	}
	return 0;
}
#endif