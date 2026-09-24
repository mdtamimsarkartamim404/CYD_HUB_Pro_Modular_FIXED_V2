// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ STORAGE =======================================
void saveCfg() {
  prefs.putUChar("theme", cfg.theme);
  prefs.putUChar("bright", cfg.bright);
  prefs.putBool("h24", cfg.h24);
  prefs.putBool("snd", cfg.sound);
  prefs.putBool("fahr", cfg.fahr);
  prefs.putUChar("clock", cfg.clockStyle);
}
void loadCfg() {
  prefs.begin("cydhub", false);
  cfg.theme  = prefs.getUChar("theme", 0);
  if (cfg.theme >= NTHEMES) cfg.theme = 0;
  cfg.bright = prefs.getUChar("bright", 200);
  if (cfg.bright < 10) cfg.bright = 10;
  cfg.h24    = prefs.getBool("h24", false);
  cfg.sound  = prefs.getBool("snd", true);
  cfg.fahr   = prefs.getBool("fahr", false);
  if (prefs.isKey("clock")) cfg.clockStyle = prefs.getUChar("clock", 0);
  else cfg.clockStyle = prefs.getBool("ana", false) ? 1 : 0;
  if (cfg.clockStyle > 19) cfg.clockStyle = 0;
  for (int i = 0; i < 10; i++) {
    char k[4]; snprintf(k, sizeof(k), "h%d", i);
    hi[i] = prefs.getUInt(k, 0);
  }
  loadAlarms();
}
void flushHi() {
  if (!hiDirty) return;
  hiDirty = false;
  for (int i = 0; i < 10; i++) {
    char k[4]; snprintf(k, sizeof(k), "h%d", i);
    prefs.putUInt(k, hi[i]);
  }
}
bool lowerBetter(int g) { return g == 2 || g == 4; }
void submitScore(int g, uint32_t s) {
  if (g < 0 || g > 9) return;
  bool better;
  if (hi[g] == 0) better = (s > 0);
  else better = lowerBetter(g) ? (s < hi[g]) : (s > hi[g]);
  if (better) { hi[g] = s; hiDirty = true; }
}

