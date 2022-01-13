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
#include "../GLCD/GLCD.h"
#include "../button_EXINT/button.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

int down_INT0=0;
int down_KEY1=0;
int down_KEY2=0;

extern int blink_mask;
extern int check;
extern uint8_t enable_int0;
extern uint8_t enable_int1;
extern uint8_t enable_key2;
extern void pong_init(void);

void RIT_IRQHandler (void)
{						
	/* button management */
	/* INT0 */
	if(down_INT0!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			down_INT0++;				
			switch(down_INT0){
				case 2:
					if ( enable_int0 ){			
						pong_init();
						enable_int0 = 0;
					}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_INT0=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	/* KEY1 */
	if(down_KEY1!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			down_KEY1++;				
			switch(down_KEY1){
				case 2:
					// start game on button INT1 click
					if ( enable_int1 ){
						enable_int1=0;
						enable_key2=1;
						reset_timer(0);
						GUI_Text(37, 300, (uint8_t *) " press KEY2 to pause ", White, Black);			   
						enable_timer(0);
						enable_timer(1);
					}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_KEY1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	/* KEY2 */
	if(down_KEY2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			down_KEY2++;				
			switch(down_KEY2){
				case 2:
					if ( enable_key2 ){
						if (LPC_TIM0->TCR == 0){
							enable_timer(0);
							GUI_Text(37, 300, (uint8_t *) " press KEY2 to pause ", White, Black);
						}
						else {
							disable_timer(0);
							GUI_Text(32, 300, (uint8_t *) " press KEY2 to resume ", White, Black);
						}
						if (LPC_TIM1->TCR == 0)
							enable_timer(1);
						else
							disable_timer(1);
					}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_KEY2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}		
			
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	reset_RIT();
}

/******************************************************************************
**                            End Of File
******************************************************************************/
