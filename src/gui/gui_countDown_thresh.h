#ifndef _GUI_COUNTDOWN_THRESH_H
#define _GUI_COUNTDOWN_THRESH_H
#include <lvgl.h>

#define  COUNTDOWN_THRESH_WIDTH 283
#define  COUNTDOWN_THRESH_HEIGHT 62

#define  COUNTDOWN_BAR_WIDTH 180
#define  COUNTDOWN_BAR_HEIGHT 20

#define COUNTDOWN_COLOR lv_color_make(81,202,238)

typedef struct CountDownEditor
{
    lv_obj_t * cont;
    lv_obj_t * title;
    lv_obj_t * btn;

    lv_obj_t * hour;
    lv_obj_t * min;
    lv_obj_t * second;

    lv_obj_t * bar;
    lv_timer_t *timer;
} CountDownEditor;

extern CountDownEditor count_down_editor;
void gui_countDown_thresh_init(lv_obj_t * parent);
void gui_count_down_fresh(void);


#endif


