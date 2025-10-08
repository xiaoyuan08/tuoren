/****************************************************************
 ******本文件用于实现蜂鸣器驱动
***************************************************************/
#include <Arduino.h>
#include "hardware/buzzer.h"

//三级报警音配置（频率、时长、间隔、重复次数）
AlarmTone highAlarm = {2000, 200, 100, 10};   // 高频急促，紧急情况
AlarmTone mediumAlarm = {1500, 300, 300, 5};  // 中频间隔，中等紧急
AlarmTone lowAlarm = {1000, 500, 800, 3};    // 低频缓慢，一般提示

void buzzer_setup(void) 
{
  pinMode(BUZZER_PIN, OUTPUT);
}

void buzzer_key_play(void) 
{
  tone(BUZZER_PIN, 2000, 100);
}

// 播放指定级别的报警音
void playAlarm(AlarmTone alarm) 
{
  for (int i = 0; i < alarm.repeat; i++) 
  {
    tone(BUZZER_PIN, alarm.frequency, alarm.duration);
    delay(alarm.duration + alarm.interval);
  }
  noTone(BUZZER_PIN);  // 确保报警结束
}



