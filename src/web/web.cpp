#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <ArduinoJSON.h>
#include <Arduino.h>
#include "web.h"
#include "systemData.h"

HTTPClient http;

void extractDateParts(String timestampStr, uint16_t &year, uint16_t &month, uint16_t &day) {
  // 初始化默认值（若解析失败，使用0作为无效标识，因uint16_t不能为负数）
  year = 0;
  month = 0;
  day = 0;

  // 1. 找到空格位置，分割出日期部分（如"2025-08-26"）
  uint16_t spaceIndex = timestampStr.indexOf(' ');
  if (spaceIndex == -1) {
    Serial.println("错误：时间字符串格式不正确（无空格）");
    return;
  }
  String datePart = timestampStr.substring(0, spaceIndex); // 截取空格前的部分

  // 2. 找到第一个连字符（年和月之间）
  uint16_t dash1 = datePart.indexOf('-');
  if (dash1 == -1) {
    Serial.println("错误：日期部分格式不正确（无第一个连字符）");
    return;
  }

  // 3. 找到第二个连字符（月和日之间）
  uint16_t dash2 = datePart.indexOf('-', dash1 + 1); // 从第一个连字符后开始查找
  if (dash2 == -1) {
    Serial.println("错误：日期部分格式不正确（无第二个连字符）");
    return;
  }

  // 4. 截取年、月、日字符串并转换为整数
  String yearStr = datePart.substring(0, dash1);
  String monthStr = datePart.substring(dash1 + 1, dash2);
  String dayStr = datePart.substring(dash2 + 1);

  year = yearStr.toInt();
  month = monthStr.toInt();
  day = dayStr.toInt();

  // 验证范围（可选）
  if (year < 2000 || year > 2100) { // 合理年份范围
    Serial.println("警告：年份数值超出合理范围");
  }
  if (month < 1 || month > 12) { // 月份范围1-12
    Serial.println("警告：月份数值超出合理范围（1-12）");
  }
  if (day < 1 || day > 31) { // 日期范围1-31（简化验证，未考虑月份天数差异）
    Serial.println("警告：日期数值超出合理范围（1-31）");
  }
}

void extractTimeParts(String timestampStr, uint16_t &hour, uint16_t &minute, uint16_t &second) {
  hour = 0;
  minute = 0;
  second = 0;
  uint16_t spaceIndex = timestampStr.indexOf(' ');
  if (spaceIndex == -1) {
    Serial.println("错误：时间字符串格式不正确（无空格）");
    return;
  }
  String timePart = timestampStr.substring(spaceIndex + 1);
  uint16_t colon1 = timePart.indexOf(':');
  if (colon1 == -1) {
    Serial.println("错误：时间部分格式不正确（无第一个冒号）");
    return;
  }
  uint16_t colon2 = timePart.indexOf(':', colon1 + 1);
  if (colon2 == -1) {
    Serial.println("错误：时间部分格式不正确（无第二个冒号）");
    return;
  }
  String hourStr = timePart.substring(0, colon1);
  String minuteStr = timePart.substring(colon1 + 1, colon2);
  String secondStr = timePart.substring(colon2 + 1);
  hour = hourStr.toInt();
  minute = minuteStr.toInt();
  second = secondStr.toInt();
  if (hour > 23 || minute > 59 || second > 59) {
    Serial.println("警告：时间数值超出合理范围");
  }
}

void http_request(void) 
{
  http.begin(SEVERNAME);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String httpData = 
  +"&machineCode=" + String(system_data.hardware_data.machine_code)
  +"&machineId=" + String(system_data.hardware_data.machine_id)
  +"&pressure=" + String(system_data.sensor_data.value_press,2)
  + "&totalVolume=" + String(system_data.sensor_data.value_quality,2)
  + "&value3="+String(system_data.sensor_data.value_3,2)
  + "&value4=" + String(system_data.sensor_data.value_4,2);

  int httpCode = http.POST(httpData);
  if (httpCode > 0) 
  {
    String response = http.getString();
    DynamicJsonDocument doc(1024);  // 创建JSON文档（容量根据实际需求调整）
    deserializeJson(doc, response);
    JsonVariant timestampVar = doc["timestamp"];
    String timestampStr = String(timestampVar.as<const char*>());
    extractDateParts(timestampStr,(system_data.time_data.date_year),(system_data.time_data.date_month),(system_data.time_data.date_day));
    extractTimeParts(timestampStr,(system_data.time_data.date_hour),(system_data.time_data.date_min),(system_data.time_data.date_second));
  }

  else
  {
    //Serial.print("http连接失败");
  }

  http.end();
}

void web_task(void *pvParameters) 
{
  const TickType_t xFrequency = pdMS_TO_TICKS(1000);
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for(;;)
  {
    http_request();
    vTaskDelayUntil( &xLastWakeTime, xFrequency );
  }
}

