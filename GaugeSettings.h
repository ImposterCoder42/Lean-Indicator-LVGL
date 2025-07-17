#ifndef GAUGE_SETTINGS_H
#define GAUGE_SETTINGS_H

#include <Arduino.h>

struct GaugeSettings {
  String backgroundNormalColor;
  String backgroundWarningColor;
  String arcMainColor;
  String arcIndicatorColor;
  String fontColor;
  String currentFont;
  String currentBike;
  float maxiumSafeAngle;
};

#endif
