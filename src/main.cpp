#include <Arduino.h>
#include "hardware/pressure.h"
#include "hardware/quality.h"
#include "hardware/ec11.h"
#include "hardware/motor.h"
#include "hardware/tft.h"
#include "hardware/buzzer.h"
#include "hardware/MyWifi.h"
#include "hardware/battery.h"
#include "hardware/led.h"
#include "hardware/laser.h"
#include "gui/gui.h"
#include "web/web.h"
#include "CountDown.h"
#include "systemData.h"
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"
#include "driver/ledc.h"

//任务句柄
TaskHandle_t Task_GUI;
TaskHandle_t Task_Pressure;
TaskHandle_t Task_Quality;
TaskHandle_t Task_MOTOR;
TaskHandle_t Task_WEB;
TaskHandle_t Task_Wifi;
TaskHandle_t Task_Flow;
TaskHandle_t Task_Web;
TaskHandle_t Task_Battery;
TaskHandle_t Task_Led;
TimerHandle_t xTimer_count_down;
11111
// void showBootAnimation() {
//     // 使用 TFT 驱动直接绘图（不要用 LVGL！）
//     tft_fillScreen(TFT_BLACK);
//     tft_drawCentreString("智能设备启动中...", 120, 60, 2);
//     tft_drawString("初始化硬件...", 10, 100, 1);
//     tft_fillRect(10, 120, 220, 10, TFT_DARKGREY);  // 进度条背景

//     // 可选：加一个小动画或 logo
//     // tft_pushImage(50, 150, 120, 60, (uint16_t*)boot_logo);
// }

void hardware_init(void)
{
  setCpuFrequencyMhz(240);
  esp_task_wdt_init(50, true); 
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);  // 全局设置衰减（部分版本支持）
  analogSetWidth(12);

  /* 先初始化屏幕，再加载硬件，防止出现视觉性的开机启动慢 */
  buzzer_setup();
  system_data_init();
  tft_setup();
  gui_init();
  wifi_setup();
  motor_setup();
  pressure_setup();
  // tft_setup();
  quality_setup();
  ec11_setup();
  battery_setup();
  // buzzer_setup();
  // system_data_init();
  //laser_setup();
}

void my_tasks_init(void)
{
  //lvgl任务
  xTaskCreatePinnedToCore(
  gui_task,   
  "gui_task",      
  4096,        
  NULL,         
  1,          
  &Task_GUI,   
  0             
  );
    
  //创建电机任务
  xTaskCreatePinnedToCore(
  motor_task,   
  "motor_task", 
  2048,       
  NULL,        
  2,            
  &Task_MOTOR, 
  1            
  );

  //创建压力采集任务
  xTaskCreatePinnedToCore(
  pressure_task,    
  "pression_task", 
  1024,       
  NULL,        
  1,           
  &Task_Pressure,  
  1       
  );

  //创建质量采集任务
  xTaskCreatePinnedToCore(
  quality_task,    
  "quality_task", 
  1024,       
  NULL,        
  1,           
  &Task_Quality,  
  1             
  );

  //创建web任务
  xTaskCreatePinnedToCore(
  web_task,    
  "web_task", 
  2048,       
  NULL,        
  2,           
  &Task_Web,  
  1             
  );

  //创建wifi任务
  xTaskCreatePinnedToCore(
  wifi_task,    
  "wifi_task", 
  1024,       
  NULL,        
  1,           
  &Task_Wifi,  
  1             
  );

  //创建电池任务
  xTaskCreatePinnedToCore(
  battery_task,    
  "battery_task", 
  1024,       
  NULL,        
  1,           
  &Task_Battery,  
  1             
  );

  //创建倒计时任务
  xTimer_count_down = xTimerCreate
  (
      "countDown",
      pdMS_TO_TICKS(1000),
      pdTRUE,                  
      (void*) 1,           
      vTimerCallback        
  );
  xTimerStart(xTimer_count_down, 0 );
}

void setup(void) 
{
  hardware_init();
  // gui_init();
  // system_data_init();
  my_tasks_init();
}


void loop(void)
{

}

