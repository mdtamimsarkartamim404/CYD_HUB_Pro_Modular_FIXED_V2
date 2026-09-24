# CYD HUB Pro — Modular Arduino Project

Board target:
- ESP32-2432S028R / CYD
- ST7789 display
- XPT2046 touch

## Files
`CYD_HUB_Pro_Modular.ino` is the only sketch file you open/upload.
The numbered `.h` files are included automatically in the original source order.

## Arduino IDE
1. Put the whole folder in your Arduino sketchbook.
2. Open `CYD_HUB_Pro_Modular.ino`.
3. Install the same libraries required by the original sketch:
   - TFT_eSPI
   - XPT2046_Touchscreen
   - ArduinoJson
   - QRCode
   - ESP32 board package
4. Select your ESP32/CYD board and compile/upload.

## Important
The split is deliberately a same-translation-unit modularization. It does not
move globals into namespaces or introduce `extern` declarations, which avoids
breaking the existing clock/timer/game/web-control dependencies.

The setup() and loop() functions remain in the `.ino` file for Arduino IDE
compatibility. All other major sections are separated into numbered files.

## Original logic
The source sections are preserved in their original order and content; only
the file organization was changed.
