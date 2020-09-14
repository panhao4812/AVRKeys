#ifndef USBDEVICE_H_
#define USBDEVICE_H_

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "usbdrv/usbdrv.h"
#include "usbdrv/usbconfig.h"

#define ENDPOINT0_SIZE		8

#define KEYBOARD_INTERFACE	0
#define KEYBOARD_ENDPOINT	1
#define KEYBOARD_SIZE		8

#define MOUSE_INTERFACE KEYBOARD_INTERFACE+1
#define MOUSE_ENDPOINT	KEYBOARD_ENDPOINT+1
#define MOUSE_SIZE		8

#define RAW_INTERFACE	 MOUSE_INTERFACE+1
#define RAW_EPSIZE  8

extern const PROGMEM uint8_t KeyboardReport[];
extern const PROGMEM uint8_t MouseReport[];
extern const PROGMEM uint8_t  RawReport[];

#define REPORT_ID_MOUSE     1
#define REPORT_ID_SYSTEM    2
#define REPORT_ID_CONSUMER  3
////////////////////macro////////////////////////////
uint8_t macroreport;
uint8_t macrobuffer;
static inline void ClearMacro(){macrobuffer=0;macroreport=0;}
void pressmacrokey(uint8_t key);
uint8_t usb_macro_send_required();
uint8_t usb_macro_send();
///////////////////struct//////////////////////////////
typedef struct {
    uint8_t report_id;
	uint8_t buttons;
	int8_t x;
	int8_t y;
	int8_t v;
	int8_t h;
} __attribute__ ((packed)) report_mouse0_t;
typedef struct {
	uint8_t  report_id;
	uint16_t usage;
} __attribute__ ((packed)) report_extra_t;
typedef struct {
	report_mouse0_t mouse;
	report_extra_t system_keys;
	report_extra_t consumer_keys;
} __attribute__ ((packed)) report_mouse_t;
typedef struct {
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode[6];
}__attribute__ ((packed))  report_keyboard_t;
typedef  union  {
	uint16_t      word[RAW_EPSIZE/2];
	uint8_t       bytes[RAW_EPSIZE];
}report_raw_t;
typedef struct {
	uint8_t mouse_keys;
	int8_t x;
	int8_t y;
	int8_t v;
	int8_t h;
	uint16_t system_keys;
	uint16_t consumer_keys;
	uint8_t mouse_protocol;
	uint8_t Send_Required;
} __attribute__ ((packed)) buffer_mouse_t;
typedef struct {
	uint8_t keyboard_modifier_keys;
	uint8_t keyboard_keys[6];
	uint8_t keyboard_leds;
	uint8_t enable_pressing;
	uint8_t keyboard_idle_config;
	uint8_t keyboard_idle_count;
	uint8_t keyboard_protocol;
	uint8_t Send_Required;
}__attribute__ ((packed))  buffer_keyboard_t;
report_mouse_t print_mouse_report;
report_mouse_t mouse_report;
buffer_mouse_t mouse_buffer;
report_raw_t raw_report_in;
report_raw_t raw_report_out;
report_keyboard_t print_keyboard_report;
report_keyboard_t keyboard_report;
buffer_keyboard_t keyboard_buffer;
//////////////////////////usb////////////////////////////////
void usb_init();
void ClearKeyboard();
void ClearMouse();
void ClearRaw();
void usb_update();
uint8_t usb_keyboard_send();
uint8_t usb_keyboard_send2();
uint8_t usb_mouse_send();
uint8_t usb_keyboard_send_required();
uint8_t usb_mouse_send_required();
void vusb_transfer_keyboard();
void pressModifierKeys(uint8_t key);
uint8_t presskey(uint8_t key);
void pressmousekey(uint8_t key);
void presssystemkey(uint8_t key);
void pressconsumerkey(uint8_t key);
void releaseAllmousekeys();
void releaseAllkeyboardkeys();
uint8_t IsBufferClear();
///////////////SOF//////////////			
uint16_t suspendedTimeCount;
uint8_t suspended;
void init_SOF();
void SOF();
/////////////////ascii////////////////////
extern const  uint8_t  ascii_to_scan_code_table[] PROGMEM;
#endif