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
ʹ�÷�ʽ
1����ѡ��USB_KEYBOARD_H����ѡ���������
2��Ȼ������Ŀ�������޸�mcu����
3�����뼴��

������AVR USB 2.0 keyboard������ 
һ���Ǻ��ӵİ��ӣ���������У�һ���������Ȳ�Σ�����ѡ��������
�ƴ����ws2812 3258 ���� 2*3*4LED���ơ�
*/
#include "Functions.h"

int main(){
return initMain();
}