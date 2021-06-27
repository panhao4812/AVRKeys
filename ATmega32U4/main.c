/*
* main.c
*
* Created: 2017/6/13 11:06:55
*  Author: Administrator
*/

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11  12  13
 * pin: F0  F1  E6  C7  C6  B6  D4  B1  B0  B5  B4  D7  D6  B3  (Rev.A)
 * pin: F0  F1  E6  C7  C6  B6  D4  B1  B7  B5  B4  D7  D6  B3  (Rev.B)
 * pin: F0  F1  E6  C7  C6  B7  D4  B1  B0  B5  B4  D7  D6  B3  (Rev.CHN)
 * pin: F0  F1  E6  C7  C6  B7  D4  B0  B1  B5  B4  D7  D6  B3  (Rev.CNY)
 */
/*
使用方式
1、首选在USB_KEYBOARD_H里面选择键盘类型
2、然后在项目属性里修改mcu类型
3、编译即可

主流的AVR USB 2.0 keyboard有两种 
一种是焊接的板子，适配多配列，一种是轴座热插拔，尽量选凯华座。
灯大多用ws2812 3258 或者 2*3*4LED方灯。
*/
#include "Functions.h"

int main(){
return initMain();
}