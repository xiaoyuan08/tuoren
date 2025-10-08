#include <lvgl.h>
#include "gui_statusBar.h"
#include "systemData.h"
#include <Wifi.h>
#include <string>
#include "gui.h"
#include "hardware/battery.h"

StatusBar status_bar;


static void gui_battery_create(void);
static void gui_time_create(lv_obj_t *parent);


/**************************************
 *****创建状态栏
 ************************************/
LV_FONT_DECLARE(hansans_14);
void gui_statusBar_create(lv_obj_t * parent)
{
    //状态栏
    status_bar.cont = lv_obj_create(parent);
    lv_obj_remove_style_all(status_bar.cont);
    lv_obj_set_width(status_bar.cont, STATUS_BAR_WIDTH);
    lv_obj_set_height(status_bar.cont, STATUS_BAR_HEIGHT);
    lv_obj_align(status_bar.cont,LV_ALIGN_TOP_MID,0,0);
    lv_obj_clear_flag(status_bar.cont, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);  
    lv_obj_set_style_bg_opa(status_bar.cont, 255, 0);
    lv_obj_set_style_bg_color(status_bar.cont,GUI_BG_COLOR,0);
    lv_obj_set_style_border_width(status_bar.cont,0,0);
    lv_obj_set_style_border_color(status_bar.cont,GUI_MARGIN_COLOR,0);
    //wifi
    gui_wifi_create(status_bar.cont);
    //电池
    gui_battery_create();
    //时间
    gui_time_create(status_bar.cont);
}

/**************************************
 *****创建时间日期
 ************************************/
static void gui_time_create(lv_obj_t *parent)
{
    //字体样式
    static lv_style_t style_white_text;
    lv_style_init(&style_white_text);
    lv_style_set_text_color(&style_white_text, lv_color_hex(0xFFFFFF));
    //日期
    status_bar.date_cont = lv_obj_create(parent);
    lv_obj_set_scrollbar_mode(status_bar.date_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(status_bar.date_cont,90,20);
    lv_obj_align(status_bar.date_cont,LV_ALIGN_CENTER,20,0);
    lv_obj_set_layout(status_bar.date_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(status_bar.date_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_bg_color(status_bar.date_cont,lv_color_hex(0x00),0);
    lv_obj_set_style_border_width(status_bar.date_cont,1,0);
    lv_obj_set_style_radius(status_bar.date_cont,1,0);
    lv_obj_set_flex_align(
        status_bar.date_cont,
        LV_FLEX_ALIGN_CENTER,  //主轴（水平）方向居中对齐
        LV_FLEX_ALIGN_CENTER,  //交叉轴（垂直）方向居中对齐
        LV_FLEX_ALIGN_CENTER   //换行后的对齐方式（这里用不到）
    );
    lv_obj_set_style_pad_column(status_bar.date_cont,2,0);
    status_bar.date_year = lv_label_create(status_bar.date_cont);
    lv_obj_align(status_bar.date_year,LV_ALIGN_CENTER,0,0);
    lv_obj_add_style(status_bar.date_year, &style_white_text, 0);

    status_bar.date_month = lv_label_create(status_bar.date_cont);
    lv_obj_add_style(status_bar.date_month, &style_white_text, 0);

    status_bar.date_day = lv_label_create(status_bar.date_cont);
    lv_obj_add_style(status_bar.date_day, &style_white_text, 0);

    //时间
    status_bar.time_cont = lv_obj_create(parent);
    lv_obj_set_scrollbar_mode(status_bar.time_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(status_bar.time_cont,65,20);
    lv_obj_align(status_bar.time_cont,LV_ALIGN_CENTER,112,0);
    lv_obj_set_layout(status_bar.time_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(status_bar.time_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_bg_color(status_bar.time_cont,lv_color_hex(0x00),0);
    lv_obj_set_style_border_width(status_bar.time_cont,1,0);
    lv_obj_set_style_radius(status_bar.time_cont,2,0);
    lv_obj_set_flex_align(
        status_bar.time_cont,
        LV_FLEX_ALIGN_CENTER,  //主轴（水平）方向居中对齐
        LV_FLEX_ALIGN_CENTER,  //交叉轴（垂直）方向居中对齐
        LV_FLEX_ALIGN_CENTER   //换行后的对齐方式（这里用不到）
    );
    lv_obj_set_style_pad_column(status_bar.time_cont,2,0);

    status_bar.time_hour = lv_label_create(status_bar.time_cont);
    lv_obj_add_style(status_bar.time_hour, &style_white_text, 0);

    status_bar.time_min =  lv_label_create(status_bar.time_cont);
    lv_obj_add_style(status_bar.time_min, &style_white_text, 0);

    status_bar.time_second = lv_label_create(status_bar.time_cont);
    lv_obj_add_style(status_bar.time_second, &style_white_text, 0); 
}


void gui_time_fresh(void)
{
     lv_label_set_text_fmt(status_bar.date_year,"%d/",system_data.time_data.date_year);
    lv_label_set_text_fmt(status_bar.date_month,"%d/",system_data.time_data.date_month);
    lv_label_set_text_fmt(status_bar.date_day,"%d",system_data.time_data.date_day);
    
    lv_label_set_text_fmt(status_bar.time_hour,"%d:",system_data.time_data.date_hour);
    lv_label_set_text_fmt(status_bar.time_min,"%d:",system_data.time_data.date_min);
    lv_label_set_text_fmt(status_bar.time_second,"%d",system_data.time_data.date_second);
}

/********************************************
 ****创建wifi图标
 ******************************************/
void gui_wifi_create(lv_obj_t *parent)
 {
    // 创建新图标容器
    status_bar.wifi_icon=lv_obj_create(parent);
    lv_obj_set_size(status_bar.wifi_icon, 35, 35);
    lv_obj_set_style_bg_opa(status_bar.wifi_icon,0,LV_PART_MAIN);
    lv_obj_set_style_border_opa(status_bar.wifi_icon,0,LV_PART_MAIN);
    lv_obj_clear_flag(status_bar.wifi_icon,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(status_bar.wifi_icon,LV_ALIGN_LEFT_MID,2,-8);
    lv_obj_move_foreground(status_bar.wifi_icon);
    
    // 计算各圆弧的参数
    const int circle_center_x = 5;
    const int circle_center_y = 16;
    const int arc_width = 2;
    
    // 定义4个圆弧的半径和角度
    const int radii[4] = {8, 12, 16, 21};
    const int angles[4] = {35, 45, 55, 75};
    
    // 绘制同心圆中心
    status_bar.wifi_center = lv_obj_create(status_bar.wifi_icon);
    lv_obj_set_size(status_bar.wifi_center, 4, 4);
    lv_obj_set_pos(status_bar.wifi_center, circle_center_x - 2, circle_center_y - 6);
    lv_obj_set_style_radius(status_bar.wifi_center, LV_RADIUS_CIRCLE, LV_PART_MAIN);

    // 绘制4个圆弧
    for(int i = 0; i < 4; i++) 
    {
        // 绘制不可见的圆弧 (信号强度外)
        status_bar.arcs[i] = lv_arc_create(status_bar.wifi_icon);
        lv_obj_set_size(status_bar.arcs[i] , radii[i] * 2, radii[i] * 2);
        lv_obj_set_pos(status_bar.arcs[i] , circle_center_x - radii[i], circle_center_y - radii[i]);
        lv_arc_set_angles(status_bar.arcs[i] , 270 - angles[i]/2, 270 + angles[i]/2);
        lv_obj_set_style_arc_width(status_bar.arcs[i] , arc_width, LV_PART_INDICATOR);
        lv_obj_set_style_arc_color(status_bar.arcs[i] , lv_color_hex(0xFFFFFF), LV_PART_INDICATOR);
        lv_obj_set_style_arc_opa(status_bar.arcs[i] , LV_OPA_50, LV_PART_INDICATOR);
        lv_obj_set_style_arc_width(status_bar.arcs[i] , 0, LV_PART_MAIN);
        lv_obj_set_style_opa(status_bar.arcs[i] , 0, LV_PART_KNOB | LV_STATE_DEFAULT);//隐藏端点
        lv_obj_set_style_opa(status_bar.arcs[i] , 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    }

    //创建RSSSI值
    status_bar.wifi_rssi = lv_label_create(parent);
    lv_obj_set_size(status_bar.wifi_rssi,75,25);
    lv_label_set_text(status_bar.wifi_rssi , "RSSI:79"); 
    lv_obj_align(status_bar.wifi_rssi,LV_ALIGN_LEFT_MID,50, 2);
    lv_obj_set_style_text_color(status_bar.wifi_rssi, lv_color_make(255,255,255),0);
}

/********************************************
 ****wifi图标刷新
 ******************************************/
void gui_wifi_fresh(void)
{
    uint8_t strength_level=0;
    if(system_data.hardware_data.rssi==-1000) strength_level=0;
    else
    {
        if (system_data.hardware_data.rssi>=-110&&system_data.hardware_data.rssi<-100)  strength_level=1;
        else if(system_data.hardware_data.rssi >= -100&&system_data.hardware_data.rssi<-90) strength_level=2;
        else if(system_data.hardware_data.rssi >= -90&&system_data.hardware_data.rssi<-80) strength_level=3;
        else if(system_data.hardware_data.rssi >= -80) strength_level=4;
    }
  
    if(system_data.hardware_data.rssi==0) 
    {
        lv_obj_set_style_bg_opa(status_bar.wifi_center,LV_OPA_30, 0);
        lv_obj_set_style_border_opa(status_bar.wifi_center,LV_OPA_30, 0);
    }
    else
    {
        lv_obj_set_style_bg_opa(status_bar.wifi_center,LV_OPA_100,0);
        lv_obj_set_style_border_opa(status_bar.wifi_center,LV_OPA_100, 0);
    } 
    uint8_t i=0;
    for(;i<strength_level;i++)
    {
        lv_obj_set_style_arc_opa(status_bar.arcs[i] , LV_OPA_100, LV_PART_INDICATOR);
    }
    for(;i<4;i++)
    {
        lv_obj_set_style_arc_opa(status_bar.arcs[i] , LV_OPA_30, LV_PART_INDICATOR);
    }

    lv_label_set_text_fmt(status_bar.wifi_rssi,"rssi:%d",system_data.hardware_data.rssi);

}

/******************************************
 *****创建电池图标
 ******************************************/
static void gui_battery_create(void)
 {
    //电池背景
    status_bar.battery_cont = lv_obj_create(status_bar.cont);
    lv_obj_set_size(status_bar.battery_cont, BATTERY_WIDTH, BATTERY_HEIGHT);
    lv_obj_set_pos(status_bar.battery_cont, 0, 0);
    lv_obj_set_style_radius(status_bar.battery_cont, 3, 0);
    lv_obj_set_style_border_width(status_bar.battery_cont, 2, 0);
    lv_obj_set_style_border_color(status_bar.battery_cont,lv_color_white(),0);
    lv_obj_set_style_bg_color(status_bar.battery_cont, lv_color_white(), 0);
    lv_obj_set_style_pad_all(status_bar.battery_cont, 0, 0);
    lv_obj_clear_flag(status_bar.battery_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(status_bar.battery_cont,LV_ALIGN_RIGHT_MID,-42,0);
    lv_obj_set_style_bg_color(status_bar.battery_cont,lv_color_black(),0);

    // 创建电池正极
    status_bar.battery_tip = lv_obj_create(status_bar.cont);
    lv_obj_set_size(status_bar.battery_tip, 6, 6);
    lv_obj_set_style_radius(status_bar.battery_tip, 0, 0);
    lv_obj_set_style_bg_color(status_bar.battery_tip,lv_color_white(), 0);
    lv_obj_set_style_border_color(status_bar.battery_tip,lv_color_white(), 0);
    lv_obj_clear_flag(status_bar.battery_tip, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(status_bar.battery_tip,LV_ALIGN_RIGHT_MID,-37,0);

    //创建电池电量指示器
    status_bar.battery_level = lv_obj_create(status_bar.battery_cont);
    lv_obj_align(status_bar.battery_level,LV_ALIGN_LEFT_MID,1,0);
    lv_obj_set_style_radius(status_bar.battery_level, 0, 0);
    lv_obj_set_style_border_width(status_bar.battery_level,0,0);
    //电池电量显示
    status_bar.battery_percent = lv_label_create(status_bar.cont);
    lv_label_set_text_fmt(status_bar.battery_percent,"%d%%",system_data.hardware_data.battery_percent);
    lv_obj_set_style_text_color(status_bar.battery_percent, lv_color_make(255,255,255),0);
    lv_obj_align(status_bar.battery_percent,LV_ALIGN_RIGHT_MID,-2,0);

    //创建充电闪电
    LV_IMG_DECLARE(icon_charge);
    status_bar.icon_charge = lv_img_create(status_bar.battery_cont);
    lv_img_set_src(status_bar.icon_charge , &icon_charge);
    lv_img_set_zoom(status_bar.icon_charge, 75);
    lv_obj_set_style_bg_opa(status_bar.icon_charge,0,0);
    lv_obj_set_style_border_width(status_bar.icon_charge,0,0);
    lv_obj_align(status_bar.icon_charge, LV_ALIGN_CENTER, 0,0);
    lv_obj_clear_flag(status_bar.battery_level, LV_OBJ_FLAG_SCROLLABLE);
 }

 /******************************************
 *****电池图标刷新函数
 ******************************************/
 void gui_battery_fresh(void)
 {
    // 根据电量设置颜色
    uint16_t level_width =((float)system_data.hardware_data.battery_percent/100.0)*(BATTERY_WIDTH-6);
    lv_obj_set_size(status_bar.battery_level, level_width, BATTERY_HEIGHT - 6);
    if(charge_flag==0) 
    {
        lv_obj_add_flag(status_bar.icon_charge,LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(status_bar.battery_level,LV_OBJ_FLAG_HIDDEN);
        if (system_data.hardware_data.battery_percent > 60) 
        {
            lv_obj_set_style_bg_color(status_bar.battery_level, lv_color_hex(0x00FF00), 0); // 绿色
        } else if (system_data.hardware_data.battery_percent > 20) 
        {
            lv_obj_set_style_bg_color(status_bar.battery_level, lv_color_hex(0xFFA500), 0); // 黄色
        } 
        else
        {
            lv_obj_set_style_bg_color(status_bar.battery_level, lv_color_hex(0xFF0000), 0); // 红色
        }
    }
    else
    {
        lv_obj_clear_flag(status_bar.icon_charge, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(status_bar.battery_level, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_color(status_bar.battery_cont, lv_color_hex(0xFFFFFF), 0); //白色
        Serial.print("充电中\n");
    }
    
    lv_label_set_text_fmt(status_bar.battery_percent,"%d %%",system_data.hardware_data.battery_percent);
 }

 