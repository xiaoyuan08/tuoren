#include "systemData.h"
#include "dataConfig.h"
SystemData system_data;


void system_data_init(void)
{   //硬件数据初始化
    system_data.hardware_data.machine_code=MACHINE_CODE;
    system_data.hardware_data.machine_id=MACHINE_ID;
    system_data.hardware_data.battery_percent=0;
    system_data.hardware_data.rssi = 0;
    system_data.hardware_data.value_forward=0;
    system_data.hardware_data.value_backward=0;
    //配置数据初始化
    system_data.count_down_time.setTime(DEFAULT_TIME_THRESHOLD,0,0);
    system_data.count_down_time.start();
    system_data.config_data.pressure_threshold = DEFAULT_PRESS_THRESHOLD;
    system_data.count_down_time.setTotalTime(DEFAULT_MAX_TIME,0,0);
    //传感器数据初始化
    system_data.sensor_data.value_press=0;
    system_data.sensor_data.value_quality=0;
    system_data.sensor_data.value_3 = 0;
    system_data.sensor_data.value_4=0;
}
