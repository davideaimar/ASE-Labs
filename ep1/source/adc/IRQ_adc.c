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
#include <stdlib.h>
#include "../led/led.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h"
#include "../pong/pong.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   

extern uint16_t paddle_x;
extern uint16_t paddle_y;

uint8_t new_hits = 0;

void ADC_IRQHandler(void) {
	uint16_t new_x;
	
	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);
	new_x = (uint16_t) AD_current*((240-PADDLE_WIDTH)) / 0xFFF;
	
	if( abs( new_x - paddle_x) > 5){
		new_hits++;
	}
	else{
		new_hits = 0;
	}
	
	if (new_hits > 5){
		new_hits=0;
		// remove old paddle
		LCD_DrawRect( paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Black);
		// draw new paddle
		paddle_x = new_x;
		LCD_DrawRect( paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Green);
	}  
	
}
