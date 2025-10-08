#ifndef __EC11_H
#define __EC11_H

#include <ESP32Encoder.h>

#define ENCODER_A_PIN 26
#define ENCODER_B_PIN 25
#define ENCODER_BUTTON_PIN 32

extern ESP32Encoder encoder;
extern int32_t encoder_delta;
extern bool encoder_button_pressed;
extern bool encoder_button_released;
extern bool encoder_button_prev_state;
void ec11_setup(void);

#endif


