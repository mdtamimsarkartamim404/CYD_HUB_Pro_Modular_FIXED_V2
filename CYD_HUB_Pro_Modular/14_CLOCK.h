// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ CLOCK =========================================
int clkLastSec = -1;
bool clkMsgShown = false;
uint32_t clkLastFrame = 0;
int clkLastTemp = -9999;
const int CLOCK_CX = 160;

const char* CLOCK_STYLE_NAMES[20] = {
  "Digital", "Analog", "Big Digital", "Minimal", "Ring",
  "Neon", "Dashboard", "Split", "Clean", "Seconds",
  "Gear Train", "Pendulum", "Roman", "Moon Phase", "Compass",
  "Industrial", "Concentric", "Binary LED", "Sun Dial", "Balance"
};

String clockTimeString() {
  char b[20];
  if (cfg.h24) snprintf(b, sizeof(b), "%02d:%02d", nowTm.tm_hour, nowTm.tm_min);
  else snprintf(b, sizeof(b), "%d:%02d", hour12(nowTm.tm_hour), nowTm.tm_min);
  return String(b);
}
String clockSecondsString() {
  char b[8]; snprintf(b, sizeof(b), "%02d", nowTm.tm_sec); return String(b);
}
String clockDateLong() {
  char b[40];
  snprintf(b, sizeof(b), "%s  %02d %s %04d",
           DOW3[nowTm.tm_wday], nowTm.tm_mday, MON3[nowTm.tm_mon], nowTm.tm_year + 1900);
  return String(b);
}
String clockDateShort() {
  char b[32];
  snprintf(b, sizeof(b), "%s  %02d %s",
           DOW3[nowTm.tm_wday], nowTm.tm_mday, MON3[nowTm.tm_mon]);
  return String(b);
}
String clockTempString() {
  return String(tempNum(wx.t)) + (cfg.fahr ? " F" : " C");
}

void clockSpriteBegin() {
  if (clockSpriteReady) return;
  clockSprite.setColorDepth(16);
  if (clockSprite.createSprite(176, 176) != nullptr) clockSpriteReady = true;
}
void clockSpriteEnd() {
  if (!clockSpriteReady) return;
  clockSprite.deleteSprite(); clockSpriteReady = false;
}

void clockMarksNew(int cx, int cy, int r) {
  for (int i = 0; i < 60; i++) {
    float a = i * PI / 30.0f;
    float sn = sinf(a), cs = cosf(a);
    int r1 = (i % 5 == 0) ? r - 14 : r - 7;
    int r2 = r - 4;
    int x1 = cx + (int)(sn * r1), y1 = cy - (int)(cs * r1);
    int x2 = cx + (int)(sn * r2), y2 = cy - (int)(cs * r2);
    uint16_t c = (i % 5 == 0) ? TH.accent : TH.dim;
    if (i % 5 == 0) thickLine(x1, y1, x2, y2, 2, c);
    else tft.drawPixel(x2, y2, c);
  }
}
void clockFaceNew(int cx, int cy, int r) {
  tft.fillCircle(cx, cy, r, TH.panel);
  tft.drawCircle(cx, cy, r, TH.accent);
  tft.drawCircle(cx, cy, r - 2, TH.dim);
  clockMarksNew(cx, cy, r);
  txt("12", cx, cy - r + 15, 1, TH.text, TH.panel, MC_DATUM);
  txt("3",  cx + r - 13, cy, 1, TH.text, TH.panel, MC_DATUM);
  txt("6",  cx, cy + r - 15, 1, TH.text, TH.panel, MC_DATUM);
  txt("9",  cx - r + 13, cy, 1, TH.text, TH.panel, MC_DATUM);
}
void clockHandsSmooth(int cx, int cy, float h, float m, float s) {
  float ah = h * 30.0f * DEG_TO_RAD;
  float am = m * 6.0f * DEG_TO_RAD;
  float as = s * 6.0f * DEG_TO_RAD;
  thickLine(cx, cy, cx + (int)(sinf(ah) * 42), cy - (int)(cosf(ah) * 42), 4, TH.text);
  thickLine(cx, cy, cx + (int)(sinf(am) * 62), cy - (int)(cosf(am) * 62), 3, TH.accent);
  tft.drawLine(cx, cy, cx + (int)(sinf(as) * 72), cy - (int)(cosf(as) * 72), TH.accent2);
  tft.fillCircle(cx, cy, 5, TH.accent);
  tft.fillCircle(cx, cy, 2, TH.bg);
}

void drawAnalogSpriteFrame() {
  clockSpriteBegin();
  if (!clockSpriteReady) {
    tft.fillRect(0, BAR, SW, SH - BAR, TH.bg);
    clockFaceNew(108, 132, 88);
    float sF = nowTm.tm_sec + ((millis() % 1000UL) / 1000.0f);
    float mF = nowTm.tm_min + sF / 60.0f;
    float hF = (nowTm.tm_hour % 12) + mF / 60.0f;
    clockHandsSmooth(108, 132, hF, mF, sF);
    return;
  }
  const int sy = 44, cx = 88, cy = 88, r = 80;
  clockSprite.fillSprite(TH.bg);
  clockSprite.fillCircle(cx, cy, r, TH.panel);
  clockSprite.drawCircle(cx, cy, r, TH.accent);
  clockSprite.drawCircle(cx, cy, r - 2, TH.dim);
  for (int i = 0; i < 60; i++) {
    float a = i * PI / 30.0f;
    float sn = sinf(a), cs = cosf(a);
    int r1 = (i % 5 == 0) ? r - 13 : r - 7;
    int r2 = r - 4;
    int x1 = cx + (int)(sn * r1), y1 = cy - (int)(cs * r1);
    int x2 = cx + (int)(sn * r2), y2 = cy - (int)(cs * r2);
    uint16_t c = (i % 5 == 0) ? TH.accent : TH.dim;
    if (i % 5 == 0) {
      clockSprite.drawLine(x1, y1, x2, y2, c);
      clockSprite.drawLine(x1 + 1, y1, x2 + 1, y2, c);
      clockSprite.drawLine(x1, y1 + 1, x2, y2 + 1, c);
    } else clockSprite.drawPixel(x2, y2, c);
  }
  clockSprite.setTextDatum(MC_DATUM);
  clockSprite.setTextColor(TH.text, TH.panel);
  clockSprite.setTextSize(1);
  clockSprite.drawString("12", cx, cy - r + 14, 1);
  clockSprite.drawString("3",  cx + r - 12, cy, 1);
  clockSprite.drawString("6",  cx, cy + r - 14, 1);
  clockSprite.drawString("9",  cx - r + 12, cy, 1);
  float secF = nowTm.tm_sec + ((millis() % 1000UL) / 1000.0f);
  float minF = nowTm.tm_min + secF / 60.0f;
  float hourF = (nowTm.tm_hour % 12) + minF / 60.0f;
  float ah = hourF * 30.0f * DEG_TO_RAD;
  float am = minF  * 6.0f  * DEG_TO_RAD;
  float as = secF  * 6.0f  * DEG_TO_RAD;
  int hx = cx + (int)(sinf(ah) * 42), hy = cy - (int)(cosf(ah) * 42);
  int mx = cx + (int)(sinf(am) * 62), my = cy - (int)(cosf(am) * 62);
  int sx2 = cx + (int)(sinf(as) * 72), sy2 = cy - (int)(cosf(as) * 72);
  clockSprite.drawLine(cx, cy, hx, hy, TH.text);
  clockSprite.drawLine(cx+1, cy, hx+1, hy, TH.text);
  clockSprite.drawLine(cx, cy+1, hx, hy+1, TH.text);
  clockSprite.drawLine(cx, cy, mx, my, TH.accent);
  clockSprite.drawLine(cx+1, cy, mx+1, my, TH.accent);
  clockSprite.drawLine(cx, cy+1, mx, my+1, TH.accent);
  clockSprite.drawLine(cx, cy, sx2, sy2, TH.accent2);
  clockSprite.fillCircle(cx, cy, 5, TH.accent);
  clockSprite.fillCircle(cx, cy, 2, TH.bg);
  clockSprite.pushSprite(20, sy);
  txtp(clockTimeString(), 260, 76, 4, TH.text, TH.bg, MC_DATUM, 1, 90);
  if (!cfg.h24) {
    txtp(nowTm.tm_hour >= 12 ? "PM" : "AM", 260, 100, 2, TH.accent2, TH.bg, MC_DATUM, 1, 70);
  } else tft.fillRect(238, 90, 44, 24, TH.bg);
  txtp(DOW3[nowTm.tm_wday], 260, 137, 2, TH.accent, TH.bg, MC_DATUM, 1, 70);
  txtp(clockDateShort(), 260, 162, 1, TH.dim, TH.bg, MC_DATUM, 1, 100);
}

void clockDrawStatic() {
  tft.fillRect(0, BAR, SW, SH - BAR, TH.bg);
  switch (cfg.clockStyle) {
    case 0:
      tft.drawRoundRect(8, BAR + 8, SW - 16, SH - BAR - 16, 14, TH.panel);
      txtp(clockDateLong(), CLOCK_CX, 164, 2, TH.text, TH.bg, MC_DATUM, 1, 296);
      if (wx.ok) txtp(clockTempString(), CLOCK_CX, 194, 2, TH.dim, TH.bg, MC_DATUM, 1, 296);
      break;
    case 1: break;
    case 2:
      txtp(clockDateLong(), CLOCK_CX, 170, 2, TH.text, TH.bg, MC_DATUM, 1, 296);
      if (wx.ok) txtp(clockTempString(), CLOCK_CX, 201, 2, TH.dim, TH.bg, MC_DATUM, 1, 296);
      break;
    case 3:
      txtp(clockDateLong(), CLOCK_CX, 181, 2, TH.dim, TH.bg, MC_DATUM, 1, 296);
      break;
    case 4: {
      const int cx = CLOCK_CX, cy = 103, r = 61;
      tft.fillCircle(cx, cy, r, TH.panel);
      tft.drawCircle(cx, cy, r, TH.accent);
      tft.drawCircle(cx, cy, r - 2, TH.dim);
      for (int i = 0; i < 60; i += 5) {
        float a = i * PI / 30.0f;
        int x = cx + (int)(cosf(a) * (r - 7));
        int y = cy + (int)(sinf(a) * (r - 7));
        tft.fillCircle(x, y, 2, TH.accent);
      }
      txtp(clockDateShort(), cx, 208, 1, TH.dim, TH.bg, MC_DATUM, 1, 296);
    } break;
    case 5:
      tft.drawFastHLine(38, 177, 244, TH.accent);
      txtp(clockDateLong(), CLOCK_CX, 198, 2, TH.text, TH.bg, MC_DATUM, 1, 296);
      break;
    case 6:
      tft.drawRoundRect(8, BAR + 8, SW - 16, SH - BAR - 16, 14, TH.panel);
      txt("TIME", 30, 48, 1, TH.dim, TH.bg, ML_DATUM);
      txt("DATE", 30, 142, 1, TH.dim, TH.bg, ML_DATUM);
      txtp(clockDateLong(), 30, 168, 2, TH.accent, TH.bg, ML_DATUM, 1, 296);
      if (wx.ok) txtp("TEMP  " + clockTempString(), 30, 200, 1, TH.dim, TH.bg, ML_DATUM, 1, 296);
      break;
    case 7:
      tft.fillRoundRect(12, BAR + 10, 296, 82, 12, TH.panel);
      tft.fillRoundRect(12, BAR + 101, 296, 88, 12, TH.panel);
      txtp(clockDateLong(), CLOCK_CX, 151, 2, TH.text, TH.panel, MC_DATUM, 1, 296);
      if (wx.ok) txtp(clockTempString(), CLOCK_CX, 179, 2, TH.dim, TH.panel, MC_DATUM, 1, 296);
      break;
    case 8:
      tft.fillRoundRect(96, 128, 128, 3, 2, TH.accent);
      txtp(clockDateLong(), CLOCK_CX, 158, 2, TH.dim, TH.bg, MC_DATUM, 1, 296);
      if (wx.ok) txtp(clockTempString(), CLOCK_CX, 191, 2, TH.accent, TH.bg, MC_DATUM, 1, 296);
      break;
    case 9:
      txtp("SECONDS", CLOCK_CX, 139, 1, TH.dim, TH.bg, MC_DATUM, 1, 296);
      break;
  }
}

void clockDrawDynamic() {
  switch (cfg.clockStyle) {
    case 0:
      txtp(clockTimeString(), CLOCK_CX, 93, 5, TH.accent, TH.bg, MC_DATUM, 1, 296);
      txtp(clockSecondsString(), CLOCK_CX, 130, 2, TH.accent2, TH.bg, MC_DATUM, 1, 120);
      break;
    case 2:
      txtp(clockTimeString(), CLOCK_CX, 91, 6, TH.accent, TH.bg, MC_DATUM, 1, 296);
      txtp(clockSecondsString(), CLOCK_CX, 132, 4, TH.accent2, TH.bg, MC_DATUM, 1, 140);
      break;
    case 3:
      txtp(clockTimeString(), CLOCK_CX, 104, 7, TH.text, TH.bg, MC_DATUM, 1, 296);
      txtp(clockSecondsString(), CLOCK_CX, 145, 3, TH.accent, TH.bg, MC_DATUM, 1, 120);
      break;
    case 4: {
      const int cx = CLOCK_CX, cy = 103, r = 61;
      tft.fillCircle(cx, cy, r - 8, TH.panel);
      float secF = nowTm.tm_sec + ((millis() % 1000UL) / 1000.0f);
      float minF = nowTm.tm_min + secF / 60.0f;
      float hourF = (nowTm.tm_hour % 12) + minF / 60.0f;
      float sa = (secF * 6.0f - 90.0f) * DEG_TO_RAD;
      float ma = (minF * 6.0f - 90.0f) * DEG_TO_RAD;
      float ha = (hourF * 30.0f - 90.0f) * DEG_TO_RAD;
      thickLine(cx, cy, cx + (int)(30 * cosf(ha)), cy + (int)(30 * sinf(ha)), 4, TH.text);
      thickLine(cx, cy, cx + (int)(45 * cosf(ma)), cy + (int)(45 * sinf(ma)), 3, TH.accent);
      tft.drawLine(cx, cy, cx + (int)(53 * cosf(sa)), cy + (int)(53 * sinf(sa)), TH.good);
      tft.fillCircle(cx, cy, 4, TH.text);
      txtp(clockTimeString(), cx, 181, 2, TH.text, TH.bg, MC_DATUM, 1, 296);
    } break;
    case 5:
      txtp(clockTimeString(), CLOCK_CX, 96, 7, TH.accent, TH.bg, MC_DATUM, 1, 296);
      txtp(clockSecondsString(), CLOCK_CX, 151, 4, TH.accent2, TH.bg, MC_DATUM, 1, 140);
      break;
    case 6:
      txtp(clockTimeString(), 30, 91, 6, TH.text, TH.bg, ML_DATUM, 1, 240);
      txtp(clockSecondsString(), 287, 91, 4, TH.accent2, TH.bg, MR_DATUM, 1, 100);
      break;
    case 7:
      txtp(clockTimeString(), CLOCK_CX, 72, 5, TH.accent, TH.panel, MC_DATUM, 1, 296);
      txtp(clockSecondsString(), CLOCK_CX, 111, 3, TH.accent2, TH.panel, MC_DATUM, 1, 120);
      break;
    case 8:
      txtp(clockTimeString(), CLOCK_CX, 98, 6, TH.text, TH.bg, MC_DATUM, 1, 296);
      break;
    case 9:
      txtp(clockTimeString(), CLOCK_CX, 91, 5, TH.text, TH.bg, MC_DATUM, 1, 296);
      txtp(clockSecondsString(), CLOCK_CX, 174, 7, TH.accent2, TH.bg, MC_DATUM, 1, 200);
      break;
  }
}

void drawAnimatedClockNew(int style) {
  // All new animated clocks use the same 176x176 double-buffered sprite as
  // the original smooth analog clock. This prevents full-screen redraw
  // flicker and keeps animation fluid on the CYD ESP32.
  clockSpriteBegin();
  if (!clockSpriteReady) return;

  const int cx = 88, cy = 88, r = 78;
  const uint32_t ms = millis();
  const float t = ms / 1000.0f;
  const float secF = nowTm.tm_sec + fmodf(ms, 1000.0f) / 1000.0f;
  const float minF = nowTm.tm_min + secF / 60.0f;
  const float hourF = (nowTm.tm_hour % 12) + minF / 60.0f;

  clockSprite.fillSprite(TH.bg);

  if (style == 110) { // Orbit
    clockSprite.drawCircle(cx, cy, 68, TH.panel);
    clockSprite.drawCircle(cx, cy, 52, TH.dim);
    for (int i = 0; i < 3; ++i) {
      float a = t * (0.8f + i * 0.23f) + i * 2.0943951f;
      int rr = 42 + i * 7;
      int x = cx + (int)(cosf(a) * rr);
      int y = cy + (int)(sinf(a) * rr);
      uint16_t col = (i == 0) ? TH.accent : (i == 1 ? TH.accent2 : TH.good);
      clockSprite.fillCircle(x, y, 4 + i, col);
    }
    clockSprite.setTextDatum(MC_DATUM);
    clockSprite.setTextColor(TH.text, TH.bg);
    clockSprite.drawString(clockTimeString(), cx, cy, 4);
  }
  else if (style == 111) { // Pulse
    float p = 0.5f + 0.5f * sinf(t * 3.0f);
    int pr = 18 + (int)(p * 18.0f);
    for (int i = 3; i >= 0; --i) {
      int rr = pr + i * 12;
      uint16_t col = (i == 0) ? TH.accent : (i == 1 ? TH.accent2 : TH.panel);
      clockSprite.drawCircle(cx, cy, rr, col);
    }
    clockSprite.fillCircle(cx, cy, 8, TH.panel);
    clockSprite.setTextDatum(MC_DATUM);
    clockSprite.setTextColor(TH.text, TH.panel);
    clockSprite.drawString(clockTimeString(), cx, cy, 4);
  }
  else if (style == 112) { // Matrix rain
    for (int i = 0; i < 13; ++i) {
      float speed = 0.55f + (i % 4) * 0.17f;
      float yy = fmodf(t * 34.0f * speed + i * 29.0f, 190.0f) - 15.0f;
      int xx = 8 + i * 13;
      int len = 3 + (i % 5);
      for (int k = 0; k < len; ++k) {
        int y = (int)yy - k * 8;
        if (y >= 0 && y < 176) clockSprite.fillRect(xx, y, 3, 5, k == 0 ? TH.accent : TH.panel);
      }
    }
    clockSprite.fillRoundRect(20, 65, 136, 52, 10, TH.bg);
    clockSprite.drawRoundRect(20, 65, 136, 52, 10, TH.accent);
    clockSprite.setTextDatum(MC_DATUM);
    clockSprite.setTextColor(TH.text, TH.bg);
    clockSprite.drawString(clockTimeString(), cx, 84, 4);
    clockSprite.setTextColor(TH.accent2, TH.bg);
    clockSprite.drawString(clockSecondsString(), cx, 106, 2);
  }
  else if (style == 113) { // Solar system
    clockSprite.drawCircle(cx, cy, 68, TH.panel);
    clockSprite.fillCircle(cx, cy, 19, TH.accent);
    for (int i = 1; i <= 3; ++i) {
      int rr = 30 + i * 14;
      clockSprite.drawCircle(cx, cy, rr, TH.dim);
      float a = (t * (0.35f + i * 0.18f)) + i * 1.8f;
      int x = cx + (int)(cosf(a) * rr);
      int y = cy + (int)(sinf(a) * rr);
      clockSprite.fillCircle(x, y, 3 + i / 2, i == 1 ? TH.accent2 : TH.good);
    }
    clockSprite.setTextDatum(MC_DATUM);
    clockSprite.setTextColor(TH.text, TH.bg);
    clockSprite.drawString(clockTimeString(), cx, 151, 2);
  }
  else if (style == 114) { // Radar
    clockSprite.drawCircle(cx, cy, 70, TH.panel);
    clockSprite.drawCircle(cx, cy, 48, TH.dim);
    clockSprite.drawCircle(cx, cy, 25, TH.dim);
    float a = fmodf(t * 1.7f, 2.0f * PI);
    for (int k = 0; k < 14; ++k) {
      float aa = a - k * 0.055f;
      uint16_t col = (k < 3) ? TH.accent : TH.panel;
      int x = cx + (int)(cosf(aa) * 68);
      int y = cy + (int)(sinf(aa) * 68);
      clockSprite.drawLine(cx, cy, x, y, col);
    }
    clockSprite.fillCircle(cx + (int)(cosf(a) * 53), cy + (int)(sinf(a) * 53), 4, TH.accent2);
    clockSprite.fillCircle(cx, cy, 3, TH.good);
    clockSprite.setTextDatum(MC_DATUM);
    clockSprite.setTextColor(TH.text, TH.bg);
    clockSprite.drawString(clockTimeString(), cx, cy, 3);
  }
  else if (style == 115) { // Flip-style digital card
    float p = 0.5f + 0.5f * sinf(t * PI);
    int split = 87 + (int)(p * 5.0f);
    clockSprite.fillRoundRect(10, 30, 156, 116, 14, TH.panel);
    clockSprite.drawRoundRect(10, 30, 156, 116, 14, TH.accent);
    clockSprite.drawFastHLine(16, split, 144, TH.dim);
    clockSprite.setTextDatum(MC_DATUM);
    clockSprite.setTextColor(TH.text, TH.panel);
    clockSprite.drawString(clockTimeString(), cx, 78, 5);
    clockSprite.setTextColor(TH.accent2, TH.panel);
    clockSprite.drawString(clockSecondsString(), cx, 111, 2);
  }
  else if (style == 116) { // Equalizer bars
    for (int i = 0; i < 7; ++i) {
      float wave = 0.5f + 0.5f * sinf(t * 2.6f + i * 0.72f);
      int h = 20 + (int)(wave * 72.0f);
      int x = 10 + i * 25;
      uint16_t col = (i & 1) ? TH.accent : TH.accent2;
      clockSprite.fillRoundRect(x, 138 - h, 16, h, 5, col);
    }
    clockSprite.setTextDatum(MC_DATUM);
    clockSprite.setTextColor(TH.text, TH.bg);
    clockSprite.drawString(clockTimeString(), cx, 155, 3);
  }
  else if (style >= 10 && style <= 19) { // 10 genuinely different skeleton clocks
    const int v = style - 10;
    const float TAU = 2.0f * PI;
    const float secA = (secF / 60.0f) * TAU - PI / 2.0f;
    const float minA = (minF / 60.0f) * TAU - PI / 2.0f;
    const float hourA = (hourF / 12.0f) * TAU - PI / 2.0f;
    auto hand = [&](float a, int len, int w, uint16_t col) {
      int x = cx + (int)(cosf(a) * len);
      int y = cy + (int)(sinf(a) * len);
      for (int q = -w; q <= w; ++q) clockSprite.drawLine(cx + q, cy + q, x + q, y + q, col);
    };

    // All ten variants share the same smooth sprite, but each has a different face/mechanism.
    if (v == 0) { // Gear train
      clockSprite.drawCircle(cx, cy, 72, TH.panel);
      for (int g=0; g<4; ++g) {
        int gx = cx + (g<2 ? -27 : 27), gy = cy + (g%2 ? 25 : -24), rr = 18 + (g%3)*3;
        float ga = t * (g&1 ? -1.2f : 1.2f) + g;
        clockSprite.drawCircle(gx,gy,rr,TH.accent);
        clockSprite.drawCircle(gx,gy,rr-5,TH.bg);
        for(int n=0;n<8;n++){float a=ga+n*PI/4; int x1=gx+cosf(a)*rr,y1=gy+sinf(a)*rr; int x2=gx+cosf(a)*(rr+4),y2=gy+sinf(a)*(rr+4);clockSprite.drawLine(x1,y1,x2,y2,TH.accent2);}
      }
      hand(hourA,28,2,TH.text); hand(minA,48,1,TH.accent); hand(secA,61,0,TH.accent2);
    } else if (v == 1) { // Pendulum
      clockSprite.drawRoundRect(30,14,116,140,18,TH.panel);
      clockSprite.drawRoundRect(38,22,100,124,14,TH.dim);
      clockSprite.drawCircle(cx,70,48,TH.bg);
      for(int n=0;n<12;n++){float a=n*PI/6-PI/2;clockSprite.drawPixel(cx+cosf(a)*43,70+sinf(a)*43,TH.dim);}
      hand(hourA,24,2,TH.text); hand(minA,38,1,TH.accent); hand(secA,43,0,TH.accent2);
      float pa=sinf(t*2.4f)*0.48f; int px=cx+(int)(sinf(pa)*38); int py=103+(int)(cosf(pa)*18);
      clockSprite.drawLine(cx,96,px,py,TH.accent); clockSprite.fillCircle(px,py,9,TH.accent2);
    } else if (v == 2) { // Roman
      clockSprite.fillCircle(cx,cy,73,TH.panel); clockSprite.drawCircle(cx,cy,70,TH.accent);
      const char* rn[12]={"XII","I","II","III","IV","V","VI","VII","VIII","IX","X","XI"};
      clockSprite.setTextDatum(MC_DATUM); clockSprite.setTextColor(TH.text,TH.panel);
      for(int n=0;n<12;n++){float a=n*PI/6-PI/2;clockSprite.drawString(rn[n],cx+cosf(a)*57,cy+sinf(a)*57,1);}
      hand(hourA,30,2,TH.text); hand(minA,48,1,TH.accent); hand(secA,61,0,TH.accent2);
    } else if (v == 3) { // Moon phases
      clockSprite.drawCircle(cx,cy,72,TH.panel);
      for(int i=0;i<8;i++){float a=i*PI/4;clockSprite.drawLine(cx+cosf(a)*61,cy+sinf(a)*61,cx+cosf(a)*68,cy+sinf(a)*68,TH.dim);}
      float ph=0.5f+0.5f*sinf(t*0.35f); int rr=25; clockSprite.fillCircle(cx,cy,rr,TH.text);
      clockSprite.fillCircle(cx+(int)((ph-0.5f)*38),cy,rr,TH.panel);
      hand(hourA,31,2,TH.accent); hand(minA,51,1,TH.accent2); hand(secA,63,0,TH.good);
    } else if (v == 4) { // Nautical compass
      clockSprite.drawCircle(cx,cy,72,TH.panel); clockSprite.drawCircle(cx,cy,58,TH.dim);
      for(int n=0;n<16;n++){float a=n*PI/8; int l=(n%4==0)?66:61;clockSprite.drawLine(cx+cosf(a)*54,cy+sinf(a)*54,cx+cosf(a)*l,cy+sinf(a)*l,TH.dim);}
      float wave=sinf(t*1.5f)*8;
      clockSprite.fillTriangle(cx,cy-48,cx-7,cy-27,cx+7,cy-27,TH.accent);
      clockSprite.drawLine(cx,cy,cx+(int)(cosf(secA)*62),cy+(int)(sinf(secA)*62),TH.accent2);
      hand(hourA,28,2,TH.text); hand(minA,49,1,TH.accent);
      clockSprite.drawArc(cx,cy+20,25,18,(int)wave+20,(int)wave+160,TH.good,TH.bg); // gentle wave accent
    } else if (v == 5) { // Industrial gauge
      clockSprite.fillRoundRect(10,30,156,110,16,TH.panel);
      clockSprite.drawRoundRect(10,30,156,110,16,TH.dim);
      for(int n=0;n<=10;n++){float a=PI*0.75f + n*(PI*1.5f/10);int x1=cx+cosf(a)*48,y1=cy+sin(a)*48;int x2=cx+cosf(a)*61,y2=cy+sinf(a)*61;clockSprite.drawLine(x1,y1,x2,y2,TH.dim);}
      float gauge=PI*0.75f + (secF/60.0f)*PI*1.5f;
      clockSprite.drawLine(cx,cy,cx+cosf(gauge)*52,cy+sinf(gauge)*52,TH.accent2); clockSprite.fillCircle(cx,cy,5,TH.good);
      hand(hourA,24,2,TH.text); hand(minA,40,1,TH.accent);
    } else if (v == 6) { // Concentric rings
      for(int i=0;i<6;i++){int rr=70-i*10;float off=t*(0.3f+i*0.05f);clockSprite.drawCircle(cx,cy,rr,(i&1)?TH.accent:TH.panel);clockSprite.drawArc(cx,cy,rr,rr-2,(int)(off*57.3f)%360,(int)(off*57.3f+120)%360,TH.accent2,TH.bg);}
      hand(hourA,28,2,TH.text); hand(minA,47,1,TH.accent); hand(secA,63,0,TH.good);
    } else if (v == 7) { // Binary / LED skeleton
      clockSprite.fillRoundRect(9,24,158,128,12,TH.panel);
      clockSprite.setTextDatum(MC_DATUM); clockSprite.setTextColor(TH.dim,TH.panel); clockSprite.drawString("H  M  S",cx,42,1);
      int vals[3]={nowTm.tm_hour%24,nowTm.tm_min,nowTm.tm_sec};
      for(int row=0;row<6;row++) for(int col=0;col<3;col++){bool on=(vals[col]>>(5-row))&1;int x=57+col*28,y=58+row*13;clockSprite.fillCircle(x,y,4,on?TH.accent:TH.dim);}
      clockSprite.setTextColor(TH.text,TH.panel); clockSprite.drawString(clockTimeString(),cx,135,2);
    } else if (v == 8) { // Sun dial
      clockSprite.fillCircle(cx,cy,71,TH.panel); clockSprite.drawCircle(cx,cy,68,TH.accent);
      for(int n=0;n<24;n++){float a=n*PI/12-PI/2;int rr=(n%3==0)?62:66;clockSprite.drawLine(cx+cosf(a)*rr,cy+sinf(a)*rr,cx+cosf(a)*70,cy+sinf(a)*70,TH.dim);}
      float sunA=(hourF/12.0f)*TAU-PI/2; int sx=cx+(int)cosf(sunA)*46, sy=cy+(int)sinf(sunA)*46;
      clockSprite.fillCircle(sx,sy,9,TH.accent2); hand(hourA,28,2,TH.text); hand(minA,49,1,TH.accent); hand(secA,62,0,TH.good);
    } else { // Skeleton balance / escapement
      clockSprite.drawCircle(cx,cy,72,TH.panel); clockSprite.drawCircle(cx,cy,69,TH.dim);
      float ea=sinf(t*3.2f)*0.55f;
      int ex=cx+(int)(sinf(ea)*42), ey=cy+39+(int)(cosf(ea)*10);
      clockSprite.drawLine(cx,cy+14,ex,ey,TH.accent); clockSprite.fillCircle(ex,ey,10,TH.accent2);
      clockSprite.drawCircle(cx,cy,31,TH.dim);
      hand(hourA,27,2,TH.text); hand(minA,49,1,TH.accent); hand(secA,63,0,TH.good);
      clockSprite.fillCircle(cx,cy,5,TH.accent2);
    }
    clockSprite.setTextDatum(MC_DATUM); clockSprite.setTextColor(TH.text,TH.panel);
    clockSprite.drawString(clockTimeString(), cx, 154, 2);
  }
  else { // Arc
    for (int i = 0; i < 4; ++i) {
      int rr = 28 + i * 15;
      float center = t * (0.8f + i * 0.13f);
      float span = 1.6f + 0.55f * sinf(t * 1.8f + i);
      for (int k = 0; k < 50; ++k) {
        float u = -span * 0.5f + span * k / 49.0f;
        float a = center + u;
        int x = cx + (int)(cosf(a) * rr);
        int y = cy + (int)(sinf(a) * rr);
        clockSprite.drawPixel(x, y, (i & 1) ? TH.accent2 : TH.accent);
      }
    }
    clockSprite.setTextDatum(MC_DATUM);
    clockSprite.setTextColor(TH.text, TH.bg);
    clockSprite.drawString(clockTimeString(), cx, cy, 4);
  }

  clockSprite.pushSprite(72, 38);
  txtp(clockDateShort(), CLOCK_CX, 218, 1, TH.dim, TH.bg, MC_DATUM, 1, 296);
}

void clockInit() {
  if (cfg.clockStyle >= 20) cfg.clockStyle = 0;
  drawBar(CLOCK_STYLE_NAMES[cfg.clockStyle], nullptr, true);
  clkLastSec = -1; clkMsgShown = false; clkLastFrame = 0; clkLastTemp = -9999;
  if (cfg.clockStyle == 1 || cfg.clockStyle >= 10) clockSpriteBegin(); else clockSpriteEnd();
  tft.fillRect(0, BAR, SW, SH - BAR, TH.bg);
  if (timeValid()) {
    clockDrawStatic();
    clockDrawDynamic();
    if (cfg.clockStyle == 1) drawAnalogSpriteFrame();
  }
}

void clockLoop() {
  if (tc.press && tc.y <= BAR && tc.x > 220) {
    cfg.theme = (cfg.theme + 1) % NTHEMES;
    saveCfg(); beep(1700, 18); clockInit(); return;
  }
  if (tc.press && tc.y > BAR + 4) {
    cfg.clockStyle = (cfg.clockStyle + 1) % 20;
    saveCfg(); beep(1300, 20); clockInit(); return;
  }
  if (!timeValid()) {
    if (!clkMsgShown) {
      clkMsgShown = true;
      tft.fillRect(0, BAR, SW, SH - BAR, TH.bg);
      txt("Syncing time...", CLOCK_CX, 115, 2, TH.dim, TH.bg, MC_DATUM);
      if (WiFi.status() != WL_CONNECTED)
        txt("Set time from Control", CLOCK_CX, 140, 1, TH.dim, TH.bg, MC_DATUM);
    }
    return;
  }
  if (clkMsgShown) { clkMsgShown = false; clockInit(); return; }

  if (cfg.clockStyle >= 10) {
    uint32_t nowMs = millis();
    if ((uint32_t)(nowMs - clkLastFrame) < 33UL) return;
    clkLastFrame = nowMs;
    drawAnimatedClockNew(cfg.clockStyle);
    return;
  }
  if (cfg.clockStyle == 1) {
    uint32_t nowMs = millis();
    if ((uint32_t)(nowMs - clkLastFrame) < 33UL) return;
    clkLastFrame = nowMs;
    drawAnalogSpriteFrame();
    return;
  }
  if (nowTm.tm_sec == clkLastSec) return;
  clkLastSec = nowTm.tm_sec;
  clockDrawDynamic();
  if (wx.ok && tempNum(wx.t) != clkLastTemp) {
    clkLastTemp = tempNum(wx.t);
    if (cfg.clockStyle == 0)      txtp(clockTempString(), CLOCK_CX, 194, 2, TH.dim, TH.bg, MC_DATUM, 1, 296);
    else if (cfg.clockStyle == 2) txtp(clockTempString(), CLOCK_CX, 201, 2, TH.dim, TH.bg, MC_DATUM, 1, 296);
    else if (cfg.clockStyle == 6) txtp("TEMP  " + clockTempString(), 30, 200, 1, TH.dim, TH.bg, ML_DATUM, 1, 296);
    else if (cfg.clockStyle == 7) txtp(clockTempString(), CLOCK_CX, 179, 2, TH.dim, TH.panel, MC_DATUM, 1, 296);
    else if (cfg.clockStyle == 8) txtp(clockTempString(), CLOCK_CX, 191, 2, TH.accent, TH.bg, MC_DATUM, 1, 296);
  }
}

