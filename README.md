# Lean Indicator Base Model
## Custom Options
- Bakground color normal
- Bakground color danger
- Font style
- Font size
- Font color normal
- Font color danger
- Vehicle image
- Acr color main bar
- Acr color indicator bar
- Acr color main thickness
- Acr color indicator thickness
- Knob present? Color / Size

## Tools
### LVGL Documention
https://docs.lvgl.io/8.3/

### Custom Fonts
https://lvgl.io/tools/fontconverter
- Use a .ttf or .oft file
- pick font size
- Bpp = 4 bit-per-pixel
- Output C file
- Range = 32-255

### Custom Images
https://lvgl.io/tools/imageconverter
- LVGL 8
- Use Cyberlink to edit image to be (transparent b.g., 150h x 170w, .png)
- select image
- Color Format = CF_RGB565A8
- Output C array