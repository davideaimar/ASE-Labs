#include "pong.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h"
#include <math.h>

volatile uint16_t paddle_x = 100;
volatile uint16_t paddle_y = 320-42;
volatile float ball_x = MAX_X-15;
volatile float ball_y = MAX_Y/2;
volatile double dir_angle = 5*PI/4; 
volatile float sin_dir_angle;
volatile float cos_dir_angle;
volatile int points = 0;
volatile int max_score = 100;

void pong_init(){
	
	sin_dir_angle = sin(dir_angle);
	cos_dir_angle = cos(dir_angle);
	
	LCD_Clear(Black);
	
	// bouncing walls
	LCD_DrawRect( 0, 0, MAX_X, 5, Red); // top border
	LCD_DrawRect( 0, 0, 5, MAX_Y-42, Red); // left border
	LCD_DrawRect( MAX_X-5, 0, 5, MAX_Y-42, Red); // right border
	
	
	// ball
	LCD_DrawRect( floor(ball_x), floor(ball_y), BALL_SIZE, BALL_SIZE, Green);
	
	GUI_Text(40, 300, (uint8_t *) " press KEY1 to play ", White, Black);
	GUI_Text(130, 30, (uint8_t *) "Record:  100", White, Black);
	GUI_Text(20, 155, (uint8_t *) "Score: ", White, Black);
	
	// enable RIT -> it implements button debouncing
	#ifdef SIMULATOR
	init_RIT(0x001E848);									/* RIT Initialization 5 msec       	*/
	#else
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	#endif
	enable_RIT();													/* RIT enabled												*/
}
