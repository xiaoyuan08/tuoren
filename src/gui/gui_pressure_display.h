#ifndef _GUI_VALUE_DISPLAYER_H
#define _GUI_VALUE_DISPLAYER_H
#include <lvgl.h>
#include "gui_statusBar.h"
#include "gui_pressure_thresh.h"

#define VALUE_PRESSURE_DISPLAYER_WIDTH 480
#define VALUE_PRESUURE_DISPLAYER_HEIGHT 150

#define BAR_WIDTH 300
#define BAR_HEIGHT 30

#define GUI_PRESUURE_DISPLAYER_COLOR lv_color_make(255,169,20)

typedef struct PressureDisplyer
{
    lv_obj_t *cont;
    lv_obj_t *title;
    lv_obj_t *value;
    lv_obj_t *unit;
    lv_obj_t *curve;
    lv_chart_series_t * ser;
}PressureDisplyer;

extern PressureDisplyer gui_pressure_display;

void gui_pressure_display_create(lv_obj_t * parent);
void gui_pressure_display_refresh(void);
#endif



