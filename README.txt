����оƬ
---------------------------------------
1��32A 16M��164PA 20M��324PA 20M��644PA 20M��4оƬ�����Ŷ���һ���ģ����Կ���һ������ͨ�á�Ƶ��Խ��Խ�ã�ʵ�ⲻ����12M���ȶ�һ˵���ȶ���ֻ���Դ�йء�
2��ģ���HID�豸��usb1.1Э��
3��ֱ��6pinˢ��ȥ hfuse D0 lfuse 0F ǧ������bface��faceU��bootloader��
4���������ϡ�
ˢ��̼��󣬼��̵�������ʶ�����������ǲ����֡�
ԭ����pid��vid��ͻ��ϵͳ�Դ���HID����ʶ��Ϊvendor������keycode��
�������Լ�����ȷʶ��Ϊkeycode��
5��hidraw�ϴ���ʱ����Ҫ��һ����������ѲŻ�������(usb���粻�㵼�£�
6��hub�ᵼ�¹��粻�㣬�ȹصƣ�������usb���֡�
---------------------------------------

HIDͨѶ
---------------------------------------
hidraw���͸�ʽ 
interface 2 endpoint 4
8byte 50ms ���账��raw�ظ���Ϣ 
���û�н�����Ӧ��Ῠ�� �ε�usb��ͷ����
�������ݶ���16λ��
FF F1 00 00 00 00 00 00 //��ʼдeep
(uint16)��ַ (uint16)data1 (uint16)data2 (uint16)data3
(uint16)��ַ (uint16)data1 (uint16)data2 (uint16)data3
(uint16)��ַ (uint16)data1 (uint16)data2 (uint16)data3
...
FF F2 00 00 00 00 00 00 //����дeep
---------------------------------------
eep��ʽ
ǰ10byte��5��16λ������row=5 col=14Ϊ��
 row��ַ ��col��ַ ������1��ַ ������2��ַ ������mask��ַ
// 10    10+5=15   10+5+14=29   10+5+14+70=99  10+5+14+140=169
������Ϣ����8λ����
row[5]
col[14]
hexakey0[5,14]
hexakey1[5,14]
keymask[5,14]
RGB_byte �� �� �� �� ��
Print_eeprom
---------------------------------------

vusb����
---------------------------------------
1=>EP������Ҫ����4�����ο�mega32u2��epΪ5����
Endpoint interrupt in ֻ��������������е�������ʹ���㱨Ҳ�Ῠ�١�
Endpoint interrupt out������һ������Ӱ�췢��

2=>USB_CFG_IMPLEMENT_HALT ��Ҫ�򿪡�
����������ж�ep

3=>USB_CFG_INTR_POLL_INTERVAL ����С��0x0A��
������Ҫд���㱨���У��㱨���ֶ���д����©�ˣ���

4=>USB_CFG_MAX_BUS_POWER 0x32

5=>USB_COUNT_SOF �豸�����������ʱ����
�������������豸���߾ʹ򿪣�����û��Ҫ����Ӱ�췢����

6=>usbMsgLen_t usbFunctionSetup(uchar data[8])
Ep0�ķ����ǱȽ�����ģ���Ϊ�Ѿ�Ĭ��д��һ�����ˡ�
�μ�static inline usbMsgLen_t usbDriverSetup(usbRequest_t *rq)

7=>��������з������⣬�ر�Զ�̻��ѡ�
PROGMEM const char usbDescriptorConfiguration[] = {      9,
    0x02,
  9+9+9+7+9+9+7+9+9+7,0x00,
  0x03,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
0x80,      //0xA0,close remote wake up PC

8=>ws2812�����ڲ���̫�̣�һ��8���Ʋ�С��0x0200����Ϊws2812ÿ��send����һ���жϣ������ep�жϳ��ֳ�ͻ�ͻ���������ж�ͬ������soft pwm��i2c��
���ws2812b�ĸ���̫�࣬�����Ϊ����io����(��ͬһ��port����һ��io�����鳬��8������������ˣ��������ӳ����ڡ�
---------------------------------------

/* ---------------------------- Hardware Config ---------------------------- */
#define USB_CFG_IOPORTNAME      D
#define USB_CFG_DMINUS_BIT      3
#define USB_CFG_DPLUS_BIT       2
#define USB_CFG_CLOCK_KHZ       20000 //����
#define USB_CFG_CHECK_CRC       0  //18M����ѡ1
/* ---------------------------- Functional Range ---------------------------- */
#define USB_CFG_HAVE_INTRIN_ENDPOINT    1 //��ep0 ep1
#define USB_CFG_HAVE_INTRIN_ENDPOINT3   1 //��ep3
#define USB_CFG_EP3_NUMBER              2 //ep3��λ��
#define USB_CFG_IMPLEMENT_HALT          1 //������ֹep
#define USB_CFG_SUPPRESS_INTR_CODE      0	//1	//
#define USB_CFG_INTR_POLL_INTERVAL    0x0A //HID�豸����������ƣ�usb1.1���ÿ10ms��һ��0.8k���ݡ�
#define USB_CFG_IS_SELF_POWERED         0 //vbus����ѡ0
#define USB_CFG_MAX_BUS_POWER      0x32 // 0xFA//���������� ������=USB_CFG_MAX_BUS_POWER*2mA  
#define USB_CFG_IMPLEMENT_FN_WRITE      1 //�����������ϱ�
#define USB_CFG_IMPLEMENT_FN_READ       0
#define USB_CFG_IMPLEMENT_FN_WRITEOUT   1 //use interrupt-out (or bulk out) endpoints.
#define USB_CFG_HAVE_FLOWCONTROL        0
#define USB_CFG_DRIVER_FLASH_PAGE       0
#define USB_CFG_LONG_TRANSFERS          0
#define USB_COUNT_SOF                 0
#define USB_CFG_CHECK_DATA_TOGGLING     0
#define USB_CFG_HAVE_MEASURE_FRAME_LENGTH   0
#define USB_USE_FAST_CRC                0
/* -------------------------- Device Description --------------------------- */
#define VENDOR_ID       0x644A
#define PRODUCT_ID      0x0260
#define DEVICE_VER      0x0001
#define USB_CFG_VENDOR_ID      (VENDOR_ID  & 0xFF), ((VENDOR_ID  >> 8) & 0xFF)
#define USB_CFG_DEVICE_ID      (PRODUCT_ID & 0xFF), ((PRODUCT_ID >> 8) & 0xFF)
#define USB_CFG_DEVICE_VERSION (DEVICE_VER & 0xFF), ((DEVICE_VER >> 8) & 0xFF)
#define USB_CFG_VENDOR_NAME     'z','i','a','n'
#define USB_CFG_VENDOR_NAME_LEN 4
#define USB_CFG_DEVICE_NAME    'v','u','s','b','_','k','e','y','_','2','0','M'
#define USB_CFG_DEVICE_NAME_LEN 12
#define USB_CFG_DEVICE_CLASS        0    
#define USB_CFG_DEVICE_SUBCLASS     0
#define USB_CFG_INTERFACE_CLASS     0x03   
#define USB_CFG_INTERFACE_SUBCLASS  0x01
#define USB_CFG_INTERFACE_PROTOCOL  0x01
#define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    0
/* ------------------- Fine Control over USB Descriptors ------------------- */
#define USB_CFG_DESCR_PROPS_DEVICE                  0
#define USB_CFG_DESCR_PROPS_CONFIGURATION           0
#define USB_CFG_DESCR_PROPS_STRINGS                 0
#define USB_CFG_DESCR_PROPS_STRING_0                0
#define USB_CFG_DESCR_PROPS_STRING_VENDOR           0
#define USB_CFG_DESCR_PROPS_STRING_PRODUCT          0
#define USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER    0
#define USB_CFG_DESCR_PROPS_HID                     0
#define USB_CFG_DESCR_PROPS_HID_REPORT              USB_PROP_IS_DYNAMIC
#define USB_CFG_DESCR_PROPS_UNKNOWN                 0
#if USB_COUNT_SOF
#define USB_INTR_CFG_SET        ((1 << ISC11) | (0 << ISC10))
#define USB_INTR_ENABLE_BIT     INT1
#define USB_INTR_PENDING_BIT    INTF1
#define USB_INTR_VECTOR         INT1_vect
#endif
#endif 
-----------------------------------------------------------------
