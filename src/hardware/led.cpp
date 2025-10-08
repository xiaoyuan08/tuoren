#include "led.h"
#include <Arduino.h>


void led_steup(void)
{
    // 将默认TX引脚(GPIO1)作为输出
    pinMode(LED_R_PIN, OUTPUT);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);
}

void led_task(void *pvParameters) 
{
  for(;;)
  {
    digitalWrite(LED_R_PIN, LOW);
    digitalWrite(LED_G_PIN, LOW);
    digitalWrite(LED_B_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(2000));
    digitalWrite(LED_R_PIN, HIGH);
    digitalWrite(LED_G_PIN, HIGH);
    digitalWrite(LED_B_PIN, HIGH);
    Serial.print("循环\n");
  }
}


