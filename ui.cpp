#include "ui.h"
#include <lvgl.h>
#include "lv_conf.h"
#include <Preferences.h> 
#include "LeanUI.h"
#include "GaugeSettingsUtils.h"
#include "indian_scout.h"
#include "sport_green.h"
#include "sport_red_blue_white.h"
#include "adventure_gray.h"
#include "bagger_black.h"
#include "marty.h"
#include "awergy.h"
#include "bloomira.h"
#include "super_crumble.h"
#include "wablo.h"

extern const lv_font_t marty;
extern const lv_font_t Awergy;
extern const lv_font_t Bloomira;
extern const lv_font_t Super_Crumble;
extern const lv_font_t Wablo;

extern const lv_img_dsc_t indian_scout;
extern const lv_img_dsc_t sport_green;
extern const lv_img_dsc_t sport_red_blue_white;
extern const lv_img_dsc_t adventure_gray;
extern const lv_img_dsc_t bagger_black;

void create_arc(const GaugeSettings& s) {
  arc = lv_arc_create(lv_scr_act());
  lv_obj_set_size(arc, 230, 230);
  lv_obj_center(arc);
  lv_arc_set_mode(arc, LV_ARC_MODE_SYMMETRICAL);
  lv_arc_set_value(arc, 0);
  lv_arc_set_range(arc, -90, 90);
  lv_arc_set_bg_angles(arc, 160, 20);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  lv_color_t arcMainColor = hexToColor(s.arcMainColor);
  lv_obj_set_style_arc_color(arc, arcMainColor, LV_PART_MAIN | LV_STATE_DEFAULT);    
  lv_color_t arcIndicatorColor = hexToColor(s.arcIndicatorColor);
  lv_obj_set_style_arc_color(arc, arcIndicatorColor, LV_PART_INDICATOR | LV_STATE_DEFAULT);    
  lv_obj_set_style_arc_opa(arc, LV_OPA_30, LV_PART_MAIN);                                              
  lv_obj_set_style_arc_opa(arc, LV_OPA_40, LV_PART_INDICATOR);                                         
  lv_obj_set_style_arc_width(arc, 7, LV_PART_INDICATOR);                                               
  lv_obj_set_style_arc_width(arc, 5, LV_PART_MAIN);                                                    
}

void create_image(const GaugeSettings& s) {
    img = lv_img_create(lv_scr_act());

    if (s.currentBike == "factory bike") {
      lv_img_set_src(img, &indian_scout);
      lv_obj_align(img, LV_ALIGN_CENTER, 0, -10);
      lv_obj_set_size(img, 110 , 171);
      lv_img_set_pivot(img, 110 / 2, 171);
    
    } else if (s.currentBike == "bike 2") {
      lv_img_set_src(img, &sport_green);
      lv_obj_align(img, LV_ALIGN_CENTER, 0, -10);
      lv_obj_set_size(img, 110 , 151);
      lv_img_set_pivot(img, 110 / 2, 151);
    
    } else if (s.currentBike == "bike 3") {
      lv_img_set_src(img, &sport_red_blue_white);
      lv_obj_align(img, LV_ALIGN_CENTER, 0, -10);
      lv_obj_set_size(img, 110 , 161);
      lv_img_set_pivot(img, 110 / 2, 161);
    
    } else if (s.currentBike == "bike 4") {
      lv_img_set_src(img, &adventure_gray);
      lv_obj_align(img, LV_ALIGN_CENTER, 0, -10);
      lv_obj_set_size(img, 110 , 171);
      lv_img_set_pivot(img, 110 / 2, 171);
    
    } else if (s.currentBike == "bike 5") {
      lv_img_set_src(img, &bagger_black);
      lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(img, 110 , 151);
      lv_img_set_pivot(img, 110 / 2, 151);
    
    } else { // catch all
      lv_img_set_src(img, &indian_scout);
      lv_obj_align(img, LV_ALIGN_CENTER, 0, -10);
      lv_obj_set_size(img, 110 , 171);
      lv_img_set_pivot(img, 110 / 2, 171);
    }
    // Smooth Rotate Anime
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);  
    lv_anim_set_values(&a, 0, 900);  // 0° to 90° (in tenths of a degree)
    lv_anim_set_time(&a, 1000);  // milliseconds
    lv_anim_set_exec_cb(&a, [](void * obj, int32_t v) {
        lv_img_set_angle((lv_obj_t *)obj, v);
    });
    lv_anim_start(&a);
}


void create_label(const GaugeSettings& s) {
  label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "0 DEG");
  lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_color_t fontColor = hexToColor(s.fontColor);
  lv_obj_set_style_text_color(label, fontColor, 0);
  if (s.currentFont == "factory font") {
    lv_obj_set_style_text_font(label, &marty, 0);    
  } else if (s.currentFont == "font 2") {
    lv_obj_set_style_text_font(label, &Awergy, 0);    
  } else if (s.currentFont == "font 3") {
    lv_obj_set_style_text_font(label, &Bloomira, 0);    
  } else if (s.currentFont == "font 4") {
    lv_obj_set_style_text_font(label, &Super_Crumble, 0);  
  } else if (s.currentFont == "font 5") {
    lv_obj_set_style_text_font(label, &Wablo, 0);    
  } else { // catch all
    lv_obj_set_style_text_font(label, &marty, 0);    
  }
}

void create_UI() {
  GaugeSettings s = loadGaugeSettings();
  create_image(s);
  create_arc(s);
  create_label(s);
}