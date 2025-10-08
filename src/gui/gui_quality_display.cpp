#include "gui_quality_display.h"
#include "gui_statusBar.h"
#include "gui_pressure_thresh.h"
#include "systemData.h"
#include "gui.h"
#include "dataConfig.h"

QualityDisplyer gui_auality_display;

LV_FONT_DECLARE(hansans_18);
LV_FONT_DECLARE(lv_font_montserrat_16);
void gui_quality_display_create(lv_obj_t * parent)
{
    gui_auality_display.cont = lv_obj_create(parent);
    lv_obj_align(gui_auality_display.cont,LV_ALIGN_TOP_MID,0,STATUS_BAR_HEIGHT+GUI_PRESSURE_THRSH_HEIGHT);
    lv_obj_set_width(gui_auality_display.cont, VALUE_QUALITY_DISPLAYER_WIDTH);
    lv_obj_set_height(gui_auality_display.cont, VALUE_QUALITY_DISPLAYER_HEIGHT);
    lv_obj_set_style_radius(gui_auality_display.cont, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(gui_auality_display.cont,lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(gui_auality_display.cont, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(gui_auality_display.cont,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(gui_auality_display.cont,4,0);
    lv_obj_set_style_border_color(gui_auality_display.cont,GUI_BG_COLOR,0);
    lv_obj_set_style_bg_color(gui_auality_display.cont,GUI_BG_COLOR,0);

    gui_auality_display.title = lv_label_create(gui_auality_display.cont);    
    lv_obj_align(gui_auality_display.title, LV_ALIGN_TOP_LEFT,0,0);
    lv_label_set_text(gui_auality_display.title,"总质量");
    lv_obj_set_style_text_font(gui_auality_display.title, &hansans_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(gui_auality_display.title,GUI_QUALITY_DISPLAYER_COLOR,0);

    gui_auality_display.value = lv_label_create(gui_auality_display.cont); 
    lv_obj_align(gui_auality_display.value , LV_ALIGN_RIGHT_MID,-15,6);
    lv_obj_set_style_text_font(gui_auality_display.value,&lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(gui_auality_display.value,GUI_QUALITY_DISPLAYER_COLOR,0);

    gui_auality_display.unit = lv_label_create(gui_auality_display.cont);
    lv_obj_align(gui_auality_display.unit,LV_ALIGN_BOTTOM_RIGHT,0,0);
    lv_obj_set_style_text_font(gui_auality_display.unit , &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(gui_auality_display.unit,"g");
    lv_obj_set_style_text_color(gui_auality_display.unit,GUI_QUALITY_DISPLAYER_COLOR,0);

    gui_auality_display.bar = lv_bar_create(gui_auality_display.cont);
    lv_bar_set_start_value(gui_auality_display.bar, 0, LV_ANIM_OFF);
    lv_obj_set_width(gui_auality_display.bar, BAR_WIDTH);
    lv_obj_set_height(gui_auality_display.bar, BAR_HEIGHT);
    lv_obj_align(gui_auality_display.bar, LV_ALIGN_CENTER,-30,6);
    lv_obj_set_style_border_width(gui_auality_display.bar,2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(gui_auality_display.bar,GUI_QUALITY_DISPLAYER_COLOR,0);
    lv_obj_set_style_bg_color(gui_auality_display.bar,GUI_QUALITY_DISPLAYER_COLOR, LV_PART_INDICATOR);
}

void gui_quality_display_refresh(void)
{
    lv_label_set_text_fmt(gui_auality_display.value,"%.1f",system_data.sensor_data.value_quality);
    lv_bar_set_value(gui_auality_display.bar, 100*(system_data.sensor_data.value_quality/DEFAULT_MAX_QUALITY), LV_ANIM_OFF);
}

