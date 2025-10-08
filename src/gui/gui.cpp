/****************************************************************
 ******本文件用于实现lvgl在TFT上的移植
***************************************************************/
#include <lvgl.h>
#include "hardware/ec11.h"
#include "hardware/tft.h"
#include "hardware/motor.h"
#include "hardware/buzzer.h"
#include "gui_Screen.h"
#include "gui.h"
#include "gui_statusBar.h"

lv_disp_draw_buf_t draw_buf;
lv_color_t buf[SCREENWIDTH*BUFFERHEIGHT];
lv_indev_t *encoder_indev = NULL;
int32_t enc_diff;
// lv_group_t  * current_group;
lv_group_t  * current_group = NULL;
enum KeyRemap key_remap;


//显示刷新函数
static void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) 
{
    uint32_t w = (area->x2 - area->x1+1);
    uint32_t h = (area->y2 - area->y1+1);
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)(&(color_p->full)), (w * h));
    lv_disp_flush_ready(disp_drv);
}

//编码器输入扫描函数
static void my_encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    static int32_t last_encoder_value = 0;

    // 读取编码器值
    int32_t current_value = encoder.getCount();
    
    // 计算差值
    enc_diff = current_value - last_encoder_value;
    last_encoder_value = current_value;
    if(key_remap==KEY_NAVIGATION)
    {
        data->enc_diff = enc_diff;
    }  
    else
    {
         lv_obj_t *focused_obj = lv_group_get_focused(current_group);
         //发送按键值变化事件
         if(enc_diff>0)
         {
            lv_event_send(focused_obj, LV_EVENT_KEY,&enc_diff);
         }
         else
         {
            lv_event_send(focused_obj, LV_EVENT_KEY,&enc_diff);
         }
        data->enc_diff =0;//屏蔽导航
    }

    //读取按键值
    bool button_state = digitalRead(ENCODER_BUTTON_PIN);
    data->state = button_state ? LV_INDEV_STATE_REL : LV_INDEV_STATE_PR;
    if(enc_diff!=0|data->state==LV_INDEV_STATE_PR)
    {
        buzzer_key_play();
    }
}

//gui初始化函数
void gui_init(void)
{
    //lvgl初始化
    lv_init();
    //显示设备初始化
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, SCREENWIDTH*BUFFERHEIGHT);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    disp_drv.hor_res = SCREENWIDTH;
    disp_drv.ver_res = SCREENHEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    //输入设备初始化
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = my_encoder_read;
    encoder_indev = lv_indev_drv_register(&indev_drv);
    //映射为导航功能 
    key_remap = KEY_NAVIGATION;
    //创建主屏幕
    gui_screen_init();
    //切换到主屏幕
    lv_scr_load(main_screen);
    lv_indev_set_group(encoder_indev,main_screen_group);
    lv_group_focus_obj(pressure_thres_editor.cont);
    current_group=main_screen_group;
    /* 新增程序-----强制刷新 */
    lv_refr_now(NULL);
    Serial.println("强制刷新完成");
}

//gui任务
void gui_task(void *pvParameters)
 {
  const TickType_t xFrequency = pdMS_TO_TICKS(1);
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while (1) 
  {
    lv_timer_handler(); 
    vTaskDelayUntil( &xLastWakeTime, xFrequency);
  }
}

