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

extern Ball ball;
extern Player player_top;
extern Player player_bottom;

extern void manage_loosing(void);
extern void manage_bounce(void);
extern float clamp(float d, int min, int max);
extern uint8_t num_sin;
extern uint8_t ticks;

uint16_t old_pixels[BALL_SIZE][BALL_SIZE] = {{Black}};
//uint16_t SinTable[45] = {
//    410, 467, 523, 576, 627, 673, 714, 749, 778,
//    799, 813, 819, 817, 807, 789, 764, 732, 694, 
//    650, 602, 550, 495, 438, 381, 324, 270, 217,
//    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
//    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
//};
uint8_t top_paddle_dir = 1; // 1 -> right; 0 -> left; 2 -> stopped

void TIMER0_IRQHandler (void)
{
	#ifdef SIMULATOR
	char str[5]; 
	#endif
	uint8_t i, j;
	uint16_t old_ball_x = ball.int_x, old_ball_y = ball.int_y, old_top_paddle_x = player_top.paddle_x;
	
	// ---- BEGIN: move top paddle	
	
	if(ball.x_hit_top < player_top.paddle_x+PADDLE_WIDTH/2-10 && ball.x_hit_top > - 45)
		top_paddle_dir = 0; // go left
	else if(ball.x_hit_top > player_top.paddle_x+PADDLE_WIDTH/2+10 && ball.x_hit_top < MAX_X + 45)
		top_paddle_dir = 1; // go right
	else
		top_paddle_dir = 2; // stop
	
	if (top_paddle_dir==1){
		// calculate new x
		player_top.paddle_x+=TOP_PADDLE_SPEED;
		if ( TOP_PADDLE_BOUNCING_RIGHT ) {
			player_top.paddle_x = MAX_X-WALL_SIZE-PADDLE_WIDTH-1;
			top_paddle_dir = !top_paddle_dir;
		}
		// if going right delete left pixels
		LCD_DrawRect( old_top_paddle_x, player_top.paddle_y, player_top.paddle_x - old_top_paddle_x, PADDLE_HEIGHT, Black);
		// draw new piece of paddle on the right
		LCD_DrawRect( player_top.paddle_x+PADDLE_WIDTH-TOP_PADDLE_SPEED, player_top.paddle_y, TOP_PADDLE_SPEED, PADDLE_HEIGHT, Green);
	}
	else if (top_paddle_dir==0){
		// calculate new x
		player_top.paddle_x-=TOP_PADDLE_SPEED;
		if ( TOP_PADDLE_BOUNCING_LEFT ) {
			player_top.paddle_x = WALL_SIZE+1;
			top_paddle_dir = !top_paddle_dir;
		}
		// if going left delete right pixels
		LCD_DrawRect( player_top.paddle_x+PADDLE_WIDTH, player_top.paddle_y, old_top_paddle_x-player_top.paddle_x, PADDLE_HEIGHT, Black);
		// draw new piece of paddle on the left
		LCD_DrawRect( player_top.paddle_x, player_top.paddle_y, TOP_PADDLE_SPEED, PADDLE_HEIGHT, Green);
	}
	
	// ---- END: move top paddle	
	
	
	// ---- BEGIN: GUI refresh	
	if ( IS_LOOSING_TOP || IS_LOOSING_BOTTOM ) {
		manage_loosing();
	}
	else{
		manage_bounce();
		
		// calculate next floating coordinates
		ball.x += ball.sin_dir_angle*SPEED*ball.spin_speed;
		ball.y -= ball.cos_dir_angle*SPEED*ball.spin_speed;
		
		// check that they don't skip the walls (could happen with higher speed for speeding up the emulator)
		ball.x = clamp(ball.x, BALL_SIZE, MAX_X-BALL_SIZE-WALL_SIZE);
		ball.y = clamp(ball.y, 0, MAX_Y-BALL_SIZE);
		
		// cache approximation
		ball.int_x = floor(ball.x);
		ball.int_y = floor(ball.y);
		
		
		// i saw that in the real board the LCD_GetPoint(...) works well, while in emulation it's not precise
		// so i decided to have different behaviours: 
		//  - in emulation: just clear the ball with black pixels and re-write the score if necessary
		//  - real board: clear the ball by putting back the old pixels saved into a matrix
		// NOTE: the real board code is not tested.
		
		#ifdef SIMULATOR
		// restore old pixels (clear ball)
		for ( i=0; i<BALL_SIZE; i++ ){
			for ( j=0; j<BALL_SIZE; j++ ){
				if ( ! ( (old_ball_y+i >= MAX_Y-PADDLE_MARGIN-PADDLE_HEIGHT && old_ball_y+i < MAX_Y-PADDLE_MARGIN && old_ball_x+j >= player_bottom.paddle_x && old_ball_x+j < player_bottom.paddle_x+PADDLE_WIDTH ) || (old_ball_y+i > PADDLE_MARGIN && old_ball_y+i < PADDLE_MARGIN+PADDLE_HEIGHT && old_ball_x+j > player_top.paddle_x && old_ball_x+j < player_top.paddle_x+PADDLE_WIDTH )))
					LCD_SetPoint(old_ball_x+j, old_ball_y+i, Black);
				LCD_SetPoint(ball.int_x+j, ball.int_y+i, Green);
			}
		}
		if (old_ball_x <= WALL_SIZE+8 && old_ball_y >= 155 && old_ball_y <= 155+16){
			// ball passed over left score
			sprintf(str, "%d", player_bottom.points);
			GUI_Text(WALL_SIZE, 155, (uint8_t *) str, White, Black);
		}
		else if(old_ball_x >= MAX_X-WALL_SIZE-BALL_SIZE-8 && old_ball_y >= 155 && old_ball_y <= 155+16){
			// ball passed over right score
			sprintf(str, "%d", player_top.points);
			GUI_TextReverse(MAX_X-WALL_SIZE, 155, (uint8_t *) str, White, Black);
		}
		#else
		// clear old ball
		for ( i=0; i<BALL_SIZE; i++ ){
			for ( j=0; j<BALL_SIZE; j++ ){
				LCD_SetPoint(old_ball_x+j, old_ball_y+i, old_pixels[i][j]);
			}
		}
		// save background pixels to restore later & draw new ball
		for ( i=0; i<BALL_SIZE; i++ ){
			for ( j=0; j<BALL_SIZE; j++ ){
				old_pixels[i][j] = LCD_GetPoint(ball.int_x+j, ball.int_y+i);
				LCD_SetPoint(ball.int_x+j, ball.int_y+i, Green);
			}
		}
		#endif
	}
	// ---- END: GUI refresh	
	
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
//	LPC_DAC->DACR = SinTable[ticks]<<6;
//	ticks++;
//	if(ticks==45) {
//		num_sin++;
//		ticks=0;
//		if (num_sin > N_SIN ){
//			num_sin = 0;
//			disable_timer(2);
//		}
//	}	
	
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
