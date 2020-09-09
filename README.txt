关于芯片
---------------------------------------
1、32A 16M、164PA 20M、324PA 20M、644PA 20M这4芯片的引脚都是一样的，所以可以一个板子通用。频率越高越好，实测不存在12M最稳定一说，稳定性只与电源有关。
2、模拟的HID设备是usb1.1协议
3、直接6pin刷上去 hfuse D0 lfuse 0F 千万不能用bface和faceU的bootloader！
4、罕见故障。
刷完固件后，键盘灯正常，识别正常，但是不出字。
原因是pid和vid冲突，系统自带的HID驱动识别为vendor而不是keycode。
重启电脑即可正确识别为keycode。
5、hidraw上传有时候需要敲一下任意键唤醒才会启动。(usb供电不足导致）
6、hub会导致供电不足，先关灯，再启动usb握手。
---------------------------------------

HID通讯
---------------------------------------
hidraw发送格式 
interface 2 endpoint 4
8byte 50ms 无需处理raw回复信息 
如果没有接收响应则会卡死 拔掉usb插头即可
发送数据都是16位数
FF F1 00 00 00 00 00 00 //开始写eep
(uint16)地址 (uint16)data1 (uint16)data2 (uint16)data3
(uint16)地址 (uint16)data1 (uint16)data2 (uint16)data3
(uint16)地址 (uint16)data1 (uint16)data2 (uint16)data3
...
FF F2 00 00 00 00 00 00 //结束写eep
---------------------------------------
eep格式
前10byte是5个16位数：以row=5 col=14为例
 row地址 ，col地址 ，矩阵1地址 ，矩阵2地址 ，矩阵mask地址
// 10    10+5=15   10+5+14=29   10+5+14+70=99  10+5+14+140=169
其它信息都是8位数：
row[5]
col[14]
hexakey0[5,14]
hexakey1[5,14]
keymask[5,14]
RGB_byte 空 空 空 空 空
Print_eeprom
---------------------------------------

vusb设置
---------------------------------------
1=>EP个数不要多于4个，参考mega32u2的ep为5个。
Endpoint interrupt in 只能有两个，如果有第三个即使不汇报也会卡顿。
Endpoint interrupt out可以有一个。不影响发包

2=>USB_CFG_IMPLEMENT_HALT 需要打开。
这个是立刻中断ep

3=>USB_CFG_INTR_POLL_INTERVAL 不能小于0x0A。
并且需要写到汇报符中（汇报符手动填写可能漏了）。

4=>USB_CFG_MAX_BUS_POWER 0x32

5=>USB_COUNT_SOF 设备挂起计数（计时）。
这个如果设置了设备休眠就打开，但是没必要，不影响发包。

6=>usbMsgLen_t usbFunctionSetup(uchar data[8])
Ep0的反馈是比较特殊的，因为已经默认写了一部分了。
参见static inline usbMsgLen_t usbDriverSetup(usbRequest_t *rq)

7=>如果还是有发包问题，关闭远程唤醒。
PROGMEM const char usbDescriptorConfiguration[] = {      9,
    0x02,
  9+9+9+7+9+9+7+9+9+7,0x00,
  0x03,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
0x80,      //0xA0,close remote wake up PC

8=>ws2812的周期不能太短，一般8个灯不小于0x0200，因为ws2812每次send会有一个中断，如果和ep中断出现冲突就会挂起。其它中断同理，包括soft pwm和i2c。
如果ws2812b的个数太多，建议分为两个io来带(在同一个port），一个io不建议超过8个。如果超过了，按倍率延长周期。
---------------------------------------

/* ---------------------------- Hardware Config ---------------------------- */
#define USB_CFG_IOPORTNAME      D
#define USB_CFG_DMINUS_BIT      3
#define USB_CFG_DPLUS_BIT       2
#define USB_CFG_CLOCK_KHZ       20000 //晶振
#define USB_CFG_CHECK_CRC       0  //18M晶振选1
/* ---------------------------- Functional Range ---------------------------- */
#define USB_CFG_HAVE_INTRIN_ENDPOINT    1 //开ep0 ep1
#define USB_CFG_HAVE_INTRIN_ENDPOINT3   1 //开ep3
#define USB_CFG_EP3_NUMBER              2 //ep3的位置
#define USB_CFG_IMPLEMENT_HALT          1 //立即终止ep
#define USB_CFG_SUPPRESS_INTR_CODE      0	//1	//
#define USB_CFG_INTR_POLL_INTERVAL    0x0A //HID设备有最大速限制，usb1.1最快每10ms报一次0.8k内容。
#define USB_CFG_IS_SELF_POWERED         0 //vbus供电选0
#define USB_CFG_MAX_BUS_POWER      0x32 // 0xFA//最大电流设置 最大电流=USB_CFG_MAX_BUS_POWER*2mA  
#define USB_CFG_IMPLEMENT_FN_WRITE      1 //开启向主机上报
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
