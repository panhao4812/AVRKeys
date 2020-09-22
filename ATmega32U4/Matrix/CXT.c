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
#define ledcount 1
uint8_t rowPins[ROWS]={10,9,15,14,13};
uint8_t colPins[COLS]={16,17,18,19,20,21,24,0,1,2,3,5,6,7,8};
//                     1  2  3  4  5  6  7 8 9 10 11 12 13 14
uint8_t ledPins[ledcount]={23};
uint8_t RGB_Rainbow[WS2812_COUNT]={
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,       0x00,             0x00,                   0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t RGB_FixColor[(WS2812_COUNT*3)]={
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
uint8_t hexaKeys0[ROWS][COLS] = {
	{MACRO2,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,0x00,KEY_BACKSPACE},
	{KEY_TAB,0x00,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK,0x00,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,KEY_ENTER,0x00},
	{0x00,KEY_LEFT_SHIFT,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,KEY_RIGHT_SHIFT,KEY_UP,KEY_RIGHT_CTRL},
	{KEY_LEFT_CTRL,KEY_FN,0x00,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,KEY_FN,KEY_FN,KEY_LEFT,KEY_DOWN,KEY_RIGHT}
};
uint8_t hexaKeys1[ROWS][COLS] = {
	{KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,0x00, KEY_DELETE},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,KEY_LEFT_SHIFT,KEY_NUM_LOCK,MACRO1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_RIGHT_SHIFT,KEY_UP,KEY_RIGHT_CTRL},
	{KEY_LEFT_CTRL,KEY_FN,0x00,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,KEY_FN,KEY_FN,KEY_LEFT,KEY_DOWN,KEY_RIGHT}
};
//keyMask_bits:7-press 654-hexatype0 3-press 210-hexatype1
//type: 1-key 2-modifykey 3-mousekey 4-systemkey 5-consumerkey 6-FN 7-macro
uint8_t keyMask[ROWS][COLS] = {
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
uint16_t delayval;
uint8_t r,c,i,FN;
uint8_t delay_after=0;//backswing 后摇
uint8_t delay_before=0;//windup 前摇
void Init_Cols(){
	for ( i=0; i<COLS; i++){
		pinMode(colPins[i],INPUT);
		digitalWrite(colPins[i],HIGH);
	}
}
void Init_Rows(){
	for ( i=0; i<ROWS; i++){
		pinMode(rowPins[i],INPUT);
		digitalWrite(rowPins[i],HIGH);
	}
}
void Open_LED(){
	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],HIGH);
	}
}
void Close_LED(){
	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],LOW);
	}
}
void Init_LED(){
	WS2812Setup();
	WS2812Clear();
	WS2812Send2();
	for ( i=0; i<ledcount; i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],LOW);
	}
	delayval=MaxDelay*8;
}
void Reset_LED(){
	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],LOW);
	}
	RGB_State=RGB_Type;//默认开关状态
	WS2812Clear();
	WS2812Send2();
}
uint8_t r,g,b;
void Update_LED(){
	if((keyboard_buffer.keyboard_leds&(1<<1))==(1<<1)){
		digitalWrite(ledPins[0],HIGH);
		}else{
		digitalWrite(ledPins[0],LOW);
	}
	//////////////full led//////////////////
	if(RGB_State & (1<<5)){}//full led on
	else{}//full led off
	////////////////rgb////////////////
	if(RGB_State & (1<<4)){
		/////////////////rianbow/////////////////////
		if((RGB_State&0x0F)==0x01){
			for(uint8_t i=0;i<WS2812_COUNT;i++){
				if(RGB_Rainbow[i]>=WS2812ColorCount) RGB_Rainbow[i]=0;
				if(delayval==MaxDelay*1) r=pgm_read_byte(Rcolors+RGB_Rainbow[i]);
				if(delayval==MaxDelay*2) g=pgm_read_byte(Gcolors+RGB_Rainbow[i]);
				if(delayval==MaxDelay*3) b=pgm_read_byte(Bcolors+RGB_Rainbow[i]);
				if(delayval==MaxDelay*4){
					WS2812SetRGB(i,r,g,b);
					RGB_Rainbow[i]++;
				}
			}
		}
		/////////////////fix/////////////////////
		else if((RGB_State&0x0F)==0x00){
			if(delayval==MaxDelay*5){
				for(uint8_t i=0;i<WS2812_COUNT;i++){
					WS2812SetRGB(i,RGB_FixColor[i*3],RGB_FixColor[i*3+1],RGB_FixColor[i*3+2]);//default
				}
			}
		}
		/////////////////print led/////////////////////
		else if((RGB_State&0x0F)==0x02){
			uint8_t wcount=WS2812_COUNT-1;
			//ws2812，1.2us一个bit，一个灯28.8us，100灯2.88ms 6灯17.28us。
			for (r = 0; r < ROWS; r++) {
				for (c = 0; c < COLS; c++) {
					if(wcount>=0 ){
						if(ledMask[r][c]){
							WS2812SetRGB(wcount,ledMask[r][c],ledMask[r][c],ledMask[r][c]);
						}
						else{
							WS2812SetRGB(wcount,0,0,0);
						}
						if((keyMask[r][c]&(~0x88))!=0)wcount--;
					}
				}
			}
			if(delayval==MaxDelay*6){
				for (r = 0; r < ROWS; r++) {
					for (c = 0; c < COLS; c++) {
						if(ledMask[r][c]){ledMask[r][c]=ledMask[r][c]>>1;}
					}
				}
			}
		}
		}else{
		/////////////////////closed///////////////////
		WS2812Clear();
	}
	///////////////////clock /////////////////
	//尽可能减少每次循环的时间，将任务错开。
	if(delayval>=MaxDelay*8){
		delayval--;
		WS2812Send2();
		}else{
		if(delayval){
			delayval--;
			}else {
			delayval=MaxDelay;
		}
	}
}
/////////////////////////////////////////////////////////////////////
void QMK_Mode(){
	for (r = 0; r < ROWS; r++) {
		pinMode(rowPins[r],OUTPUT);
		digitalWrite(rowPins[r],LOW);
		for (c = 0; c < COLS; c++) {
			if (digitalRead(colPins[c])) {keyMask[r][c]&= ~0x88;}
			else {keyMask[r][c]|= 0x88;delay_after=_delay_after;ledMask[r][c]=0xFF;}
			if(keyMask[r][c]==0xEE )FN=0x0F;
		}
		Init_Rows();
	}
	releaseAllkeyboardkeys();
	releaseAllmousekeys();
	macrobuffer=0;
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			switch(keyMask[r][c]&FN){
				case 0x90:
				presskey(hexaKeys0[r][c]);
				break;
				case 0xA0:
				pressModifierKeys(hexaKeys0[r][c]);
				break;
				case 0xB0:
				pressmousekey(hexaKeys0[r][c]);
				break;
				case 0xC0:
				presssystemkey(hexaKeys0[r][c]);
				break;
				case 0xD0:
				pressconsumerkey(hexaKeys0[r][c]);
				break;
				case 0xF0:
				pressmacrokey(hexaKeys0[r][c]);
				break;
				case 0x09:
				presskey(hexaKeys1[r][c]);
				break;
				case 0x0A:
				pressModifierKeys(hexaKeys1[r][c]);
				break;
				case 0x0B:
				pressmousekey(hexaKeys1[r][c]);
				break;
				case 0x0C:
				presssystemkey(hexaKeys1[r][c]);
				break;
				case 0x0D:
				pressconsumerkey(hexaKeys1[r][c]);
				break;
				case 0x0F:
				pressmacrokey(hexaKeys1[r][c]);
				break;
			}
		}
	}
	if(!IsBufferClear())FN=0xF0;//Fix FN key state error
	if(usb_macro_send_required())delay_before=_delay_before;
	if(usb_keyboard_send_required())delay_before=_delay_before;
	if(usb_mouse_send_required())delay_before=_delay_before;
	if(delay_after==_delay_after && delay_before==1)
	{usb_macro_send();usb_keyboard_send();usb_mouse_send();}
	if(delay_after==1)
	{usb_macro_send();usb_keyboard_send();usb_mouse_send();}
	if(delay_after>0)delay_after-=1;
	if(delay_before>0)delay_before-=1;
}
int Init_Main(void) {
	CPU_PRESCALE(CPU_16MHz);//16M晶振分频设置
	closeJtag();
	Init_LED();//插电亮灯会掉电，导致hub掉电不识别。所以要提前关灯。
	_delay_ms(500);
	usb_init();
	while (!usb_configured()){_delay_ms(300);}
	//  TCCR0A = 0x00;
	//	TCCR0B =(1<<CS00);
	//	TIMSK0 = (1<<TOIE0);
	////////////////////////////////////////////////
	Init_Cols();
	Init_Rows();
	while (1) {//重启
		EnableRecv=1;
		keyboard_buffer.enable_pressing=1;
		RGB_Type=0x02;//set default rgb
		releaseAllkeyboardkeys();
		releaseAllmousekeys();
		//	ResetMatrixFormEEP();
		Reset_LED();
		FN=0xF0;
		_delay_ms(500);
		usb_send(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
		while (1) {
			eepwrite();
			if(keyboard_buffer.enable_pressing==2){
				break;
			}
			else if(keyboard_buffer.enable_pressing==1){
				QMK_Mode();
				if(delay_before==0) Update_LED();
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