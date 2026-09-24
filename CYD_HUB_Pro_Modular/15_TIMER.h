// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ TIMER =========================================
int tmTab = 0;
uint32_t cdTotal = 300000, cdLeft = 300000, cdLast = 0, cdDoneAt = 0, alarmNext = 0;
bool cdRun = false, cdDone = false;
uint32_t swMs = 0, swLast = 0;
bool swRun = false;
uint32_t lapMs[2] = {0, 0};
int lapNo = 0;
int tmLastA = -1, tmLastB = -1;
bool tmFlash = false;

void timerBackground() {
  uint32_t now = millis();
  if (cdRun) {
    uint32_t dt = now - cdLast;
    cdLast = now;
    if (dt >= cdLeft) {
      cdLeft = 0; cdRun = false; cdDone = true; cdDoneAt = now; alarmNext = 0;
      if (screen != SCR_TIMER) { tmTab = 0; goScreen(SCR_TIMER); }
    } else cdLeft -= dt;
  }
  if (swRun) { swMs += now - swLast; swLast = now; }
}
void tmDrawTabs() {
  button(8, 30, 150, 24, "Timer", tmTab == 0 ? TH.accent : TH.panel, tmTab == 0 ? TH.onacc : TH.text, 2);
  button(162, 30, 150, 24, "Stopwatch", tmTab == 1 ? TH.accent : TH.panel, tmTab == 1 ? TH.onacc : TH.text, 2);
}
void tmDrawMain() {
  char b[16];
  uint16_t c = TH.text;
  if (tmTab == 0) {
    int s = (int)((cdLeft + 999) / 1000);
    snprintf(b, sizeof(b), "%02d:%02d", s / 60, s % 60);
    if (cdDone && tmFlash) c = TH.bad;
  } else {
    uint32_t s = swMs / 1000;
    snprintf(b, sizeof(b), "%02d:%02d", (int)((s / 60) % 100), (int)(s % 60));
    if (swRun) c = TH.text; else c = swMs ? TH.accent2 : TH.text;
  }
  txtp(b, 160, 104, 7, c, TH.bg, MC_DATUM, 2, 296);
}
int tmBarW() { return cdTotal ? (int)((uint64_t)cdLeft * 280 / cdTotal) : 0; }
void tmDrawSub() {
  if (tmTab == 0) {
    int f = tmBarW();
    tft.fillRoundRect(20, 160, 280, 8, 4, TH.panel);
    uint16_t c = cdDone ? TH.bad : TH.accent;
    if (f >= 8) tft.fillRoundRect(20, 160, f, 8, 4, c); else if (f > 0) tft.fillRect(20, 160, f, 8, c);
  } else {
    char b[8];
    snprintf(b, sizeof(b), ".%02d", (int)((swMs / 10) % 100));
    txtp(b, 160, 166, 4, TH.accent, TH.bg, MC_DATUM, 1, 90);
  }
}
void tmDrawLaps() {
  tft.fillRect(0, 178, SW, 32, TH.bg);
  if (tmTab != 1) return;
  for (int i = 0; i < 2; i++) {
    int n = lapNo - i;
    if (n < 1) break;
    char b[32];
    uint32_t ms = lapMs[i];
    snprintf(b, sizeof(b), "Lap %d   %02d:%02d.%02d", n, (int)((ms / 60000) % 100), (int)((ms / 1000) % 60), (int)((ms / 10) % 100));
    txt(b, 160, 186 + i * 15, 2, i == 0 ? TH.text : TH.dim, TH.bg, MC_DATUM);
  }
}
void tmDrawButtons() {
  if (tmTab == 0) {
    bool en = !cdRun && !cdDone;
    uint16_t f = en ? TH.panel : TH.bg, t = en ? TH.text : TH.dim;
    button(4, 180, 74, 26, "-1m", f, t, 2);
    button(84, 180, 74, 26, "+1m", f, t, 2);
    button(164, 180, 74, 26, "-10s", f, t, 2);
    button(244, 180, 74, 26, "+10s", f, t, 2);
    if (cdDone) button(8, 212, 150, 24, "Stop", TH.bad, C565(255, 255, 255), 2);
    else if (cdRun) button(8, 212, 150, 24, "Pause", TH.accent2, TH.onacc, 2);
    else button(8, 212, 150, 24, "Start", TH.good, TH.onacc, 2);
    button(162, 212, 150, 24, "Reset", TH.panel, TH.text, 2);
  } else {
    button(8, 212, 150, 24, swRun ? "Stop" : "Start", swRun ? TH.accent2 : TH.good, TH.onacc, 2);
    button(162, 212, 150, 24, swRun ? "Lap" : "Reset", TH.panel, TH.text, 2);
  }
}
void timerInit() {
  tft.fillScreen(TH.bg);
  drawBar("Timer", nullptr, true);
  tmDrawTabs();
  tmLastA = -1; tmLastB = -1; tmFlash = false;
  tmDrawMain(); tmDrawSub(); tmDrawButtons(); tmDrawLaps();
}
void tmPress() {
  if (pressIn(8, 30, 150, 24)) { tmTab = 0; timerInit(); return; }
  if (pressIn(162, 30, 150, 24)) { tmTab = 1; timerInit(); return; }
  if (tmTab == 0) {
    int delta = 0;
    if (pressIn(4, 180, 74, 26)) delta = -60;
    else if (pressIn(84, 180, 74, 26)) delta = 60;
    else if (pressIn(164, 180, 74, 26)) delta = -10;
    else if (pressIn(244, 180, 74, 26)) delta = 10;
    if (delta && !cdRun && !cdDone) {
      int t = (int)(cdTotal / 1000) + delta;
      t = constrain(t, 10, 5990);
      cdTotal = (uint32_t)t * 1000UL;
      cdLeft = cdTotal;
      beep(1800, 15);
      tmLastA = -1; tmLastB = -1;
    }
    if (pressIn(8, 212, 150, 24)) {
      if (cdDone) { cdDone = false; cdLeft = cdTotal; }
      else if (cdRun) cdRun = false;
      else { if (cdLeft == 0) cdLeft = cdTotal; cdRun = true; cdLast = millis(); }
      beep(1500, 20);
      tmDrawButtons(); tmLastA = -1; tmLastB = -1;
    }
    if (pressIn(162, 212, 150, 24)) {
      cdRun = false; cdDone = false; cdLeft = cdTotal;
      beep(1000, 20);
      tmDrawButtons(); tmLastA = -1; tmLastB = -1;
    }
  } else {
    if (pressIn(8, 212, 150, 24)) {
      swRun = !swRun;
      if (swRun) swLast = millis();
      beep(1500, 20);
      tmDrawButtons(); tmLastA = -1; tmLastB = -1;
    }
    if (pressIn(162, 212, 150, 24)) {
      if (swRun) {
        lapMs[1] = lapMs[0]; lapMs[0] = swMs; lapNo++;
        beep(1900, 20);
        tmDrawLaps();
      } else {
        swMs = 0; lapNo = 0; lapMs[0] = lapMs[1] = 0;
        beep(1000, 20);
        tmDrawButtons(); tmDrawLaps(); tmLastA = -1; tmLastB = -1;
      }
    }
  }
}
void timerLoop() {
  uint32_t now = millis();
  if (tc.press) tmPress();
  if (tmTab == 0) {
    int s = (int)((cdLeft + 999) / 1000);
    bool fl = cdDone && ((now / 450) % 2);
    if (s != tmLastA || fl != tmFlash) { tmLastA = s; tmFlash = fl; tmDrawMain(); }
    int f = tmBarW();
    if (f != tmLastB) { tmLastB = f; tmDrawSub(); }
    if (cdDone) {
      if (now - cdDoneAt > 30000) { cdDone = false; cdLeft = cdTotal; tmDrawButtons(); tmLastA = -1; tmLastB = -1; }
      else if ((int32_t)(now - alarmNext) >= 0) { alarmNext = now + 450; beep(2400, 220); }
    }
  } else {
    int a = (int)(swMs / 1000), b = (int)(swMs / 50);
    if (a != tmLastA) { tmLastA = a; tmDrawMain(); }
    if (b != tmLastB) { tmLastB = b; tmDrawSub(); }
  }
}

