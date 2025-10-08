#ifndef __WEB_H
#define __WEB_H
#include <HTTPClient.h>

#define SEVERNAME "http://172.31.15.13:8082/api/sensor-data"

void web_setup(void);
void web_task(void *pvParameters);

extern HTTPClient http;

#endif

