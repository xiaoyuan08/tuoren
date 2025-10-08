#ifndef __GUI_H
#define __GUI_H
#include <lvgl.h>
#include "gui_pressure_thresh.h"
#include "gui_countDown_thresh.h"

#define SCREENWIDTH  480
#define SCREENHEIGHT 320
#define BUFFERHEIGHT 50

#define GUI_BG_COLOR lv_color_make(0,0,0)
#define GUI_MARGIN_COLOR_FOCUS lv_palette_main(LV_PALETTE_ORANGE)
#define GUI_MARGIN_COLOR lv_palette_main(LV_PALETTE_BLUE)

enum KeyRemap {KEY_VALUECHANGE,KEY_NAVIGATION};

extern enum KeyRemap key_remap;
extern lv_indev_t *encoder_indev;
extern int32_t enc_diff;
extern lv_group_t  * current_group;

void gui_init(void);
void gui_task(void *pvParameters);

#endif


