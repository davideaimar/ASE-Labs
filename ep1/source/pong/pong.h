
#ifndef __PONG_H 
#define __PONG_H

#define PADDLE_WIDTH 80
#define PADDLE_HEIGHT 10
#define BALL_SIZE 5
#define WALL_SIZE 5
#define PI 3.141592654


#define IS_GOOD_BORDER_LEFT ( GOING_LEFT && ball_x<=5 && ball_y<=320-47 ) 
#define IS_GOOD_BORDER_RIGHT ( GOING_RIGHT &&  ball_x>=230&&ball_y<=320-47 )
#define IS_BORDER_TOP ( GOING_UP && ball_y<=5 )

#define GOING_DOWN ( dir_angle > PI/2 && dir_angle < (PI + PI/2) ) 
#define GOING_UP ( dir_angle < PI/2 || dir_angle > (PI + PI/2))
#define GOING_RIGHT ( dir_angle > 0 && dir_angle < PI ) 
#define GOING_LEFT ( dir_angle < 2*PI && dir_angle > PI )

#define IS_LOOSING ( ball_y>MAX_Y-32-PADDLE_HEIGHT )
#define IS_BOUNCING ( (GOING_LEFT && IS_GOOD_BORDER_LEFT) || (GOING_UP && IS_BORDER_TOP) || (GOING_RIGHT && IS_GOOD_BORDER_RIGHT) )
#define IS_HITTING_PADDLE ( GOING_DOWN && ((ball_y+BALL_SIZE)>=paddle_y&&(ball_y+BALL_SIZE)<paddle_y+10) && ((ball_x+BALL_SIZE) >= paddle_x && ball_x <= paddle_x + PADDLE_WIDTH) ) 


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

void pong_init(void);
void manage_loosing(void);
void manage_bounce(void);
float clamp(float d, int min, int max);

#endif
