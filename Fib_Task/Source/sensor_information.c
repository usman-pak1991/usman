/*
 * sensor_information.c
 *
 *  Created on: Mar 30, 2021
 *      Author: Muhammad Usman
 */

#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include "sensor_information.h"
#include "led_operation.h"
#include "rest_api.h"

static Sensor_Information sensor_information;




void set_temperature_threshold(float _temperature_threshold)
{

	FILE *file = fopen(sensor_information.config_file,"w");
	fprintf(file,"%.1f",_temperature_threshold);
	sensor_information.temperature_threshold = _temperature_threshold;
	fclose(file);
}

String get_temperature_threshold()
{
	char temperature_threshold_str[5];
	String temperature_threshold;
	FILE *config_file = fopen(sensor_information.config_file,"r");
	if(config_file==NULL)
	{
		set_temperature_threshold(sensor_information.temperature_threshold);
	}
	else
	{
		fscanf(config_file,"%s",temperature_threshold_str);
		temperature_threshold_str[4] = '\0';
		//printf("\n%s\n",temperature_threshold_str);
		sensor_information.temperature_threshold = atof(temperature_threshold_str);
		//printf("\nDefault Value: %.1f\n",sensor_information.temperature_threshold);
		fclose(config_file);
	}

	temperature_threshold.len = 4;
	temperature_threshold.ptr = malloc(temperature_threshold.len+1);
	sprintf(temperature_threshold.ptr,"%.1f",sensor_information.temperature_threshold);
	temperature_threshold.ptr[temperature_threshold.len] = '\0';
	#ifdef DEBUG
	printf("\n%s : %.1f\n",temperature_threshold.ptr,sensor_information.temperature_threshold);
	#endif
	return temperature_threshold;
}


String get_temperature_current()
{
	String current_temperature;
	current_temperature.len = 5;
	current_temperature.ptr = malloc(current_temperature.len+1);
	sprintf(current_temperature.ptr,"%.1f",sensor_information.current_temperature);
	current_temperature.ptr[current_temperature.len] = '\0';

	#ifdef DEBUG
	printf("\n%s : %.1f\n",current_temperature.ptr,sensor_information.current_temperature);
	#endif

	return current_temperature;
}

void sensor_initialization()
{
	sensor_information.config_file = malloc(100);
	sensor_information.config_file = "/usman/temperature-sensor-config.txt";
	sensor_information.temperature_threshold = 26.0; // default threshold value;
	get_temperature_threshold();
}

void generate_alarm_temperature()
{
	(sensor_information.current_temperature <= sensor_information.temperature_threshold)? set_blue_led_off():set_blue_led_on();
}

void get_dht11_information()
{
		uint8_t laststate	= HIGH;
		uint8_t counter		= 0;
		uint8_t j			= 0, i;
		int data[5] = { 0, 0, 0, 0, 0 };
		data[0] = data[1] = data[2] = data[3] = data[4] = 0;

		/* pull pin down for 18 milliseconds */
		pinMode( DHT_PIN, OUTPUT );
		digitalWrite( DHT_PIN, LOW );
		delay( 18 );

		/* prepare to read the pin */
		pinMode( DHT_PIN, INPUT );

		/* detect change and read data */
		for ( i = 0; i < MAX_TIMINGS; i++ )
		{
			counter = 0;
			while ( digitalRead( DHT_PIN ) == laststate )
			{
				counter++;
				delayMicroseconds( 1 );
				if ( counter == 255 )
				{
					break;
				}
			}
			laststate = digitalRead( DHT_PIN );

			if ( counter == 255 )
				break;

			/* ignore first 3 transitions */
			if ( (i >= 4) && (i % 2 == 0) )
			{
				/* shove each bit into the storage bytes */
				data[j / 8] <<= 1;
				if ( counter > 16 )
					data[j / 8] |= 1;
				j++;
			}
		}

		/*
		 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
		 * print it out if data is good
		 */
		if ( (j >= 40) &&
			 (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
		{
			/*float h = (float)((data[0] << 8) + data[1]) / 10;
			if ( h > 100 )
			{
				h = data[0];	// for DHT11
			}*/
			float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
			if ( c > 125 )
			{
				c = data[2];	// for DHT11
			}
			if ( data[2] & 0x80 )
			{
				c = -c;
			}
			//float f = c * 1.8f + 32;
			sensor_information.current_temperature = c;
			//.humidity = h;
			#ifdef DEBUG
			printf("\r\n -- %.1f",c);
			#endif
		}
		else
		{
			#ifdef DEBUG
			printf("\r\n -- INVALID");
			#endif
		}
}

void moniter_temperature()
{
	while(1)
	{
		get_dht11_information();
		generate_alarm_temperature();
		delay(2000);
	}

}



