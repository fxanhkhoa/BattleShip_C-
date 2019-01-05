//
// LCD_Game_Frogger : emulate Frogger game on LCD
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
//
// 5 rows of buses moving horizontally on the street
// 1 frog          moving veritically across the street
// press keypad to move the frog
// pass to the top get score
// hit  by the bus lose life

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"

//11100111
//00100100
//00111110
//00111111
//00111111
//00111110
//00100100
//11100111
// bitmap is in upside down order
unsigned char frog[16] = {
0xE7,0x24,0x3E,0x3F,0x3F,0x3E,0x24,0xE7};
// bitmap up-side down
unsigned char bus1[16] = {
	0x3F,0x3F,0xF9,0xF9,0x3F,0x39,0x39,0x3F,0x39,0x39,0x3F,0x39,0xF9,0xFF,0x31,0x3F};
unsigned char bus2[16] = {
	0x3F,0x3F,0x3F,0xFF,0xFF,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0xF9,0xF9,0x3F,0x3C};
unsigned char bus3[16] = {
	0x38,0x38,0x38,0xFC,0xFE,0x3F,0x33,0x33,0x33,0x33,0xF6,0xFC,0x38,0x30,0x30,0x30};
unsigned char bus4[16] = {
	0x00,0x00,0x00,0x70,0xD0,0x90,0x9F,0x97,0x96,0x94,0xD0,0x70,0x00,0x00,0x00,0x00};
unsigned char bus5[16] = {
	0x00,0x60,0x91,0x8F,0x99,0x69,0x08,0x6C,0x9C,0x8C,0x90,0x60,0x00,0x00,0x00,0x00};

unsigned char GameTitle[128*8] ={
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x00,0x04,0x00,0x04,0x08,0x00,0x00,0x10,0x08,0x00,0x04,0x80,0x80,0x04,0x00,0x08,0x10,0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x20,0x60,0xF0,0xE8,0x98,0x0C,0x1C,0xBE,0xFE,0xFA,0xEF,0xF0,0xE0,0xC0,0x80,0x8E,0x8F,0x8F,0x86,0xC0,0xA0,0xD0,0xE0,0xE0,0xC7,0xC7,0xCF,0xC7,0xC0,0xE0,0xF0,0xFC,0xFB,0xF8,0xD0,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xD0,0xE8,0xFC,0x64,0x22,0x43,0xE7,0xFF,0xC7,0x82,0x82,0x83,0xC7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x80,0xC0,0xF0,0xE8,0xF4,0xFE,0xFF,0xF0,0xF0,0xF0,0xF9,0xFF,0x7F,0xFF,0xBE,0xFE,0xFF,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0xFB,0xF7,0xEF,0xFF,0xD7,0xBF,0xFF,0xFF,0xEF,0x7F,0x77,0x3B,0x1D,0x0F,0x0C,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xF8,0xF8,0x08,0x08,0x08,0x88,0x18,0x38,0x00,0x00,0x40,0xC0,0xC0,0x80,0x40,0xC0,0xC0,0x00,0x00,0x80,0xC0,0x40,0x40,0xC0,0x80,0x00,0x00,0x00,0x80,0xC0,0x40,0x40,0x40,0xC0,0xC0,0x40,0x00,0x80,0xC0,0x40,0x40,0x40,0xC0,0xC0,0x40,0x00,0x00,0x80,0xC0,0x40,0x40,0xC0,0x80,0x00,0x00,0x40,0xC0,0xC0,0x80,0x40,0xC0,0xC0,0x00,0x00,
	0xE0,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x31,0x3F,0x3F,0x3F,0x0F,0x3F,0xFF,0xFF,0xFF,0xDF,0x7F,0x9F,0x1F,0x1F,0x1F,0x1F,0x3F,0xBF,0x7F,0x1F,0xEB,0x7C,0xF0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x3F,0x21,0x01,0x01,0x03,0x00,0x00,0x00,0x00,0x20,0x3F,0x3F,0x20,0x00,0x00,0x00,0x00,0x0F,0x1F,0x30,0x20,0x20,0x30,0x1F,0x0F,0x00,0x00,0xC3,0xFF,0x34,0x34,0x34,0x37,0xF3,0x60,0x00,0xC3,0xFF,0x34,0x34,0x34,0x37,0xF3,0x60,0x00,0x0F,0x1F,0x32,0x22,0x22,0x23,0x13,0x00,0x00,0x20,0x3F,0x3F,0x20,0x00,0x00,0x00,0x00,0x00,
	0x07,0x1F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0F,0x9F,0x7F,0xFB,0xF6,0xF8,0xF8,0xE6,0xC1,0x80,0x80,0x80,0xFF,0xFF,0x7F,0xFF,0xFE,0xFC,0xE8,0xF0,0xF0,0xE0,0xE0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x30,0x7C,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xFB,0xFD,0xD2,0xF1,0xF1,0xF8,0xF8,0xF8,0xE8,0xF8,0xF8,0xFC,0xFC,0xFC,0xF4,0xF4,0xF4,0xFC,0xF4,0xF4,0xFE,0x3A,0x31,0x11,0x00,0x00,0xF9,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xEF,0xF6,0x7F,0x3F,0x3D,0x6F,0x7A,0x77,0x7D,0x3F,0x03,0x05,0x0F,0x0F,0x0F,0x0B,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0E,0x0E,0x0F,0x07,0x00,0x00,0x0F,0x1F,0x0F,0x07,0x01,0x00,0x00,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

int main(void)
{
	char Text[25];
	int8_t x,y;
	int8_t i,keyin;
	int8_t hit;
	int8_t score, life;

	int8_t frog_x, frog_y;
	int8_t bus1_x[3], bus1_y, bus1_speed;
	int8_t bus2_x[3], bus2_y, bus2_speed;
	int8_t bus3_x[3], bus3_y, bus3_speed;
	int8_t bus4_x[3], bus4_y, bus4_speed;
	int8_t bus5_x[3], bus5_y, bus5_speed;	
	
	SYS_Init();
	  
	init_LCD();
	clear_LCD();
  OpenKeyPad();// initialize 3x3 keypad
	
	draw_LCD(GameTitle);   // draw Game Title scene
	for (i=0;i<3;i++) CLK_SysTickDelay(1000000);
	clear_LCD();

	printS_5x7( 0, 0,"LCD Game - Frogger");	
	printS_5x7(20,16,"Keypad 3x3    ");	
	printS_5x7(20,24,"2 : move up   ");
	printS_5x7(20,32,"8 : move down ");		
	printS_5x7(20,40,"4 : move left ");
	printS_5x7(20,48,"6 : move right");	
	printS_5x7(10,56,"Press Any Key To Start!");	
	
	keyin=ScanKey();
  while(keyin==0) { keyin=ScanKey();}
	clear_LCD();
	
  // set initial location of bus & its speed
	bus1_x[0]=0; bus1_x[1]=48; bus1_x[2]=64; bus1_y=16; bus1_speed=1;
	bus2_x[0]=0; bus2_x[1]=48; bus2_x[2]=64; bus2_y=24; bus2_speed=2;
	bus3_x[0]=0; bus3_x[1]=56; bus3_x[2]=72; bus3_y=32; bus3_speed=1;
	bus4_x[0]=0; bus4_x[1]=64; bus4_x[2]=80; bus4_y=40; bus4_speed=1;
	bus5_x[0]=0; bus5_x[1]=64; bus5_x[2]=80; bus5_y=48;	bus5_speed=1;
	
	// set initial location of frog
	frog_x=64; frog_y=56;	
	
	score=0; life=3; hit=0;

	while(1) {		
	  // Display Score & Life
	  sprintf(Text,"SCORE %3d000     LIFE %3d",score, life);	
	  x=0; y=0; printS_5x7(x,y,Text);
		if (hit==0 && !(frog_y+7<bus1_y))
	  {
		  // Display Ojbects : draw foreground color to display object
		  for (i=0;i<3;i++) {
		  draw_Bmp16x8(bus1_x[i], bus1_y,FG_COLOR,BG_COLOR, bus1);
		  draw_Bmp16x8(bus2_x[i], bus2_y,FG_COLOR,BG_COLOR, bus2);
		  draw_Bmp16x8(bus3_x[i], bus3_y,FG_COLOR,BG_COLOR, bus3);
		  draw_Bmp16x8(bus4_x[i], bus4_y,FG_COLOR,BG_COLOR, bus4);
		  draw_Bmp16x8(bus5_x[i], bus5_y,FG_COLOR,BG_COLOR, bus5);
			}
	    draw_Bmp8x8(frog_x, frog_y, FG_COLOR,BG_COLOR, frog); 		
		
		  // Delay for Vision
		  CLK_SysTickDelay(200000);
		
		  // Erase Ojbects : draw background color at the ojbect location
		  for (i=0;i<3;i++) {
		  draw_Bmp16x8(bus1_x[i], bus1_y,BG_COLOR,BG_COLOR, bus1);
		  draw_Bmp16x8(bus2_x[i], bus2_y,BG_COLOR,BG_COLOR, bus2);
		  draw_Bmp16x8(bus3_x[i], bus3_y,BG_COLOR,BG_COLOR, bus3);
		  draw_Bmp16x8(bus4_x[i], bus4_y,BG_COLOR,BG_COLOR, bus4);
		  draw_Bmp16x8(bus5_x[i], bus5_y,BG_COLOR,BG_COLOR, bus5);
			}		
	    draw_Bmp8x8(frog_x, frog_y, BG_COLOR,BG_COLOR, frog); 		
		
	    // Object Move by changing its x,y
		
		  keyin=ScanKey(); // get keypad
		  switch(keyin) {  // to increment Frog's x,y
			case 2: frog_y-=8; break;
			case 4: frog_x-=8; break;
			case 6: frog_x+=8; break;
			case 8: frog_y+=8; break;
			default:           break;
		  }	
	    //inc/dec Bus x for moving horizontally
			for (i=0; i<3; i++) {
		  bus1_x[i] = bus1_x[i] + bus1_speed;
		  bus2_x[i] = bus2_x[i] - bus2_speed;
		  bus3_x[i] = bus3_x[i] + bus3_speed;
		  bus4_x[i] = bus4_x[i] - bus4_speed;
		  bus5_x[i] = bus5_x[i] + bus5_speed;
			}
	 			
	    // Boundary Check : if >max, set to min; if <min, set to max;
			for (i=0; i<3; i++) {
		  if (bus1_x[i]>(LCD_Xmax-16)) bus1_x[i]=0;
			if (bus1_x[i]<0)             bus1_x[i]=LCD_Xmax-16;
		  if (bus2_x[i]>(LCD_Xmax-16)) bus2_x[i]=0;
			if (bus2_x[i]<0)             bus2_x[i]=LCD_Xmax-16;
		  if (bus3_x[i]>(LCD_Xmax-16)) bus3_x[i]=0;
			if (bus3_x[i]<0)             bus3_x[i]=LCD_Xmax-16;
		  if (bus4_x[i]>(LCD_Xmax-16)) bus4_x[i]=0;
			if (bus4_x[i]<0)             bus4_x[i]=LCD_Xmax-16;
		  if (bus5_x[i]>(LCD_Xmax-16)) bus5_x[i]=0;
			if (bus5_x[i]<0)             bus5_x[i]=LCD_Xmax-16;
			}
		
		  if (frog_x>(LCD_Xmax-16))	frog_x=0;
      if (frog_x<0)             frog_x=LCD_Xmax-16;
		  if (frog_y>(LCD_Ymax-8))	frog_y=0;
      if (frog_y<0)             frog_y=LCD_Ymax-8;
				
	    // Collision Detection : check if Frog hit by Bus
			
			for (i=0; i<3; i++) {			
		    if (frog_y+7>=bus1_y && frog_y<bus1_y+7)	// check bus1
			     if (frog_x+7>=bus1_x[i] && frog_x<bus1_x[i]+16) hit=1;
			  if (frog_y+7>=bus2_y && frog_y<bus2_y+7)	// check bus2
			     if (frog_x+7>=bus2_x[i] && frog_x<bus2_x[i]+16) hit=1;
			  if (frog_y+7>=bus3_y && frog_y<bus3_y+7)	// check bus3
			     if (frog_x+7>=bus3_x[i] && frog_x<bus3_x[i]+16) hit=1;
			  if (frog_y+7>=bus4_y && frog_y<bus4_y+7)	// check bus4
					 if (frog_x+7>=bus4_x[i] && frog_x<bus4_x[i]+16) hit=1; 	
			  if (frog_y+7>=bus5_y && frog_y<bus5_y+7)	// check bus5
			     if (frog_x+7>=bus5_x[i] && frog_x<bus5_x[i]+16) hit=1;
	    }	
		}	
	  else if (frog_y+7<bus1_y) {
		  score++;
		  frog_x=64; frog_y=56;	}
		else { // hit
				hit=0;
				life--;
		    if (life>0) {
				  x=0; y=0;	 printS_5x7(x,y,"    *** Try Again ***    ");			
			    printS_5x7( 0,56,"Press Any Key To Start!"); }
				else {
					sprintf(Text,"SCORE %3d000     LIFE %3d",score, life);
					x=0; y=0;  printS_5x7(x,y,Text);
				  x=0; y=32; printS_5x7(x,y,"  ===== GAME OVER =====  ");				
				  score=0; life=3;
			  } 
				CLK_SysTickDelay(1000000);
	      keyin=0;
        while(keyin==0) { keyin=ScanKey();}
				clear_LCD();
			  frog_x=64; frog_y=56; 				
		}	// if-else
	} // which(1)
}