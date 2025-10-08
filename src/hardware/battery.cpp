#include "battery.h"
#include "systemData.h"
#include <Arduino.h>

bool charge_flag=0;

void battery_setup(void)
{
    pinMode(CHARGE_FEEDBACH_PIN, INPUT);
}

void battery_task(void *pvParameters) 
{
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);// 周期为1000毫秒
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  int adcValue_total;
  for(;;)
  {
    if(digitalRead(CHARGE_FEEDBACH_PIN)==LOW) 
    {
       charge_flag=0;
    }
    else
    {
      charge_flag=1;
    }  
    for(uint16_t i=0;i<100;i++)
    {
      adcValue_total = adcValue_total+analogRead(CHARGE_ADINPUT_PIN);
      vTaskDelay(pdMS_TO_TICKS(1));
    }
    adcValue_total = adcValue_total/100;
    float voltage = adcValue_total*2*(3.3f/4095.0f);
    system_data.hardware_data.battery_percent = 100*(voltage-3)/(4.2-3);
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
    
}


