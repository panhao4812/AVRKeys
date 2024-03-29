/*
 * Keycode.h
 *
 * Created: 2020/9/14 13:18:01
 *  Author: panhao4812
 */ 
#ifndef KEYCODE_H_
#define KEYCODE_H_

#define KEY_CTRL	0x01
#define KEY_SHIFT	0x02
#define KEY_ALT		0x04
#define KEY_GUI		0x08
#define KEY_LEFT_CTRL	0x01
#define KEY_LEFT_SHIFT	0x02
#define KEY_LEFT_ALT	0x04
#define KEY_LEFT_GUI	0x08
#define KEY_RIGHT_CTRL	0x10
#define KEY_RIGHT_SHIFT	0x20
#define KEY_RIGHT_ALT	0x40
#define KEY_RIGHT_GUI	0x80

#define KEY_A		4
#define KEY_B		5
#define KEY_C		6
#define KEY_D		7
#define KEY_E		8
#define KEY_F		9
#define KEY_G		10
#define KEY_H		11
#define KEY_I		12
#define KEY_J		13
#define KEY_K		14
#define KEY_L		15
#define KEY_M		16
#define KEY_N		17
#define KEY_O		18
#define KEY_P		19
#define KEY_Q		20
#define KEY_R		21
#define KEY_S		22
#define KEY_T		23
#define KEY_U		24
#define KEY_V		25
#define KEY_W		26
#define KEY_X		27
#define KEY_Y		28
#define KEY_Z		29
#define KEY_1		30
#define KEY_2		31
#define KEY_3		32
#define KEY_4		33
#define KEY_5		34
#define KEY_6		35
#define KEY_7		36
#define KEY_8		37
#define KEY_9		38
#define KEY_0		39
#define KEY_ENTER	40
#define KEY_ESC		41
#define KEY_BACKSPACE	42
#define KEY_TAB		43
#define KEY_SPACE	44
#define KEY_MINUS	45
#define KEY_EQUAL	46
#define KEY_LEFT_BRACE	47
#define KEY_RIGHT_BRACE	48
#define KEY_BACKSLASH	49
#define KEY_NUMBER	50
#define KEY_SEMICOLON	51
#define KEY_QUOTE	52
#define KEY_TILDE	53
#define KEY_COMMA	54
#define KEY_PERIOD	55
#define KEY_SLASH	56
#define KEY_CAPS_LOCK	57
#define KEY_F1		58
#define KEY_F2		59
#define KEY_F3		60
#define KEY_F4		61
#define KEY_F5		62
#define KEY_F6		63
#define KEY_F7		64
#define KEY_F8		65
#define KEY_F9		66
#define KEY_F10		67
#define KEY_F11		68
#define KEY_F12		69
#define KEY_PRINTSCREEN	70
#define KEY_SCROLL_LOCK	71
#define KEY_PAUSE	72
#define KEY_INSERT	73
#define KEY_HOME	74
#define KEY_PAGE_UP	75
#define KEY_DELETE	76
#define KEY_END		77
#define KEY_PAGE_DOWN	78
#define KEY_RIGHT	79
#define KEY_LEFT	80
#define KEY_DOWN	81
#define KEY_UP		82
#define KEY_NUM_LOCK	83
#define KEYPAD_SLASH	84	// /
#define KEYPAD_ASTERIX	85	// *
#define KEYPAD_MINUS	86
#define KEYPAD_PLUS	87
#define KEYPAD_ENTER	88
#define KEYPAD_1	89
#define KEYPAD_2	90
#define KEYPAD_3	91
#define KEYPAD_4	92
#define KEYPAD_5	93
#define KEYPAD_6	94
#define KEYPAD_7	95
#define KEYPAD_8	96
#define KEYPAD_9	97
#define KEYPAD_0	98
#define KEYPAD_PERIOD	99		//.

#define MOUSE_LEFT	1<<0
#define MOUSE_RIGHT	1<<1
#define MOUSE_MID	1<<2
#define MOUSE_4	    1<<3
#define MOUSE_5	    1<<4

#define KEY_FN 0
#define REPORT_ID_MOUSE     1
#define REPORT_ID_SYSTEM    2
#define REPORT_ID_CONSUMER  3
/* Generic Desktop Page(0x01) - system power control */
#define SYSTEM_POWER_DOWN       0x81
#define SYSTEM_SLEEP            0x82
#define SYSTEM_WAKE_UP          0x83
////////////////////////////////////////////////////////
#define AUDIO_MUTE              0xE2
#define AUDIO_VOL_UP            0xE9
#define AUDIO_VOL_DOWN          0xEA
#define TRANSPORT_RECORD        0xB2
#define TRANSPORT_FAST_FORWARD  0xB3
#define TRANSPORT_REWIND        0xB4
#define TRANSPORT_NEXT_TRACK    0xB5
#define TRANSPORT_PREV_TRACK    0xB6
#define TRANSPORT_STOP          0xB7
#define TRANSPORT_EJECT         0xB8
#define TRANSPORT_STOP_EJECT    0xCC
#define TRANSPORT_PLAY_PAUSE    0xCD
//application launch
#define AL_CC_CONFIG           0x83// 0x0183
#define AL_EMAIL               0x8A// 0x018A
#define AL_CALCULATOR          0x92// 0x0192
#define AL_LOCAL_BROWSER       0x94// 0x0194
#define AL_LOCK                0x9E// 0x019E
//application control
#define AC_SEARCH              0x21// 0x0221
#define AC_HOME                0x23// 0x0223
#define AC_BACK                0x24// 0x0224
#define AC_FORWARD             0x25// 0x0225
#define AC_STOP                0x26// 0x0226
#define AC_REFRESH             0x27// 0x0227
#define AC_BOOKMARKS           0x2A// 0x022A
#define AC_MINIMIZE            0x06// 0x0206

//前4个macro是被预定的，后四个自定义
#define MACRO0 0x01//full led
#define MACRO1 0x02//rgb led
#define MACRO2 0x04//esc ~
#define MACRO3 0x08//print eep
#define MACRO4 0x10
#define MACRO5 0x20
#define MACRO6 0x40
#define MACRO7 0x80


#endif /* KEYCODE_H_ */