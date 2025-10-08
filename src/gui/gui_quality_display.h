#ifndef _GUI_QUALITY_DISPLAYER_H
#define _GUI_QUALITY_DISPLAYER_H
#include <lvgl.h>
#include "gui_statusBar.h"
#include "gui_pressure_thresh.h"

#define VALUE_QUALITY_DISPLAYER_WIDTH 480
#define VALUE_QUALITY_DISPLAYER_HEIGHT 75

#define GUI_QUALITY_DISPLAYER_COLOR lv_color_make(77,251,115)

#define BAR_WIDTH 300
#define BAR_HEIGHT 30

typedef struct QualityDisplyer
{
    lv_obj_t *cont;
    lv_obj_t *title;
    lv_obj_t *value;
    lv_obj_t *unit;
    lv_obj_t *bar;
}QualityDisplyer;

extern QualityDisplyer value_displyer_pressure;

void gui_quality_display_create(lv_obj_t * parent);
void gui_quality_display_refresh(void);
#endif



