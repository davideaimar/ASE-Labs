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
                  
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "button_EXINT/button.h"
#include "adc/adc.h"
#include "pong/pong.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

// IMPORTANT: in order to avoid keil bug, when you debug using scaling factors always keep the RIT window opened.

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();           			
  BUTTON_init();												/* BUTTON Initialization */ 
	ADC_init();
	
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	init_timer(1, 0x65B9B);								/*  Paddle refresh */
	init_timer(0, 0x2625A); 							/*  GUI refresh  */
	
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
