#include "pong.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../GLCD/GLCD.h"
#include <math.h>
#include <stdio.h>

volatile Player player_top;
volatile Player player_bottom;
volatile Ball ball;

volatile uint8_t enable_int0 = 0;
volatile uint8_t enable_int1 = 0;
volatile uint8_t enable_key2= 0;
volatile uint8_t num_sin = 0;
volatile uint8_t ticks = 0;

void pong_init(){
	
	//player_bottom initialization
	player_bottom.paddle_x = 100;
	player_bottom.paddle_y = 320-PADDLE_MARGIN-PADDLE_HEIGHT;
	player_bottom.points = 0;
	
	//player_top initialization
	player_top.paddle_x = 100;
	player_top.paddle_y = PADDLE_MARGIN;
	player_top.points = 0;
	
	// initial position of the ball
	ball.x = MAX_X-WALL_SIZE-BALL_SIZE;
	ball.y = MAX_Y/2;
	ball.int_x = MAX_X-WALL_SIZE-BALL_SIZE;
	ball.int_y = MAX_Y/2;
	ball.spin_speed = 1;
	ball.dir_angle = PI + PI/4;
	ball.sin_dir_angle = sin(PI + PI/4);
	ball.cos_dir_angle = cos(PI + PI/4);
	
	LCD_Clear(Black);
	
	// bouncing walls
	LCD_DrawRect( 0, 0, WALL_SIZE, MAX_Y, Red); // left border
	LCD_DrawRect( MAX_X-WALL_SIZE, 0, WALL_SIZE, MAX_Y, Red); // right border
	
	
	// ball
	LCD_DrawRect( ball.int_x, ball.int_y, BALL_SIZE, BALL_SIZE, Green);
	// paddle top
	LCD_DrawRect( player_top.paddle_x, player_top.paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Green);
	// paddle bottom
	LCD_DrawRect( player_bottom.paddle_x, player_bottom.paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, Green);
	
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
	
	if ( IS_LOOSING_TOP )
		GUI_Text(85, 195, (uint8_t *) " You Win ", White, Black);
	else
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
	while(ball.dir_angle<0) ball.dir_angle+=2*PI;
	while(ball.dir_angle>=2*PI) ball.dir_angle-=2*PI;
}

void manage_bounce(){
	if ( IS_GOOD_BORDER_LEFT || IS_GOOD_BORDER_RIGHT ){
		emit_tone(LOW_TONE);
		ball.dir_angle = 2*PI-ball.dir_angle;
		normalize_dir_angle();
		ball.sin_dir_angle = sin(ball.dir_angle);
		ball.cos_dir_angle = cos(ball.dir_angle);
	}
	else if ( IS_HITTING_PADDLE ){
		char str[5]; 
		int rel_x;
		if ( IS_HITTING_PADDLE_BOTTOM ){
			rel_x = ball.int_x-player_bottom.paddle_x; // ball x position relative to paddle
			player_bottom.points = player_bottom.points >= 100 ? player_bottom.points+10 : player_bottom.points+5;
			sprintf(str, "%d", player_bottom.points);
			GUI_Text(5, 155, (uint8_t *) str, White, Black);
			ball.dir_angle = (((rel_x) * PI) / (PADDLE_WIDTH)) + 3*PI/2;
		}
		else{
			rel_x = PADDLE_WIDTH - (ball.int_x - player_top.paddle_x);
			player_top.points = player_top.points >= 100 ? player_top.points+10 : player_top.points+5;
			sprintf(str, "%d", player_top.points);
			GUI_Text(MAX_X-30, 155, (uint8_t *) str, White, Black);
			ball.dir_angle = (((rel_x) * PI) / (PADDLE_WIDTH)) + PI/2;
		}
		emit_tone(HIGH_TONE);
		// this removes horizontal bouncing by limiting the angle
		rel_x = clamp(rel_x, 5, 75);
		// this adds a pseudo-random behavior that remove game loops
		LPC_RIT->RICOUNTER % 2 == 0 ? rel_x++ : rel_x--;
		// map rel_x=5..40 -> spin=2..1 and rel_x=40...75 -> spin=1..2
		if (rel_x < 40)
			ball.spin_speed = 2 - ((float)rel_x-5)/35;
		else if ( rel_x > 40 )
			ball.spin_speed = 1 + ((float)rel_x-40)/35;
		else{
			ball.spin_speed = 1;
			rel_x++; // this remove the perfectly vertical bounce (too easy otherwise)
		}
		// map hitting pixel to relative angle
		normalize_dir_angle();
		ball.sin_dir_angle = sin(ball.dir_angle);
		ball.cos_dir_angle = cos(ball.dir_angle);
	}
}
