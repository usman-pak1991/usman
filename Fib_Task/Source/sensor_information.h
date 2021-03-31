/*
 * sensor_infomration.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Muhammad Usman
 */

#ifndef SENSOR_INFORMATION_H_
#define SENSOR_INFORMATION_H_

#include "rest_api.h"

#define DHT_PIN		7	// GPIO-04
#define MAX_TIMINGS 85  // Unknown for nows

typedef struct Sensor_Information
{
	float temperature_threshold;
	float current_temperature;
	char *config_file;
}Sensor_Information;

void sensor_initialization();
void set_temperature_threshold(float);
String get_temperature_threshold();
String get_temperature_current();
void moniter_temperature();						// Execute as thread
void generate_alarm_temperature();
void get_dht11_information();




#endif /* SENSOR_INFORMATION_H_ */
