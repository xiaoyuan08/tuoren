#include <lvgl.h>
#include "systemData.h"
#include "gui.h"
#include "gui_pressure_thresh.h"
#include "gui_statusBar.h"

PressureThresEditor pressure_thres_editor;

//压力阈值回调函数
static void event_handler_pressure_thres_cont(lv_event_t* e) 
{
    lv_event_code_t  code = lv_event_get_code(e);
    if(code==LV_EVENT_CLICKED)
    {
        if(key_remap == KEY_VALUECHANGE) 
        {
            key_remap=KEY_NAVIGATION;
            lv_obj_set_style_bg_color(pressure_thres_editor.value, GUI_BG_COLOR, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(pressure_thres_editor.value,255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else if(key_remap == KEY_NAVIGATION) 
        {
            key_remap=KEY_VALUECHANGE;
            lv_obj_set_style_bg_color(pressure_thres_editor.value,lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(pressure_thres_editor.value,255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }

    else if(code==LV_EVENT_KEY)
    {
      system_data.config_data.pressure_threshold = system_data.config_data.pressure_threshold+=
      *(int32_t*)lv_event_get_param(e);
    }

    else if(code==LV_EVENT_FOCUSED)
    {
        lv_timer_reset(pressure_thres_editor.timer);
        lv_timer_resume(pressure_thres_editor.timer);
    }
}

static void pressure_thresh_editor_timer_callback(lv_timer_t *timer) 
{
    lv_timer_pause(timer);
    lv_obj_clear_state(pressure_thres_editor.cont,LV_STATE_FOCUSED);
}

LV_FONT_DECLARE(hansans_18);
void gui_pressure_thresh_editor_init(lv_obj_t * parent)
{
    pressure_thres_editor.cont = lv_obj_create(parent);
    lv_obj_remove_style_all(pressure_thres_editor.cont );
    lv_obj_set_width( pressure_thres_editor.cont,GUI_PRESSURE_THRSH_WIDTH);
    lv_obj_set_height(pressure_thres_editor.cont,GUI_PRESSURE_THRSH_HEIGHT);
    lv_obj_align(pressure_thres_editor.cont,LV_ALIGN_TOP_LEFT,2,STATUS_BAR_HEIGHT);   
 
    lv_obj_set_style_border_color(pressure_thres_editor.cont ,GUI_PRESSURE_THRES_COLOR,LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(pressure_thres_editor.cont ,GUI_MARGIN_COLOR_FOCUS,LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(pressure_thres_editor.cont ,2,LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(pressure_thres_editor.cont ,2,LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(pressure_thres_editor.cont,GUI_BG_COLOR,0);
    
    lv_obj_set_style_radius(pressure_thres_editor.cont, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(pressure_thres_editor.cont,GUI_PRESSURE_THRES_MARGIN,0);

    lv_obj_clear_flag(pressure_thres_editor.cont ,LV_OBJ_FLAG_SCROLLABLE); 
    lv_obj_add_flag(pressure_thres_editor.cont ,LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(pressure_thres_editor.cont , event_handler_pressure_thres_cont, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(pressure_thres_editor.cont , event_handler_pressure_thres_cont, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(pressure_thres_editor.cont , event_handler_pressure_thres_cont, LV_EVENT_KEY, &enc_diff);

    pressure_thres_editor.title = lv_label_create(pressure_thres_editor.cont);
    lv_obj_set_width(pressure_thres_editor.title, LV_SIZE_CONTENT);
    lv_obj_set_height(pressure_thres_editor.title, LV_SIZE_CONTENT);
    lv_obj_align(pressure_thres_editor.title, LV_ALIGN_TOP_LEFT,0,0);
    lv_label_set_text(pressure_thres_editor.title, "压力阈值");
    lv_obj_set_style_text_font(pressure_thres_editor.title, &hansans_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(pressure_thres_editor.title,GUI_PRESSURE_THRES_COLOR,0);

    pressure_thres_editor.unit  = lv_label_create(pressure_thres_editor.cont);
    lv_obj_set_width(pressure_thres_editor.unit, LV_SIZE_CONTENT); 
    lv_obj_set_height(pressure_thres_editor.unit, LV_SIZE_CONTENT);    
    lv_obj_align(pressure_thres_editor.unit, LV_ALIGN_BOTTOM_RIGHT,0,0);
    lv_label_set_text(pressure_thres_editor.unit, "mmHg");
    lv_obj_set_style_text_font(pressure_thres_editor.unit , &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(pressure_thres_editor.unit,GUI_PRESSURE_THRES_COLOR,0);

    pressure_thres_editor.value = lv_label_create(pressure_thres_editor.cont);
    lv_obj_set_width(pressure_thres_editor.value , LV_SIZE_CONTENT);
    lv_obj_set_height(pressure_thres_editor.value , LV_SIZE_CONTENT);
    lv_obj_align(pressure_thres_editor.value,LV_ALIGN_BOTTOM_LEFT,20,0);
    lv_obj_set_style_text_font(pressure_thres_editor.value , &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(pressure_thres_editor.value ,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(pressure_thres_editor.value ,LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_obj_set_style_text_color(pressure_thres_editor.value,GUI_PRESSURE_THRES_COLOR,0);
    lv_obj_set_style_bg_color(pressure_thres_editor.value,lv_palette_main(LV_PALETTE_GREY),LV_STATE_FOCUSED);
    //定时器
    pressure_thres_editor.timer = lv_timer_create(pressure_thresh_editor_timer_callback,5000,NULL);
}

void gui_pressure_thresh_refresh(void)
{
    lv_label_set_text_fmt(pressure_thres_editor.value,"%.1f",system_data.config_data.pressure_threshold);
}

