#ifndef _GUI_PRESSURE_THRSH_H
#define _GUI_PRESSURE_THRSH_H
#include<lvgl.h>

#define GUI_PRESSURE_THRSH_WIDTH 188+2
#define GUI_PRESSURE_THRSH_HEIGHT 62
#define GUI_PRESSURE_THRES_MARGIN 2

#define GUI_PRESSURE_THRES_COLOR lv_color_make(81,202,238)

typedef struct PressureThresEditor
{
    lv_obj_t * cont;
    lv_obj_t * title;
    lv_obj_t * value;
    lv_obj_t * unit;
    lv_timer_t * timer;
} PressureThresEditor;

extern PressureThresEditor pressure_thres_editor;

void gui_pressure_thresh_editor_init(lv_obj_t * parent);
void gui_pressure_thresh_refresh(void);

#endif








