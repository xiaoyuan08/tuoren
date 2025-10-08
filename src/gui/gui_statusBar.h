#ifndef __GUI_STATUSBAR_H
#define __GUI_STATUSBAR_H
#include <lvgl.h>


#define STATUS_BAR_WIDTH 480
#define STATUS_BAR_HEIGHT 35
#define BATTERY_WIDTH 50
#define BATTERY_HEIGHT 20

typedef struct StatusBar
{
    lv_obj_t * cont;
    //wifi图标
    lv_obj_t *wifi_icon;
    lv_obj_t *wifi_rssi;
    lv_obj_t *arcs[4];
    lv_obj_t * wifi_center;
    //电池电量
    lv_obj_t *battery_cont;
    lv_obj_t *battery_level;
    lv_obj_t *battery_tip;
    lv_obj_t *battery_percent;
    lv_obj_t *icon_charge;
    //信息
    lv_obj_t * label_infor;
    //时间
    lv_obj_t * date_cont;
    lv_obj_t *date_year;
    lv_obj_t *date_month;
    lv_obj_t *date_day;

    lv_obj_t * time_cont;
    lv_obj_t *time_hour;
    lv_obj_t *time_min;
    lv_obj_t *time_second;
} StatusBar;

extern StatusBar status_bar;

void gui_statusBar_create(lv_obj_t * parent);
void gui_wifi_create(lv_obj_t *parent);
void gui_wifi_fresh(void);
void gui_time_fresh(void);
void gui_battery_fresh(void);

#endif


