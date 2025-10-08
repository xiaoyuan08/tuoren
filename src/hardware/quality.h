#ifndef __QUALITY_H
#define __QUALITY_H

#define QUALITY_SDA_PIN 17
#define QUALITY_SCK_PIN 5

#define VALUE_MAX 16777216.0
#define VALUE_REF 2.645

void quality_setup(void);
void quality_task(void *pvParameters);

#endif

