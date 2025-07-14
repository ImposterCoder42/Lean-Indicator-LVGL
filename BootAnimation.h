#ifndef BOOT_ANIMATION_H
#define BOOT_ANIMATION_H

#include <lvgl.h>

extern lv_obj_t* max_label;

void play_boot_animation();

extern bool boot_done;
extern uint32_t boot_start_time;


#endif