/*
 * rest_api.h
 *
 *  Created on: Mar 30, 2021
 *      Author: Muhammad Usman
 */

#ifndef REST_API_H_
#define REST_API_H_

#include "mongoose.h"
/*
 * method			:	GET, PUT/POST
 * information_type	:	sensor-info,led-op,
 * operation		:	temp-thold,temp-curr,red=on,red=off
 * possible_REST_api:	/sensor-info/temp-curr	[Method Support: GET]
 * 						/sensor-info/temp-thold [Method Support: GET,PUT/POST]
 * 						/led-op/red=on			[Method Support: PUT/POST]
 * 						/led-op/red=off			[Method Support: PUT/POST]
 */
#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define METHOD_PUT	"PUT"



typedef struct String
{
	char* ptr;
	size_t len;
}String;

typedef struct REST_Api
{
	String method;
	String uri;
	String query;
	String body;
	String information_type;
	String device_id;
	String operation;
	String value;
	String value_keyword;

}REST_Api;

void copy_mg_str_to_String(String*, struct mg_str*);
const char* parse_String(const char*, const char*, const char*,String*);
void rest_api_parse_header(struct mg_connection *);
void rest_api_perform_operation();
void rest_api_send_responce(struct mg_connection *con,char* http_code,String message);


#endif /* REST_API_H_ */
