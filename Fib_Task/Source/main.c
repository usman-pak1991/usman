/*
 * main.c
 *
 *  Created on: Mar 24, 2021
 *      Author: Muhammad Usman
 */

#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "mongoose.h"
#include "sensor_information.h"
#include "led_operation.h"

static const char *s_web_root_dir = "/usman/";
static const char *s_listening_address = "http://localhost:80";

static void http_server_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  //struct mg_http_serve_opts opts = {.root_dir = s_web_root_dir,.ssi_pattern = "#.shtml"};
  if (ev == MG_EV_HTTP_MSG)
  {
  	  #ifdef DEBUG
	  printf("\nRcvBuff: %s \r\n",c->recv.buf); // Debug Usman
	  #endif
	  rest_api_parse_header(c);
	  rest_api_perform_operation(c);
  }
}


void system_init(struct mg_mgr* _mgr)
{
	if ( wiringPiSetup() == -1 )
	{
		printf("\nGPIO Initialization FAILED!!!\n");
		exit(1);
	}
	sensor_initialization();
	led_initialization();
	mg_mgr_init(_mgr);
}

int main()
{
	pthread_t temperature_moniter_thread;
	struct mg_mgr mgr;
	printf(s_web_root_dir);
	system_init(&mgr);
	mg_http_listen(&mgr, s_listening_address, http_server_handler, &mgr);
	pthread_create(&temperature_moniter_thread, NULL, ( void* )moniter_temperature, NULL );
	for (;;) mg_mgr_poll(&mgr, 1000);
	mg_mgr_free(&mgr);

	return 1;
}
