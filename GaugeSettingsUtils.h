#ifndef GAUGE_SETTINGS_UTILS_H
#define GAUGE_SETTINGS_UTILS_H

#include <Arduino.h>
#include "lvgl.h" 
#include "GaugeSettings.h"

GaugeSettings loadGaugeSettings();
void saveGaugeSettings(
  const char* bgNormal,
  const char* bgWarning,
  const char* arcMain,
  const char* arcIndicator,
  const char* fontColor,
  const char* font,
  const char* bike,
  float maxAngle
);


void applyGaugeSettings(const GaugeSettings& s);
lv_color_t hexToColor(const String& hex);
void rebuildUIFromSettings();
void factoryResetUI();

#endif
