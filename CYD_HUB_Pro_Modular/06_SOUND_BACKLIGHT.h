// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ SOUND / BACKLIGHT =============================
uint32_t beepEnd = 0;
// Alarm state is declared later; beepTick must be aware of it.
extern bool alarmRinging;
void soundTone(int f) {
#if CORE3
  ledcWriteTone(PIN_SPK, f);
#else
  ledcWriteTone(2, f);
#endif
}
void beep(int f, int ms) {
  if (!cfg.sound) return;
  soundTone(f);
  beepEnd = millis() + ms;
  if (beepEnd == 0) beepEnd = 1;
}
void beepTick() {
  if (alarmRinging) return;
  if (beepEnd && (int32_t)(millis() - beepEnd) >= 0) { soundTone(0); beepEnd = 0; }
}
void setBacklight(uint8_t v) {
#if CORE3
  ledcWrite(PIN_BL, v);
#else
  ledcWrite(0, v);
#endif
}
void hwPwmInit() {
#if CORE3
  ledcAttachChannel(PIN_BL, 5000, 8, 0);
  ledcAttachChannel(PIN_SPK, 2000, 8, 2);
#else
  ledcSetup(0, 5000, 8);  ledcAttachPin(PIN_BL, 0);
  ledcSetup(2, 2000, 8);  ledcAttachPin(PIN_SPK, 2);
#endif
  setBacklight(cfg.bright);
  soundTone(0);
}

