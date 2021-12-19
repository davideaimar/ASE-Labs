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
#include <stdio.h>

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
extern uint16_t paddle_x;
extern uint16_t paddle_y;
extern double dir_angle;
extern float sin_dir_angle;
extern float cos_dir_angle;
extern int points;
extern int max_score;

uint16_t old_pixels[BALL_SIZE][BALL_SIZE] = {{Black}};


void detect_bounce(){
	if ( IS_GOOD_BORDER_LEFT || IS_GOOD_BORDER_RIGHT ){
		dir_angle = fmod(2*PI-dir_angle + 2*PI, 2*PI);
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
	}
	else if ( IS_BORDER_TOP && GOING_UP ){
		dir_angle = fmod(PI - dir_angle + 2*PI, 2*PI);
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
	}
	else if ( IS_HITTING_PADDLE ){
		char str[5];
		int rel_x = floor(ball_x)-paddle_x; // ball x position relative to paddle
		rel_x = rel_x < 5 ? 5 : rel_x;
		rel_x = rel_x >=80 ? 75 : rel_x;
		// 0...PADDLE_WIDTH+BALL_SIZE -> 3/2PI--PI/2
		dir_angle = fmod((((rel_x) * PI) / (PADDLE_WIDTH)) + 3*PI/2 + 2*PI, 2*PI);
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
		points = points >= 100 ? points+10 : points+5;
		sprintf(str, "%d", points);
		GUI_Text(200, 300, (uint8_t *) str, White, Black);
		if (points > max_score){
			max_score = points;
			sprintf(str, "%d", max_score);
			GUI_Text(83, 300, (uint8_t *) str, White, Black);
		}
	}
}

void TIMER0_IRQHandler (void)
{
	int i, j;
	/* GUI refresh */
	detect_bounce();
	// clear old ball
	for ( i=0; i<BALL_SIZE; i++ ){
		for ( j=0; j<BALL_SIZE; j++ ){
			LCD_SetPoint(ball_x+j, ball_y+i, old_pixels[i][j]);
		}
	}
	
	//LCD_DrawRect( floor(ball_x),floor(ball_y), BALL_SIZE, BALL_SIZE, Black);
	ball_x +=      sin_dir_angle*SPEED;
	ball_y -= 		 cos_dir_angle*SPEED;
	
	for ( i=0; i<BALL_SIZE; i++ ){
		for ( j=0; j<BALL_SIZE; j++ ){
			old_pixels[i][j] = LCD_GetPoint(floor(ball_x)+j, floor(ball_y)+i);
		}
	}
	// draw new ball
	LCD_DrawRect( floor(ball_x),floor(ball_y), BALL_SIZE, BALL_SIZE, Green);
	
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
