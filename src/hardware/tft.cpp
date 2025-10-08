/****************************************************************
 ******本文件用于实现TFTLCD驱动
***************************************************************/
#include "tft.h"
#include <TFT_eSPI.h> // 引入TFT_eSPI库
TFT_eSPI tft = TFT_eSPI(); // 创建TFT对象

// 设置ILI9341对比度
void setContrast(uint8_t contrast) 
{
  tft.writecommand(0x25);       // ILI9341对比度指令
  tft.writedata(contrast);      // 写入对比度值（0-255）
}

void tft_setup(void) 
{
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);  //打开背光
  
  tft.begin();
  tft.setRotation(3); // 设置显示方向（0-3）
  setContrast(255);
}


