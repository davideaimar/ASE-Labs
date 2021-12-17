#include "pong.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h"

volatile uint16_t paddle_x = 100;
volatile uint16_t paddle_y = 320-42;
volatile uint16_t ball_x = 117;
volatile uint16_t ball_y = 320-47;
volatile double dir_angle = PI/4; 

void pong_init(){
	LCD_Clear(Black);
	GUI_Text(0, 190, (uint8_t *) " press KEY1 to start the game ", White, Black);
	
	// bouncing walls
	LCD_DrawRect( 0, 0, 240, 5, Red);
	LCD_DrawRect( 0, 0, 5, 320-42, Red);
	LCD_DrawRect( 235, 0, 5, 320-42, Red);
	
	
	// ball
	LCD_DrawRect( 117, 320-42-BALL_SIZE, BALL_SIZE, BALL_SIZE, Green);
	
	// enable RIT -> it implements button debouncing
	#ifdef SIMULATOR
	init_RIT(0x001E848);									/* RIT Initialization 5 msec       	*/
	#else
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	#endif
	enable_RIT();													/* RIT enabled												*/
	
	// enable timer1 -> it implements potentiometer readings
	#ifdef SIMULATOR
	init_timer(1, 0x00C4);
	#else
	init_timer(1, 0x004C4);							 
	#endif
	enable_timer(1);
}
