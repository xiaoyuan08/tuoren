/****************************************************************
 ******本文件用于实现压力传感器驱动
***************************************************************/
#include <HX711.h>
#include <Arduino.h>
#include "pressure.h"
#include "systemData.h"
#include "hardware/buzzer.h"
#include "hardware/motor.h"

HX711 hx711_pressure;

void pressure_setup(void)
{
  hx711_pressure.begin(HX711_SDA_PIN, HX711_SCK_PIN,128);
}

void pressure_task(void *pvParameters) 
{
  long tem;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(250);
  for(;;)
  {
    tem=hx711_pressure.read_average(10);
    system_data.sensor_data.value_press = -(1000000*(tem/VALUE_MAX)*(VALUE_REF/128.0))/(5*VALUE_REF);
    if(system_data.sensor_data.value_press>system_data.config_data.pressure_threshold)
    {
      playAlarm(highAlarm);
      rotateDegrees(-1000);
    }
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

