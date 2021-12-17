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
#include "../GLCD/GLCD.h" 
#include "../adc/adc.h"
#include "../pong/pong.h"
#include <math.h>

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern uint16_t ball_x;
extern uint16_t ball_y;
extern uint16_t paddle_x;
extern uint16_t paddle_y;
extern double dir_angle;

void calc_next_coord(uint16_t* x, uint16_t* y){
	
}

int is_good_border(){
	return ((ball_y<=10) || (ball_x<=10&&ball_y<=320-47) || (ball_x>=230&&ball_y<=320-47));
}
int hit_paddle(){
	uint16_t bottom_y = ball_y+BALL_SIZE;
	return ( (dir_angle>PI/2  && dir_angle<(PI+PI/2)) && (bottom_y>=paddle_y&&bottom_y<paddle_y+10) && (ball_x >= paddle_x && ball_x <= paddle_x + PADDLE_WIDTH) ) ;
}

void detect_bounce(){
	if (is_good_border()){
		dir_angle += 2*PI;
	}
	if (hit_paddle()){
		dir_angle += 2*PI;
	}
		
}

void TIMER0_IRQHandler (void)
{
	//calc_next_coord(&x, &y);
	detect_bounce();
	/* GUI refresh */
	// clear old ball
	LCD_DrawRect(ball_x, ball_y, 5, 5, Black);
	// TODO: calculate next x, y
	ball_x += (int)      sin(dir_angle)*SPEED;
	ball_y += (int) -1 * cos(dir_angle)*SPEED;
	// draw new ball
	LCD_DrawRect(ball_x, ball_y, 5, 5, Green);
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
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
	ADC_start_conversion();	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
