/****************************************************************
 ******本文件用于实现压力传感器驱动
***************************************************************/
#include <HX711.h>
#include <Arduino.h>
#include "pressure.h"
#include "systemData.h"
#include "hardware/buzzer.h"
#include "hardware/motor.h"
#include "hardware/quality.h"

HX711 quality_hx711;
float scale = 509.6;//比例因子：用于将读数转换为实际重量
float offset = 56350;// 偏移量：用于消除零点误差

void quality_setup(void)
{
  pinMode(34, INPUT);//将34引脚浮空
  quality_hx711.begin(QUALITY_SDA_PIN, QUALITY_SCK_PIN,64);
}

void quality_task(void *pvParameters) 
{
  long tem;
  const TickType_t xFrequency = pdMS_TO_TICKS(250);
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for(;;)
  {
    tem=-quality_hx711.read_average(50);
    system_data.sensor_data.value_quality=(tem-offset)/scale;
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}



