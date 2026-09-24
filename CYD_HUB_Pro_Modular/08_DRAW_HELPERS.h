// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ DRAW HELPERS ==================================
void txtp(const String &s, int x, int y, uint8_t font, uint16_t fg, uint16_t bg,
          uint8_t datum, uint8_t size, uint16_t pad) {
  tft.setTextDatum(datum);
  tft.setTextColor(fg, bg);
  tft.setTextSize(size);
  tft.setTextPadding(pad);
  tft.drawString(s, x, y, font);
  tft.setTextPadding(0);
  tft.setTextSize(1);
}
void txt(const String &s, int x, int y, uint8_t font, uint16_t fg, uint16_t bg, uint8_t datum) {
  txtp(s, x, y, font, fg, bg, datum, 1, 0);
}
bool inRect(int px, int py, int x, int y, int w, int h) {
  return px >= x && px < x + w && py >= y && py < y + h;
}
bool pressIn(int x, int y, int w, int h) {
  return tc.press && inRect(tc.x, tc.y, x, y, w, h);
}
void thickLine(int x0, int y0, int x1, int y1, int t, uint16_t c) {
  int r = t / 2;
  for (int k = -r; k <= r; k++) {
    tft.drawLine(x0 + k, y0, x1 + k, y1, c);
    tft.drawLine(x0, y0 + k, x1, y1 + k, c);
  }
}
void button(int x, int y, int w, int h, const String &label, uint16_t fill, uint16_t fg, uint8_t font) {
  tft.fillRoundRect(x, y, w, h, 7, fill);
  txt(label, x + w / 2, y + h / 2 + 1, font, fg, fill, MC_DATUM);
}
void drawWifi(int x, int y) {
  int bars = 0;
  if (WiFi.status() == WL_CONNECTED) {
    int r = WiFi.RSSI();
    bars = (r > -55) ? 4 : (r > -65) ? 3 : (r > -75) ? 2 : 1;
  }
  for (int i = 0; i < 4; i++) {
    int h = 3 + i * 3;
    tft.fillRect(x + i * 4, y + 12 - h, 3, h, i < bars ? TH.accent : TH.dim);
  }
}
void drawBar(const char* title, const char* action, bool wifi) {
  tft.fillRect(0, 0, SW, BAR, TH.panel);
  tft.fillRoundRect(4, 3, 42, 20, 6, TH.accent);
  tft.fillTriangle(16, 13, 25, 7, 25, 19, TH.onacc);
  tft.fillRect(25, 11, 10, 5, TH.onacc);
  txtp(title, 54, BAR / 2 + 1, 2, TH.text, TH.panel, ML_DATUM, 1, 0);
  if (action) button(SW - 68, 3, 40, 20, action, TH.accent2, TH.onacc, 1);
  if (wifi) drawWifi(SW - 20, 7);
}
void clearContent() { tft.fillRect(0, BAR, SW, SH - BAR, TH.bg); }

