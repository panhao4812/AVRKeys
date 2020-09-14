#include "ws2812.h"

#include <avr/interrupt.h>
void WS2812Setup()
{
	WS2812_DDR |= WS2812_MASK;
}

void WS2812Clear()
{
	for(int i = 0; i < WS2812_COUNT*3; i++) WS2812Buffer[i] = 0;
}

void WS2812SetHSV(uint16_t led, uint16_t hue, uint16_t saturation, uint16_t value)
{
	if(hue < 1536 && saturation < 256 && value < 256)
	{
		uint8_t red=0x00, green=0x00, blue=0x00;
		uint8_t min, max, inc, dec, hquot, hrem;
		
		if(saturation == 0)
		{
			WS2812SetRGB(led, value, value, value);
			return;
		}
		
		hquot = hue / 256;
		hrem = hue % 256;
		
		max = value;
		min = (value * (255 - saturation)) / 255;
		inc = (value * ((saturation * hrem) / 255)) / 255;
		dec = (value * ((saturation * (255-hrem)) / 255)) / 255;
		
		
		switch (hquot)
		{
			case 0:
			red = max;
			green = inc;
			blue = min;
			break;
			case 1:
			red = dec;
			green = max;
			blue = min;
			break;
			case 2:
			red = min;
			green = max;
			blue = inc;
			break;
			case 3:
			red = min;
			green = dec;
			blue = max;
			break;
			case 4:
			red = inc;
			green = min;
			blue = max;
			break;
			case 5:
			red = max;
			green = min;
			blue = dec;
			break;
		}
		WS2812SetRGB(led, red, green, blue);
	}
}

void WS2812SetRGB(uint16_t led, uint8_t red, uint8_t green, uint8_t blue)
{
	WS2812Buffer[led*3] = green / WS2812_SAVE;
	WS2812Buffer[1+led*3] = red / WS2812_SAVE;
	WS2812Buffer[2+led*3] = blue / WS2812_SAVE;
}

void WS2812Send()
{cli();
	for(uint16_t c = 0; c < (WS2812_COUNT * 3); c++)
	{
		for(uint8_t b = 8; b; b--)
		{
			
			if(WS2812Buffer[c] & (1<<b))
			{
				WS2812_PORT |= WS2812_MASK;
				nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
				WS2812_PORT &= ~WS2812_MASK;
				
			}
			else
			{
				WS2812_PORT |= WS2812_MASK;
				nop();
				WS2812_PORT &= ~WS2812_MASK;
				nop();nop();nop();nop();nop();nop();nop();nop();
			}
		}
	}
	sei();
}
void WS2812Send2()
{
	cli();
	uint8_t masklo = ~(WS2812_MASK) & WS2812_PORT;
	uint8_t maskhi = WS2812_MASK | WS2812_PORT;
	for(uint16_t c = 0; c < (WS2812_COUNT * 3); c++)
	{
		ledcontrol_led_sendbyte(WS2812Buffer[c], masklo, maskhi);
	}
	sei();
}
const  uint8_t Rcolors [WS2812ColorCount] PROGMEM=
{
	243,243,243,243,243,243,243,243,243,242,242,242,242,242,242,242,241,241,241,241,241,240,240,240,240,
	240,239,239,239,239,239,238,238,238,238,238,237,237,237,237,237,237,237,236,236,236,236,236,236,236,
	236,236,236,236,236,236,235,235,235,235,234,234,233,233,232,232,231,230,230,229,228,228,227,226,225,
	224,224,223,222,221,220,220,219,218,217,217,216,215,215,214,214,213,213,212,212,211,211,211,210,210,
	210,210,210,210,210,209,209,208,207,206,204,203,201,199,197,195,192,190,187,185,182,179,176,173,169,
	166,163,160,156,153,150,147,144,140,137,134,131,128,126,123,121,118,116,114,112,110,109,107,106,105,
	104,104,103,103,103,103,103,102,102,101,101,100,99,98,97,95,94,93,91,90,88,86,85,83,81,
	79,77,75,73,71,69,67,66,64,62,60,58,56,55,53,51,50,48,47,46,45,44,43,42,41,
	40,40,39,39,39,39,39,39,39,38,38,38,37,37,36,35,35,34,33,32,31,30,29,28,27,
	26,25,24,23,22,20,19,18,17,16,15,14,13,11,10,9,8,8,7,6,5,4,4,3,3,
	2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5,5,6,
	6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,15,16,16,16,17,17,
	17,17,18,18,18,18,18,18,18,18,18,19,19,20,21,21,22,23,24,26,27,28,30,31,33,
	34,36,38,40,41,43,45,47,49,51,53,54,56,58,60,62,63,65,66,68,69,71,72,73,74,
	75,76,77,77,78,78,79,79,79,79,79,80,81,82,83,85,87,89,91,93,96,99,102,105,109,
	113,116,120,124,128,132,137,141,145,150,154,158,162,167,171,175,179,183,186,190,194,197,200,203,206,
	208,210,212,214,216,217,218,219,220,220,220,220,220,220,220,221,221,221,222,222,223,223,224,224,225,
	226,226,227,228,228,229,230,231,232,233,233,234,235,236,237,237,238,239,240,240,241,242,242,243,244,
	244,245,245,245,246,246,246,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,246,246,
	246,246,246,246,246,246,246,246,245,245,245,245,245,245,245,245,244,244,244,244,244,244,244,244,244,
	244,243,243,243,243,243,243,243,243,243,243,243
};
const  uint8_t Gcolors[WS2812ColorCount] PROGMEM=
{
	57,57,57,57,57,56,56,56,55,55,54,54,53,52,52,51,50,50,49,48,47,46,45,44,43,
	43,42,41,40,39,38,37,36,35,35,34,33,32,32,31,30,30,29,29,28,28,28,27,27,27,
	27,27,27,27,27,27,26,26,26,25,25,25,24,24,23,22,22,21,20,20,19,18,17,17,16,
	15,14,13,13,12,11,10,9,8,8,7,6,6,5,4,4,3,3,2,2,1,1,1,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,1,2,3,4,5,6,7,8,10,11,13,15,17,19,21,23,25,
	27,29,31,33,36,38,40,42,44,46,49,51,53,55,56,58,60,61,63,64,66,67,68,69,70,
	70,71,71,72,72,72,72,73,73,74,75,76,78,79,81,83,86,88,91,93,96,99,102,106,109,
	112,116,120,123,127,131,134,138,142,145,149,152,156,159,162,165,168,171,174,176,179,181,183,185,187,
	188,189,190,191,192,192,192,192,192,192,192,192,192,191,191,191,191,190,190,190,190,189,189,189,188,
	188,187,187,187,186,186,185,185,184,184,184,183,183,182,182,182,181,181,180,180,180,180,179,179,179,
	179,179,178,178,178,178,178,178,178,178,178,178,178,178,178,178,179,179,179,179,179,179,180,180,180,
	180,180,181,181,181,181,182,182,182,182,183,183,183,183,183,184,184,184,184,185,185,185,185,185,185,
	185,186,186,186,186,186,186,186,186,186,186,186,187,187,187,188,189,189,190,191,192,193,194,196,197,
	198,199,201,202,204,205,207,208,210,212,213,215,216,218,219,221,222,224,225,226,227,229,230,231,232,
	233,234,234,235,236,236,236,237,237,237,237,237,237,236,236,235,234,233,232,231,229,228,226,225,223,
	221,219,217,215,212,210,208,206,203,201,198,196,194,191,189,187,184,182,180,178,176,174,172,170,169,
	167,166,165,163,162,161,161,160,160,159,159,159,159,159,158,157,156,155,154,153,151,149,147,145,143,
	141,138,136,133,130,128,125,122,119,115,112,109,106,103,100,97,94,91,88,85,82,80,77,75,72,
	70,68,66,65,63,62,60,59,59,58,57,57
};
const  uint8_t Bcolors[WS2812ColorCount] PROGMEM=
{
	0,0,0,1,1,1,2,3,4,4,5,7,8,9,10,12,13,15,17,18,20,22,23,25,27,
	29,31,33,34,36,38,40,41,43,45,46,48,49,51,52,53,54,55,56,57,58,58,59,59,60,
	60,60,60,60,60,61,61,62,62,63,64,65,66,67,68,70,71,73,74,76,77,79,81,82,84,
	86,88,89,91,93,95,96,98,100,101,103,105,106,107,109,110,111,112,113,114,115,116,116,117,117,
	118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,119,119,119,119,119,
	119,119,119,119,119,119,119,119,119,119,119,119,120,120,120,120,120,120,120,120,120,120,120,120,120,
	120,120,120,120,120,120,120,120,121,121,122,122,123,123,124,125,126,127,127,128,130,131,132,133,134,
	135,137,138,139,140,142,143,144,145,146,148,149,150,151,152,153,154,155,156,157,157,158,159,159,160,
	160,160,161,161,161,161,161,161,162,162,163,163,164,165,166,167,169,170,172,173,175,176,178,180,182,
	184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,217,219,220,222,223,224,225,226,
	227,227,228,228,229,229,229,229,228,228,227,226,224,223,221,219,216,214,211,208,205,202,198,194,191,
	187,183,179,175,171,166,162,158,154,149,145,141,137,133,129,125,122,118,115,112,109,106,103,101,99,
	97,95,94,92,91,91,90,90,90,90,89,89,88,87,86,85,84,83,81,80,78,76,74,72,70,
	68,66,63,61,58,56,54,51,48,46,43,41,39,36,34,31,29,27,25,23,21,19,18,16,15,
	13,12,11,10,9,9,8,8,8,8,8,8,8,8,9,9,9,10,10,10,11,11,12,13,13,
	14,14,15,16,16,17,18,19,19,20,21,21,22,23,24,24,25,26,26,27,27,28,29,29,30,
	30,30,31,31,31,32,32,32,32,32,32,32,32,32,32,32,32,31,31,31,31,31,30,30,30,
	30,29,29,29,28,28,28,27,27,26,26,26,25,25,25,24,24,24,23,23,23,22,22,22,22,
	21,21,21,21,21,20,20,20,20,20,20,20,20,20,20,20,19,19,19,19,18,18,18,17,17,
	16,16,15,15,14,14,13,13,12,11,11,10,10,9,8,8,7,7,6,6,5,4,4,3,3,
	3,2,2,1,1,1,1,0,0,0,0,0
};