/*
 * rest_api.c
 *
 *  Created on: Mar 31, 2021
 *      Author: Muhammad Usman
 */
#include <stdio.h>
#include <stdlib.h>
#include "mongoose.h"
#include "rest_api.h"
#include "sensor_information.h"
#include "led_operation.h"

static REST_Api rest_api;

void print_rest_api()
{
	printf("\r\n REST_API_DEBUG_INFO\r\n");
	printf("\nMETHOD: %s\r\n",rest_api.method.ptr);
	printf("\nURI: %s\r\n",rest_api.uri.ptr);
	printf("\nBODY: %s\r\n",rest_api.body.ptr);
	printf("\nINFORMATION_TYPE: %s\r\n",rest_api.information_type.ptr);
	printf("\nDEVICE_ID: %s\r\n",rest_api.device_id.ptr);
	printf("\nOPERATION: %s\r\n",rest_api.operation.ptr);
	printf("\nVALUE_KEWORD: %s\r\n",rest_api.value_keyword.ptr);
	printf("\nVALUE: %s\r\n",rest_api.value.ptr);

}

void copy_mg_str_to_String(String *to, struct mg_str* from)
{
	to->len = from->len;
	to->ptr = malloc(to->len+1);
	to->ptr[to->len]='\0';
	memcpy(to->ptr,from->ptr,from->len);
}

const char* parse_String(const char *start, const char *end, const char *d,String *value)
{
	struct mg_str local_buf;
	local_buf.ptr = start;
	while (start < end && *start != '\n' && strchr(d, *start) == NULL) start++;
	local_buf.len = start - local_buf.ptr;
	while (start < end && strchr(d, *start) != NULL) start++;
	copy_mg_str_to_String(value,&local_buf);
	return start;
}

void rest_api_parse_header(struct mg_connection *con)
{
	struct mg_http_message hm;
	//char *local_con_buf = malloc(con->recv.len);
	//memcpy(local_con_buf,con->recv.buf)
	mg_http_parse((char *) con->recv.buf, con->recv.len, &hm);
	copy_mg_str_to_String(&(rest_api.method),&(hm.method));
	copy_mg_str_to_String(&(rest_api.uri),&(hm.uri));
	copy_mg_str_to_String(&(rest_api.query),&(hm.query));
    copy_mg_str_to_String(&(rest_api.body),&(hm.body));
	if(hm.uri.len > 2)
	{
		const char *end = rest_api.uri.ptr + rest_api.uri.len;
		const char *s = parse_String(&(rest_api.uri.ptr[1]), end, "/", &(rest_api.information_type));
		s = parse_String(s, end, "/", &(rest_api.device_id));
		s = parse_String(s, end, "\0", &(rest_api.operation));

		end = rest_api.body.ptr + rest_api.body.len;
		s = parse_String(rest_api.body.ptr, end, "=",&(rest_api.value_keyword));
		s = parse_String(s, end, "\0", &(rest_api.value));
	}
	#ifdef DEBUG
	print_rest_api();
	#endif
}
void rest_api_perform_operation(struct mg_connection *con)
{
	String null_response;
	null_response.len = 1;
	null_response.ptr = malloc(1);
	null_response.ptr[0] = '\0';
	if(strncmp(rest_api.method.ptr,"GET",rest_api.method.len)==0)
	{
		if(strncmp(rest_api.information_type.ptr,"sensor",rest_api.information_type.len)==0)
		{
			if(strncmp(rest_api.device_id.ptr,"temperature",rest_api.device_id.len)==0)
			{
				if(strncmp(rest_api.operation.ptr,"current",rest_api.operation.len)==0)
				{
					rest_api_send_responce(con,"200 OK",get_temperature_current());
				}
				else if(strncmp(rest_api.operation.ptr,"threshold",rest_api.operation.len)==0)
				{
					rest_api_send_responce(con,"200 OK",get_temperature_threshold());
				}
				else
				{
					rest_api_send_responce(con,"403 BAD-Request",null_response);
				}
			}
			else
			{
				rest_api_send_responce(con,"403 BAD-Request",null_response);
			}
		}
		else
		{
			rest_api_send_responce(con,"403 BAD-Request",null_response);
		}
	}
	else if(strncmp(rest_api.method.ptr,"POST",rest_api.method.len)==0)
	{
		if(strncmp(rest_api.information_type.ptr,"sensor",rest_api.information_type.len)==0)
		{
			if(strncmp(rest_api.device_id.ptr,"temperature",rest_api.device_id.len)==0)
			{
				if(strncmp(rest_api.operation.ptr,"threshold",rest_api.operation.len)==0)
				{
					if(strncmp(rest_api.value_keyword.ptr,"value",rest_api.value_keyword.len)==0)
					{
						set_temperature_threshold(atof(rest_api.value.ptr));
						rest_api_send_responce(con,"200 OK",null_response);
					}
					else
					{
						rest_api_send_responce(con,"403 BAD-Request",null_response);
					}
				}
				else
				{
					rest_api_send_responce(con,"403 BAD-Request",null_response);
				}
			}
			else
			{
				rest_api_send_responce(con,"403 BAD-Request",null_response);
			}
		}
		else if(strncmp(rest_api.information_type.ptr,"led",rest_api.information_type.len)==0)
		{
			if(strncmp(rest_api.device_id.ptr,"red",rest_api.device_id.len)==0)
			{
				if(strncmp(rest_api.value_keyword.ptr,"value",rest_api.value_keyword.len)==0)
				{
					int led_value=0;
					led_value = atoi(rest_api.value.ptr);
					if(led_value==LED_ON)
					{
						set_red_led_on();
						rest_api_send_responce(con,"200 OK - RED LED-ON",null_response);
					}
					else if(led_value==LED_OFF)
					{
						set_red_led_off();
						rest_api_send_responce(con,"200 OK - BLUE LED-OFF",null_response);
					}
					else
					{
						rest_api_send_responce(con,"403 BAD-Request",null_response);
					}

				}
				else
				{
					rest_api_send_responce(con,"403 BAD-Request",null_response);
				}
			}
			else
			{
				rest_api_send_responce(con,"403 BAD-Request",null_response);
			}
		}
		else
		{
			rest_api_send_responce(con,"403 BAD-Request",null_response);
		}
	}

}
void rest_api_send_responce(struct mg_connection *con,char* http_code,String message)
{
	mg_printf(con, "HTTP/1.1 %s\r\n"
						  "Content-Type: text/html\r\n"
						  "Content-Length: %d\r\n\r\n"
						  "%s\r\n",http_code,message.len,message.ptr);
}


