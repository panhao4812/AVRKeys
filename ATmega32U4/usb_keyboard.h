#ifndef USB_KEYBOARD_H_
#define USB_KEYBOARD_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

//#define LILILI84
//#define Vem84
//#define CXT64
//#define  xd60
//#define  xd75
//#define  staryu
//#define  xd004
//#define PG60
#define WS64
#if defined(xd60)
#define ROWS  5
#define COLS  14
#define PRODUCT_ID		0xD060
#define WS2812_COUNT	12
#define WS2812_PIN_F    6
#define RGB_EFFECT_COUNT 2
#elif defined(xd75)
#define ROWS  5
#define COLS  15
#define PRODUCT_ID		0xD075
#define WS2812_COUNT	6
#define WS2812_PIN_F    6
#define RGB_EFFECT_COUNT 2
#elif defined(xd004)
#define ROWS  1
#define COLS  4
#define PRODUCT_ID		0xD004
#define WS2812_COUNT	2
#define WS2812_PIN_C    6
#define RGB_EFFECT_COUNT 2
#elif defined(staryu)
#define ROWS  1
#define COLS  5
#define PRODUCT_ID		0xD005
#define WS2812_COUNT	1
#define WS2812_PIN_C    6
#define RGB_EFFECT_COUNT 2
#elif defined(CXT64)
#define ROWS  5
#define COLS  15
#define PRODUCT_ID		0xF060
#define WS2812_COUNT	64
#define WS2812_PIN_D    7
#define RGB_EFFECT_COUNT 3
#elif defined(LILILI84)
#define ROWS  6
#define COLS  16
#define PRODUCT_ID		0xF184
#define WS2812_COUNT	84
#define WS2812_PIN_B    1
#define RGB_EFFECT_COUNT 3
#elif defined(Vem84)
#define ROWS  6
#define COLS  16
#define PRODUCT_ID		0xF284
#define WS2812_COUNT	84
#define WS2812_PIN_C    7
#define RGB_EFFECT_COUNT 3
#elif defined(PG60)
#define ROWS  5
#define COLS  15
#define PRODUCT_ID		0xF160
#define WS2812_COUNT	61
#define WS2812_PIN_D    7
#define RGB_EFFECT_COUNT 3
#elif defined(WS64)
#define ROWS  5
#define COLS  14
#define PRODUCT_ID		0xF164
#define WS2812_COUNT	64
#define WS2812_PIN_F    6
#define RGB_EFFECT_COUNT 3
#endif
///////////////////////////////////////////////
#if defined(__AVR_ATmega32U2__)
#define VENDOR_ID		0x32C2//芯片类型
#define MAX_FLASH (uint16_t)0x8000
#define ADD_FLASH (uint16_t)(0x8000 - 0x0800 - 1) // 32K-2K-1
#define MAX_EEP (uint16_t)0x03FF // (eeprom 1k-1)
#define MAX_DELAY 0x0010
#elif defined(__AVR_ATmega16U2__)
#define VENDOR_ID		0x16C2
#define MAX_FLASH (uint16_t)0x4000
#define ADD_FLASH (uint16_t)(0x4000 - 0x0400 - 1) // 16K-1K-1
#define MAX_EEP (uint16_t)0x01FF // (eeprom 512-1)
#define MAX_DELAY 0x0010
#elif defined(__AVR_ATmega32U4__)
#define VENDOR_ID		0x32C4
#define MAX_FLASH (uint16_t)0x8000
#define ADD_FLASH (uint16_t)(0x8000 - 0x0800 - 1) // 32K-2K-1
#define MAX_EEP (uint16_t)0x03FF // (eeprom 1k-1)
#define MAX_DELAY 0x0010
/////////////////not use///////////////////
#elif defined(__AVR_ATmega8U2__)
#define VENDOR_ID		0x08C2
#define MAX_DELAY 0x0020
#elif defined(__AVR_AT90USB162__)
#define VENDOR_ID		0xC162
#define MAX_DELAY 0x0020
#elif defined(__AVR_AT90USB82__)
#define VENDOR_ID		0xC082
#define MAX_DELAY 0x0020
/*
#elif defined(__AVR_ATMEGA64A__)
#define VENDOR_ID		0x64A0
#if defined _AVR_ATMEGA32A_H_INCLUDED
#define VENDOR_ID		0x32A0
#define USB_CFG_CLOCK_KHZ       16000
#define MAX_EEP (uint16_t)0x0400 //1k
#elif defined _AVR_ATmega644PA_H_
#define VENDOR_ID		0x644A
#define USB_CFG_CLOCK_KHZ       20000
#define MAX_EEP (uint16_t)0x0800 //2k
#elif defined(__AVR_ATMEGA328P__)
#define VENDOR_ID		0x3280
#elif defined(__AVR_ATMEGA8A__)
#define VENDOR_ID		0x08A0
#elif defined(__AVR_ATMEGA48A__)
#define VENDOR_ID		0x48A0
#elif defined(__AVR_ATMEGA88A__)
#define VENDOR_ID		0x88A0
/////////////////////////////////////
#elif defined(__AVR_ATTINY85__)
#define VENDOR_ID       0xD850
*/
#endif
//////////////////////////////////////////////////////
#define SUPPORT_ENDPOINT_HALT //是否立即中断ep

#define ENDPOINT0_SIZE		8

#define KEYBOARD_INTERFACE	0
#define KEYBOARD_ENDPOINT	1
#define KEYBOARD_SIZE		8

#define MOUSE_INTERFACE KEYBOARD_INTERFACE+1
#define MOUSE_ENDPOINT	KEYBOARD_ENDPOINT+1
#define MOUSE_SIZE		8

#define RAW_INTERFACE	 MOUSE_INTERFACE+1
#define RAW_ENDPOINT_IN	 MOUSE_ENDPOINT+1
#define RAW_ENDPOINT_OUT MOUSE_ENDPOINT+2
#define RAW_EPSIZE  8

////////////////////////macro////////////////////////
uint8_t macro_report;
uint8_t macro_buffer;
static inline void clearMacro(){macro_buffer=0;macro_report=0;}
void pressMacroKey(uint8_t key);
uint8_t usbMacroSendRequired();
uint8_t usbMacroSend();
////////////////////struct////////////////////////
typedef struct  {
	uint8_t bLength;
	uint8_t bDescriptorType;
	int16_t wString[];
}__attribute__ ((packed)) usb_string_descriptor_struct;
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
}__attribute__ ((packed))report_raw_t;
typedef union {
	uint16_t    word;
	uint8_t     bytes[2];
}__attribute__ ((packed))usbWord_t;
typedef struct {
	uint8_t mouse_keys;
	int8_t x;
	int8_t y;
	int8_t v;
	int8_t h;
	uint16_t system_keys;
	uint16_t consumer_keys;
	// protocol setting from the host.  We use exactly the same report
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use.
	uint8_t mouse_protocol;
	uint8_t Send_Required;
} __attribute__ ((packed)) buffer_mouse_t;
typedef struct {
	uint8_t keyboard_modifier_keys;
	uint8_t keyboard_keys[6];
	uint8_t keyboard_leds;
	uint8_t enable_pressing;
	// the idle configuration, how often we send the report to the
	// host (ms * 4) even when it hasn't changed
	uint8_t keyboard_idle_config;
	uint8_t keyboard_idle_count;
	// protocol setting from the host.  We use exactly the same report
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use
	uint8_t keyboard_protocol;
	uint8_t Send_Required;
}__attribute__ ((packed))  buffer_keyboard_t;
report_mouse_t mouse_report;
report_mouse_t print_mouse_report;
buffer_mouse_t mouse_buffer;
report_raw_t raw_report_in;
report_raw_t raw_report_out;
report_keyboard_t keyboard_report;
report_keyboard_t print_keyboard_report;
buffer_keyboard_t keyboard_buffer;
//////////////////////////usb/////////////////////////
void usbInit();			// initialize everything
uint8_t usbConfigured();		// is the USB port configured
void releaseAllKeyboardKeys();
uint8_t releaseKey(uint8_t key);
uint8_t pressKey(uint8_t key);
void pressModifierKeys(uint8_t key);
void releaseModifierKeys(uint8_t key);
volatile uint8_t enableReset;//eep change
void clearKeyboard();
void clearMouse();
void clearRaw();
void releaseAllMousekeys();
void releaseMouseKey(uint8_t key);
void pressMouseKey(uint8_t key);
void releaseSystemKey(uint8_t key);
void pressSystemKey(uint8_t key);
void releaseConsumerKey(uint8_t key);
void pressConsumerKey(uint8_t key);
uint8_t usbKeyboardSendRequired();		// initialize everything
uint8_t usbKeyboardSend();
uint8_t usbMouseSendRequired();
uint8_t usbMouseSend();
uint8_t usbRecv(uint8_t endpoint,uint8_t *buffer, uint8_t buffersize,uint8_t timeout);
uint8_t usbSend(uint8_t endpoint,const uint8_t *buffer, uint8_t buffersize,uint8_t timeout);
void eventUSBDeviceStartOfFrame();
void eepWrite();
uint8_t isBufferClear();
////////////////////hardware///////////////////////////
//#define usb_debug_putchar(c)
//#define usb_debug_flush_output()
//supplement for Bluegiga iWRAP HID(not supported by Windows?)

// Everything below this point is only intended for usb_serial.c
// Misc functions to wait for ready and send/receive packets
/*
uint8_t USB_INT_HasOccurred(const uint8_t Interrupt){
switch (Interrupt)
{
case USB_INT_WAKEUPI:
return (UDINT  & (1 << WAKEUPI));
case USB_INT_SUSPI:
return (UDINT  & (1 << SUSPI));
case USB_INT_EORSTI:
return (UDINT  & (1 << EORSTI));
case USB_INT_SOFI:
return (UDINT  & (1 << SOFI));
case USB_INT_RXSTPI:
return (UEINTX & (1 << RXSTPI));
default:
return 0;
}
}
uint8_t USB_INT_IsEnabled(const uint8_t Interrupt)
{
switch (Interrupt)
{
case USB_INT_WAKEUPI:
return (UDIEN  & (1 << WAKEUPE));
case USB_INT_SUSPI:
return (UDIEN  & (1 << SUSPE));
case USB_INT_EORSTI:
return (UDIEN  & (1 << EORSTE));
case USB_INT_SOFI:
return (UDIEN  & (1 << SOFE));
case USB_INT_RXSTPI:
return (UEIENX & (1 << RXSTPE));
default:
return 0;
}
}
void USB_INT_Clear(const uint8_t Interrupt){
switch (Interrupt)
{
case USB_INT_WAKEUPI:
UDINT  &= ~(1 << WAKEUPI);
break;
case USB_INT_SUSPI:
UDINT  &= ~(1 << SUSPI);
break;
case USB_INT_EORSTI:
UDINT  &= ~(1 << EORSTI);
break;
case USB_INT_SOFI:
UDINT  &= ~(1 << SOFI);
break;
case USB_INT_RXSTPI:
UEINTX &= ~(1 << RXSTPI);
break;
default:
return false;
}
}
*/
/*
UDCON寄存器 见手册22.18.1 - - - - RSTCPU LSM RMWKUP DETACH
*RSTCPU：USB Reset CPU bit
*LSM：全速和高速切换
*RMWKUP：remote wake-up
*DETACH ：控制D+ D-电流的开关总闸
*/
/*
UDIEN寄存器 - UPRSME EORSME WAKEUPE EORSTE SOFE - SUSPE 能使
*UPRSME UPRSMI中断 Up stream Resume
*EORSME EORSMI中断 End Of Resume
*WAKEUPE WAKEUPI中断 Wake-up CPU
*EORSTE EORSTI中断 End Of Reset
*SOFE SOFI中断 Start Of Frame
*SUSPE SUSPI中断 Suspend

UDINT寄存器 - UPRSMI EORSMI WAKEUPI EORSTI SOFI - SUSPI 判断flag
*/
static inline void UpStreamResume(void)
{
	UDIEN = (1<<UPRSME);
}
static inline void EndOfResume(void)
{
	UDIEN = (1<<EORSMI);
}
static inline void WakeUpCPU(void)
{
	UDIEN = (1<<WAKEUPE);
}
static inline void EndOfReset(void)
{
	UDIEN = (1<<EORSTE);
}
static inline void StartOfFrame(void)
{
	UDIEN = (1<<SOFE);
}

/*
UEINTX寄存器 FIFOCON NAKINI RWAL NAKOUTI RXSTPI RXOUTI STALLEDI TXINI
*FIFOCON  FIFO Control Bit
*NAKINI NAK IN Received Interrupt Flag
*RWAL Read/Write Allowed Flag
*NAKOUTI NAK OUT Received Interrupt Flag
*RXSTPI Received SETUP Interrupt Flag
*RXOUTI Received OUT Data Interrupt Flag
*STALLEDI Set by hardware to signal that a STALL handshake has been sent, or that a CRC error has been detected in a
OUT isochronous endpoint
*TXINI Transmitter Ready Interrupt Flag

UEIENX寄存器 FLERRE NAKINE - NAKOUTE RXSTPE RXOUTE STALLEDE TXINE 能使
*/
static inline void WaitIN(void)
{
	while (!(UEINTX & (1<<TXINI)));
}
static inline void ClearIN(void)
{
	UEINTX = ~(1<<TXINI);
}
static inline void WaitOUT(void)
{
	while (!(UEINTX & (1<<RXOUTI)));
}
static inline uint8_t WaitForINOrOUT()
{
	while (!(UEINTX & ((1<<TXINI)|(1<<RXOUTI))));
	return (UEINTX & (1<<RXOUTI)) == 0;
}
static inline void ClearOUT(void)
{
	UEINTX = ~(1<<RXOUTI);
}
static inline uint8_t ReceivedSetupInt()
{
	return UEINTX & (1<<RXSTPI);
}
static inline void ClearSetupInt()
{
	UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
}
static inline uint8_t ReadWriteAllowed()
{
	return UEINTX & (1<<RWAL);
}
static inline uint8_t Stalled()
{
	return UEINTX & (1<<STALLEDI);
}
static inline uint8_t FifoFree()
{
	return UEINTX & (1<<FIFOCON);
}
static inline void ReleaseRX()
{
	UEINTX =((1<<NAKINI) | (1<<RWAL) | (1<<RXSTPI)| (1<<STALLEDI)| (1<<TXINI));// 0x6B;
}
static inline void ReleaseTX()
{
	UEINTX = ((1<<RWAL) | (1<<NAKOUTI) | (1<<RXSTPI)| (1<<STALLEDI));//0x3A;//
}

/*
UEDATX寄存器
Set by the software to read/write a byte from/to the endpoint FIFO selected by EPNUM

UECONX寄存器 - - STALLRQ STALLRQC RSTDT - - EPEN
*STALLRQ STALL Request Handshake Bit 停止握手包
*STALLRQC STALL Request Clear Handshake Bit
*RSTDT  Reset Data Toggle Bit
*EPEN Endpoint Enable Bit

UECFG0X寄存器 EPTYPE1:0 - - - - - EPDIR
*EPTYPE Endpoint Type  0：bulk 1：interrupt
*EPDIR Endpoint Direction  1：in 0：out

UECFG1X寄存器 - EPSIZE2:0 EPBK1:0 ALLOC -
*EPBK   Endpoint Bank
*ALLOC allocate the endpoint memory 分配存储空间
*/
static inline uint8_t Recv8()
{
	return UEDATX;
}
static inline void Send8(uint8_t d)
{
	UEDATX = d;
}
static inline void Stall()
{
	UECONX = (1<<STALLRQ) | (1<<EPEN);
}
static inline void SetEP(uint8_t ep)
{
	UENUM = ep;
}
static inline uint8_t FifoByteCount()
{
	return UEBCLX;
	//This field is the MSB of the byte count of the FIFO endpoint
}
static inline uint8_t FrameNumber()
{
	return UDFNUML;
	//These bits are the 8 LSB of the 11-bits Frame Number information
}
static inline void EnableEndpoint(){
	UECONX = 1;
}
////////////////////////////////////////
#define USB_INT_WAKEUPI  2
#define USB_INT_SUSPI    3
#define USB_INT_EORSTI   4
#define USB_INT_SOFI     5
#define USB_INT_RXSTPI   6
#define USB_SERIAL_PRIVATE_INCLUDE
#ifdef USB_SERIAL_PRIVATE_INCLUDE
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define EP_TYPE_CONTROL			0x00
#define EP_TYPE_BULK_IN			0x81
#define EP_TYPE_BULK_OUT		0x80
#define EP_TYPE_INTERRUPT_IN		0xC1
#define EP_TYPE_INTERRUPT_OUT		0xC0
#define EP_TYPE_ISOCHRONOUS_IN		0x41
#define EP_TYPE_ISOCHRONOUS_OUT		0x40

#define EP_SINGLE_BUFFER		0x02
#define EP_DOUBLE_BUFFER		0x06

#define EP_SIZE(s)	((s) == 64 ? 0x30 :	\
((s) == 32 ? 0x20 :	\
((s) == 16 ? 0x10 :	\
0x00)))
//size只能为8 16 32 64

#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

#if (defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || \
defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega16U2__) || \
defined(__AVR_ATmega32U2__))
#   define MAX_ENDPOINT     5
#   define UERST_MASK       0x1E
#else
#   define MAX_ENDPOINT     7
#   define UERST_MASK       0x7E
//0b 01111110 UERST是代表ep是否重置，0-6位分别表示ep0-6 第7位锁死为0 1代表重置，0代表不重置
#endif
/*
UHWCON寄存器 见手册21.13.1  - - - - - - - UVREGE
*UVREGE：USB pad 寄存器开关

USBCON寄存器 见手册21.13.1 USBE - FRZCLK OTGPADE - - - VBUSTE
*USBE  能使 USB controller
*FRZCLK 能使 clock inputs
*OTGPADE 是否由vbus供电
*VBUSTE 能使 VBUS Transition interrupt generation

PLLCSR寄存器 见手册6.11.5 - - - PINDIV - - PLLE PLOCK
*PINDIV 区分8M和16M输入
*PLLE 启动倍频 到48M
*PLOCK 倍频是否启动完成
*/
static inline uint8_t PLLConfigured()
{
	return (PLLCSR & (1<<PLOCK));
}
#if (defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || \
defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega16U2__) || \
defined(__AVR_ATmega32U2__))
#define HW_CONFIG()
#define PLL_CONFIG() (PLLCSR = ((1<<PLLE)|(1<<PLLP0)))
#define USB_CONFIG() (USBCON = (1<<USBE))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif (defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__))
#define HW_CONFIG() (UHWCON = 0x01)
#define PLL_CONFIG() (PLLCSR = 0x12)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif  (defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__))
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x1A)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif (defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__))
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x16)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#endif

// standard control endpoint request types
#define GET_STATUS			0
#define CLEAR_FEATURE			1
#define SET_FEATURE			3
#define SET_ADDRESS			5
#define GET_DESCRIPTOR			6
#define GET_CONFIGURATION		8
#define SET_CONFIGURATION		9
#define GET_INTERFACE			10
#define SET_INTERFACE			11
// HID (human interface device)
#define HID_GET_REPORT			1
#define HID_GET_IDLE			2
#define HID_GET_PROTOCOL		3
#define HID_SET_REPORT			9
#define HID_SET_IDLE			10
#define HID_SET_PROTOCOL		11
// CDC (communication class device)
#define CDC_SET_LINE_CODING		0x20
#define CDC_GET_LINE_CODING		0x21
#define CDC_SET_CONTROL_LINE_STATE	0x22
#endif
#endif