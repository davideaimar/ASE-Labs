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

extern Player player_bottom;

uint8_t new_hits = 0;

void ADC_IRQHandler(void) {
	uint16_t new_x;
	int delta_x;
	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);
	new_x = ( (uint16_t) AD_current*((MAX_X-PADDLE_WIDTH)) / 0xFFF );
	
	if (new_x < WALL_SIZE) new_x=WALL_SIZE;
	else if (new_x>MAX_X-PADDLE_WIDTH-WALL_SIZE) new_x=MAX_X-PADDLE_WIDTH-WALL_SIZE;
	
	if( abs( new_x - player_bottom.paddle_x) > 5){
		new_hits++;
	}
	else{
		new_hits = 0;
	}
	
	if (new_hits > 3){
		new_hits=0;
		delta_x = new_x - player_bottom.paddle_x;
		if (abs(delta_x)>=80) {
			// remove old paddle
			LCD_DrawRect( player_bottom.paddle_x, player_bottom.paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Black);
			// draw new paddle
			player_bottom.paddle_x = new_x;
			LCD_DrawRect( player_bottom.paddle_x, player_bottom.paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Green);
		}
		else{
			if (delta_x>0){
				// going right
				// delete left pixels
				LCD_DrawRect( player_bottom.paddle_x, player_bottom.paddle_y, delta_x, PADDLE_HEIGHT, Black);
				// draw new piece of paddle on the right
				LCD_DrawRect( player_bottom.paddle_x+PADDLE_WIDTH, player_bottom.paddle_y, delta_x, PADDLE_HEIGHT, Green);
			}
			else {
				// going left
				// delete right pixels
				LCD_DrawRect( new_x+PADDLE_WIDTH, player_bottom.paddle_y, abs(delta_x), PADDLE_HEIGHT, Black);
				// draw new piece of paddle on the left
				LCD_DrawRect( new_x, player_bottom.paddle_y, abs(delta_x), PADDLE_HEIGHT, Green);
			}
			player_bottom.paddle_x = new_x;
		}
	}  
	
}
