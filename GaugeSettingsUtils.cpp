#include "GaugeSettingsUtils.h"
#include <Preferences.h>
#include "LeanUI.h"    
#include <lvgl.h>      
#include "ui.h"   

void saveGaugeSettings(
  const char* bgNormal,
  const char* bgWarning,
  const char* arcMain,
  const char* arcIndicator,
  const char* fontColor,
  const char* font,
  const char* bike,
  float maxAngle
) {
  Preferences prefs;
  prefs.begin("gauge", false);

  prefs.putString("bgNormal", bgNormal);
  prefs.putString("bgWarning", bgWarning);
  prefs.putString("arcMain", arcMain);
  prefs.putString("arcInd", arcIndicator);
  prefs.putString("fontColor", fontColor);
  prefs.putString("font", font);
  prefs.putString("bike", bike);
  prefs.putFloat("maxAngle", maxAngle);

  prefs.end();

  Serial.println("Settings saved to flash.");
}

GaugeSettings loadGaugeSettings() {
  Preferences prefs;
  GaugeSettings s;
  prefs.begin("gauge", true);
  s.backgroundNormalColor = prefs.getString("bgNormal", "#313131");
  s.backgroundWarningColor = prefs.getString("bgWarning", "#FF771616");
  s.arcMainColor = prefs.getString("arcMain", "#FF9A9A9A");
  s.arcIndicatorColor = prefs.getString("arcInd", "#FF16DD16");
  s.fontColor = prefs.getString("fontColor", "#FFFFFF");
  s.currentFont = prefs.getString("font", "marty");
  s.currentBike = prefs.getString("bike", "indian_scout");
  s.maxiumSafeAngle = prefs.getFloat("maxAngle", 29.0f);
  prefs.end();
  return s;
}

void applyGaugeSettings(const GaugeSettings& s) {
  lv_color_t main = hexToColor(s.arcMainColor);
  lv_color_t ind = hexToColor(s.arcIndicatorColor);
  lv_color_t font = hexToColor(s.fontColor);

  lv_obj_set_style_arc_color(arc, main, LV_PART_MAIN);
  lv_obj_set_style_arc_color(arc, ind, LV_PART_INDICATOR);
  lv_obj_set_style_text_color(label, font, 0);
}

lv_color_t hexToColor(const String& hex) {
  String cleaned = hex;
  if (cleaned.startsWith("#")) cleaned.remove(0, 1);

  if (cleaned.length() == 8) cleaned = cleaned.substring(2);

  uint8_t r = strtol(cleaned.substring(0, 2).c_str(), NULL, 16);
  uint8_t g = strtol(cleaned.substring(2, 4).c_str(), NULL, 16);
  uint8_t b = strtol(cleaned.substring(4, 6).c_str(), NULL, 16);

  return lv_color_make(r, g, b);
}

void rebuildUIFromSettings() {
  lv_obj_clean(lv_scr_act());

  GaugeSettings s = loadGaugeSettings();

  create_image(s);
  create_arc(s);
  create_label(s);
}

void factoryResetUI () {
  Preferences prefs;
  prefs.begin("gauge", false);
  prefs.begin("gauge", false);
  prefs.clear();
  prefs.end();
  Serial.println("UI Reset Completed");
}


// TODO: get Flutter to grab these strings and store them for future use in dropdown menus
const char* preLoadedFonts[] = { // Files Names
  "marty",
  "awergy",
  "bloomira",
  "super_crumble",
  "wablo",
};
const int NUM_FONTS = sizeof(preLoadedFonts) / sizeof(preLoadedFonts[0]);

const char* preLoadedBikes[] = { // Files Names
  "indian_scout",
  "adventure_gray",
  "bagger_black",
  "sport_green",
  "sport_red_blue_white",
};
const int NUM_BIKES = sizeof(preLoadedBikes) / sizeof(preLoadedBikes[0]);
