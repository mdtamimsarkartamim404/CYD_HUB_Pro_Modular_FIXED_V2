// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ SETTINGS ======================================
const int SET_Y0 = 28, SET_STEP = 29, SET_H = 26;
bool sliderDrag = false;

void setRowBase(int i, const char* label) {
  int y = SET_Y0 + i * SET_STEP;
  tft.fillRoundRect(6, y, 308, SET_H, 8, TH.panel);
  txt(label, 16, y + SET_H / 2 + 1, 2, TH.text, TH.panel, ML_DATUM);
}
void toggleSwitch(int x, int y, bool on) {
  tft.fillRoundRect(x, y, 54, 22, 11, on ? TH.good : TH.dim);
  tft.fillCircle(on ? x + 42 : x + 11, y + 11, 8, C565(255,255,255));
  txt(on ? "ON" : "OFF", x - 10, y + 11, 1, on ? TH.good : TH.dim, TH.panel, MR_DATUM);
}
void setRowTheme() {
  setRowBase(0, "Theme");
  int y = SET_Y0;
  button(168, y + 1, 30, 24, "<", TH.bg, TH.accent, 2);
  button(278, y + 1, 28, 24, ">", TH.bg, TH.accent, 2);
  txt(TH.name, 238, y + SET_H / 2 + 1, 2, TH.text, TH.panel, MC_DATUM);
}
void setRowBright() {
  setRowBase(1, "Brightness");
  int y = SET_Y0 + SET_STEP;
  int kx = 140 + (int)(cfg.bright - 10) * 150 / 245;
  tft.fillRoundRect(140, y + 9, 150, 7, 4, TH.bg);
  tft.fillRoundRect(140, y + 9, kx - 140 + 4, 7, 4, TH.accent);
  tft.fillCircle(kx, y + 12, 9, TH.accent);
  tft.fillCircle(kx, y + 12, 3, TH.onacc);
}
void setRowH24() { setRowBase(2, "24-hour clock"); toggleSwitch(242, SET_Y0 + 2 * SET_STEP + 2, cfg.h24); }
void setRowSound() { setRowBase(3, "Sound"); toggleSwitch(242, SET_Y0 + 3 * SET_STEP + 2, cfg.sound); }
void setRowTemp() {
  setRowBase(4, "Temperature");
  int y = SET_Y0 + 4 * SET_STEP + 1;
  button(226, y, 36, 23, "C", cfg.fahr ? TH.bg : TH.accent, cfg.fahr ? TH.text : TH.onacc, 2);
  button(266, y, 36, 23, "F", cfg.fahr ? TH.accent : TH.bg, cfg.fahr ? TH.onacc : TH.text, 2);
}
void setRowClockStyle() {
  setRowBase(5, "Clock style");
  int y = SET_Y0 + 5 * SET_STEP;
  button(166, y + 1, 30, 24, "<", TH.bg, TH.accent, 2);
  button(278, y + 1, 28, 24, ">", TH.bg, TH.accent, 2);
  txt(CLOCK_STYLE_NAMES[cfg.clockStyle], 238, y + SET_H / 2 + 1, 2, TH.text, TH.panel, MC_DATUM);
}
void drawSetupButton() {
  button(55, 204, 210, 28, "Wi-Fi Setup / Scanner", TH.accent, TH.onacc, 2);
}
void settingsInit() {
  tft.fillScreen(TH.bg);
  drawBar("Settings", nullptr, false);
  setRowTheme(); setRowBright(); setRowH24(); setRowSound(); setRowTemp(); setRowClockStyle();
  drawSetupButton();
}
void settingsLoop() {
  if (tc.press) {
    if (pressIn(168, SET_Y0 + 1, 34, 28)) {
      cfg.theme = (cfg.theme + NTHEMES - 1) % NTHEMES;
      saveCfg(); beep(1200, 20); settingsInit(); return;
    }
    if (pressIn(276, SET_Y0 + 1, 34, 28)) {
      cfg.theme = (cfg.theme + 1) % NTHEMES;
      saveCfg(); beep(1200, 20); settingsInit(); return;
    }
    if (pressIn(120, SET_Y0 + SET_STEP, 192, SET_H)) sliderDrag = true;
    if (pressIn(236, SET_Y0 + 2 * SET_STEP, 78, SET_H)) {
      cfg.h24 = !cfg.h24; saveCfg(); beep(1400, 20); setRowH24();
    }
    if (pressIn(236, SET_Y0 + 3 * SET_STEP, 78, SET_H)) {
      cfg.sound = !cfg.sound; saveCfg(); beep(1400, 20); setRowSound();
    }
    if (pressIn(220, SET_Y0 + 4 * SET_STEP, 86, SET_H)) {
      cfg.fahr = tc.x >= 264; saveCfg(); beep(1400, 20); setRowTemp();
    }
    if (pressIn(164, SET_Y0 + 5 * SET_STEP, 148, SET_H)) {
      if (tc.x < 205) cfg.clockStyle = (cfg.clockStyle + 19) % 20;
      else cfg.clockStyle = (cfg.clockStyle + 1) % 20;
      saveCfg(); beep(1200, 20); setRowClockStyle();
    }
    if (pressIn(50, 202, 220, 34)) {
      beep(1500, 20); goScreen(SCR_SETUP); return;
    }
  }
  if (sliderDrag) {
    if (tc.down) {
      int v = constrain((int)map(tc.x, 140, 290, 10, 255), 10, 255);
      if (v != cfg.bright) { cfg.bright = v; setBacklight(v); setRowBright(); }
    }
    if (tc.release) { sliderDrag = false; saveCfg(); }
  }
}

