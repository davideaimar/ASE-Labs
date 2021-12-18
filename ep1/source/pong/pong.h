
#ifndef __PONG_H 

#define __PONG_H

#define PADDLE_WIDTH 80
#define PADDLE_HEIGHT 10
#define BALL_SIZE 5
#define PI 3.141592654


#define IS_GOOD_BORDER_LEFT ( ball_x<=10&&ball_y<=320-47 ) 
#define IS_GOOD_BORDER_RIGHT ( ball_x>=230&&ball_y<=320-47 )
#define IS_BORDER_TOP ( ball_y<=10 )

#define GOING_DOWN ( dir_angle > PI/2 && dir_angle < (PI + PI/2) ) 
#define GOING_UP ( dir_angle < PI/2 || dir_angle > (PI + PI/2))
#define GOING_RIGHT ( dir_angle > 0 && dir_angle < (PI + PI/2) ) 
#define GOING_LEFT ( dir_angle < PI/2 || dir_angle > (PI + PI/2))

#define IS_BOUNCING ( (GOING_LEFT && IS_GOOD_BORDER_LEFT) || (GOING_UP && IS_BORDER_TOP) || (GOING_RIGHT && IS_GOOD_BORDER_RIGHT) )
#define IS_HITTING_PADDLE ( GOING_DOWN && ((ball_y+BALL_SIZE)>=paddle_y&&(ball_y+BALL_SIZE)<paddle_y+10) && ((ball_x+BALL_SIZE) >= paddle_x && ball_x <= paddle_x + PADDLE_WIDTH) ) 

#ifdef SIMULATOR
#define SPEED 10
#else
#define SPEED 1
#endif

void pong_init(void);

#endif
