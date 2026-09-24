// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ ICONS =========================================
void iconClock(int cx, int cy, uint16_t c) {
  for (int r = 18; r <= 20; r++) tft.drawCircle(cx, cy, r, c);
  thickLine(cx, cy, cx, cy - 12, 2, c);
  thickLine(cx, cy, cx + 9, cy + 5, 2, c);
  tft.fillCircle(cx, cy, 2, c);
}
void iconTimer(int cx, int cy, uint16_t c) {
  for (int r = 15; r <= 17; r++) tft.drawCircle(cx, cy + 3, r, c);
  tft.fillRect(cx - 4, cy - 19, 8, 4, c);
  tft.fillRect(cx - 2, cy - 15, 4, 3, c);
  thickLine(cx, cy + 3, cx + 8, cy - 6, 2, c);
}
void iconGear(int cx, int cy, uint16_t c, uint16_t bg) {
  for (int i = 0; i < 8; i++) {
    float a = i * PI / 4.0f;
    tft.fillCircle(cx + (int)(cosf(a) * 16), cy + (int)(sinf(a) * 16), 4, c);
  }
  tft.fillCircle(cx, cy, 14, c);
  tft.fillCircle(cx, cy, 6, bg);
}
void iconPad(int cx, int cy, uint16_t c, uint16_t bg) {
  tft.fillRoundRect(cx - 26, cy - 13, 52, 28, 12, c);
  tft.fillRect(cx - 17, cy - 1, 12, 4, bg);
  tft.fillRect(cx - 13, cy - 5, 4, 12, bg);
  tft.fillCircle(cx + 10, cy + 3, 3, bg);
  tft.fillCircle(cx + 17, cy - 2, 3, bg);
}
void iconIslamic(int cx, int cy, uint16_t c, uint16_t bg) {
  tft.fillCircle(cx + 2, cy, 16, c);
  tft.fillCircle(cx + 11, cy - 4, 15, bg);
  int sx = cx - 10, sy = cy - 8;
  for (int i = 0; i < 4; i++) {
    float a = i * PI / 2.0f;
    tft.drawLine(sx + (int)(cosf(a)*6), sy + (int)(sinf(a)*6),
                 sx - (int)(cosf(a)*6), sy - (int)(sinf(a)*6), c);
  }
}
void iconAlarm(int cx, int cy, uint16_t c) {
  tft.fillCircle(cx, cy - 2, 14, c);
  tft.fillRect(cx - 14, cy - 2, 28, 12, c);
  tft.fillRect(cx - 18, cy + 8, 36, 4, c);
  tft.fillCircle(cx, cy - 16, 3, c);
  tft.fillCircle(cx, cy + 14, 3, c);
}

