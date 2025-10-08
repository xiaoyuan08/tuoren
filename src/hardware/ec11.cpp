/****************************************************************
 ******本文件用于实现编码器驱动
***************************************************************/
#include <ESP32Encoder.h>
#include <Arduino.h>
#include "ec11.h"

ESP32Encoder encoder;
int32_t encoder_delta;
bool encoder_button_pressed;
bool encoder_button_released;
bool encoder_button_prev_state;

void ec11_setup(void)
{
  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);

  encoder.attachSingleEdge(ENCODER_A_PIN, ENCODER_B_PIN);
  encoder.setFilter(100);
  encoder.clearCount();
}

