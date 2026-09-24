// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ ALARM =========================================
struct Alarm {
  uint8_t hour;
  uint8_t minute;
  bool    enabled;
  uint8_t days;
};

Alarm alarms[3] = {
  {7,  0, false, 0x7F},
  {12,30, false, 0x7F},
  {18, 0, false, 0x7F}
};

bool     alarmRinging  = false;
int      alarmRingIdx  = -1;
uint32_t alarmBeepAt   = 0;
bool     alarmBeepHigh = false;
uint32_t alarmSnoozeUntil = 0;
int      alarmSnoozeIdx   = -1;
int      alarmLastMinute  = -1;
int      alarmLastDay      = -1;
int64_t  alarmFiredKey = -1;

bool     alarmSuppress    = false;
int      alarmEditIdx      = -1;
uint32_t alarmPopupUntil   = 0;
uint32_t alarmLastScanMs   = 0;

const char* alarmDaysLabel(uint8_t d) {
  if (d == 0x7F) return "DAILY";
  if (d == 0x3E) return "WEEKDAY";
  if (d == 0x41) return "WEEKEND";
  return "CUSTOM";
}

void saveAlarms() {
  for (int i = 0; i < 3; i++) {
    char k[12];
    snprintf(k, sizeof(k), "a%d_h", i); prefs.putUChar(k, alarms[i].hour);
    snprintf(k, sizeof(k), "a%d_m", i); prefs.putUChar(k, alarms[i].minute);
    snprintf(k, sizeof(k), "a%d_e", i); prefs.putBool(k, alarms[i].enabled);
    snprintf(k, sizeof(k), "a%d_d", i); prefs.putUChar(k, alarms[i].days);
  }
}
void loadAlarms() {
  for (int i = 0; i < 3; i++) {
    char k[12];
    snprintf(k, sizeof(k), "a%d_h", i); alarms[i].hour    = prefs.getUChar(k, alarms[i].hour);
    snprintf(k, sizeof(k), "a%d_m", i); alarms[i].minute  = prefs.getUChar(k, alarms[i].minute);
    snprintf(k, sizeof(k), "a%d_e", i); alarms[i].enabled = prefs.getBool(k, false);
    snprintf(k, sizeof(k), "a%d_d", i); alarms[i].days    = prefs.getUChar(k, 0x7F);
  }
}

void alarmDrawRow(int i) {
  int y = 31 + i * 60, x = 5, w = 310, h = 56;
  tft.fillRoundRect(x, y, w, h, 10, TH.panel);
  tft.drawRoundRect(x, y, w, h, 10, alarms[i].enabled ? TH.accent : TH.dim);

  char lbl[12]; snprintf(lbl, sizeof(lbl), "ALARM %d", i + 1);
  txt(lbl, x + 8, y + 5, 1, TH.dim, TH.panel, TL_DATUM);
  txt(alarmDaysLabel(alarms[i].days), x + 62, y + 5, 1, TH.accent, TH.panel, TL_DATUM);

  char t[8]; snprintf(t, sizeof(t), "%02d:%02d", alarms[i].hour, alarms[i].minute);
  // Large touch target for direct on-device time editing.
  tft.fillRoundRect(x + 6, y + 18, 106, 31, 8, TH.bg);
  txtp(t, x + 59, y + 34, 4, alarms[i].enabled ? TH.text : TH.dim, TH.bg, MC_DATUM, 1, 100);
  txt("TAP TIME", x + 59, y + 48, 1, TH.dim, TH.bg, MC_DATUM);

  button(x + 118, y + 19, 34, 29, "-H", TH.bg, TH.accent, 2);
  button(x + 156, y + 19, 34, 29, "+H", TH.bg, TH.accent, 2);
  button(x + 194, y + 19, 34, 29, "-M", TH.bg, TH.accent2, 2);
  button(x + 232, y + 19, 34, 29, "+M", TH.bg, TH.accent2, 2);
  tft.fillRoundRect(x + 272, y + 4, 31, 22, 10, alarms[i].enabled ? TH.good : TH.dim);
  tft.fillCircle(alarms[i].enabled ? x + 294 : x + 281, y + 15, 7, C565(255,255,255));
}

void alarmDrawEditor() {
  if (alarmEditIdx < 0 || alarmEditIdx >= 3) return;
  int i = alarmEditIdx;
  tft.fillScreen(TH.bg);
  drawBar((String("SET ALARM ") + String(i + 1)).c_str(), nullptr, false);

  tft.fillRoundRect(12, 38, 296, 91, 16, TH.panel);
  tft.drawRoundRect(12, 38, 296, 91, 16, TH.accent);
  char t[8]; snprintf(t, sizeof(t), "%02d:%02d", alarms[i].hour, alarms[i].minute);
  txt(t, 160, 78, 7, TH.text, TH.panel, MC_DATUM);
  txt("HOUR        MINUTE", 160, 110, 1, TH.dim, TH.panel, MC_DATUM);

  button(18, 140, 62, 30, "- H", TH.panel, TH.accent, 2);
  button(88, 140, 62, 30, "+ H", TH.panel, TH.accent, 2);
  button(170, 140, 62, 30, "- M", TH.panel, TH.accent2, 2);
  button(240, 140, 62, 30, "+ M", TH.panel, TH.accent2, 2);

  button(18, 181, 88, 30, "CANCEL", TH.panel, TH.text, 2);
  button(116, 181, 88, 30, alarmDaysLabel(alarms[i].days), TH.panel, TH.accent, 1);
  button(214, 181, 88, 30, "SAVE", TH.good, TH.onacc, 2);
  txt("Tap SAVE to keep the alarm", 160, 225, 1, TH.dim, TH.bg, MC_DATUM);
}

void alarmDrawRinging() {
  int w = 288, h = 178, x = (SW - w) / 2, y = (SH - h) / 2;
  tft.fillRoundRect(x, y, w, h, 16, TH.panel);
  for (int k = 0; k < 3; k++) tft.drawRoundRect(x + k, y + k, w - 2*k, h - 2*k, 16-k, k == 0 ? TH.accent2 : TH.accent);

  int cx = SW / 2, cy = y + 39;
  tft.fillCircle(cx, cy, 20, TH.accent2);
  tft.fillRect(cx - 20, cy, 40, 16, TH.accent2);
  tft.fillRect(cx - 24, cy + 14, 48, 5, TH.accent2);
  tft.fillCircle(cx, cy - 22, 4, TH.accent2);
  tft.fillCircle(cx, cy + 22, 4, TH.accent2);

  char t[12];
  snprintf(t, sizeof(t), "%02d:%02d", alarms[alarmRingIdx].hour, alarms[alarmRingIdx].minute);
  txt(t, SW / 2, y + 87, 4, TH.text, TH.panel, MC_DATUM);
  txt("ALARM RINGING", SW / 2, y + 111, 2, TH.accent2, TH.panel, MC_DATUM);

  button(x + 16,      y + h - 44, 118, 32, "SNOOZE 5m", TH.accent2, TH.onacc, 2);
  button(x + w - 134, y + h - 44, 118, 32, "STOP",      TH.bad, C565(255,255,255), 2);
}

void alarmInit() {
  alarmEditIdx = -1;
  tft.fillScreen(TH.bg);
  drawBar("ALARMS", nullptr, true);
  for (int i = 0; i < 3; i++) alarmDrawRow(i);
  button(5, 211, 72, 24, "TEST", TH.accent2, TH.onacc, 1);
  txt("TIME = edit  •  switch = ON/OFF", 196, 223, 1, TH.dim, TH.bg, MC_DATUM);
  if (alarmRinging) alarmDrawRinging();
}

void triggerAlarm(int idx) {
  if (idx < 0 || idx >= 3) return;
  alarmRinging = true;
  alarmRingIdx = idx;
  alarmBeepAt = millis();
  alarmBeepHigh = true;
  alarmSnoozeUntil = 0;
  alarmSnoozeIdx = -1;
  alarmPopupUntil = millis() + 600000UL;

  // Always force the alarm screen. This is intentionally independent of
  // the normal UI sound setting and of the current screen.
  screen = SCR_ALARM;
  tft.fillScreen(TH.bg);
  alarmDrawRinging();

  // Start the hardware buzzer immediately.
  soundTone(2800);
  beepEnd = 0;
  Serial.printf("ALARM TRIGGERED: %02u:%02u idx=%d\n", alarms[idx].hour, alarms[idx].minute, idx);
}

void dismissAlarm() {
  alarmRinging = false;
  alarmRingIdx = -1;
  soundTone(0); beepEnd = 0;
  alarmPopupUntil = 0;
  alarmSuppress = false;
  if (timeValid()) {
    alarmLastMinute = nowTm.tm_hour * 60 + nowTm.tm_min;
    alarmLastDay = nowTm.tm_yday;
    
  }
}

void snoozeAlarm() {
  if (alarmRingIdx < 0) return;
  alarmSnoozeIdx   = alarmRingIdx;
  alarmSnoozeUntil = millis() + 300000UL;
  dismissAlarm();
}

void alarmTick() {
  const uint32_t ms = millis();

  // Ringing is fully non-blocking: only the small alarm icon is animated.
  // The complete popup is NOT redrawn every few hundred milliseconds.
  if (alarmRinging) {
    if ((uint32_t)(ms - alarmBeepAt) >= 300UL) {
      alarmBeepAt = ms;
      alarmBeepHigh = !alarmBeepHigh;
      soundTone(alarmBeepHigh ? 2800 : 1750);
    }
    if (screen != SCR_ALARM) {
      screen = SCR_ALARM;
      tft.fillScreen(TH.bg);
      alarmDrawRinging();
    }
    // Smooth pulse without repainting text/buttons.
    static uint32_t pulseAt = 0;
    if ((int32_t)(ms - pulseAt) >= 0) {
      int w = 288, h = 178, x = (SW - w) / 2, y = (SH - h) / 2;
      int phase = (ms / 120) % 4;
      uint16_t c = (phase < 2) ? TH.accent2 : TH.accent;
      tft.drawRoundRect(x, y, w, h, 16, c);
      pulseAt = ms + 120;
    }
    return;
  }

  if (alarmSnoozeUntil && (int32_t)(ms - alarmSnoozeUntil) >= 0) {
    int idx = alarmSnoozeIdx;
    alarmSnoozeUntil = 0;
    alarmSnoozeIdx = -1;
    if (idx >= 0) triggerAlarm(idx);
    return;
  }

  // IMPORTANT: alarms always use the same local clock that the CYD displays.
  // This is independent of the web page refresh rate.
  if (!timeValid()) return;

  const int nowMin = nowTm.tm_hour * 60 + nowTm.tm_min;
  const int today = nowTm.tm_yday;
  const int64_t minuteKey = (int64_t)(nowTm.tm_year + 1900) * 60000LL +
                            (int64_t)today * 1440LL + nowMin;

  // Check every pass; only an actual fired minute is remembered.
  // Therefore an alarm edited/turned on during the current minute can fire.
  for (int i = 0; i < 3; ++i) {
    if (!alarms[i].enabled) continue;
    if (alarms[i].hour != nowTm.tm_hour || alarms[i].minute != nowTm.tm_min) continue;
    if (!(alarms[i].days == 0x7F || (alarms[i].days & (1 << nowTm.tm_wday)))) continue;
    if (alarmFiredKey == minuteKey) continue;

    alarmFiredKey = minuteKey;
    alarmLastScanMs = ms;
    triggerAlarm(i);
    return;
  }
}

void alarmLoop() {
  if (alarmRinging) {
    if (!tc.press) return;
    int w = 288, h = 178, x = (SW - w) / 2, y = (SH - h) / 2;
    if (pressIn(x + 16, y + h - 44, 118, 32)) { beep(1200,20); snoozeAlarm(); alarmInit(); return; }
    if (pressIn(x + w - 134, y + h - 44, 118, 32)) { beep(1000,20); dismissAlarm(); alarmInit(); return; }
    return;
  }

  if (alarmEditIdx >= 0) {
    if (!tc.press) return;
    int i = alarmEditIdx;
    if (pressIn(18,140,62,30)) { alarms[i].hour = (alarms[i].hour + 23) % 24; alarmFiredKey = -1; }
    else if (pressIn(88,140,62,30)) { alarms[i].hour = (alarms[i].hour + 1) % 24; alarmFiredKey = -1; }
    else if (pressIn(170,140,62,30)) { alarms[i].minute = (alarms[i].minute + 59) % 60; alarmFiredKey = -1; }
    else if (pressIn(240,140,62,30)) { alarms[i].minute = (alarms[i].minute + 1) % 60; alarmFiredKey = -1; }
    else if (pressIn(18,181,88,30)) { beep(900,20); alarmEditIdx = -1; alarmInit(); return; }
    else if (pressIn(116,181,88,30)) {
      uint8_t d = alarms[i].days;
      alarms[i].days = (d == 0x7F) ? 0x3E : (d == 0x3E) ? 0x41 : 0x7F;
    }
    else if (pressIn(214,181,88,30)) { saveAlarms(); beep(1600,25); alarmEditIdx = -1; alarmInit(); return; }
    else return;
    beep(1300,15); alarmDrawEditor(); return;
  }

  if (!tc.press) return;
  if (pressIn(5, 211, 72, 24)) { beep(1500, 20); triggerAlarm(0); return; }
  for (int i = 0; i < 3; i++) {
    int y = 31 + i * 60, x = 5;
    if (pressIn(x + 272, y + 4, 31, 22)) {
      alarms[i].enabled = !alarms[i].enabled; alarmFiredKey = -1; saveAlarms(); beep(1400,20); alarmDrawRow(i); return;
    }
    if (pressIn(x + 6, y + 18, 106, 31)) { alarmEditIdx = i; beep(1300,20); alarmDrawEditor(); return; }
    if (pressIn(x + 118, y + 19, 34, 29)) { alarms[i].hour=(alarms[i].hour+23)%24; alarmFiredKey=-1; saveAlarms(); beep(1200,15); alarmDrawRow(i); return; }
    if (pressIn(x + 156, y + 19, 34, 29)) { alarms[i].hour=(alarms[i].hour+1)%24; alarmFiredKey=-1; saveAlarms(); beep(1200,15); alarmDrawRow(i); return; }
    if (pressIn(x + 194, y + 19, 34, 29)) { alarms[i].minute=(alarms[i].minute+59)%60; alarmFiredKey=-1; saveAlarms(); beep(1200,15); alarmDrawRow(i); return; }
    if (pressIn(x + 232, y + 19, 34, 29)) { alarms[i].minute=(alarms[i].minute+1)%60; alarmFiredKey=-1; saveAlarms(); beep(1200,15); alarmDrawRow(i); return; }
  }
}

