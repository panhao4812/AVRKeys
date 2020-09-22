#include "../functions.h"
#include "../ws2812.h"

#if (defined xd004 )||(defined staryu)
#if defined xd004
//SW d3 d0 c4 b4
//LED d5 d2 b7 b5
//RGB C6
#define ledcount 4
uint8_t colPins[COLS]={19,16,12,4};
uint8_t rowPins[ROWS]={0xFF};
uint8_t ledPins[ledcount]={21,18,7,5};
uint8_t hexaKeys0[ROWS][COLS]={
	{KEY_1,KEY_2,KEY_3,KEY_FN}
};
uint8_t hexaKeys1[ROWS][COLS]={
	{MACRO0,MACRO1,MACRO3,KEY_FN}
};
uint8_t keyMask[ROWS][COLS]={
	{0x17,0x17,0x17,0x66}
};
uint8_t RGB_Rainbow[WS2812_COUNT]={0,170};
////////////////////////////////////////////////////
#elif defined staryu
//SW D0 D1 D2 D3 D4
//LED C2 C7 D5 D6 B0
//RGB C6
#define ledcount 5
uint8_t colPins[COLS]={16,17,18,19,20};
uint8_t rowPins[ROWS]={0xFF};
uint8_t ledPins[ledcount]={10,15,21,22,0};
uint8_t hexaKeys0[ROWS][COLS]={
	{KEY_UP,KEY_FN,KEY_RIGHT,KEY_DOWN,KEY_LEFT}
};
uint8_t hexaKeys1[ROWS][COLS]={
	{MACRO0,KEY_FN,MACRO4,MACRO5,MACRO1}
};
uint8_t keyMask[ROWS][COLS]={
	{0x17,0x66,0x17,0x17,0x17}
};
uint8_t RGB_Rainbow[WS2812_COUNT]={0};
#endif
/////////////////////////////////////////////////////////////////////////continue
uint16_t delayval;//rgb速率
uint8_t r,c,i,FN;
uint8_t delay_after=0;//backswing
uint8_t delay_before=0;//windup
void Init_Cols(){
	for ( i=0; i<COLS; i++){
		pinMode(colPins[i],INPUT);
		digitalWrite(colPins[i],HIGH);
	}
}
void Init_Rows(){
}
void Open_LED(){
	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],LOW);
	}
}
void Close_LED(){
	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],HIGH);
	}
}
void Init_LED(){
	WS2812Setup();
	WS2812Clear();
	WS2812Send2();
	for ( i=0; i<ledcount; i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],HIGH);
	}
	delayval=MaxDelay;
}
void Reset_LED(){
	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],HIGH);
	}
	RGB_State=RGB_Type;
	WS2812Clear();
	WS2812Send2();
}
void Update_LED(){
	//////////////////////////////full led/////////////////////
	if(RGB_State & (1<<5)){
		Open_LED();
	}
	else{
		Close_LED();
	}
	////////////////////////////RGB////////////////////////
	if(delayval>=MaxDelay){
		if(RGB_State & (1<<4)){
			for(uint8_t i=0;i<WS2812_COUNT;i++){
				if((RGB_State&0x0F)==0x01){
					if(RGB_Rainbow[i]>=WS2812ColorCount) RGB_Rainbow[i]=0;
					uint8_t r=pgm_read_byte(Rcolors+RGB_Rainbow[i]);
					uint8_t g=pgm_read_byte(Gcolors+RGB_Rainbow[i]);
					uint8_t b=pgm_read_byte(Bcolors+RGB_Rainbow[i]);
					WS2812SetRGB(i,r,g,b);
					RGB_Rainbow[i]++;
				}
				else if((RGB_State&0x0F)==0x00){
					WS2812SetRGB(i,RGB_FixColor[i*3],RGB_FixColor[i*3+1],RGB_FixColor[i*3+2]);
				}
			}
			}else{
		WS2812Clear();}
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
void QMK_Mode(){
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			if (digitalRead(colPins[c])) {keyMask[r][c]&= ~0x88;}
			else {keyMask[r][c]|= 0x88;delay_after=_delay_after;}
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
int Init_Main(void)
{
	CPU_PRESCALE(CPU_16MHz);//16M晶振分频设置
	#if defined __AVR_ATmega32U4__
	void closeJtag();
	#endif
	Init_LED();//插电亮灯会掉电，导致hub掉电不识别。所以要提前关灯。
	_delay_ms(500);
	usb_init();
	while (!usb_configured()){_delay_ms(300);}
	Init_Cols();
	Init_Rows();
	while (1) {//重启
		EnableRecv=1;
		keyboard_buffer.enable_pressing=1;
		RGB_Type=0x01;///set default on & rainbow
		releaseAllkeyboardkeys();
		releaseAllmousekeys();
		ResetMatrixFormEEP();
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