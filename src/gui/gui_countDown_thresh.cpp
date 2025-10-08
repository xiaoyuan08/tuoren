#include <lvgl.h>
#include "gui_countDown_thresh.h"
#include "gui_Screen.h"
#include "gui.h"
#include "systemData.h"
#include "gui_statusBar.h"
#include "gui_pressure_thresh.h"

CountDownEditor count_down_editor;

lv_group_t  *countdown_group;
lv_obj_t *placeholder;


static lv_obj_t * gui_count_down_value_create(lv_obj_t * parent, uint16_t x,uint16_t y);
static void count_down_cont_event_handler(lv_event_t* e);
static void count_down_value_event_handler(lv_event_t* e); 
static lv_obj_t * gui_count_down_value_create(lv_obj_t  *parent, uint16_t x,uint16_t y);
static void count_down_editor_timer_callback(lv_timer_t *timer);
static void count_down_cont_event_handler(lv_event_t* e);
static void count_down_btn_event_handler(lv_event_t* e);


  /**************************
  * 倒计时创建函数
  *************************/
LV_FONT_DECLARE(hansans_18);
void gui_countDown_thresh_init(lv_obj_t * parent)
{ 
    //定时器
    count_down_editor.timer = lv_timer_create(count_down_editor_timer_callback, 5000, NULL);

    //容器
    count_down_editor.cont = lv_obj_create(parent);
    lv_obj_align(count_down_editor.cont,LV_ALIGN_TOP_LEFT,GUI_PRESSURE_THRSH_WIDTH+6,STATUS_BAR_HEIGHT);
    lv_obj_set_width(count_down_editor.cont , COUNTDOWN_THRESH_WIDTH);
    lv_obj_set_height(count_down_editor.cont , COUNTDOWN_THRESH_HEIGHT);
    lv_obj_set_style_border_width(count_down_editor.cont,2,LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(count_down_editor.cont,2,LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(count_down_editor.cont,COUNTDOWN_COLOR,LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(count_down_editor.cont,GUI_MARGIN_COLOR_FOCUS,LV_STATE_FOCUSED);
    lv_obj_set_style_pad_all(count_down_editor.cont,0,0);
    lv_obj_set_style_bg_color(count_down_editor.cont,GUI_BG_COLOR,0);
    lv_obj_set_style_radius(count_down_editor.cont, 4, LV_PART_MAIN | LV_STATE_DEFAULT);   
    lv_obj_clear_flag(count_down_editor.cont,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(count_down_editor.cont,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(count_down_editor.cont,count_down_cont_event_handler,LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(count_down_editor.cont,count_down_cont_event_handler,LV_EVENT_FOCUSED,NULL);
    //标题
    count_down_editor.title = lv_label_create(count_down_editor.cont);
    lv_obj_set_width(count_down_editor.title, LV_SIZE_CONTENT);   
    lv_obj_set_height(count_down_editor.title, LV_SIZE_CONTENT);   
    lv_obj_align(count_down_editor.title,LV_ALIGN_TOP_LEFT,3,3);
    lv_label_set_text(count_down_editor.title, "倒计时");
    lv_obj_set_style_text_font(count_down_editor.title, &hansans_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(count_down_editor.title,COUNTDOWN_COLOR,0);
    lv_obj_set_style_border_color(count_down_editor.title,COUNTDOWN_COLOR,LV_STATE_DEFAULT);
    //时
    count_down_editor.hour = gui_count_down_value_create(count_down_editor.cont,3,-3);
    lv_obj_set_style_bg_color(count_down_editor.hour,lv_palette_main(LV_PALETTE_GREY),LV_STATE_FOCUSED);
    //第一个冒号
    lv_obj_t * label_intervel1 = lv_label_create(count_down_editor.cont);
    lv_obj_align(label_intervel1,LV_ALIGN_BOTTOM_LEFT,30,-4);
    lv_obj_set_style_text_font(label_intervel1, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label_intervel1,":");
    lv_obj_set_style_text_color(label_intervel1,COUNTDOWN_COLOR,0);
    //分
    count_down_editor.min = gui_count_down_value_create(count_down_editor.cont,3+30,-3);
    lv_obj_set_style_bg_color(count_down_editor.min,lv_palette_main(LV_PALETTE_GREY),LV_STATE_FOCUSED);
    //第二个冒号
    lv_obj_t * label_intervel2 = lv_label_create(count_down_editor.cont);
    lv_obj_align(label_intervel2,LV_ALIGN_BOTTOM_LEFT,58,-4);
    lv_obj_set_style_text_font(label_intervel2, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(label_intervel2,":");
    lv_obj_set_style_text_color(label_intervel2,COUNTDOWN_COLOR,0);
    //秒
    count_down_editor.second = gui_count_down_value_create(count_down_editor.cont,3+30*2,-3);
    lv_obj_set_style_bg_color(count_down_editor.second,lv_palette_main(LV_PALETTE_GREY),LV_STATE_FOCUSED); 
    //返回按钮
    count_down_editor.btn = lv_label_create(count_down_editor.cont);
    lv_obj_set_size(count_down_editor.btn,75,30);   
    lv_label_set_text(count_down_editor.btn, "返回");
    lv_obj_set_style_text_font(count_down_editor.btn, &hansans_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(count_down_editor.btn,COUNTDOWN_COLOR,0);
    lv_obj_clear_flag(count_down_editor.btn,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(count_down_editor.btn,LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_add_flag(count_down_editor.btn,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_border_width(count_down_editor.btn,1, LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(count_down_editor.btn,COUNTDOWN_COLOR,LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(count_down_editor.btn,GUI_MARGIN_COLOR_FOCUS,LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(count_down_editor.btn,lv_palette_main(LV_PALETTE_GREY),LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(count_down_editor.btn,125,LV_STATE_FOCUSED);
    lv_obj_add_event_cb(count_down_editor.btn, count_down_btn_event_handler, LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(count_down_editor.btn, count_down_btn_event_handler, LV_EVENT_FOCUSED,NULL);
    lv_obj_align(count_down_editor.btn,LV_ALIGN_TOP_RIGHT,-4,3);
   
    count_down_editor.bar = lv_bar_create(count_down_editor.cont);
    lv_bar_set_value(count_down_editor.bar,25, LV_ANIM_OFF);
    lv_bar_set_start_value(count_down_editor.bar, 0, LV_ANIM_OFF);
    lv_obj_set_width(count_down_editor.bar, COUNTDOWN_BAR_WIDTH);
    lv_obj_set_height(count_down_editor.bar, COUNTDOWN_BAR_HEIGHT);
    lv_obj_align(count_down_editor.bar, LV_ALIGN_BOTTOM_RIGHT,-8,-5);
    lv_obj_set_style_border_width(count_down_editor.bar,2,0);
    lv_obj_set_style_border_color(count_down_editor.bar,COUNTDOWN_COLOR,0);
    lv_obj_set_style_bg_color(count_down_editor.bar,GUI_BG_COLOR,LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(count_down_editor.bar,COUNTDOWN_COLOR,LV_PART_INDICATOR);
    //控制组
    countdown_group = lv_group_create();
    lv_group_add_obj(countdown_group,count_down_editor.hour);
    lv_group_add_obj(countdown_group,count_down_editor.min);
    lv_group_add_obj(countdown_group,count_down_editor.second);
    lv_group_add_obj(countdown_group,count_down_editor.btn);
}

  /**************************
  * 倒计时刷新函数
  *************************/
 void gui_count_down_fresh(void)
 {
    uint8_t hour,min,second;
    system_data.count_down_time.getTime(hour,min,second);
    lv_label_set_text_fmt(count_down_editor.hour,"%02d",hour);
    lv_label_set_text_fmt(count_down_editor.min,"%02d",min);
    lv_label_set_text_fmt(count_down_editor.second,"%02d",second);
    lv_bar_set_value(count_down_editor.bar,system_data.count_down_time.getPercent(), LV_ANIM_OFF);
 }

 /**************************
  * 数值创建函数
  *************************/
 static lv_obj_t * gui_count_down_value_create(lv_obj_t  *parent, uint16_t x,uint16_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_align(label,LV_ALIGN_BOTTOM_LEFT,x,y);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(label,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(label,LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_add_flag(label,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_border_color(label,GUI_MARGIN_COLOR_FOCUS, LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(label,1, 0);
    lv_obj_set_style_text_color(label,COUNTDOWN_COLOR,0);
    lv_obj_set_style_bg_opa(label,200,LV_STATE_FOCUSED);
    lv_obj_add_event_cb(label, count_down_value_event_handler,LV_EVENT_FOCUSED,NULL);
    lv_obj_add_event_cb(label, count_down_value_event_handler, LV_EVENT_CLICKED,NULL);
    lv_obj_add_event_cb(label, count_down_value_event_handler, LV_EVENT_KEY, &enc_diff);
    return label;
}


  /**************************
  * 容器回调函数函数
  *************************/
static void count_down_cont_event_handler(lv_event_t* e) 
{
    lv_event_code_t  code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        lv_indev_set_group(encoder_indev,countdown_group);
        lv_group_focus_obj(count_down_editor.hour);
        current_group=countdown_group;
    }
    else if (code==LV_EVENT_FOCUSED)
    {
        lv_timer_resume(count_down_editor.timer);
        lv_timer_reset(count_down_editor.timer);
    }
}

//时间数值回调函数
static void count_down_value_event_handler(lv_event_t* e) 
{
    lv_event_code_t  code = lv_event_get_code(e);
    lv_obj_t *obj =  lv_event_get_target(e);
    if(code==LV_EVENT_CLICKED)
    {
        if(key_remap == KEY_VALUECHANGE) 
        {
            key_remap=KEY_NAVIGATION;
            lv_obj_set_style_bg_color(obj,GUI_BG_COLOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj,255, LV_PART_MAIN | LV_STATE_DEFAULT);
            system_data.count_down_time.start();
        }
        else if(key_remap == KEY_NAVIGATION) 
        {
            key_remap=KEY_VALUECHANGE;
            lv_obj_set_style_bg_color(obj,lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj,255, LV_PART_MAIN | LV_STATE_DEFAULT);
            system_data.count_down_time.stop();
        }
    }
    else if(code==LV_EVENT_KEY)
    {
      uint8_t hour,min,second;
      system_data.count_down_time.getTime(hour,min,second);
      if(obj==count_down_editor.hour)
      {
        hour += *(int32_t*)lv_event_get_param(e);
      }
      else if (obj==count_down_editor.min)
      {
        min+= *(int32_t*)lv_event_get_param(e);
      }
      else if(obj==count_down_editor.second)
      {
        second+= *(int32_t*)lv_event_get_param(e);
      }
     system_data.count_down_time.setTime(hour,min,second);
    }

    else if(code == LV_EVENT_FOCUSED) 
    {
        lv_timer_reset(count_down_editor.timer);
        lv_timer_resume(count_down_editor.timer);
        Serial.print("启计时");
    }
}

//时间阈值返回键
static void count_down_btn_event_handler(lv_event_t* e) 
{
    lv_event_code_t  code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        lv_indev_set_group(encoder_indev,main_screen_group);
        lv_group_focus_obj(count_down_editor.btn);
        current_group=main_screen_group;
        lv_obj_clear_state(count_down_editor.btn, LV_STATE_FOCUSED);
    }
    else if (code==LV_EVENT_FOCUSED)
    {
        lv_timer_resume(count_down_editor.timer);
        lv_timer_reset(count_down_editor.timer);
    }
}

static void count_down_editor_timer_callback(lv_timer_t *timer) 
{
    lv_timer_pause(count_down_editor.timer);
    lv_obj_clear_state(count_down_editor.cont,LV_STATE_FOCUSED);
    lv_obj_clear_state(count_down_editor.hour,LV_STATE_FOCUSED);
    lv_obj_clear_state(count_down_editor.min,LV_STATE_FOCUSED);
    lv_obj_clear_state(count_down_editor.second,LV_STATE_FOCUSED);
    lv_obj_clear_state(count_down_editor.btn,LV_STATE_FOCUSED);
}


