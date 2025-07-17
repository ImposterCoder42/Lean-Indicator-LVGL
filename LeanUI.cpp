#include <Arduino.h>  // for millis()
#include "LeanUI.h"
#include <Preferences.h>
#include <string>
#include "Preferences.h"
#include "GaugeSettingsUtils.h"

int max_angle_left = 0;
int max_angle_right = 0;

// Internal tracking for 1-second hold
static int candidateMaxAngle = 0;
static unsigned long candidateStartTime = 0;
static bool candidateTimerRunning = false;
static const unsigned long HOLD_DURATION = 200;

Preferences preferences;


static int maxAngleThreshold = 29; // The Bikes Max Angle

void update_UI(int angle) {
    GaugeSettings s = loadGaugeSettings();
    // Update Arc
    lv_arc_set_value(arc, angle);
    // Update Image
    lv_img_set_angle(img, angle * 10);

    // Format label text
    std::string angle_as_str = std::to_string(abs(angle));
    std::string full_label_as_str = angle_as_str + " DEG";
    const char* label_as_char = full_label_as_str.c_str();
    lv_label_set_text(label, label_as_char);

    // Style based on lean severity
    if (abs(angle) > maxAngleThreshold) {
        // Preferences prefs;
        lv_obj_set_style_text_color(label, lv_color_hex(0xFF3B30), 0);  // Red
        lv_color_t backgroundWarningColor = hexToColor(s.backgroundWarningColor);
        lv_obj_set_style_bg_color(screen, backgroundWarningColor, LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_color_t fontColor = hexToColor(s.fontColor);
        lv_obj_set_style_text_color(label, fontColor, 0);
        lv_color_t backgroundNormalColor = hexToColor(s.backgroundNormalColor);
        lv_obj_set_style_bg_color(screen, backgroundNormalColor, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void check_and_set_max_angles(int current_angle, float accX) {
    if (current_angle < 0) {
        if (current_angle < max_angle_left && accX > 14.0) {
            if (!candidateTimerRunning || current_angle < candidateMaxAngle) {
                candidateMaxAngle = current_angle;
                candidateStartTime = millis();
                candidateTimerRunning = true;
            } else if (millis() - candidateStartTime >= HOLD_DURATION) {
                max_angle_left = candidateMaxAngle;
                preferences.begin("lean_data", false);
                preferences.putInt("left_max_angle", max_angle_left);
                preferences.end();
                Serial.print("updated");
                candidateTimerRunning = false;
            }
        } else {
            candidateTimerRunning = false;
            candidateMaxAngle = 0;
        }
    } else if (current_angle > 0) {
        if (current_angle > max_angle_right && accX > 14.0) {
            if (!candidateTimerRunning || current_angle > candidateMaxAngle) {
                candidateMaxAngle = current_angle;
                candidateStartTime = millis();
                candidateTimerRunning = true;
            } else if (millis() - candidateStartTime >= HOLD_DURATION) {
                max_angle_right = candidateMaxAngle;
                preferences.begin("lean_data", false);
                preferences.putInt("right_max_angle", max_angle_right);
                preferences.end();
                Serial.print("updated");
                candidateTimerRunning = false;
            }
        } else {
            candidateTimerRunning = false;
            candidateMaxAngle = 0;
        }
    }
}

void get_max_angles() {
    preferences.begin("lean_data", true);
    max_angle_left = preferences.getInt("left_max_angle", 1);
    max_angle_right = preferences.getInt("right_max_angle", -1);
    preferences.end();
}

void do_angle_reset() {
    get_max_angles();
    if (max_angle_left < -85 && max_angle_right > 85) {
        preferences.begin("lean_data", false);
        preferences.clear();
        preferences.end();
    }
}

