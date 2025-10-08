#ifndef __BUZZER_H
#define __BUZZER_H

#define BUZZER_PIN 15

// 定义各级报警音参数
struct AlarmTone {
  int frequency;     // 频率(Hz)
  int duration;      // 单次鸣叫持续时间(ms)
  int interval;      // 间隔时间(ms)
  int repeat;        // 重复次数
};

extern AlarmTone highAlarm;   // 高频急促，紧急情况
extern AlarmTone mediumAlarm;  // 中频间隔，中等紧急
extern AlarmTone lowAlarm ;    // 低频缓慢，一般提示

void buzzer_setup(void);
void buzzer_key_play(void);
void playAlarm(AlarmTone alarm);

#endif 







