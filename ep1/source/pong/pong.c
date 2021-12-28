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
volatile uint16_t int_ball_x;
volatile uint16_t int_ball_y;
volatile double dir_angle; 
volatile float sin_dir_angle;
volatile float cos_dir_angle;
volatile int points = 0;
volatile int max_score = 100;
volatile uint8_t enable_int0 = 0;
volatile uint8_t enable_int1 = 0;
volatile uint8_t enable_key2= 0;
volatile float spin_speed = 1.0; // used in range 1.0 to 2.0 to give a boost when bouncing on the edge
volatile uint8_t num_sin = 0;
volatile uint8_t ticks = 0;

void pong_init(){
	char str[5];
	
	// initial position of the ball
	int_ball_x = ball_x = MAX_X-WALL_SIZE-BALL_SIZE;
	int_ball_y = ball_y = MAX_Y/2;
	spin_speed = 1.0; 
	
	dir_angle = PI + PI/4; 
	sin_dir_angle = sin(dir_angle);
	cos_dir_angle = cos(dir_angle);
	
	LCD_Clear(Black);
	
	// bouncing walls
	LCD_DrawRect( 0, 0, MAX_X, 5, Red); // top border
	LCD_DrawRect( 0, 0, 5, MAX_Y-32-PADDLE_HEIGHT, Red); // left border
	LCD_DrawRect( MAX_X-WALL_SIZE, 0, WALL_SIZE, MAX_Y-32-PADDLE_HEIGHT, Red); // right border
	
	
	// ball
	LCD_DrawRect( int_ball_x, int_ball_y, BALL_SIZE, BALL_SIZE, Green);
	// paddle
	LCD_DrawRect( paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Green);
	
	sprintf(str, "%d", max_score);
	GUI_Text(205, 5, (uint8_t *) str, White, Black);
	GUI_Text(40, 300, (uint8_t *) " press KEY1 to play ", White, Black);
	
	// enable timer1 -> it implements potentiometer readings
	enable_timer(1);	
	enable_int1 = 1;
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
	ticks=0;
	num_sin=0;
	enable_timer(2);
}

void normalize_dir_angle(){
	while(dir_angle<0) dir_angle+=2*PI;
	while(dir_angle>=2*PI) dir_angle-=2*PI;
}

void manage_bounce(){
	if ( IS_GOOD_BORDER_LEFT || IS_GOOD_BORDER_RIGHT ){
		emit_tone(LOW_TONE);
		dir_angle = 2*PI-dir_angle;
		normalize_dir_angle();
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
	}
	else if ( IS_BORDER_TOP && GOING_UP ){
		emit_tone(LOW_TONE);
		dir_angle = PI - dir_angle;
		normalize_dir_angle();
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
	}
	else if ( IS_HITTING_PADDLE ){
		char str[5];
		int rel_x = int_ball_x-paddle_x; // ball x position relative to paddle
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
		dir_angle = (((rel_x) * PI) / (PADDLE_WIDTH)) + 3*PI/2;
		normalize_dir_angle();
		sin_dir_angle = sin(dir_angle);
		cos_dir_angle = cos(dir_angle);
		points = points >= 100 ? points+10 : points+5;
		sprintf(str, "%d", points);
		GUI_Text(5, 155, (uint8_t *) str, White, Black);
	}
}
