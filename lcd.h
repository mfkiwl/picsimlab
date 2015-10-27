


#ifndef LCD_H
#define	LCD_H
    
#include<lxrad/lxrad.h>
    
#define DDRMAX 80


#define L_FNT 0x0200  //Sets character font
#define L_NLI 0x0100  //Sets number of display line

#define L_DL  0x0080  //Sets interface data length

#define L_LR  0x0040  //Sets shift direction
#define L_CD  0x0020  //Sets cursor-move or display-shift

#define L_DID 0x0010  //Sets cursor move direction
#define L_DSH 0x0008  //specifies to shift the display

#define L_DON 0x0004  //Sets On/Off of all display
#define L_CON 0x0002  //Sets cursor On/Off 
#define L_CBL 0x0001  //Set blink of cursor position character

/*
I/D  	0 = Decrement cursor position  	1 = Increment cursor position
S 	0 = No display shift 		1 = Display shift
D 	0 = Display off 		1 = Display on
C 	0 = Cursor off 			1 = Cursor on
B 	0 = Cursor blink off 		1 = Cursor blink on
S/C 	0 = Move cursor 		1 = Shift display
R/L 	0 = Shift left 			1 = Shift right
DL 	0 = 4-bit interface 		1 = 8-bit interface
N 	0 = 1/8 or 1/11 Duty (1 line) 	1 = 1/16 Duty (2 lines)
F 	0 = 5x7 dots 			1 = 5x10 dots
BF  	0 = Can accept instruction  	1 = Internal operation in progress
*/


typedef struct
{
unsigned short int flags;
unsigned char ddram_ad;
unsigned char cgram_ad;  
unsigned char update;   //redraw 
unsigned int blinkc;    //blink count timer
unsigned char blink;    //cursor state
char shift;             //display shift
char ddram[DDRMAX][5];
char cgram[8][5];
char bc;
char buff;
}lcd_t;



void lcd_cmd(lcd_t * lcd, char cmd);

void lcd_data(lcd_t * lcd, char data);

void lcd_rst(lcd_t * lcd);

void lcd_on(lcd_t * lcd,int onoff);

void lcd_blink(lcd_t * lcd);

void lcd_draw(lcd_t * lcd, CDraw *draw,int x1,int y1,int w1,int h1, int picpwr);


const unsigned char LCDfont[224][5]={ 
{0x00, 0x00, 0x00, 0x00, 0x00},// (space) 
{0x00, 0x00, 0x5F, 0x00, 0x00},// ! 
{0x00, 0x07, 0x00, 0x07, 0x00},// " 
{0x14, 0x7F, 0x14, 0x7F, 0x14},// # 
{0x24, 0x2A, 0x7F, 0x2A, 0x12},// $ 
{0x23, 0x13, 0x08, 0x64, 0x62},// % 
{0x36, 0x49, 0x55, 0x22, 0x50},// & 
{0x00, 0x05, 0x03, 0x00, 0x00},// ' 
{0x00, 0x1C, 0x22, 0x41, 0x00},// ( 
{0x00, 0x41, 0x22, 0x1C, 0x00},// ) 
{0x08, 0x2A, 0x1C, 0x2A, 0x08},// * 
{0x08, 0x08, 0x3E, 0x08, 0x08},// + 
{0x00, 0x50, 0x30, 0x00, 0x00},// , 
{0x08, 0x08, 0x08, 0x08, 0x08},// - 
{0x00, 0x30, 0x30, 0x00, 0x00},// . 
{0x20, 0x10, 0x08, 0x04, 0x02},// / 
{0x3E, 0x51, 0x49, 0x45, 0x3E},// 0 
{0x00, 0x42, 0x7F, 0x40, 0x00},// 1 
{0x42, 0x61, 0x51, 0x49, 0x46},// 2 
{0x21, 0x41, 0x45, 0x4B, 0x31},// 3 
{0x18, 0x14, 0x12, 0x7F, 0x10},// 4 
{0x27, 0x45, 0x45, 0x45, 0x39},// 5 
{0x3C, 0x4A, 0x49, 0x49, 0x30},// 6 
{0x01, 0x71, 0x09, 0x05, 0x03},// 7 
{0x36, 0x49, 0x49, 0x49, 0x36},// 8 
{0x06, 0x49, 0x49, 0x29, 0x1E},// 9 
{0x00, 0x36, 0x36, 0x00, 0x00},// : 
{0x00, 0x56, 0x36, 0x00, 0x00},// ; 
{0x00, 0x08, 0x14, 0x22, 0x41},// < 
{0x14, 0x14, 0x14, 0x14, 0x14},// = 
{0x41, 0x22, 0x14, 0x08, 0x00},// > 
{0x02, 0x01, 0x51, 0x09, 0x06},// ? 
{0x32, 0x49, 0x79, 0x41, 0x3E},// @ 
{0x7E, 0x11, 0x11, 0x11, 0x7E},// A 
{0x7F, 0x49, 0x49, 0x49, 0x36},// B 
{0x3E, 0x41, 0x41, 0x41, 0x22},// C 
{0x7F, 0x41, 0x41, 0x22, 0x1C},// D 
{0x7F, 0x49, 0x49, 0x49, 0x41},// E 
{0x7F, 0x09, 0x09, 0x01, 0x01},// F 
{0x3E, 0x41, 0x41, 0x51, 0x32},// G 
{0x7F, 0x08, 0x08, 0x08, 0x7F},// H 
{0x00, 0x41, 0x7F, 0x41, 0x00},// I 
{0x20, 0x40, 0x41, 0x3F, 0x01},// J 
{0x7F, 0x08, 0x14, 0x22, 0x41},// K 
{0x7F, 0x40, 0x40, 0x40, 0x40},// L 
{0x7F, 0x02, 0x04, 0x02, 0x7F},// M 
{0x7F, 0x04, 0x08, 0x10, 0x7F},// N 
{0x3E, 0x41, 0x41, 0x41, 0x3E},// O 
{0x7F, 0x09, 0x09, 0x09, 0x06},// P 
{0x3E, 0x41, 0x51, 0x21, 0x5E},// Q 
{0x7F, 0x09, 0x19, 0x29, 0x46},// R 
{0x46, 0x49, 0x49, 0x49, 0x31},// S 
{0x01, 0x01, 0x7F, 0x01, 0x01},// T 
{0x3F, 0x40, 0x40, 0x40, 0x3F},// U 
{0x1F, 0x20, 0x40, 0x20, 0x1F},// V 
{0x7F, 0x20, 0x18, 0x20, 0x7F},// W 
{0x63, 0x14, 0x08, 0x14, 0x63},// X 
{0x03, 0x04, 0x78, 0x04, 0x03},// Y 
{0x61, 0x51, 0x49, 0x45, 0x43},// Z 
{0x00, 0x00, 0x7F, 0x41, 0x41},// [
{0x15 ,0x16 ,0x7C ,0x16 ,0x15},// Y_ 
{0x41, 0x41, 0x7F, 0x00, 0x00},// ] 
{0x04, 0x02, 0x01, 0x02, 0x04},// ^ 
{0x40, 0x40, 0x40, 0x40, 0x40},// _ 
{0x00, 0x01, 0x02, 0x04, 0x00},// ` 
{0x20, 0x54, 0x54, 0x54, 0x78},// a 
{0x7F, 0x48, 0x44, 0x44, 0x38},// b 
{0x38, 0x44, 0x44, 0x44, 0x20},// c 
{0x38, 0x44, 0x44, 0x48, 0x7F},// d 
{0x38, 0x54, 0x54, 0x54, 0x18},// e 
{0x08, 0x7E, 0x09, 0x01, 0x02},// f 
{0x08, 0x14, 0x54, 0x54, 0x3C},// g 
{0x7F, 0x08, 0x04, 0x04, 0x78},// h 
{0x00, 0x44, 0x7D, 0x40, 0x00},// i 
{0x20, 0x40, 0x44, 0x3D, 0x00},// j 
{0x00, 0x7F, 0x10, 0x28, 0x44},// k 
{0x00, 0x41, 0x7F, 0x40, 0x00},// l 
{0x7C, 0x04, 0x18, 0x04, 0x78},// m 
{0x7C, 0x08, 0x04, 0x04, 0x78},// n 
{0x38, 0x44, 0x44, 0x44, 0x38},// o 
{0x7C, 0x14, 0x14, 0x14, 0x08},// p 
{0x08, 0x14, 0x14, 0x18, 0x7C},// q 
{0x7C, 0x08, 0x04, 0x04, 0x08},// r 
{0x48, 0x54, 0x54, 0x54, 0x20},// s 
{0x04, 0x3F, 0x44, 0x40, 0x20},// t 
{0x3C, 0x40, 0x40, 0x20, 0x7C},// u 
{0x1C, 0x20, 0x40, 0x20, 0x1C},// v 
{0x3C, 0x40, 0x30, 0x40, 0x3C},// w 
{0x44, 0x28, 0x10, 0x28, 0x44},// x 
{0x0C, 0x50, 0x50, 0x50, 0x3C},// y 
{0x44, 0x64, 0x54, 0x4C, 0x44},// z 
{0x00, 0x08, 0x36, 0x41, 0x00},// { 
{0x00, 0x00, 0x7F, 0x00, 0x00},// | 
{0x00, 0x41, 0x36, 0x08, 0x00},// } 
{0x08, 0x08, 0x2A, 0x1C, 0x08},// -> 
{0x08, 0x1C, 0x2A, 0x08, 0x08},// <- 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty  first block
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty second block
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00, 0x00, 0x00, 0x00, 0x00},//empty 
{0x00 ,0x00 ,0x00 ,0x00 ,0x00},//kanji
{0x70 ,0x50 ,0x70 ,0x00 ,0x00},
{0x00 ,0x00 ,0x0F ,0x01 ,0x01},
{0x40 ,0x40 ,0x78 ,0x00 ,0x00},
{0x10 ,0x20 ,0x40 ,0x00 ,0x00},
{0x00 ,0x18 ,0x18 ,0x00 ,0x00},
{0x0A ,0x0A ,0x4A ,0x2A ,0x1E},
{0x04 ,0x44 ,0x34 ,0x14 ,0x0C},
{0x20 ,0x10 ,0x78 ,0x04 ,0x00},
{0x18 ,0x08 ,0x4C ,0x48 ,0x38},
{0x48 ,0x48 ,0x78 ,0x48 ,0x48},
{0x48 ,0x28 ,0x18 ,0x7C ,0x08},
{0x08 ,0x7C ,0x08 ,0x28 ,0x18},
{0x40 ,0x48 ,0x48 ,0x78 ,0x40},
{0x54 ,0x54 ,0x54 ,0x7C ,0x00},
{0x1C ,0x00 ,0x5C ,0x40 ,0x3C},
{0x08 ,0x08 ,0x08 ,0x08 ,0x08},
{0x01 ,0x41 ,0x3D ,0x09 ,0x07},
{0x10 ,0x08 ,0x7C ,0x02 ,0x01},
{0x0E ,0x02 ,0x43 ,0x22 ,0x1E},
{0x42 ,0x42 ,0x7E ,0x42 ,0x42},
{0x22 ,0x12 ,0x0A ,0x7F ,0x02},
{0x42 ,0x3F ,0x02 ,0x42 ,0x3E},
{0x0A ,0x0A ,0x7F ,0x0A ,0x0A},
{0x08 ,0x46 ,0x42 ,0x22 ,0x1E},
{0x04 ,0x03 ,0x42 ,0x3E ,0x02},
{0x42 ,0x42 ,0x42 ,0x42 ,0x7E},
{0x02 ,0x4F ,0x22 ,0x1F ,0x02},
{0x4A ,0x4A ,0x40 ,0x20 ,0x1C},
{0x42 ,0x22 ,0x12 ,0x2A ,0x46},
{0x02 ,0x3F ,0x42 ,0x4A ,0x46},
{0x06 ,0x48 ,0x40 ,0x20 ,0x1E},
{0x08 ,0x46 ,0x4A ,0x32 ,0x1E},
{0x0A ,0x4A ,0x3E ,0x09 ,0x08},
{0x0E ,0x00 ,0x4E ,0x20 ,0x1E},
{0x04 ,0x45 ,0x3D ,0x05 ,0x04},
{0x00 ,0x7F ,0x08 ,0x10 ,0x00},
{0x44 ,0x24 ,0x1F ,0x04 ,0x04},
{0x40 ,0x42 ,0x42 ,0x42 ,0x40},
{0x42 ,0x2A ,0x12 ,0x2A ,0x06},
{0x22 ,0x12 ,0x7B ,0x16 ,0x22},
{0x00 ,0x40 ,0x20 ,0x1F ,0x00},
{0x78 ,0x00 ,0x02 ,0x04 ,0x78},
{0x3F ,0x44 ,0x44 ,0x44 ,0x44},
{0x02 ,0x42 ,0x42 ,0x22 ,0x1E},
{0x04 ,0x02 ,0x04 ,0x08 ,0x30},
{0x32 ,0x02 ,0x7F ,0x02 ,0x32},
{0x02 ,0x12 ,0x22 ,0x52 ,0x0E},
{0x00 ,0x2A ,0x2A ,0x2A ,0x40},
{0x38 ,0x24 ,0x22 ,0x20 ,0x70},
{0x40 ,0x28 ,0x10 ,0x28 ,0x06},
{0x0A ,0x3E ,0x4A ,0x4A ,0x4A},
{0x04 ,0x7F ,0x04 ,0x14 ,0x0C},
{0x40 ,0x42 ,0x42 ,0x7E ,0x40},
{0x4A ,0x4A ,0x4A ,0x4A ,0x7E},
{0x04 ,0x05 ,0x45 ,0x25 ,0x1C},
{0x0F ,0x40 ,0x20 ,0x1F ,0x00},
{0x7C ,0x00 ,0x7E ,0x40 ,0x30},
{0x7E ,0x40 ,0x20 ,0x10 ,0x08},
{0x7E ,0x42 ,0x42 ,0x42 ,0x7E},
{0x0E ,0x02 ,0x42 ,0x22 ,0x1E},
{0x42 ,0x42 ,0x40 ,0x20 ,0x18},
{0x02 ,0x04 ,0x01 ,0x02 ,0x00},
{0x07 ,0x05 ,0x07 ,0x00 ,0x00},
{0x38 ,0x44 ,0x48 ,0x30 ,0x48},//gre
{0x20 ,0x55 ,0x54 ,0x55 ,0x78},
{0xF8 ,0x54 ,0x54 ,0x54 ,0x28},
{0x28 ,0x54 ,0x54 ,0x44 ,0x20},
{0xFC ,0x20 ,0x20 ,0x10 ,0x3C},
{0x38 ,0x44 ,0x4C ,0x54 ,0x24},
{0xF0 ,0x48 ,0x44 ,0x44 ,0x38},
{0x38 ,0x44 ,0x44 ,0x44 ,0xFC},
{0x20 ,0x40 ,0x3C ,0x04 ,0x04},
{0x04 ,0x04 ,0x00 ,0x0E ,0x00},
{0x00 ,0x00 ,0x04 ,0xFD ,0x00},
{0x0A ,0x04 ,0x0A ,0x00 ,0x00},
{0x18 ,0x24 ,0x7E ,0x24 ,0x10},
{0x14 ,0x7F ,0x54 ,0x40 ,0x40},
{0x7C ,0x09 ,0x05 ,0x05 ,0x78},
{0x38 ,0x45 ,0x44 ,0x45 ,0x38},
{0xFC ,0x28 ,0x24 ,0x24 ,0x18},
{0x38 ,0x44 ,0x44 ,0x48 ,0xFC},
{0x3C ,0x4A ,0x4A ,0x4A ,0x3C},
{0x30 ,0x28 ,0x10 ,0x28 ,0x18},
{0x58 ,0x64 ,0x04 ,0x64 ,0x58},
{0x3C ,0x41 ,0x40 ,0x21 ,0x7C},
{0x63 ,0x55 ,0x49 ,0x41 ,0x41},
{0x24 ,0x1C ,0x04 ,0x3C ,0x24},
{0x45 ,0x29 ,0x11 ,0x29 ,0x45},
{0x3C ,0x40 ,0x40 ,0x40 ,0xFC},
{0x14 ,0x14 ,0x7C ,0x14 ,0x12},
{0x44 ,0x3C ,0x14 ,0x14 ,0x74},
{0x7C ,0x14 ,0x1C ,0x14 ,0x7C},
{0x10 ,0x10 ,0x54 ,0x10 ,0x10},
{0x00 ,0x00 ,0x00 ,0x00 ,0x00},
{0xFF ,0xFF ,0xFF ,0xFF ,0xFF},
}; 

#endif

