// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ HOME ==========================================
struct Rect { int x, y, w, h; };
const Rect TILES[8] = {
  {  4, 54, 75, 88}, { 83, 54, 75, 88}, {162, 54, 75, 88}, {241, 54, 75, 88},
  {  4,146, 75, 88}, { 83,146, 75, 88}, {162,146, 75, 88}, {241,146, 75, 88}
};
const char* TILE_NAME[8] = {"Clock","Timer","Alarm","Weather","Games","Islamic","Settings","Knowledge"};

void drawTile(int i) {
  int x = TILES[i].x, y = TILES[i].y, w = TILES[i].w, h = TILES[i].h;
  tft.fillRoundRect(x, y, w, h, 12, TH.panel);
  int cx = x + w / 2, cy = y + 34;
  switch (i) {
    case 0: iconClock(cx, cy, TH.accent); break;
    case 1: iconTimer(cx, cy, TH.accent2); break;
    case 2: iconAlarm(cx, cy, TH.accent2); break;
    case 3: drawWxIcon(cx, cy, 4, 1); break;
    case 4: iconPad(cx, cy, TH.good, TH.panel); break;
    case 5: iconIslamic(cx, cy, TH.accent2, TH.panel); break;
    case 6: iconGear(cx, cy, TH.good, TH.panel); break;
    case 7:
      tft.drawCircle(cx, cy, 18, TH.accent);
      txt("?", cx, cy + 1, 4, TH.accent, TH.panel, MC_DATUM);
      break;
    default: break;
  }
  txt(TILE_NAME[i], cx, y + h - 14, 1, TH.text, TH.panel, MC_DATUM);
}
void homeInit() {
  tft.fillScreen(TH.bg);
  for (int i = 0; i < 8; i++) drawTile(i);
  homeHeader(true);
}
void homeHeader(bool force) {
  String t = "--:--", d = "Set time from Control";
  if (timeValid()) {
    char b[24];
    if (cfg.h24) snprintf(b, sizeof(b), "%02d:%02d", nowTm.tm_hour, nowTm.tm_min);
    else snprintf(b, sizeof(b), "%d:%02d %s", hour12(nowTm.tm_hour), nowTm.tm_min, nowTm.tm_hour >= 12 ? "PM" : "AM");
    t = String(b);
    snprintf(b, sizeof(b), "%s, %d %s", DOW3[nowTm.tm_wday], nowTm.tm_mday, MON3[nowTm.tm_mon]);
    d = String(b);
  } else if (WiFi.status() == WL_CONNECTED) d = "Syncing time...";

  static String lastT = "", lastD = "";
  static int lastWx = -9999, lastBars = -1;

  if (force || t != lastT) { lastT = t; txtp(t, 12, 6, 4, TH.text, TH.bg, TL_DATUM, 1, 140); }
  if (force || d != lastD) { lastD = d; txtp(d, 12, 34, 2, TH.dim, TH.bg, TL_DATUM, 1, 150); }

  int bars = WiFi.status() == WL_CONNECTED ? (WiFi.RSSI() / 5) : -99;
  if (force || bars != lastBars) {
    lastBars = bars;
    tft.fillRect(SW - 30, 6, 26, 24, TH.bg);
    drawWifi(SW - 22, 8);
  }

  int wt = (WiFi.status() == WL_CONNECTED && wx.ok) ? tempNum(wx.t) : -9999;
  if (force || wt != lastWx) {
    lastWx = wt;
    if (WiFi.status() == WL_CONNECTED && wx.ok) {
      tft.fillRect(SW - 110, 22, 106, 30, TH.bg);
      drawWxIcon(SW - 92, 30, 2, wmoIcon(wx.code));
      drawTemp(SW - 62, 26, wx.t, 2, 1, TH.text, TH.bg);
    }
  }
}
void homeLoop() {
  static uint32_t last = 0;
  if (millis() - last > 1000) { last = millis(); homeHeader(false); }
  if (!tc.press) return;
  for (int i = 0; i < 8; i++) {
    if (pressIn(TILES[i].x, TILES[i].y, TILES[i].w, TILES[i].h)) {
      beep(1500, 20);
      switch (i) {
        case 0: goScreen(SCR_CLOCK);  break;
        case 1: goScreen(SCR_TIMER);  break;
        case 2: goScreen(SCR_ALARM);  break;
        case 3: goScreen(SCR_WEATHER);break;
        case 4: goScreen(SCR_GAMES);  break;
        case 5: goScreen(SCR_ISLAMIC);break;
        case 6: goScreen(SCR_SETTINGS);break;
        case 7: goScreen(SCR_KNOWLEDGE);break;
        default: break;
      }
      return;
    }
  }
}

