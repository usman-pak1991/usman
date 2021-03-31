/*
 * led_operation.c
 *
 *  Created on: Mar 30, 2021
 *      Author: Muhammad Usman
 */

#include <stdio.h>
#include <wiringPi.h>
#include "led_operation.h"

static Led led;

void led_initialization()
{
	pinMode( BLUE_LED, OUTPUT );
	pinMode( RED_LED, OUTPUT );
}
void set_red_led_on()
{
	led.red = HIGH;
	digitalWrite( RED_LED, HIGH );
}
void set_red_led_off()
{
	led.red = LOW;
	digitalWrite( RED_LED, LOW );
}
void set_blue_led_on()
{
	led.blue = HIGH;
	digitalWrite( BLUE_LED, HIGH );
}
void set_blue_led_off()
{
	led.blue = LOW;
	digitalWrite( BLUE_LED, LOW );
}
//int get_led_status(int);
