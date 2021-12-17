
#ifndef __PONG_H 

#define __PONG_H

#define PADDLE_WIDTH 80
#define PADDLE_HEIGHT 10
#define BALL_SIZE 5
#define PI 3.141592654

#ifdef SIMULATOR
#define SPEED 10
#else
#define SPEED 1
#endif

void pong_init(void);

#endif
