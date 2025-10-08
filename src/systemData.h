#ifndef  SYSTEM_DATA_H
#define  SYSTEM_DATA_H
#include "systemData.h"
#include "CountDown.h"
#include <Arduino.h>

typedef struct ConfigData
{
    float pressure_threshold;//压力阈值
    float value_time_threshold;//时间阈值
}ConfigData;

typedef struct SensorData
{
    float value_press;//膀胱压力
    float value_quality;//总尿量
    float value_3;//模拟值3
    float value_4;//模拟值4
}SensorData;

typedef struct HardwareData
{
    int machine_code;//机器码
    int machine_id;//机器ID
    float value_forward;//电机前进圈数
    float value_backward;//电机后退圈数
    int32_t  rssi; //wifi信号轻度
    uint8_t  battery_percent; //电量百分比
}HardwareData;

typedef struct TimeData
{
    //日期时间
    uint16_t date_year;
    uint16_t date_month;
    uint16_t date_day;

    uint16_t date_hour;
    uint16_t date_min;
    uint16_t date_second;
}TimeData;

//系统数据结构
typedef struct SystemData
{
    ConfigData config_data;
    SensorData sensor_data;
    HardwareData hardware_data;
    TimeData time_data;
    CountDown count_down_time;
} SystemData;

extern SystemData system_data;
void system_data_init(void);

#endif 
