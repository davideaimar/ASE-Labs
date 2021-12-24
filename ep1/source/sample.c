/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT and joystick buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "button_EXINT/button.h"
#include "adc/adc.h"
#include "pong/pong.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h"


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();           
  BUTTON_init();												/* BUTTON Initialization */ 
	ADC_init();
	
	#ifdef SIMULATOR
	init_RIT(0x04C4B40);									/* RIT Initialization 50 msec       	*/
	init_timer(1, 0x1E848);								/* GUI refresh every 5ms     	*/
	init_timer(0, 0x001E848 ); 	
	#else
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	init_timer(1, 0x65B9B);								/*  GUI refresh every 60fps */
	init_timer(0, 0x002625A ); 	
	#endif
		
	// enable RIT: it implements button debouncing
	enable_RIT();
	
	pong_init();
		
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }

}
