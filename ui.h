#ifndef UI_H
#define UI_H

#include <lvgl.h>

// External LVGL objects (you must define them in your main file)
extern lv_obj_t* arc;
extern lv_obj_t* img;
extern lv_obj_t* label;
extern lv_obj_t* screen;

// UI Functions
void create_arc(); // Did this need gauge setting too?

void create_image();

void create_label(); 

// void create_UI(const GaugeSettings& settings);
void create_UI();

#endif