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

extern float ball_x;
extern float ball_y;
extern uint16_t int_ball_x;
extern uint16_t int_ball_y;
extern uint16_t paddle_x;
extern uint16_t paddle_y;
extern double dir_angle;
extern float sin_dir_angle;
extern float cos_dir_angle;
extern int points;
extern int max_score;
extern float spin_speed;
extern void manage_loosing(void);
extern void manage_bounce(void);
extern float clamp(float d, int min, int max);
extern uint8_t num_sin;
extern uint8_t ticks;

uint16_t old_pixels[BALL_SIZE][BALL_SIZE] = {{Black}};
uint16_t SinTable[45] = {
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

void TIMER0_IRQHandler (void)
{
	uint8_t i, j;
	uint16_t old_ball_x = int_ball_x, old_ball_y = int_ball_y;
	
	/* GUI refresh */
	if ( IS_LOOSING ) {
		manage_loosing();
	}
	else{
		manage_bounce();
		
		// calculate next floating coordinates
		ball_x += sin_dir_angle*SPEED*spin_speed;
		ball_y -= cos_dir_angle*SPEED*spin_speed;
		
		// check that they don't skip the walls (could happen with higher speed for speeding up the emulator)
		ball_x = clamp(ball_x, BALL_SIZE, MAX_X-BALL_SIZE-WALL_SIZE);
		ball_y = clamp(ball_y, BALL_SIZE, MAX_Y-BALL_SIZE);
		
		// cache approximation
		int_ball_x = floor(ball_x);
		int_ball_y = floor(ball_y);
		
		
		// restore old pixels (clear ball)
		for ( i=0; i<BALL_SIZE; i++ ){
			for ( j=0; j<BALL_SIZE; j++ ){
				LCD_SetPoint(old_ball_x+j, old_ball_y+i, old_pixels[i][j]);
			}
		}
		// save background pixels to restore later
		for ( i=0; i<BALL_SIZE; i++ ){
			for ( j=0; j<BALL_SIZE; j++ ){
				old_pixels[i][j] = LCD_GetPoint(int_ball_x+j, int_ball_y+i);
			}
		}
		// draw new ball
		LCD_DrawRect( int_ball_x, int_ball_y, BALL_SIZE, BALL_SIZE, Green);
	}
	
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
	
	/* DAC management */	
	LPC_DAC->DACR = SinTable[ticks]<<6;
	ticks++;
	if(ticks==45) {
		num_sin++;
		ticks=0;
		if (num_sin > N_SIN ){
			num_sin = 0;
			disable_timer(2);
		}
	}	
	
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void)
{
	
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
