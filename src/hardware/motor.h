#ifndef __MOTOR_H
#define __MOTOR_H
#include <AccelStepper.h>

#define motorPin1_1 13    // IN1
#define motorPin2_1 12    // IN2
#define motorPin3_1 14    // IN3
#define motorPin4_1 27    // IN4

#define EVENT_BIT_MOTOR (1 << 1) 
#define BUTTON_LIMIT_PIN  33
#define HAND_SWITCH_PIN  35
#define STEPS_PER_REVOLUTION 2048
#define DEGREES_PER_STEP (360.0 / STEPS_PER_REVOLUTION)


enum MotorState{RUNNING, STOP};
extern AccelStepper stepper;
extern EventGroupHandle_t xEventGroup_motor;

void motor_setup(void);
bool motor_limit_read(void);
bool motor_hand_read(void);
void motor_zero_find(void);
void rotateDegrees(float degrees);
void motor_task(void *pvParameters);

#endif


