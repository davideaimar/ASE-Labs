#include "pong.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h"
#include <math.h>
#include <stdio.h>

volatile uint16_t paddle_x = 100;
volatile uint16_t paddle_y = 320-42;
volatile float ball_x;
volatile float ball_y;
volatile double dir_angle; 
volatile float sin_dir_angle;
volatile float cos_dir_angle;
volatile int points = 0;
volatile int max_score = 100;
volatile uint8_t enable_int0 = 0;
volatile uint8_t enable_int1 = 0;
volatile uint8_t enable_key2= 0;
volatile float spin_speed = 1.0;

void pong_init(){
	char str[5];
	
	ball_x = MAX_X-10;
	ball_y = MAX_Y/2;
	dir_angle = PI + PI/4; 
	spin_speed = 1.0;
	
	sin_dir_angle = sin(dir_angle);
	cos_dir_angle = cos(dir_angle);
	
	LCD_Clear(Black);
	
	// bouncing walls
	LCD_DrawRect( 0, 0, MAX_X, 5, Red); // top border
	LCD_DrawRect( 0, 0, 5, MAX_Y-42, Red); // left border
	LCD_DrawRect( MAX_X-5, 0, 5, MAX_Y-42, Red); // right border
	
	
	// ball
	LCD_DrawRect( floor(ball_x), floor(ball_y), BALL_SIZE, BALL_SIZE, Green);
	// paddle
	LCD_DrawRect( paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Green);
	
	GUI_Text(40, 300, (uint8_t *) " press KEY1 to play ", White, Black);
	GUI_Text(130, 30, (uint8_t *) "Record:  ", White, Black);
	sprintf(str, "%d", max_score);
	GUI_Text(203, 30, (uint8_t *) str, White, Black);
	GUI_Text(20, 155, (uint8_t *) "Score: ", White, Black);
	
	// enable timer1 -> it implements potentiometer readings
	enable_timer(1);	
	enable_int1 = 1;
	
	// restart the RIT because sometimes it gets stuck 
	disable_RIT();
	reset_RIT();
	enable_RIT();
}

float clamp(float d, int min, int max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}

void manage_loosing(){
	disable_timer(0);
	disable_timer(1);
	enable_key2 = 0;
	if (points > max_score){
		max_score = points;
	}
	points = 0;
	GUI_Text(85, 195, (uint8_t *) " You Lose ", White, Black);
	GUI_Text(40, 300, (uint8_t *) " press INT0 to reset ", White, Black);
	enable_int0 = 1;
}

void emit_tone(int intensity){
	disable_timer(2);
	reset_timer(2);
	init_timer(2,intensity);
	enable_timer(2);
}

void manage_bounce(){
	if ( IS_GOOD_BORDER_LEFT || IS_GOOD_BORDER_RIGHT ){
		emit_tone(LOW_TONE);
		dir_angle = fmod(2*PI-dir_angle + 2*PI, 2*PI);
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
	}
	else if ( IS_BORDER_TOP && GOING_UP ){
		emit_tone(LOW_TONE);
		dir_angle = fmod(PI - dir_angle + 2*PI, 2*PI);
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
	}
	else if ( IS_HITTING_PADDLE ){
		char str[5];
		int rel_x = floor(ball_x)-paddle_x; // ball x position relative to paddle
		emit_tone(HIGH_TONE);
		// this removes horizontal bouncing by limiting the angle
		rel_x = clamp(rel_x, 5, 75);
		// map rel_x=5..40 -> spin=2..0 and rel_x=40...75 -> spin=0..2
		if (rel_x < 40)
			spin_speed = 2 - ((float)rel_x-5)/35;
		else if ( rel_x > 40 )
			spin_speed = 1 + ((float)rel_x-40)/35;
		else{
			spin_speed = 1;
			rel_x++; // this remove the perfectly vertical bounce (too easy otherwise)
		}
		// map hitting pixel to relative angle
		dir_angle = fmod((((rel_x) * PI) / (PADDLE_WIDTH)) + 3*PI/2 + 2*PI, 2*PI);
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
		points = points >= 100 ? points+10 : points+5;
		sprintf(str, "%d", points);
		GUI_Text(80, 155, (uint8_t *) str, White, Black);
	}
}
