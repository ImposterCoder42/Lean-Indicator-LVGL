#ifndef LEAN_UI_H
#define LEAN_UI_H

#include <lvgl.h>
#include <Arduino.h>


// External LVGL objects (you must define them in your main file)
extern lv_obj_t* arc;
extern lv_obj_t* img;
extern lv_obj_t* label;
extern lv_obj_t* screen;

// UI Functions
void update_UI(int angle);

// Angle Management
void check_and_set_max_angles(int current_angle, float accX);
void get_max_angles();
void do_angle_reset();

// Max angles (global vars)
extern int max_angle_left;
extern int max_angle_right;


#endif