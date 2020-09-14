#ifndef USB_KEYBOARD_H_
#define USB_KEYBOARD_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
//////////////////////////////////////////////////////
#define SUPPORT_ENDPOINT_HALT //�Ƿ������ж�ep

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
///////////////////////////////////////////////////
void usb_init();			// initialize everything
uint8_t usb_configured();		// is the USB port configured
void releaseAllkeyboardkeys();
uint8_t releasekey(uint8_t key);
uint8_t presskey(uint8_t key);
void pressModifierKeys(uint8_t key);
void releaseModifierKeys(uint8_t key);
volatile uint8_t EnableRecv;//eep change
void ClearKeyboard();
void ClearMouse();
void ClearRaw();
void releaseAllmousekeys();
void releasemousekey(uint8_t key);
void pressmousekey(uint8_t key);
void releasesystemkey(uint8_t key);
void presssystemkey(uint8_t key);
void releaseconsumerkey(uint8_t key);
void pressconsumerkey(uint8_t key);
uint8_t usb_keyboard_send_required();		// initialize everything
uint8_t usb_keyboard_send();
uint8_t usb_mouse_send_required();
uint8_t usb_mouse_send();
uint8_t usb_recv(uint8_t endpoint,uint8_t *buffer, uint8_t buffersize,uint8_t timeout);
uint8_t usb_send(uint8_t endpoint,const uint8_t *buffer, uint8_t buffersize,uint8_t timeout);
void EVENT_USB_Device_StartOfFrame();
void keyPrintWordEEP(uint16_t address);
void keyPrintChinese(uint8_t data[5]);
void keyPrintEnglish(uint8_t data);
void keyPrintChar(uint16_t wrapdata);
void eepwrite();
uint8_t IsBufferClear();
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
#define maxEEP (uint16_t)0x03FF // (eeprom 4k-1)
report_raw_t raw_report_in;
report_raw_t raw_report_out;
report_keyboard_t keyboard_report;
report_keyboard_t print_keyboard_report;
buffer_keyboard_t keyboard_buffer;

////////////////////////macro////////////////////////
uint8_t macroreport;
uint8_t macrobuffer;
static inline void ClearMacro(){macrobuffer=0;macroreport=0;}
void pressmacrokey(uint8_t key);
uint8_t usb_macro_send_required();
uint8_t usb_macro_send();
////////////////////hardware///////////////////////////
#define usb_debug_putchar(c)
#define usb_debug_flush_output()
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
UDCON�Ĵ��� ���ֲ�22.18.1 - - - - RSTCPU LSM RMWKUP DETACH
*RSTCPU��USB Reset CPU bit
*LSM��ȫ�ٺ͸����л�
*RMWKUP��remote wake-up
*DETACH ������D+ D-�����Ŀ�����բ
*/
/*
UDIEN�Ĵ��� - UPRSME EORSME WAKEUPE EORSTE SOFE - SUSPE ��ʹ
*UPRSME UPRSMI�ж� Up stream Resume
*EORSME EORSMI�ж� End Of Resume
*WAKEUPE WAKEUPI�ж� Wake-up CPU
*EORSTE EORSTI�ж� End Of Reset
*SOFE SOFI�ж� Start Of Frame
*SUSPE SUSPI�ж� Suspend

UDINT�Ĵ��� - UPRSMI EORSMI WAKEUPI EORSTI SOFI - SUSPI �ж�flag
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
UEINTX�Ĵ��� FIFOCON NAKINI RWAL NAKOUTI RXSTPI RXOUTI STALLEDI TXINI
*FIFOCON  FIFO Control Bit
*NAKINI NAK IN Received Interrupt Flag
*RWAL Read/Write Allowed Flag
*NAKOUTI NAK OUT Received Interrupt Flag
*RXSTPI Received SETUP Interrupt Flag
*RXOUTI Received OUT Data Interrupt Flag
*STALLEDI Set by hardware to signal that a STALL handshake has been sent, or that a CRC error has been detected in a
OUT isochronous endpoint
*TXINI Transmitter Ready Interrupt Flag

UEIENX�Ĵ��� FLERRE NAKINE - NAKOUTE RXSTPE RXOUTE STALLEDE TXINE ��ʹ
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
UEDATX�Ĵ���
Set by the software to read/write a byte from/to the endpoint FIFO selected by EPNUM

UECONX�Ĵ��� - - STALLRQ STALLRQC RSTDT - - EPEN
*STALLRQ STALL Request Handshake Bit ֹͣ���ְ�
*STALLRQC STALL Request Clear Handshake Bit
*RSTDT  Reset Data Toggle Bit
*EPEN Endpoint Enable Bit

UECFG0X�Ĵ��� EPTYPE1:0 - - - - - EPDIR
*EPTYPE Endpoint Type  0��bulk 1��interrupt
*EPDIR Endpoint Direction  1��in 0��out

UECFG1X�Ĵ��� - EPSIZE2:0 EPBK1:0 ALLOC -
*EPBK   Endpoint Bank 
*ALLOC allocate the endpoint memory ����洢�ռ�
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
//sizeֻ��Ϊ8 16 32 64 

#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

#if defined (__AVR_AT90USB162__) || defined (__AVR_AT90USB82__)
#   define MAX_ENDPOINT     5
#   define UERST_MASK       0x1E
#else
#   define MAX_ENDPOINT     7
#   define UERST_MASK       0x7E
//0b 01111110 UERST�Ǵ���ep�Ƿ����ã�0-6λ�ֱ��ʾep0-6 ��7λ����Ϊ0 1�������ã�0��������
#endif
/*
UHWCON�Ĵ��� ���ֲ�21.13.1  - - - - - - - UVREGE
*UVREGE��USB pad �Ĵ�������

USBCON�Ĵ��� ���ֲ�21.13.1 USBE - FRZCLK OTGPADE - - - VBUSTE
*USBE  ��ʹ USB controller
*FRZCLK ��ʹ clock inputs
*OTGPADE �Ƿ���vbus����
*VBUSTE ��ʹ VBUS Transition interrupt generation

PLLCSR�Ĵ��� ���ֲ�6.11.5 - - - PINDIV - - PLLE PLOCK
*PINDIV ����8M��16M����
*PLLE ������Ƶ ��48M
*PLOCK ��Ƶ�Ƿ��������
*/
static inline uint8_t PLL_configured()
{
	return (PLLCSR & (1<<PLOCK));
}
#if defined(__AVR_AT90USB162__)
#define HW_CONFIG()
#define PLL_CONFIG() (PLLCSR = ((1<<PLLE)|(1<<PLLP0)))
#define USB_CONFIG() (USBCON = (1<<USBE))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_ATmega32U4__)
#define HW_CONFIG() (UHWCON = 0x01)
#define PLL_CONFIG() (PLLCSR = 0x12)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB646__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x1A)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB1286__)
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