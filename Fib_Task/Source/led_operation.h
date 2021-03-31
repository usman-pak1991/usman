/*
 * led_operation.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Muhammad Usman
 */

#ifndef LED_OPERATION_H_
#define LED_OPERATION_H_


#define BLUE_LED	12	/* GPIO-18 */
#define RED_LED		32	/* GPIO-12 */
#define LED_ON 		1
#define LED_OFF		0

typedef struct Led
{
	int red;
	int blue;
}Led;

void led_initialization();
void set_red_led_on();
void set_red_led_off();
void set_blue_led_on();
void set_blue_led_off();
int get_led_status(int);


#endif /* LED_OPERATION_H_ */
