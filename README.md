# 🏍️ Active Gauges – The Adventure Begins!

Welcome to **Active Gauges**, a sleek and daring ESP32-powered lean-angle gauge designed for two-wheeled thrill seekers and builders who love code as much as the open road.

<p>
  <img src="lean_gauge.png" alt="Active Gauges Logo" width="200" style="border-radius: 15px;"/>
</p>

---

### 🧭 What is This?

This project transforms an **ESP32**, a **240x240 round LCD**, and a **QMI8658 IMU sensor** into a dynamic **motorcycle lean-angle display**. Alongside this core functionality, you’ll find:

- Multiple themed **gauge skins** like `adventure_gray`, `bloomira`, `marty`, and more  
- A smooth **boot animation system**  
- Easy customization of colors, fonts, and thresholds  
- Real-time **sensor data processing** with the QMI8658  
- A modern and lightweight UI using LVGL  

---

### 📦 Project Structure

```plaintext
Main/
├── Main.ino                  # Arduino sketch entry point
├── QMI8658.cpp/.h           # IMU sensor driver
├── LeanUI.cpp/.h            # UI rendering logic
├── GaugeSettings*.h/.cpp    # Persistent settings management
├── BootAnimation*.h/.cpp    # Startup animation logic
├── Skins/*.c/.h             # Gauge styles (skins)
├── CST816S.*                # Capacitive touch screen support (if used)
├── README.md                # You're reading it!
├──   ....                   # Images and Fonts
```
---
### ⚙️ Hardware Requirements
- 🧠 ESP32 (any dev board with enough memory and SPI pins)
- 🖼️ 240x240 Round LCD Display (ST7789 driver preferred)
- 🌀 QMI8658C IMU sensor
- 🔌 Optional: CST816S capacitive touch controller
---
### 🚀 Getting Started
1) Install LVGL and Dependencies via PlatformIO or Arduino IDE

2) Clone this repo and open Main.ino

3) Flash to your ESP32 board

4) Mount the hardware, power up, and go lean into those corners! 🏁
---
### 🧠 Configuration
#### **Settings are stored in Preferences (GaugeSettingsUtils.cpp). You can customize:**
- backgroundNormalColor, arcMainColor, fontColor, etc.
- Fonts: marty, Marty_30, etc.
- Bike presets (e.g., indian_scout)
- Max lean angle warning (maxiumSafeAngle)
---
### 🎨 Skins / Themes
#### **All skins are in .c/.h pairs inside the Skins/ directory. You can:**
- Swap which skin is included
- Create your own by copying one and tweaking its colors/fonts
#### **Available Skins:**
- adventure_gray
- bloomira
- bagger_black
- super_crumble
- and more!
---
🐛 Troubleshooting
- **Weird Colors?** Hex strings passed to lv_color_make() must be 6-digit RGB, not 8-digit ARGB.
- **Sensor not responding?** Double-check I2C wiring and address in QMI8658.cpp.
- **Touch screen unresponsive?** *(if availavble)* CST816S must be connected and optionally initialized.
---
### 🔮 Next Goals (Optional Ideas)
- Add BLE for ride data sync with a mobile app
- Enable theme switching from a button or BLE
- Log and replay lean angles and speed data
- Add G-force and tilt event capture
- **Expand into the 4x4 world!**
---
#### **Made with 🤘 by a builder who codes and rides.**