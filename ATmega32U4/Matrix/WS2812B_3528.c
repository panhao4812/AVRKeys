#include "../Functions.h"
#include "../ws2812.h"
#if (defined CXT64 )||(defined CXT84 )||(defined Vem84 )||(defined LILILI84 )
#if defined CXT64
/*
#define MATRIX_ROW_PINS { C7, C6, B6, B5, B4 }5
#define MATRIX_COL_PINS { F7, F6, F5, F4, F1, F0, E6, B0, B1, B2, B3, D0, D1, D2, D3 }15
uint8_t LED_caps=D5;
#define BACKLIGHT_PIN B7
#define RGB_DI_PIN D7
#define RGBLED_NUM 64
键盘的灯比较多，所以启动需要等一段时间电容蓄电，不然会掉电，可以开灯加速蓄电。
掉电包括不能开rgb，无法用上位机程序上传matrix到键盘
*/
#define LED_COUNT 1
uint8_t row_pins[ROWS]={10,9,15,14,13};
uint8_t col_pins[COLS]={16,17,18,19,20,21,24,0,1,2,3,5,6,7,8};
//                     1  2  3  4  5  6  7 8 9 10 11 12 13 14
uint8_t led_pins[LED_COUNT]={23};
//颜色靠上位机程序生成
uint16_t rgb_rainbow[WS2812_COUNT]={
	14,31,47,64,81,98,115,132,149,166,183,200,217,234,
	14,39,56,73,90,107,124,141,158,174,191,208,225,242,
	14,43,60,77,94,111,128,145,162,179,196,213,230,
	14,47,64,81,98,115,132,149,166,183,200,217,234,251,
	14,35,56,77,183,200,217,234,251
};
uint8_t rgb_fixcolor[(WS2812_COUNT*3)]={
	237,32,50,222,13,91,198,0,118,103,0,120,77,29,136,41,70,160,19,134,196,2,191,219,17,179,98,49,182,48,90,190,10,212,234,31,234,197,26,247,151,19,
	237,32,50,211,1,116,138,0,119,96,8,125,54,55,151,36,83,167,3,187,226,10,184,153,24,179,82,74,185,14,147,211,20,222,232,31,245,165,21,245,110,10,
	237,32,50,210,0,118,114,0,120,87,18,130,46,64,157,28,105,180,1,192,229,14,181,121,35,180,67,79,186,8,188,226,27,227,215,29,247,159,20,
	237,32,50,198,0,118,103,0,120,77,29,136,41,70,160,19,134,196,2,191,219,17,179,98,49,182,48,90,190,10,212,234,31,234,197,26,247,151,19,243,63,1,
	237,32,50,216,6,106,138,0,119,87,18,130,90,190,10,212,234,31,234,197,26,247,151,19,243,63,1
};
uint8_t rgb_pos[WS2812_COUNT]={
	63,62,61,60,59,58,57,56,55,54,53,52,51,50,
	49,48,47,46,45,44,43,42,41,40,39,38,37,36,
	35,34,33,32,31,30,29,28,27,26,25,24,23,
	22,21,20,19,18,17,16,15,14,13,12,11,10,9,
	8,7,6,5,4,3,2,1,0
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
#elif defined Vem84
//#define MATRIX_ROW_PINS { B0, B1, B2, B3, D0, D1}6
//#define MATRIX_COL_PINS { F7, F6, F5, F4, F1, F0, D2, D3, D5, D4, D6, D7, B4, B5, B6, C6 }16
//#define RGB_DI_PIN C7
//#define RGBLED_NUM 84

uint8_t row_pins[ROWS]={0,1,2,3,5,6};
uint8_t col_pins[COLS]={16,17,18,19,20,21,7,8,23,22,11,12,13,14,15,9};
#define LED_COUNT 0
uint8_t led_pins[LED_COUNT]={};
uint8_t hexa_keys0[ROWS][COLS] = {
	{KEY_ESC,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,KEY_PRINTSCREEN,KEY_PAUSE,KEY_DELETE},
	{KEY_TILDE,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,0x00,KEY_BACKSPACE,KEY_HOME},
	{KEY_TAB,0x00,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH,KEY_PAGE_UP},
	{KEY_CAPS_LOCK,0x00,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,KEY_ENTER,0x00,KEY_PAGE_DOWN},
	{0x00,KEY_LEFT_SHIFT,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,0x00,KEY_RIGHT_SHIFT,KEY_UP,KEY_END},
	{KEY_LEFT_CTRL,KEY_LEFT_GUI,0x00,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_FN,KEY_RIGHT_CTRL,KEY_LEFT,KEY_DOWN,KEY_RIGHT}
};
uint8_t hexa_keys1[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
uint8_t key_mask[ROWS][COLS] = {
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x10,0x10},
	{0x10,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x10},
	{0x00,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x20,0x10,0x10},
	{0x20,0x20,0x00,0x20,0x00,0x00,0x10,0x00,0x00,0x00,0x20,0x66,0x20,0x10,0x10,0x10},
	// 0   1    2    3    4    5    6    7    8    9    10    11   12   13   14  15
};
uint8_t ledMask[ROWS][COLS] = {
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x80,0x80},
	{0x80,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},
	{0x80,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x80},
	{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x80,0x80,0x80},
	{0x80,0x80,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80},
	// 0   1    2    3    4    5    6    7    8    9    10    11   12   13   14  15
};
uint16_t rgb_rainbow[WS2812_COUNT]={
	13,29,44,60,76,92,108,124,140,156,171,187,203,219,235,251,
	13,29,44,60,76,92,108,124,140,156,171,187,203,219,251,
	13,37,52,68,84,100,116,132,148,164,179,195,211,227,251,
	13,41,56,72,88,104,120,136,152,168,183,199,215,251,
	13,48,64,80,96,112,128,144,160,175,191,207,235,251,
	13,33,52,72,171,187,203,219,235,251,
};
uint8_t rgb_fixcolor[(WS2812_COUNT*3)]={
	238,34,46,226,16,83,208,0,118,114,0,120,90,15,129,50,60,154,34,88,170,3,187,226,9,
	185,162,20,178,87,67,184,24,114,199,14,219,237,32,237,187,24,247,148,18,243,63,1,238,
	34,46,226,16,83,208,0,118,114,0,120,90,15,129,50,60,154,34,88,170,3,187,226,9,185,162,
	20,178,87,67,184,24,114,199,14,219,237,32,237,187,24,243,63,1,238,34,46,213,3,111,167,
	0,119,102,1,121,69,38,142,39,71,161,17,141,200,2,191,219,16,179,103,42,181,58,79,186,8,
	181,223,25,225,223,30,246,161,20,243,63,1,238,34,46,210,0,118,138,0,119,97,7,124,59,50,
	148,38,74,162,9,168,215,5,188,194,18,178,91,57,183,38,90,190,10,207,232,30,231,206,27,
	243,63,1,238,34,46,193,0,118,103,0,120,80,26,135,43,67,158,26,112,184,1,192,229,13,182,
	129,29,179,76,76,186,12,147,211,20,221,234,32,247,148,18,243,63,1,238,34,46,219,9,99,
	167,0,119,97,7,124,67,184,24,114,199,14,219,237,32,237,187,24,247,148,18,243,63,1
};
//位置从空格开始，先占位
uint8_t rgb_pos[WS2812_COUNT]={
	26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,
	27,28,29,30,31,32,33,34,35,36,37,38,39,40,10,
	54,53,52,51,50,49,48,47,46,45,44,43,42,41,9,
	55,56,57,58,59,60,61,62,63,64,65,66,67,8,
	80,79,78,77,76,75,74,73,72,71,70,69,68,7,
	81,82,83,0,1,2,3,4,5,6
};
//////////////////////////////////////////////////////////////////////////////

#elif defined LILILI84
//#define MATRIX_ROW_PINS {  B7, B6, B5, B4, B3, B2}6
//#define MATRIX_COL_PINS { f0, F1,f4, f5, f6, f7, C7, C6, D7, D6, D5, D4, D3, D2, D1, D0 }16
//#define RGB_DI_PIN B1
//#define RGBLED_NUM 84
uint8_t row_pins[ROWS]={4,15,14,13,3,2};
uint8_t col_pins[COLS]={21,20,19,18,17,16,10,9,12,11,23,22,8,7,6,5};
#define LED_COUNT 0
uint8_t led_pins[LED_COUNT]={};

uint8_t hexa_keys0[ROWS][COLS] = {
	{KEY_ESC,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,KEY_PRINTSCREEN,KEY_PAUSE,KEY_DELETE},
	{KEY_TILDE,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,0x00,KEY_BACKSPACE,KEY_HOME},
	{KEY_TAB,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,0x00,KEY_BACKSLASH,KEY_PAGE_UP},
	{KEY_CAPS_LOCK,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,0x00,0x00,KEY_ENTER,KEY_PAGE_DOWN},
	{KEY_LEFT_SHIFT,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,0x00,0x00,KEY_RIGHT_SHIFT,KEY_UP,KEY_END},
	{KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_FN,KEY_RIGHT_CTRL,0x00,KEY_LEFT,KEY_DOWN,KEY_RIGHT}
};
uint8_t hexa_keys1[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};
uint8_t key_mask[ROWS][COLS] = {
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x10,0x10},
	{0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x20,0x10,0x10},
	{0x20,0x20,0x20,0x00,0x00,0x10,0x00,0x00,0x00,0x20,0x60,0x20,0x00,0x10,0x10,0x10},
	// 0   1    2    3    4    5    6    7    8    9    10    11   12   13   14  15
};
uint8_t ledMask[ROWS][COLS] = {
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x80,0x80},
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x80,0x80},
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x80,0x80},
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80},
	{0x80,0x80,0x80,0x00,0x00,0x80,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x80},
};
uint16_t rgb_rainbow[WS2812_COUNT]={
	13,29,44,60,76,92,108,124,140,156,171,187,203,219,235,251,
	13,29,44,60,76,92,108,124,140,156,171,187,203,219,251,
	13,37,52,68,84,100,116,132,148,164,179,195,211,227,251,
	13,41,56,72,88,104,120,136,152,168,183,199,215,251,
	13,48,64,80,96,112,128,144,160,175,191,207,235,251,
	13,33,52,72,171,187,203,219,235,251,
};
uint8_t rgb_fixcolor[(WS2812_COUNT*3)]={
	238,34,46,226,16,83,208,0,118,114,0,120,90,15,129,50,60,154,34,88,170,3,187,226,9,
	185,162,20,178,87,67,184,24,114,199,14,219,237,32,237,187,24,247,148,18,243,63,1,238,
	34,46,226,16,83,208,0,118,114,0,120,90,15,129,50,60,154,34,88,170,3,187,226,9,185,162,
	20,178,87,67,184,24,114,199,14,219,237,32,237,187,24,243,63,1,238,34,46,213,3,111,167,
	0,119,102,1,121,69,38,142,39,71,161,17,141,200,2,191,219,16,179,103,42,181,58,79,186,8,
	181,223,25,225,223,30,246,161,20,243,63,1,238,34,46,210,0,118,138,0,119,97,7,124,59,50,
	148,38,74,162,9,168,215,5,188,194,18,178,91,57,183,38,90,190,10,207,232,30,231,206,27,
	243,63,1,238,34,46,193,0,118,103,0,120,80,26,135,43,67,158,26,112,184,1,192,229,13,182,
	129,29,179,76,76,186,12,147,211,20,221,234,32,247,148,18,243,63,1,238,34,46,219,9,99,
	167,0,119,97,7,124,67,184,24,114,199,14,219,237,32,237,187,24,247,148,18,243,63,1
};
uint8_t rgb_pos[WS2812_COUNT]={
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
	31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
	46,47,48,49,50,51,52,53,54,55,56,57,58,59,
	60,61,62,63,64,65,66,67,68,69,70,71,72,73,
	74,75,76,77,78,79,80,81,82,83
};
#endif
//////////////////////////////////////////////////////////////////////
uint16_t delay_val;
uint8_t r,c,i,FN;
uint8_t delay_after=0;//backswing 后摇
uint8_t delay_before=0;//windup 前摇
uint8_t r,g,b;
#define MAX_DELAY_1 MAX_DELAY*1
#define MAX_DELAY_2 MAX_DELAY*2
#define MAX_DELAY_3 MAX_DELAY*3
#define MAX_DELAY_4 MAX_DELAY*4
#define MAX_DELAY_5 MAX_DELAY*5
#define MAX_DELAY_6 MAX_DELAY*6
#define MAX_DELAY_7 MAX_DELAY*7
///////////////////////////////
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
		digitalWrite(led_pins[i],LOW);
	}
}
void closeLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(led_pins[i],HIGH);
	}
}
void initLED(){
	ws2812Setup();
	ws2812Clear();
	ws2812Send2();
	for ( i=0; i<LED_COUNT; i++){
		pinMode(led_pins[i],OUTPUT);
		digitalWrite(led_pins[i],HIGH);
	}
	delay_val=MAX_DELAY*8;
}
void resetLED(){
	for ( i=0; i<LED_COUNT; i++){
		digitalWrite(led_pins[i],HIGH);
	}
	rgb_state=rgb_type;//默认开关状态
	ws2812Clear();
	ws2812Send2();
}
void updateLED(){
	/*
	for ( i=0; i<LED_COUNT; i++){
	if((keyboard_buffer.keyboard_leds&(1<<i))==(1<<i)){
	digitalWrite(led_pins[i],HIGH);}
	else{
	digitalWrite(led_pins[i],LOW);}
	}
	*/
	for ( i=0; i<LED_COUNT; i++){
		//没有灯的需要for循环排除
		if((keyboard_buffer.keyboard_leds&(1<<1))==(1<<1)){
			digitalWrite(led_pins[i],LOW);
			}else{
			digitalWrite(led_pins[i],HIGH);
		}
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
				if(delay_val==MAX_DELAY_4){r=pgm_read_byte(Rcolors+rgb_rainbow[i]);ws2812SetR(rgb_pos[i],r);}
				if(delay_val==MAX_DELAY_3){g=pgm_read_byte(Gcolors+rgb_rainbow[i]);ws2812SetG(rgb_pos[i],g);}
				if(delay_val==MAX_DELAY_2){b=pgm_read_byte(Bcolors+rgb_rainbow[i]);ws2812SetB(rgb_pos[i],b);}
				if(delay_val==MAX_DELAY_1){rgb_rainbow[i]++;}
			}
		}
		/////////////////fix/////////////////////
		else if((rgb_state&0x0F)==0x00){
			if(delay_val==MAX_DELAY_5){
				for(uint8_t i=0;i<WS2812_COUNT;i++){
					ws2812SetRGB(rgb_pos[i],rgb_fixcolor[i*3],rgb_fixcolor[i*3+1],rgb_fixcolor[i*3+2]);//default
				}
			}
		}
		/////////////////print led/////////////////////
		else if((rgb_state&0x0F)==0x02){
			uint8_t wcount=0;
			//ws2812，1.2us一个bit，一个灯28.8us，100灯2.88ms 6灯17.28us。
			for (r = 0; r < ROWS; r++) {
				for (c = 0; c < COLS; c++) {
					if(wcount<WS2812_COUNT ){
						if(ledMask[r][c]){
							ws2812SetRGB(rgb_pos[wcount],ledMask[r][c],ledMask[r][c],ledMask[r][c]);
						}
						else{
							ws2812SetRGB(rgb_pos[wcount],0,0,0);
						}
						if((key_mask[r][c]&(~0x88))!=0)wcount++;//靠key_mask识别并跳过空键位
					}
				}
			}
			//*
			if(delay_val==MAX_DELAY_6){
				for (r = 0; r < ROWS; r++) {
					for (c = 0; c < COLS; c++) {
						if(ledMask[r][c]){ledMask[r][c]=ledMask[r][c]>>1;}
					}
				}
			}
			//*/
		}
		}else{
		/////////////////////closed///////////////////
		ws2812Clear();
	}
	///////////////////clock /////////////////
	//尽可能减少每次循环的时间，将任务错开。
	if(delay_val==MAX_DELAY_7){
		delay_val--;
		ws2812Send2();
		}else{
		if(delay_val){
			delay_val--;
			}else {
			delay_val=MAX_DELAY_7;
		}
	}
}
/////////////////////////////////////////////////////////////////////
void qmkMode(){
	for (r = 0; r < ROWS; r++) {
		//	initCols();
		pinMode(row_pins[r],OUTPUT);
		digitalWrite(row_pins[r],LOW);
		//串键问题，如果没有delay_us会导致col1或者col2串键，不一定每个板子都会串键，不串键可以取消掉delay_us
		//_delay_us(1);
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
		enableReset=1;
		keyboard_buffer.enable_pressing=1;
		rgb_type=0x02;//set default rgb
		releaseAllKeyboardKeys();
		releaseAllMousekeys();
		resetMatrixFormEEP();
		resetLED();
		FN=0xF0;
		_delay_ms(300);
		usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
		while (1) {
			if(delay_before==0&&delay_before==0)eepWrite();
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
/*
ISR(TIMER0_OVF_vect)
{

}
*/