#ifndef LEAN_UI_H
#define LEAN_UI_H

#include <lvgl.h>
#include <Arduino.h>
#include "GaugeSettingsUtils.h"

extern lv_obj_t* arc;
extern lv_obj_t* img;
extern lv_obj_t* label;
extern lv_obj_t* screen;

void update_UI(int angle);

void check_and_set_max_angles(int current_angle);
void get_max_angles();
void do_angle_reset();

extern int max_angle_left;
extern int max_angle_right;


#endif