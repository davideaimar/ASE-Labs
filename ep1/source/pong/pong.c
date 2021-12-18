#include "pong.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h"
#include <math.h>

volatile uint16_t paddle_x = 100;
volatile uint16_t paddle_y = 320-42;
volatile float ball_x = 117;
volatile float ball_y = 320-47;
volatile double dir_angle = PI/4; 
volatile float sin_dir_angle;
volatile float cos_dir_angle;

void pong_init(){
	
	sin_dir_angle = sin(dir_angle);
	cos_dir_angle = cos(dir_angle);
	
	LCD_Clear(Black);
	
	// bouncing walls
	LCD_DrawRect( 0, 0, 240, 5, Red);
	LCD_DrawRect( 0, 0, 5, 320-42, Red);
	LCD_DrawRect( 235, 0, 5, 320-42, Red);
	
	
	// ball
	LCD_DrawRect( 117, 320-42-BALL_SIZE, BALL_SIZE, BALL_SIZE, Green);
	
	GUI_Text(0, 190, (uint8_t *) " press KEY1 to start the game ", White, Black);
	
	// enable RIT -> it implements button debouncing
	#ifdef SIMULATOR
	init_RIT(0x001E848);									/* RIT Initialization 5 msec       	*/
	#else
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	#endif
	enable_RIT();													/* RIT enabled												*/
}
