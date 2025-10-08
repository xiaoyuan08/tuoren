#ifndef __PRESSURE_H
#define __PRESSURE_H

#define HX711_SDA_PIN 16
#define HX711_SCK_PIN 4


#define VALUE_MAX 16777216.0
#define VALUE_REF 2.645

void pressure_setup(void);
void pressure_task(void *pvParameters);

#endif

