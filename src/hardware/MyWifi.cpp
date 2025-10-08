#include "MyWifi.h"
#include <Wifi.h>
#include "systemData.h"
#include <Arduino.h>

#define SSID "HNTR"
#define PASSWORD "tuoren@123"

void wifi_setup(void)
{
  //初始化Wifi物理连接
  Serial.println("Connecting to WiFi\n");
  Serial.print("SSID: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) 
  {
    delay(250);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("wifi连接成功\n");
    Serial.print("IP: \n");
    Serial.println(WiFi.localIP());
  } 
  else
  {
    Serial.println("wifi连接失败\n");
  }
}

void wifi_task(void *pvParameters) 
{
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);// 周期为500毫秒
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  int rssi_total;
  for(;;)
  {
    //WIFI重连
    if (WiFi.status() == WL_CONNECTED)
    {
      rssi_total=0;
      for(uint16_t i=0;i<500;i++)
      {
        rssi_total = rssi_total+WiFi.RSSI();  //通过RSSI()函数获取wifi信号强度
        vTaskDelay(1);
      }
      system_data.hardware_data.rssi = rssi_total/500;
    } 
    else
    {
      Serial.println("WIFI断开\n");
      system_data.hardware_data.rssi=-1000;
      WiFi.begin(SSID, PASSWORD);
    }
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}



