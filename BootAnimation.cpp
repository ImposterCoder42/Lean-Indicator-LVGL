#include "BootAnimation.h"
#include "ui.h"
#include "LeanUI.h"
#include <lvgl.h>
#include "lv_conf.h"
// #include "marty_30.h"
#include "awergy.h"
#include <string>


bool boot_done = false;
uint32_t boot_start_time = 0;

// UI Boot Animation

void play_boot_animation() {
  lv_obj_clean(lv_scr_act());
  lv_obj_t *boot_logo = lv_img_create(lv_scr_act());
  LV_IMG_DECLARE(logo);
  lv_img_set_src(boot_logo, &logo);
  // lv_img_set_zoom(boot_logo, 153);   // ~60%
  lv_obj_center(boot_logo);
  // After 2.5 sec, remove logo and show max angles
  lv_timer_create([](lv_timer_t *t) {
    lv_obj_t *logo = (lv_obj_t *)t->user_data;
    lv_obj_del(logo);

    lv_obj_clean(lv_scr_act());
    // --- Step 1: Get and show max angles ---
    get_max_angles();

    int left_int = abs(static_cast<int>(max_angle_left));
    int right_int = abs(static_cast<int>(max_angle_right));
    char buf[64];
    sprintf(buf, "MAX LEAN\nl %dDEG : %dDEG r", left_int, right_int);

    max_label = lv_label_create(lv_scr_act());  // CHATGPT Here the Background is white and arc visible, also a 2nd bike image loads 90 deg right frozen
    lv_label_set_text(max_label, buf);
    lv_obj_set_style_text_align(max_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(max_label);
    lv_obj_set_style_text_font(max_label, &Awergy, 0);  // Apply your custom font
    lv_obj_set_style_text_color(max_label, lv_color_hex(0xFFFFFF), 0);

    // --- Step 2: After 4.5 sec, remove label and start UI + animation ---
    lv_timer_create([](lv_timer_t *t2) {
      lv_obj_del(max_label);
      lv_obj_clean(lv_scr_act());
      create_UI();

      // --- Step 3: Animate UI lean angle --- // CHATGPT Here the Background is corrent but there are 2 labels and cant be read, arc disapears and never comes back
      lv_timer_create([](lv_timer_t *animTimer) {
        static int angle = 0;
        static int step = -9;
        static bool is_done = false;

        update_UI(angle);
        angle += step;

        if (angle <= -90) step = 9;
        else if (angle >= 90) { step = -9; is_done = true; }
        else if (angle == 0 && is_done) {
          boot_done = true;
          lv_timer_del(animTimer);
        }
      }, 30, NULL);

      lv_timer_del(t2);
    }, 4500, NULL);  // Wait 4.5 sec to remove label

    lv_timer_del(t);
  }, 2500, boot_logo);  // Wait 2.5 sec to remove logo
}
