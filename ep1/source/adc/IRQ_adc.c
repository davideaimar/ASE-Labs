/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"
#include "../pong/pong.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short current_slot = 0;     /* Last converted value               */

extern uint16_t paddle_x;
extern uint16_t paddle_y;

void ADC_IRQHandler(void) {
	short new_slot;
	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */  
	#ifdef SIMULATOR
	new_slot = AD_current*((240-PADDLE_WIDTH)/10) / 0xFFF;
	#else
	new_slot = AD_current*((240-PADDLE_WIDTH)) / 0xFFF;
	#endif
	
	if(new_slot != current_slot){
		// remove old paddle
		LCD_DrawRect( paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Black);
		// draw new paddle
		#ifdef SIMULATOR
		paddle_x = new_slot*10;
		#else
		paddle_x = new_slot*5;
		#endif
		LCD_DrawRect( paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Green);
		current_slot = new_slot;
	}  
	
}
