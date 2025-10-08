#include "gui_pressure_display.h"
#include "gui_statusBar.h"
#include "gui_pressure_thresh.h"
#include "systemData.h"
#include "gui_quality_display.h"
#include "gui.h"

PressureDisplyer gui_pressure_display;

LV_FONT_DECLARE(hansans_18);
LV_FONT_DECLARE(lv_font_montserrat_16);
void gui_pressure_display_create(lv_obj_t * parent)
{
    gui_pressure_display.cont = lv_obj_create(parent);
    lv_obj_align(gui_pressure_display.cont,LV_ALIGN_TOP_MID,0,STATUS_BAR_HEIGHT+GUI_PRESSURE_THRSH_HEIGHT+VALUE_QUALITY_DISPLAYER_HEIGHT-2);
    lv_obj_set_width(gui_pressure_display.cont, VALUE_PRESSURE_DISPLAYER_WIDTH);
    lv_obj_set_height(gui_pressure_display.cont, VALUE_PRESUURE_DISPLAYER_HEIGHT);
    lv_obj_set_style_radius(gui_pressure_display.cont, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(gui_pressure_display.cont,lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(gui_pressure_display.cont, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(gui_pressure_display.cont,GUI_BG_COLOR,LV_STATE_DEFAULT);
    lv_obj_clear_flag(gui_pressure_display.cont,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(gui_pressure_display.cont,4,LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(gui_pressure_display.cont,GUI_BG_COLOR,0);

    gui_pressure_display.title = lv_label_create(gui_pressure_display.cont);    
    lv_obj_align(gui_pressure_display.title, LV_ALIGN_TOP_LEFT,0,0);
    lv_label_set_text(gui_pressure_display.title,"压力曲线");
    lv_obj_set_style_text_font(gui_pressure_display.title, &hansans_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(gui_pressure_display.title,GUI_PRESUURE_DISPLAYER_COLOR,0);
    
    gui_pressure_display.value = lv_label_create(gui_pressure_display.cont); 
    lv_obj_align(gui_pressure_display.value , LV_ALIGN_RIGHT_MID,2,10);
    lv_obj_set_style_text_font(gui_pressure_display.value,&lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(gui_pressure_display.value,GUI_PRESUURE_DISPLAYER_COLOR,0);

    gui_pressure_display.unit = lv_label_create(gui_pressure_display.cont);
    lv_obj_align(gui_pressure_display.unit,LV_ALIGN_BOTTOM_RIGHT,0,0);
    lv_obj_set_style_text_font(gui_pressure_display.unit , &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(gui_pressure_display.unit,"mmHg");
    lv_obj_set_style_text_color(gui_pressure_display.unit,GUI_PRESUURE_DISPLAYER_COLOR,0);
    //曲线
    gui_pressure_display.curve = lv_chart_create(gui_pressure_display.cont);
    lv_obj_set_size(gui_pressure_display.curve, 270, 105);
    lv_obj_align(gui_pressure_display.curve, LV_ALIGN_LEFT_MID, 90, -2);
	lv_chart_set_range(gui_pressure_display.curve, LV_CHART_AXIS_PRIMARY_Y, 0, 250);//设置y轴范围
	lv_chart_set_range(gui_pressure_display.curve, LV_CHART_AXIS_PRIMARY_X, 0, 100);//设置x轴范围
	lv_chart_set_type(gui_pressure_display.curve, LV_CHART_TYPE_LINE);//设置曲线显示
	lv_chart_set_update_mode(gui_pressure_display.curve,LV_CHART_UPDATE_MODE_SHIFT);//以移动的方式进行数据刷新
	lv_chart_set_point_count(gui_pressure_display.curve, 100);//设置显示点数
	lv_chart_set_axis_tick(gui_pressure_display.curve, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 8, 1, true, 40);//设置y刻度
	lv_chart_set_axis_tick(gui_pressure_display.curve, LV_CHART_AXIS_PRIMARY_X, 10, 5, 10, 1, true, 40);//设置x刻度
	lv_chart_set_div_line_count(gui_pressure_display.curve, 12, 25);//设置栅格数
	lv_obj_set_style_line_width(gui_pressure_display.curve,1, 0);//设置线宽
	lv_obj_set_style_width(gui_pressure_display.curve, 1, LV_PART_INDICATOR);
    lv_obj_set_style_height(gui_pressure_display.curve, 1, LV_PART_INDICATOR);//设置点大小
	lv_obj_set_style_text_font(gui_pressure_display.curve, &lv_font_montserrat_10, 0);//设置文本大小
    lv_obj_set_style_bg_color(gui_pressure_display.curve,GUI_BG_COLOR,LV_PART_MAIN);
    lv_obj_set_style_line_color(gui_pressure_display.curve,lv_color_make(80,80,80),0);

    //设置序列
	gui_pressure_display.ser = lv_chart_add_series(gui_pressure_display.curve,lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);//数据数列
	//设置默认缩放
	lv_chart_set_zoom_x(gui_pressure_display.curve,255);
	lv_chart_set_zoom_y(gui_pressure_display.curve,255);
	//样式美化
	lv_obj_set_style_outline_color(gui_pressure_display.curve, lv_color_make(200, 200, 0), LV_STATE_FOCUSED);
	lv_obj_set_style_outline_width(gui_pressure_display.curve, 1, LV_STATE_FOCUSED);
}

void gui_pressure_display_refresh(void)
{
    lv_label_set_text_fmt(gui_pressure_display.value,"%.1f",system_data.sensor_data.value_press);
    lv_chart_set_next_value(gui_pressure_display.curve,gui_pressure_display.ser,system_data.sensor_data.value_press);
}




