/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include <stdbool.h>

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
bool in_rit_0=false;
bool in_rit_1=false;
bool in_rit_2=false;
uint8_t old_value = 0;
uint8_t curr_value = 1;

void RIT_IRQHandler (void)
{			
	static int down_0=0;	
	static int down_1=0;
	static int down_2=0;
	
	if( in_rit_1 == true ){
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){
			down_1++;
			// eint1 -> increment
			reset_RIT();
			switch(down_1){
				case 1:
					if (curr_value < 233){
						curr_value = curr_value + old_value;
						old_value = curr_value - old_value;
					}
					LED_Out(curr_value);
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_1=0;	
			in_rit_1=false;
			NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if (in_rit_0==true){
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
			down_0++;
			// eint0 -> increment
			reset_RIT();
			switch(down_0){
				case 1:
					LED_Out(1);
					old_value = 0;
					curr_value = 1;
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_0=0;	
			in_rit_0=false;
			NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	
	if (in_rit_2==true){
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
			down_2++;
			// eint2 -> decrement
			reset_RIT();
			switch(down_2){
				case 1:
					if (!(old_value == 0 && curr_value == 1)){
						uint8_t tmp = old_value;
						old_value = curr_value - old_value;
						curr_value = tmp;
					}
					LED_Out(curr_value);
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_2=0;	
			in_rit_2 = false;
			NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
			
	if (in_rit_0==false && in_rit_1==false && in_rit_2==false){
		disable_RIT();
		reset_RIT();
	}
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
