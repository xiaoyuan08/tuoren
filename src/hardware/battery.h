#ifndef _BATTERY_H
#define _BATTERY_H

#define CHARGE_FEEDBACH_PIN 2
#define CHARGE_ADINPUT_PIN 36

extern bool charge_flag;

void battery_task(void *pvParameters);
void battery_setup(void);

#endif


