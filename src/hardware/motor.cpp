/****************************************************************
 ******本文件用于实现电机驱动
***************************************************************/
#include <AccelStepper.h>
#include "motor.h"

enum MotorState motor_state;
EventGroupHandle_t xEventGroup_motor;


AccelStepper stepper(AccelStepper::FULL4WIRE, motorPin1_1, motorPin3_1, motorPin2_1, motorPin4_1);

void motor_setup(void) 
{
  stepper.setMaxSpeed(500.0); 
  stepper.setAcceleration(100.0);
  stepper.setSpeed(800);  
  stepper.runSpeed();            
  stepper.setCurrentPosition(0);
  
  pinMode(BUTTON_LIMIT_PIN, INPUT_PULLUP);
  pinMode(HAND_SWITCH_PIN, INPUT_PULLUP);

  xEventGroup_motor = xEventGroupCreate();
  motor_state = STOP;
}

void motor_zero_find(void) 
{
  while(motor_limit_read()==0)
  { 
    rotateDegrees(300);
  }
  delay(3000);
  rotateDegrees(-10*360);
}

bool motor_limit_read(void) 
{
  if(digitalRead(BUTTON_LIMIT_PIN)==0) return 1;
  else return 0;
}

bool motor_hand_read(void) 
{
  if(digitalRead(HAND_SWITCH_PIN)==0) return 1;
  else return 0;
}

void rotateDegrees(float degrees) 
{
  float steps = -degrees / DEGREES_PER_STEP;
  if(motor_state == STOP)
  {
      stepper.moveTo(stepper.currentPosition() + steps);
      motor_state=RUNNING;
      xEventGroupSetBits(xEventGroup_motor, EVENT_BIT_MOTOR); 
  }
}

void motor_task(void *pvParameters) 
{
  EventBits_t uxBits;
  for(;;)
  {
     uxBits = xEventGroupWaitBits
     (
      xEventGroup_motor,  // 事件组句柄
      EVENT_BIT_MOTOR,    // 等待的事件位
      pdTRUE,             // 退出前清除这些位
      pdTRUE,             // 逻辑与模式(所有位都设置才返回)
      portMAX_DELAY       // 无限等待
    );
    
    if((uxBits&EVENT_BIT_MOTOR)== EVENT_BIT_MOTOR) 
    {
      while (stepper.distanceToGo() != 0) 
      {
        stepper.run();
        vTaskDelay(1);
      }
      motor_state=STOP;
    }
  }
}


