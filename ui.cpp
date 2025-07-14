#include "ui.h"
#include <lvgl.h>
#include "lv_conf.h"
#include "image.h"
#include "Marty_30.h"
#include <Preferences.h> 
#include "LeanUI.h"

void create_arc() {
  arc = lv_arc_create(lv_scr_act());
  lv_obj_set_size(arc, 230, 230);
  lv_obj_center(arc);
  lv_arc_set_mode(arc, LV_ARC_MODE_SYMMETRICAL);
  lv_arc_set_value(arc, 0);
  lv_arc_set_range(arc, -90, 90);
  lv_arc_set_bg_angles(arc, 160, 20);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  lv_obj_set_style_arc_color(arc, lv_color_hex(0x77777777), LV_PART_MAIN | LV_STATE_DEFAULT);    
  lv_obj_set_style_arc_color(arc, lv_color_hex(0x7700FF00), LV_PART_INDICATOR | LV_STATE_DEFAULT);    
  lv_obj_set_style_arc_opa(arc, LV_OPA_30, LV_PART_MAIN);                                              
  lv_obj_set_style_arc_opa(arc, LV_OPA_40, LV_PART_INDICATOR);                                         
  lv_obj_set_style_arc_width(arc, 7, LV_PART_INDICATOR);                                               
  lv_obj_set_style_arc_width(arc, 5, LV_PART_MAIN);                                                    
}

void create_image() {
  extern const lv_img_dsc_t image;
    img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, &image);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_size(img, 110 , 171);
    lv_img_set_pivot(img, 110 / 2, 171);
    // Smooth Rotate Anime
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);  // The image object
    lv_anim_set_values(&a, 0, 900);  // 0° to 90° (in tenths of a degree)
    lv_anim_set_time(&a, 1000);  // Duration in milliseconds
    lv_anim_set_exec_cb(&a, [](void * obj, int32_t v) {
        lv_img_set_angle((lv_obj_t *)obj, v);
    });
    lv_anim_start(&a);
}
void create_label() {
  label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "0 DEG");
  lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);  
  lv_obj_set_style_text_font(label, &Marty_30, 0);    
  lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
}

void create_UI() {
  create_image();
  create_arc();
  create_label();
}