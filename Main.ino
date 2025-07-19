// ===========
//   IMPORTS
// ===========
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "lv_conf.h"
#include "CST816S.h"
#include "QMI8658.h"
#include "logo.h"
#include "LeanUI.h"
#include "ui.h"
#include "BootAnimation.h"
#include "GaugeSettingsUtils.h" 
#include <Preferences.h>
#include <string>


// ++++++++++++
//   BLE SETUP
// ++++++++++++
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ArduinoJson.h>

BLECharacteristic *pAngleCharacteristic;
bool deviceConnected = false;
GaugeSettings s = loadGaugeSettings();
Preferences prefs;

#define SERVICE_UUID_LEAN_ANGLE  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID_LEAN_ANGLE     "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SERVICE_UUID_SETTINGS    "9703c1da-f099-4fb7-b7e3-6ac16aae762a"
#define CHAR_UUID_SETTINGS       "c8bfda4a-3a28-4ba0-8df5-cbf1a7214cb9"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

// // BLE SECURITY 
class MySecurity : public BLESecurityCallbacks {
  uint32_t onPassKeyRequest() {
    Serial.println("PassKeyRequest");
    return 123456; // static passkey
  }

  void onPassKeyNotify(uint32_t pass_key) {
    Serial.printf("On passkey Notify number: %d\n", pass_key);
  }

  bool onConfirmPIN(uint32_t pass_key) {
    Serial.printf("Confirm passkey: %d\n", pass_key);
    return true; // auto-confirm
  }

  bool onSecurityRequest() {
    Serial.println("Security Request");
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl) {
    if (auth_cmpl.success) {
      Serial.println("Authentication success");
    } else {
      Serial.printf("Authentication failed, status: %d\n", auth_cmpl.fail_reason);
    }
  }
};

class SettingsCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pSettingsCharacteristic) override {
    std::string value = pSettingsCharacteristic->getValue();
    Serial.println("Received settings over BLE:");
    Serial.println(value.c_str());

    if (value == "PR_RESET"){
      prefs.begin("lean_data", false);
      prefs.clear();
      prefs.end();
    } else if (value == "FACTORY_RESET"){
      prefs.begin("lean_data", false);
      prefs.clear();
      prefs.end();

      factoryResetUI();
      rebuildUIFromSettings();
    } else {
      // Parse JSON
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, value);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      GaugeSettings settings;
      settings.backgroundNormalColor = doc["backgroundNormalColor"] | s.backgroundNormalColor;
      settings.backgroundWarningColor = doc["backgroundWarningColor"] | s.backgroundWarningColor;
      settings.arcMainColor = doc["arcMainColor"] | s.arcMainColor;
      settings.arcIndicatorColor = doc["arcIndicatorColor"] | s.arcIndicatorColor;
      settings.fontColor = doc["fontColor"] | s.fontColor;
      settings.currentFont = doc["currentFont"] | s.currentFont;
      settings.currentBike = doc["currentBike"] | s.currentBike;
      settings.maxiumSafeAngle = doc["maxiumSafeAngle"] | s.maxiumSafeAngle;

      saveGaugeSettings(
        settings.backgroundNormalColor.c_str(),
        settings.backgroundWarningColor.c_str(),
        settings.arcMainColor.c_str(),
        settings.arcIndicatorColor.c_str(),
        settings.fontColor.c_str(),
        settings.currentFont.c_str(),
        settings.currentBike.c_str(),
        settings.maxiumSafeAngle
      );

      rebuildUIFromSettings();
    }
  }
};



// // ===============
// //   SCREEN PREP    --> This is what Waveshare provides. This is the minimum for screen & QMI8658 to work
// // ===============
#define LVGL_TICK_PERIOD_MS 2

static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);
CST816S touch(6, 7, 13, 5);

void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(disp_drv);
}

void example_increase_lvgl_tick(void *arg) {
  lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  bool touched = touch.available();
  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touch.data.x;
    data->point.y = touch.data.y;
  }
}

// // ============
// //    My Prep
// // ============
// //  - Global Variables

lv_obj_t * label;
lv_obj_t * screen;
lv_obj_t * arc;
lv_obj_t * img;
lv_obj_t* max_label;


void setup() {
  Serial.begin(115200);
  BLEDevice::init("Active-Gauges");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // --- 1. Settings Service ---
  BLEService *settingsService = pServer->createService(SERVICE_UUID_SETTINGS);
  BLECharacteristic *settingsCharacteristic = settingsService->createCharacteristic(
    CHAR_UUID_SETTINGS,
    BLECharacteristic::PROPERTY_WRITE
  );
  settingsCharacteristic->setCallbacks(new SettingsCallbacks());
  settingsService->start();

  // --- 2. Lean Angle Stream Service ---
  BLEService *streamService = pServer->createService(SERVICE_UUID_LEAN_ANGLE);
  pAngleCharacteristic = streamService->createCharacteristic(
    CHAR_UUID_LEAN_ANGLE,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pAngleCharacteristic->addDescriptor(new BLE2902()); // Enable notifications
  streamService->start();

  // BLE Security 
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  BLEDevice::setSecurityCallbacks(new MySecurity());

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
  pSecurity->setCapability(ESP_IO_CAP_OUT);
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
  pSecurity->setStaticPIN(123456);

  pServer->getAdvertising()->start();

  Serial.println("BLE server is running...");

  // // SCREEN SETUP - Again Provide by WaveShare, needed to work
  lv_init();
  tft.begin();
  tft.setRotation(0);
  touch.begin();

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };

  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000);
  
  // // Turn on Gyro/Accelerometer
  QMI8658_init();
  
  // // SCREEN BACKGROUND
  
  lv_disp_set_theme(lv_disp_get_default(), NULL);
  screen = lv_scr_act();
  lv_obj_clean(screen);
  lv_color_t backgroundNormalColor = hexToColor(s.backgroundNormalColor);
  lv_obj_set_style_bg_color(screen, backgroundNormalColor, LV_PART_MAIN | LV_STATE_DEFAULT);
  create_UI();              
  do_angle_reset();
  play_boot_animation();

  Serial.println("Setup done");
}

void loop() {
  lv_timer_handler();
  delay(5);

  if (!boot_done) return;

  int16_t angle;

  float gyro[3], acc[3];
  unsigned int tim_count = 0;
  QMI8658_read_xyz(gyro, acc, &tim_count);
  angle = gyro[0] * -0.09;


  update_UI(angle);
  check_and_set_max_angles(angle);

  if (deviceConnected) {
    struct SensorData {
      int16_t angle;
    };

    SensorData data = {angle};
    pAngleCharacteristic->setValue((uint8_t*)&data, sizeof(data));
    pAngleCharacteristic->notify();
  }

  
  
  delay(100);
}
