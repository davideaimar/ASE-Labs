
#ifndef __PONG_H 
#define __PONG_H

#include <inttypes.h>

#define PADDLE_MARGIN 32
#define PADDLE_WIDTH 80
#define PADDLE_HEIGHT 10
#define BALL_SIZE 5
#define WALL_SIZE 5
#define PI 3.141592654
#define TOP_PADDLE_SPEED 2


#define IS_GOOD_BORDER_LEFT ( GOING_LEFT && ball.x<=5) 
#define IS_GOOD_BORDER_RIGHT ( GOING_RIGHT &&  ball.x>=230)

#define GOING_DOWN ( ball.dir_angle > PI/2 && ball.dir_angle < (PI + PI/2) ) 
#define GOING_UP ( ball.dir_angle < PI/2 || ball.dir_angle > (PI + PI/2))
#define GOING_RIGHT ( ball.dir_angle > 0 && ball.dir_angle < PI ) 
#define GOING_LEFT ( ball.dir_angle < 2*PI && ball.dir_angle > PI )

#define IS_LOOSING_TOP ( ball.y <= 0 )
#define IS_LOOSING_BOTTOM ( ball.y >= MAX_Y-BALL_SIZE )
#define IS_BOUNCING ( (GOING_LEFT && IS_GOOD_BORDER_LEFT) || (GOING_RIGHT && IS_GOOD_BORDER_RIGHT) )
#define TOP_PADDLE_BOUNCING_RIGHT ( top_paddle_dir && player_top.paddle_x >= MAX_X-WALL_SIZE-PADDLE_WIDTH )
#define TOP_PADDLE_BOUNCING_LEFT ( !top_paddle_dir && player_top.paddle_x <= WALL_SIZE )
#define IS_HITTING_PADDLE_TOP ( GOING_UP && ( ball.y <= (player_top.paddle_y+PADDLE_HEIGHT) && ball.y > player_top.paddle_y ) && ((ball.x+BALL_SIZE) >= player_top.paddle_x && ball.x <= player_top.paddle_x + PADDLE_WIDTH) ) 
#define IS_HITTING_PADDLE_BOTTOM ( GOING_DOWN && ((ball.y+BALL_SIZE)>=player_bottom.paddle_y&&(ball.y+BALL_SIZE)<player_bottom.paddle_y+10) && ((ball.x+BALL_SIZE) >= player_bottom.paddle_x && ball.x <= player_bottom.paddle_x + PADDLE_WIDTH) ) 
#define IS_HITTING_PADDLE (IS_HITTING_PADDLE_TOP || IS_HITTING_PADDLE_BOTTOM)

// EDIT HERE TO OPTIMIZE SPEED AND AUDIO FOR EMULATOR
#ifdef SIMULATOR
#define SPEED 4
#define N_SIN 1
#define LOW_TONE 7000 // (use scaling on timer2)
#define HIGH_TONE 5000 // (use scaling on timer2)
#else
#define SPEED 1
#define N_SIN 15
#define LOW_TONE 2000
#define HIGH_TONE 400
#endif

typedef struct {
	uint16_t paddle_x;
	uint16_t paddle_y;
	int points;
} Player;

typedef struct {
	float x;
	float y;
	uint16_t int_x;
	uint16_t int_y;
	float x_hit_top;
	double dir_angle; 
	float sin_dir_angle;
	float cos_dir_angle;
	float spin_speed; // used in range 1.0 to 2.0 to give a boost when bouncing on the edge
} Ball;

void pong_init(void);
void manage_loosing(void);
void manage_bounce(void);
float clamp(float d, int min, int max);

#endif
