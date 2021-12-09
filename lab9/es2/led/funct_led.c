/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_led.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        High level led management functions
** Correlated files:    lib_led.c, funct_led.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "led.h"
#include "../timer/timer.h"
#include <stdbool.h>

#define LED_NUM     8                   /* Number of user LEDs                */
const unsigned long led_mask[] = { 1UL<<0, 1UL<<1, 1UL<<2, 1UL<< 3, 1UL<< 4, 1UL<< 5, 1UL<< 6, 1UL<< 7 };

extern unsigned char led_value;
unsigned int led_number;

/*----------------------------------------------------------------------------
	** Function name:		calc_time
	** Descriptions:		Calculate K value to assign to MR* from time in msec at 25MHz
	**
	** parameters:			time wanted in msec
	** Returned value:		corresponding constant value
	**
 *----------------------------------------------------------------------------*/
int calc_time(float msec){
	unsigned int frequency = 25000000;
	float sec = msec/1000;
	return (int)(sec*frequency);
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On(unsigned int num) {
 
  LPC_GPIO2->FIOPIN |= led_mask[num];
	led_value = LPC_GPIO2->FIOPIN;
}


/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off(unsigned int num) {

  LPC_GPIO2->FIOPIN &= ~led_mask[num];
	led_value = LPC_GPIO2->FIOPIN;
}


/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;
	
  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
	led_value = LPC_GPIO2->FIOPIN;
	
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED at requested lum
 *----------------------------------------------------------------------------*/
void LED_On_Lum(unsigned int num, unsigned int lum) {
	unsigned int t_on;
	unsigned int t_off;
	bool use_timer = true;  
	
	LED_Off(led_number);
	disable_timer(2);
	reset_timer(2);
	switch(lum){
		case 0:
			use_timer = false;
			LED_Off(num);
			break;
		case 25:
			t_on = calc_time(0.3);
			t_off = calc_time(1);
			break;
		case 50:
			t_on = calc_time(0.65);
			t_off = calc_time(0.65);
			break;
		case 75:
			t_on = calc_time(0.1);
			t_off = calc_time(0.3);
			break;
		case 100:
			use_timer = false;
			LED_On(num);
			break;
		default:
			use_timer = false;
			LED_On(num);
			break;
	}
	if (use_timer){
		led_number = num;
		init_timer(2, t_on, t_on+t_off);
		enable_timer(2);
	}
  LPC_GPIO2->FIOPIN |= led_mask[num];
	led_value = LPC_GPIO2->FIOPIN;
}
