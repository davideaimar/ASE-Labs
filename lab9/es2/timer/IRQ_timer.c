/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern unsigned char led_value;					/* defined in funct_led								*/
extern unsigned int led_number;					/* defined in funct_led								*/

void TIMER0_IRQHandler (void)
{
	static uint8_t position = 7;
	LED_Off(position);
	if(position == 7)
		position = 2;
	else
		position++;
	LED_On(position);
	/* alternatively to LED_On and LED_off try to use LED_Out */
	//LED_Out((1<<position)|(led_value & 0x3));							
	/* LED_Out is CRITICAL due to the shared led_value variable */
	/* LED_Out MUST NOT BE INTERRUPTED */
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER2_IRQHandler (void)
{
	if (LPC_TIM2->IR == 1){
		LED_Off(led_number);
		LPC_TIM2->IR = 1;			/* clear interrupt flag */
		LPC_TIM2->TC += 1;
		LPC_TIM2->TCR = 1;
	}
	else if(LPC_TIM2->IR == 2) {
		LED_On(led_number);
		LPC_TIM2->IR = 0x2;			/* clear interrupt flag */
		LPC_TIM2->TCR = 1;
	}
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
