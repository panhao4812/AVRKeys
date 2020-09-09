#include "usbdevice.h"
usbRequest_t* rq ;
uint8_t vusb_idle_rate = 0;

void usb_init()
{
	usbInit();
	/* enforce USB re-enumerate: */
	usbDeviceDisconnect();  /* do this while interrupts are disabled */
	//wdt_reset();
	/* fake USB disconnect for > 250 ms */
	_delay_ms(300);
	usbDeviceConnect();
	sei();
	ClearKeyboard();
	#if MOUSE_ENABLE
	ClearMouse();
	#endif
	#if RAW_ENABLE
	ClearRaw();
	#endif
}
void ClearKeyboard(){
	memset( &keyboard_report, 0,sizeof(keyboard_report));
	memset( &keyboard_buffer, 0,sizeof(keyboard_buffer));
	keyboard_buffer.enable_pressing=1;
}
void ClearMouse(){
	memset(&mouse_report, 0, sizeof(mouse_report));
	memset(&mouse_buffer,0,sizeof(mouse_buffer));
	mouse_report.mouse.report_id= REPORT_ID_MOUSE;
	mouse_report.system_keys.report_id= REPORT_ID_SYSTEM;
	mouse_report.consumer_keys.report_id= REPORT_ID_CONSUMER;
}
void ClearRaw(){
	memset(&raw_report_out, 0,sizeof(raw_report_out));
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	rq = (usbRequest_t *)((void *)data);
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){
		if(rq->bRequest == USBRQ_HID_GET_REPORT){
			usbMsgPtr = (void *)&keyboard_report;
			return sizeof(keyboard_report);
		}
		else if(rq->bRequest == USBRQ_HID_GET_IDLE){
			usbMsgPtr = &vusb_idle_rate;
			return 1;
		}
		else if(rq->bRequest == USBRQ_HID_SET_IDLE){
			vusb_idle_rate = rq->wValue.bytes[1];
		}
		else if(rq->bRequest == USBRQ_HID_SET_REPORT){
			if (rq->wValue.word == 0x0200 && rq->wIndex.word == 0) {
				//rq->wLength.word==0x0001
				keyboard_buffer.keyboard_leds|=(1<<7);//用最高位标注状态灯，赋值后自动还原0
			}
			return USB_NO_MSG;
		}
	}
	return 0;
}
uchar usbFunctionWrite(uchar *data, uchar len)
{
	if ((keyboard_buffer.keyboard_leds&(1<<7))==(1<<7)) {
		keyboard_buffer.keyboard_leds = data[0];
		return 1;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////
