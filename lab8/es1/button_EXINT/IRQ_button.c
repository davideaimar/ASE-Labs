#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"

uint8_t old_value = 0;
uint8_t curr_value = 1;


void EINT0_IRQHandler (void)	  
{
	// reset button
	LED_Out(1);
	old_value = 0;
	curr_value = 1;
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  
{
	// increment button
	if (curr_value < 233){
		curr_value = curr_value + old_value;
		old_value = curr_value - old_value;
	}
	LED_Out(curr_value);
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  
{
	// decrement button
	if (!(old_value == 0 && curr_value == 1)){
		uint8_t tmp = old_value;
		old_value = curr_value - old_value;
		curr_value = tmp;
	}
	LED_Out(curr_value);
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


