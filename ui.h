#ifndef UI_H
#define UI_H

#include <lvgl.h>
#include "GaugeSettingsUtils.h"

extern lv_obj_t* arc;
extern lv_obj_t* img;
extern lv_obj_t* label;
extern lv_obj_t* screen;

// UI Functions
void create_arc(const GaugeSettings& s); // Did this need gauge setting too?

void create_image(const GaugeSettings& s);

void create_label(const GaugeSettings& s); 

void create_UI();

#endif