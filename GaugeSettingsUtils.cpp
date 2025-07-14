// #include "GaugeSettingsUtils.h"
// #include <Preferences.h>
// #include "LeanUI.h"    
// #include <lvgl.h>      
// #include "ui.h"   

// void saveGaugeSettings(
//   const char* bgNormal,
//   const char* bgWarning,
//   const char* arcMain,
//   const char* arcIndicator,
//   const char* fontColor,
//   const char* font,
//   const char* bike,
//   float maxAngle
// ) {
//   Preferences prefs;
//   prefs.begin("gauge", false);

//   prefs.putString("bgNormal", bgNormal);
//   prefs.putString("bgWarning", bgWarning);
//   prefs.putString("arcMain", arcMain);
//   prefs.putString("arcInd", arcIndicator);
//   prefs.putString("fontColor", fontColor);
//   prefs.putString("font", font);
//   prefs.putString("bike", bike);
//   prefs.putFloat("maxAngle", maxAngle);

//   prefs.end();

//   Serial.println("Settings saved to flash.");
// }

// GaugeSettings loadGaugeSettings() {
//   Preferences prefs;
//   GaugeSettings s;
//   prefs.begin("gauge", true);
//   s.backgroundNormalColor = prefs.getString("bgNormal", "#313131");
//   s.backgroundWarningColor = prefs.getString("bgWarning", "#800000C8");
//   s.arcMainColor = prefs.getString("arcMain", "#004609D1");
//   s.arcIndicatorColor = prefs.getString("arcInd", "#FFFFFF73");
//   s.fontColor = prefs.getString("fontColor", "#FFFFFF");
//   s.currentFont = prefs.getString("font", "marty");
//   s.currentBike = prefs.getString("bike", "21 INDIAN SCOUT");
//   s.maxiumSafeAngle = prefs.getFloat("maxAngle", 29.0f);
//   prefs.end();
//   return s;
// }

// void applyGaugeSettings(const GaugeSettings& s) {
//   lv_color_t main = hexToColor(s.arcMainColor);
//   lv_color_t ind = hexToColor(s.arcIndicatorColor);
//   lv_color_t font = hexToColor(s.fontColor);

//   lv_obj_set_style_arc_color(arc, main, LV_PART_MAIN);
//   lv_obj_set_style_arc_color(arc, ind, LV_PART_INDICATOR);
//   lv_obj_set_style_text_color(label, font, 0);
// }
