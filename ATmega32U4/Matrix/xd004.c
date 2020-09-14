#include "../functions.h"
#include "../ws2812.h"

#if (defined xd004 )||  (defined staryu )
#if defined xd004
#ifdef xd004
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
uint8_t keymask[ROWS][COLS]={
	{0x17,0x17,0x17,0x66}
};
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
uint8_t keymask[ROWS][COLS]={
	{0x17,0x66,0x17,0x17,0x17}
};
#endif
/////////////////////////////////////////////////////////////////////////continue
uint16_t delayval;//rgb速率
uint8_t r,c,i,FN;
uint8_t delay_after=0;//backswing
uint8_t delay_before=0;//windup
uint8_t ledmacro=0;//记录led状态

void init_cols(){
	for ( i=0; i<COLS; i++){
		pinMode(colPins[i],INPUT);
		digitalWrite(colPins[i],HIGH);
	}
}
void init_rows(){
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
void init_LED(){
	for(i=0;i<COLS;i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],HIGH);
	}
	ledmacro=0;if((RGB_Type&0xF0)==0x10)ledmacro=0x02;
	WS2812Setup();delayval=Maxdelay;
	WS2812Clear();
	WS2812Send();
}
void init_LED(){
	WS2812Setup();
	WS2812Clear();
	WS2812Send2();
	for ( i=0; i<ledcount; i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],HIGH);
	}
	delayval=Maxdelay;
}
void Reset_LED(){
	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],HIGH);
	}
	ledmacro=0;if((RGB_Type&0xF0)==0x10)ledmacro=0x02;
	WS2812Clear();
	WS2812Send2();
}
uint8_t usb_macro_send(){
	ledmacro^=macroreport;
	if(macroreport&MACRO3){
		keyPrintWordEEP(addPrint+6);
		return 1;
	}	
	else if(macroreport&MACRO4){
		keyPrintWordFlash2();
		return 1;
	}
	else if(macroreport&MACRO5){
		keyPrintWordDebug(0);
		return 1;
	}
	else if(macroreport&MACRO6){
		keyPrintWordDebug(1);
		return 1;
	}
	
	return 0;
}
uint16_t cindex[WS2812_COUNT]={0,0};
void LED(){
	//////////////////////////////FULL LRF/////////////////////
	if((ledmacro & (1<<0))==0){Close_LED();}else{ Open_LED();}
	////////////////////////////RGB////////////////////////
	if(delayval>=Maxdelay){
		if(ledmacro & (1<<1)){
			for(uint8_t i=0;i<WS2812_COUNT;i++){
				if((RGB_Type&0x0F)==0x01){
					if(cindex[i]>=WS2812ColorCount) cindex[i]=0;
					uint8_t r=pgm_read_byte(Rcolors+cindex[i]);
					uint8_t g=pgm_read_byte(Gcolors+cindex[i]);
					uint8_t b=pgm_read_byte(Bcolors+cindex[i]);
					WS2812SetRGB(i,r,g,b);
					cindex[i]++;
				}
				else if((RGB_Type&0x0F)==0x00){
					WS2812SetRGB(i,WS2812fix[i*3],WS2812fix[i*3+1],WS2812fix[i*3+2]);
				}
			}
			}else{WS2812Clear();}
			delayval--;
			WS2812Send2();
			}else{if(delayval){delayval--;}else {delayval=Maxdelay;}}
		}
		void QMKMode(){
			for (r = 0; r < ROWS; r++) {
				for (c = 0; c < COLS; c++) {
					if (digitalRead(colPins[c])) {keymask[r][c]&= ~0x88;}
					else {keymask[r][c]|= 0x88;delay_after=_delay_after;}
					if(keymask[r][c]==0xEE )FN=0x0F;
				}
				init_rows();
			}
			releaseAllkeyboardkeys();
			releaseAllmousekeys();
			macrobuffer=0;
			for (r = 0; r < ROWS; r++) {
				for (c = 0; c < COLS; c++) {
					switch(keymask[r][c]&FN){
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
		int init_main(void)
		{
			CPU_PRESCALE(CPU_16MHz);//16M??????????
			#if  (defined(__AVR_ATmega32U4__))
			void closeJtag();
			#endif
			usb_init();
			while (!usb_configured()){_delay_ms(300);}
			init_cols();
			init_rows();
			while (1) {//重启
				EnableRecv=1;
				keyboard_buffer.enable_pressing=1;
				RGB_Type=0x11;///set default on & rainbow
				ResetMatrixFormEEP();
				releaseAllkeyboardkeys();
				releaseAllmousekeys();
				FN=0xF0;
				init_LED();
				_delay_ms(500);
				usb_send(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
				while (1) {
					eepwrite();
					if(keyboard_buffer.enable_pressing==2){
						break;
					}
					else if(keyboard_buffer.enable_pressing==1){
						QMKMode();
						LED();
					}
				}
			}
			return 0;
		}
		#endif