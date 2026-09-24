// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ GAME COMMON ===================================
const char* GAME_TITLE[10] = {"X & O", "Snake", "Memory", "Whack", "Reflex", "2048", "Bricks", "Simon", "Tetris", "Pong"};
uint32_t gameOverAt = 0;

void gameScoreText(const String &s) { txtp(s, SW - 74, BAR / 2 + 1, 2, TH.text, TH.panel, MR_DATUM, 1, 110); }
void overlay(const char* title, const String &sub, uint16_t col) {
  int w = 220, h = 84, x = (SW - w) / 2, y = (SH + BAR - h) / 2;
  tft.fillRoundRect(x, y, w, h, 12, TH.panel);
  tft.drawRoundRect(x, y, w, h, 12, col);
  tft.drawRoundRect(x + 1, y + 1, w - 2, h - 2, 11, col);
  txt(title, SW / 2, y + 24, 4, col, TH.panel, MC_DATUM);
  txt(sub, SW / 2, y + 52, 2, TH.text, TH.panel, MC_DATUM);
  txt("tap to play again", SW / 2, y + 72, 1, TH.dim, TH.panel, MC_DATUM);
  gameOverAt = millis();
}
bool tapAfterOver() { return tc.press && tc.y > BAR && (millis() - gameOverAt) > 350; }

// ---- 0: TIC-TAC-TOE ----
const uint8_t TLINES[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
const int TX0 = 12, TY0 = 34, TCELL = 66;
int ttt[9], tttState = 0, tttWin = -1, tttW = 0, tttL = 0, tttD = 0;

int tttResult() {
  tttWin = -1;
  for (int i = 0; i < 8; i++) {
    int a = ttt[TLINES[i][0]], b = ttt[TLINES[i][1]], c = ttt[TLINES[i][2]];
    if (a && a == b && b == c) { tttWin = i; return a; }
  }
  for (int i = 0; i < 9; i++) if (!ttt[i]) return 0;
  return 3;
}
void tttCell(int i, bool hl) {
  int x = TX0 + (i % 3) * TCELL, y = TY0 + (i / 3) * TCELL;
  tft.fillRoundRect(x + 2, y + 2, TCELL - 4, TCELL - 4, 9, TH.panel);
  int cx = x + TCELL / 2, cy = y + TCELL / 2;
  if (ttt[i] == 1) {
    thickLine(cx - 15, cy - 15, cx + 15, cy + 15, 5, TH.accent);
    thickLine(cx - 15, cy + 15, cx + 15, cy - 15, 5, TH.accent);
  } else if (ttt[i] == 2) {
    for (int r = 15; r <= 19; r++) tft.drawCircle(cx, cy, r, TH.accent2);
  }
  if (hl) {
    tft.drawRoundRect(x + 2, y + 2, TCELL - 4, TCELL - 4, 9, TH.good);
    tft.drawRoundRect(x + 3, y + 3, TCELL - 6, TCELL - 6, 8, TH.good);
  }
}
void tttStatus(const String &s, uint16_t c) { txtp(s, 268, 56, 2, c, TH.bg, MC_DATUM, 1, 100); }
void tttScore() {
  txtp(String("You  ") + tttW, 268, 100, 2, TH.accent, TH.bg, MC_DATUM, 1, 100);
  txtp(String("CPU  ") + tttL, 268, 122, 2, TH.accent2, TH.bg, MC_DATUM, 1, 100);
  txtp(String("Draw ") + tttD, 268, 144, 2, TH.dim, TH.bg, MC_DATUM, 1, 100);
}
void tttNew() {
  memset(ttt, 0, sizeof(ttt));
  tttState = 0; tttWin = -1;
  clearContent();
  for (int i = 0; i < 9; i++) tttCell(i, false);
  tttStatus("Your turn", TH.text);
  tttScore();
  txt("You = X", 268, 180, 1, TH.accent, TH.bg, MC_DATUM);
  txt("CPU = O", 268, 194, 1, TH.accent2, TH.bg, MC_DATUM);
}
int tttAI() {
  for (int p = 0; p < 2; p++) {
    int who = (p == 0) ? 2 : 1;
    if (p == 1 && random(100) < 15) continue;
    for (int i = 0; i < 8; i++) {
      int cnt = 0, e = -1;
      for (int k = 0; k < 3; k++) {
        int idx = TLINES[i][k];
        if (ttt[idx] == who) cnt++; else if (!ttt[idx]) e = idx;
      }
      if (cnt == 2 && e >= 0) return e;
    }
  }
  if (!ttt[4]) return 4;
  int corners[4] = {0, 2, 6, 8}, cl[4], n = 0;
  for (int i = 0; i < 4; i++) if (!ttt[corners[i]]) cl[n++] = corners[i];
  if (n) return cl[random(n)];
  int all[9]; n = 0;
  for (int i = 0; i < 9; i++) if (!ttt[i]) all[n++] = i;
  return n ? all[random(n)] : -1;
}
bool tttFinish() {
  int r = tttResult();
  if (!r) return false;
  tttState = 1;
  if (r == 3) { tttD++; tttStatus("Draw!", TH.dim); beep(500, 150); }
  else {
    for (int k = 0; k < 3; k++) tttCell(TLINES[tttWin][k], true);
    if (r == 1) { tttW++; hi[0]++; hiDirty = true; tttStatus("You win!", TH.good); beep(1800, 200); }
    else { tttL++; tttStatus("CPU wins", TH.bad); beep(300, 250); }
  }
  tttScore();
  txt("tap to continue", 268, 166, 1, TH.dim, TH.bg, MC_DATUM);
  return true;
}
void tttInit() { tttW = tttL = tttD = 0; tttNew(); }
void tttLoop() {
  if (!tc.press || tc.y < TY0) return;
  if (tttState == 1) { tttNew(); return; }
  if (tc.x < TX0 || tc.x >= TX0 + 3 * TCELL || tc.y >= TY0 + 3 * TCELL) return;
  int i = ((tc.y - TY0) / TCELL) * 3 + (tc.x - TX0) / TCELL;
  if (ttt[i]) return;
  ttt[i] = 1; tttCell(i, false); beep(880, 40);
  if (tttFinish()) return;
  tttStatus("Thinking...", TH.dim);
  delay(260);
  int m = tttAI();
  if (m >= 0) { ttt[m] = 2; tttCell(m, false); beep(600, 40); }
  if (tttFinish()) return;
  tttStatus("Your turn", TH.text);
}

// ---- 1: SNAKE ----
const int SN_C = 16, SN_COLS = 20, SN_ROWS = 13, SN_Y = 28;
int snx[SN_COLS * SN_ROWS + 2], sny[SN_COLS * SN_ROWS + 2];
int snLen, snDir, snNext, snFx, snFy, snState, snScore, snAx, snAy;
uint32_t snLast;
const int SDX[4] = {0, 1, 0, -1}, SDY[4] = {-1, 0, 1, 0};

void snCell(int x, int y, uint16_t c) { tft.fillRoundRect(x * SN_C + 1, SN_Y + y * SN_C + 1, SN_C - 2, SN_C - 2, 4, c); }
void snClear(int x, int y) { tft.fillRect(x * SN_C, SN_Y + y * SN_C, SN_C, SN_C, TH.bg); }
void snFood() {
  for (int tries = 0; tries < 400; tries++) {
    int x = random(SN_COLS), y = random(SN_ROWS);
    bool ok = true;
    for (int i = 0; i < snLen; i++) if (snx[i] == x && sny[i] == y) { ok = false; break; }
    if (ok) { snFx = x; snFy = y; break; }
  }
  tft.fillCircle(snFx * SN_C + SN_C / 2, SN_Y + snFy * SN_C + SN_C / 2, 6, TH.bad);
}
void snInit() {
  clearContent();
  snLen = 3; snDir = 1; snNext = 1; snScore = 0; snState = 0;
  for (int i = 0; i < snLen; i++) { snx[i] = 10 - i; sny[i] = 6; }
  for (int i = 1; i < snLen; i++) snCell(snx[i], sny[i], TH.good);
  snCell(snx[0], sny[0], TH.accent);
  snFood();
  gameScoreText("Score 0");
  txt("swipe to steer - tap to start", SW / 2, SN_Y + 9 * SN_C, 2, TH.dim, TH.bg, MC_DATUM);
}
void snOver() {
  snState = 2;
  submitScore(1, snScore); flushHi();
  beep(200, 400);
  overlay("Game Over", String("Score ") + snScore + "   Best " + hi[1], TH.bad);
}
void snStep() {
  if (snNext != (snDir + 2) % 4) snDir = snNext;
  int nx = (snx[0] + SDX[snDir] + SN_COLS) % SN_COLS;
  int ny = (sny[0] + SDY[snDir] + SN_ROWS) % SN_ROWS;
  bool grow = (nx == snFx && ny == snFy);
  int chk = grow ? snLen : snLen - 1;
  for (int i = 0; i < chk; i++) if (snx[i] == nx && sny[i] == ny) { snOver(); return; }
  int tx = snx[snLen - 1], ty = sny[snLen - 1];
  if (grow && snLen < SN_COLS * SN_ROWS) snLen++;
  for (int i = snLen - 1; i > 0; i--) { snx[i] = snx[i - 1]; sny[i] = sny[i - 1]; }
  snx[0] = nx; sny[0] = ny;
  if (!grow) snClear(tx, ty);
  snCell(snx[1], sny[1], TH.good);
  snCell(nx, ny, TH.accent);
  if (grow) {
    snScore++; beep(1500, 40);
    gameScoreText(String("Score ") + snScore);
    snFood();
  }
}
void snLoop() {
  if (snState == 2) { if (tapAfterOver()) snInit(); return; }
  if (tc.press) {
    snAx = tc.x; snAy = tc.y;
    if (snState == 0) {
      snState = 1; snLast = millis();
      tft.fillRect(0, SN_Y + 8 * SN_C, SW, SN_C * 2, TH.bg);
      snCell(snx[0], sny[0], TH.accent);
      for (int i = 1; i < snLen; i++) snCell(snx[i], sny[i], TH.good);
      tft.fillCircle(snFx * SN_C + SN_C / 2, SN_Y + snFy * SN_C + SN_C / 2, 6, TH.bad);
    }
  }
  if (tc.down) {
    int dx = tc.x - snAx, dy = tc.y - snAy;
    if (abs(dx) > 16 || abs(dy) > 16) {
      int nd = (abs(dx) > abs(dy)) ? (dx > 0 ? 1 : 3) : (dy > 0 ? 2 : 0);
      if (nd != (snDir + 2) % 4) snNext = nd;
      snAx = tc.x; snAy = tc.y;
    }
  }
  if (snState == 1) {
    int iv = max(70, 150 - snScore * 3);
    if (millis() - snLast >= (uint32_t)iv) { snLast = millis(); snStep(); }
  }
}

// ---- 2: MEMORY ----
const int MM_X0 = 12, MM_Y0 = 32, MM_W = 68, MM_H = 44, MM_G = 8;
const uint16_t MMC[8] = {C565(255,90,90), C565(255,170,50), C565(250,225,70), C565(90,220,120),
                         C565(60,200,230), C565(90,130,255), C565(190,110,255), C565(255,110,190)};
uint8_t mmCard[16];
bool mmUp[16], mmDone[16];
int mmFirst, mmSecond, mmMoves, mmPairs, mmState;
uint32_t mmCheckAt;

void mmSymbol(int cx, int cy, int id, uint16_t col, uint16_t bg) {
  int s = 13;
  switch (id) {
    case 0: tft.fillCircle(cx, cy, s, col); break;
    case 1: tft.fillRoundRect(cx - s, cy - s, 2 * s, 2 * s, 4, col); break;
    case 2: tft.fillTriangle(cx, cy - s, cx - s, cy + s, cx + s, cy + s, col); break;
    case 3:
      tft.fillTriangle(cx, cy - s, cx - s, cy, cx + s, cy, col);
      tft.fillTriangle(cx, cy + s, cx - s, cy, cx + s, cy, col);
      break;
    case 4: tft.fillCircle(cx, cy, s, col); tft.fillCircle(cx, cy, s - 5, bg); break;
    case 5: tft.fillRect(cx - 3, cy - s, 6, 2 * s, col); tft.fillRect(cx - s, cy - 3, 2 * s, 6, col); break;
    case 6: tft.fillRoundRect(cx - s, cy - s, 2 * s, 2 * s, 3, col); tft.fillRect(cx - s + 4, cy - s + 4, 2 * s - 8, 2 * s - 8, bg); break;
    default:
      tft.fillRect(cx - s, cy - s, 2 * s, 5, col);
      tft.fillRect(cx - s, cy - 2, 2 * s, 5, col);
      tft.fillRect(cx - s, cy + s - 5, 2 * s, 5, col);
      break;
  }
}
void mmDraw(int i) {
  int x = MM_X0 + (i % 4) * (MM_W + MM_G), y = MM_Y0 + (i / 4) * (MM_H + MM_G);
  int cx = x + MM_W / 2, cy = y + MM_H / 2;
  if (mmDone[i]) {
    tft.fillRoundRect(x, y, MM_W, MM_H, 8, TH.panel);
    tft.drawRoundRect(x, y, MM_W, MM_H, 8, TH.good);
    mmSymbol(cx, cy, mmCard[i], MMC[mmCard[i]], TH.panel);
  } else if (mmUp[i]) {
    tft.fillRoundRect(x, y, MM_W, MM_H, 8, TH.panel);
    tft.drawRoundRect(x, y, MM_W, MM_H, 8, TH.accent2);
    mmSymbol(cx, cy, mmCard[i], MMC[mmCard[i]], TH.panel);
  } else {
    tft.fillRoundRect(x, y, MM_W, MM_H, 8, TH.accent);
    txt("?", cx, cy + 1, 4, TH.onacc, TH.accent, MC_DATUM);
  }
}
void mmInit() {
  clearContent();
  for (int i = 0; i < 16; i++) { mmCard[i] = i / 2; mmUp[i] = false; mmDone[i] = false; }
  for (int i = 15; i > 0; i--) { int j = random(i + 1); uint8_t t = mmCard[i]; mmCard[i] = mmCard[j]; mmCard[j] = t; }
  mmFirst = mmSecond = -1; mmMoves = 0; mmPairs = 0; mmState = 0;
  for (int i = 0; i < 16; i++) mmDraw(i);
  gameScoreText("Moves 0");
}
void mmLoop() {
  if (mmState == 1) { if (tapAfterOver()) mmInit(); return; }
  if (mmSecond >= 0 && (int32_t)(millis() - mmCheckAt) >= 0) {
    if (mmCard[mmFirst] == mmCard[mmSecond]) {
      mmDone[mmFirst] = mmDone[mmSecond] = true; mmPairs++; beep(1600, 80);
    } else {
      mmUp[mmFirst] = mmUp[mmSecond] = false; beep(300, 60);
    }
    mmDraw(mmFirst); mmDraw(mmSecond);
    mmFirst = mmSecond = -1;
    if (mmPairs == 8) {
      mmState = 1; submitScore(2, mmMoves); flushHi();
      beep(2000, 250);
      overlay("Solved!", String(mmMoves) + " moves   Best " + hi[2], TH.good);
    }
    return;
  }
  if (!tc.press || mmSecond >= 0) return;
  int rx = tc.x - MM_X0, ry = tc.y - MM_Y0;
  if (rx < 0 || ry < 0) return;
  int col = rx / (MM_W + MM_G), row = ry / (MM_H + MM_G);
  if (col > 3 || row > 3) return;
  if (rx % (MM_W + MM_G) >= MM_W || ry % (MM_H + MM_G) >= MM_H) return;
  int i = row * 4 + col;
  if (mmUp[i] || mmDone[i]) return;
  mmUp[i] = true; mmDraw(i); beep(1100, 30);
  if (mmFirst < 0) mmFirst = i;
  else {
    mmSecond = i; mmMoves++;
    gameScoreText(String("Moves ") + mmMoves);
    mmCheckAt = millis() + 650;
  }
}

// ---- 3: WHACK-A-MOLE ----
const int WK_X[3] = {64, 160, 256}, WK_Y[3] = {74, 140, 206};
int wkMole, wkScore, wkState, wkLastHole, wkSecShown;
uint32_t wkUntil, wkNextAt, wkEnd;

void wkHole(int i, bool mole) {
  int cx = WK_X[i % 3], cy = WK_Y[i / 3];
  tft.fillRect(cx - 36, cy - 34, 72, 66, TH.bg);
  tft.fillEllipse(cx, cy + 12, 32, 11, C565(55, 34, 22));
  if (mole) {
    tft.fillCircle(cx, cy - 4, 22, C565(166, 112, 70));
    tft.fillCircle(cx - 8, cy - 9, 3, C565(20, 14, 10));
    tft.fillCircle(cx + 8, cy - 9, 3, C565(20, 14, 10));
    tft.fillCircle(cx, cy, 5, C565(255, 150, 160));
  }
}
void wkInit() {
  clearContent();
  for (int i = 0; i < 9; i++) wkHole(i, false);
  wkMole = -1; wkScore = 0; wkState = 0; wkLastHole = -1; wkSecShown = -1;
  gameScoreText("0 | 30s");
  txt("tap to start (30s)", SW / 2, 40, 2, TH.dim, TH.bg, MC_DATUM);
}
void wkLoop() {
  uint32_t now = millis();
  if (wkState == 2) { if (tapAfterOver()) wkInit(); return; }
  if (wkState == 0) {
    if (tc.press && tc.y > BAR) {
      wkState = 1; wkEnd = now + 30000UL; wkNextAt = now + 500;
      tft.fillRect(0, BAR + 2, SW, 22, TH.bg);
    }
    return;
  }
  int left = (int)((wkEnd - now + 999) / 1000);
  if ((int32_t)(now - wkEnd) >= 0) {
    if (wkMole >= 0) { wkHole(wkMole, false); wkMole = -1; }
    wkState = 2; submitScore(3, wkScore); flushHi();
    beep(700, 300);
    overlay("Time's up!", String("Score ") + wkScore + "   Best " + hi[3], TH.accent);
    return;
  }
  if (left != wkSecShown) { wkSecShown = left; gameScoreText(String(wkScore) + " | " + left + "s"); }
  if (wkMole < 0 && (int32_t)(now - wkNextAt) >= 0) {
    int h;
    do { h = random(9); } while (h == wkLastHole);
    wkMole = h; wkLastHole = h; wkHole(h, true);
    wkUntil = now + max(420, 950 - wkScore * 18);
  } else if (wkMole >= 0 && (int32_t)(now - wkUntil) >= 0) {
    wkHole(wkMole, false); wkMole = -1; wkNextAt = now + random(150, 420);
  }
  if (tc.press && wkMole >= 0) {
    int dx = tc.x - WK_X[wkMole % 3], dy = tc.y - WK_Y[wkMole / 3];
    if (dx * dx + dy * dy < 34 * 34) {
      wkScore++; beep(1700, 40);
      wkHole(wkMole, false); wkMole = -1; wkNextAt = now + random(100, 320);
      gameScoreText(String(wkScore) + " | " + left + "s");
    }
  }
}

// ---- 4: REFLEX ----
int rxState;
uint32_t rxAt, rxT0;
void rxFill(uint16_t col, const String &a, const String &b, uint16_t fg) {
  tft.fillRect(0, BAR, SW, SH - BAR, col);
  txt(a, SW / 2, 110, 4, fg, col, MC_DATUM);
  txt(b, SW / 2, 150, 2, fg, col, MC_DATUM);
}
void rxInit() {
  rxState = 0;
  rxFill(TH.panel, "Reflex Test", hi[4] ? String("Best ") + hi[4] + " ms   -  tap to start" : String("Tap to start"), TH.text);
  gameScoreText("");
}
void rxLoop() {
  uint32_t now = millis();
  if (rxState == 1 && (int32_t)(now - rxAt) >= 0) {
    rxState = 2;
    rxFill(C565(40, 190, 90), "TAP NOW!", "", C565(255, 255, 255));
    rxT0 = millis();
    return;
  }
  if (!tc.press || tc.y <= BAR) return;
  if (rxState == 0 || rxState == 3 || rxState == 4) {
    rxState = 1;
    rxAt = now + random(1500, 4500);
    rxFill(C565(200, 50, 50), "Wait for green...", "", C565(255, 255, 255));
  } else if (rxState == 1) {
    rxState = 4; beep(250, 200);
    rxFill(TH.panel, "Too early!", "tap to try again", TH.bad);
  } else if (rxState == 2) {
    uint32_t t = millis() - rxT0;
    rxState = 3;
    submitScore(4, t); flushHi(); beep(1800, 60);
    rxFill(TH.panel, String(t) + " ms", String("Best ") + hi[4] + " ms   -  tap to retry", TH.accent);
    gameScoreText(String(t) + " ms");
  }
}

// ---- 5: 2048 ----
const int G48X = 10, G48Y = 31, G48T = 44, G48G = 5;
int g48[4][4], g48Score, g48Ax, g48Ay;
bool g48Over, g48Consumed;

uint16_t g48Col(int v) {
  switch (v) {
    case 0: return TH.bg;
    case 2: return C565(238, 228, 218);
    case 4: return C565(237, 224, 200);
    case 8: return C565(242, 177, 121);
    case 16: return C565(245, 149, 99);
    case 32: return C565(246, 124, 95);
    case 64: return C565(246, 94, 59);
    case 128: return C565(237, 207, 114);
    case 256: return C565(237, 204, 97);
    case 512: return C565(237, 200, 80);
    case 1024: return C565(237, 197, 63);
    case 2048: return C565(237, 194, 46);
    default: return C565(60, 58, 50);
  }
}
void g48Tile(int r, int c) {
  int x = G48X + c * (G48T + G48G), y = G48Y + r * (G48T + G48G), v = g48[r][c];
  uint16_t col = g48Col(v);
  tft.fillRoundRect(x, y, G48T, G48T, 6, col);
  if (v) {
    uint16_t fg = (v <= 4) ? C565(110, 100, 92) : C565(255, 255, 255);
    txtp(String(v), x + G48T / 2, y + G48T / 2 + 1, 2, fg, col, MC_DATUM, (v < 100) ? 2 : 1, 0);
  }
}
void g48Side() {
  txt("SCORE", 222, 40, 1, TH.dim, TH.bg, TL_DATUM);
  txtp(String(g48Score), 222, 52, 4, TH.text, TH.bg, TL_DATUM, 1, 92);
  txt("BEST", 222, 96, 1, TH.dim, TH.bg, TL_DATUM);
  uint32_t b = (hi[5] > (uint32_t)g48Score) ? hi[5] : (uint32_t)g48Score;
  txtp(String(b), 222, 108, 4, TH.accent, TH.bg, TL_DATUM, 1, 92);
}
void g48All() { for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++) g48Tile(r, c); }
void g48Spawn() {
  int cells[16], n = 0;
  for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++) if (!g48[r][c]) cells[n++] = r * 4 + c;
  if (!n) return;
  int k = cells[random(n)];
  g48[k / 4][k % 4] = (random(10) < 9) ? 2 : 4;
}
int &g48Ref(int dir, int i, int j) {
  switch (dir) {
    case 0: return g48[i][j];
    case 1: return g48[i][3 - j];
    case 2: return g48[j][i];
    default: return g48[3 - j][i];
  }
}
bool g48Move(int dir) {
  bool moved = false;
  for (int i = 0; i < 4; i++) {
    int line[4], out[4] = {0, 0, 0, 0}, k = 0;
    bool cm = false;
    for (int j = 0; j < 4; j++) line[j] = g48Ref(dir, i, j);
    for (int j = 0; j < 4; j++) {
      int v = line[j];
      if (!v) continue;
      if (k > 0 && cm && out[k - 1] == v) { out[k - 1] *= 2; g48Score += out[k - 1]; cm = false; }
      else { out[k++] = v; cm = true; }
    }
    for (int j = 0; j < 4; j++) {
      if (g48Ref(dir, i, j) != out[j]) { moved = true; g48Ref(dir, i, j) = out[j]; }
    }
  }
  return moved;
}
bool g48CanMove() {
  for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++) {
    if (!g48[r][c]) return true;
    if (c < 3 && g48[r][c] == g48[r][c + 1]) return true;
    if (r < 3 && g48[r][c] == g48[r + 1][c]) return true;
  }
  return false;
}
void g48Init() {
  clearContent();
  memset(g48, 0, sizeof(g48));
  g48Score = 0; g48Over = false; g48Consumed = false;
  tft.fillRoundRect(G48X - 5, G48Y - 5, 4 * G48T + 3 * G48G + 10, 4 * G48T + 3 * G48G + 10, 8, TH.panel);
  g48Spawn(); g48Spawn();
  g48All(); g48Side();
  txt("Swipe to", 222, 160, 2, TH.dim, TH.bg, TL_DATUM);
  txt("slide tiles", 222, 178, 2, TH.dim, TH.bg, TL_DATUM);
}
void g48Loop() {
  if (g48Over) { if (tapAfterOver()) g48Init(); return; }
  if (tc.press) { g48Ax = tc.x; g48Ay = tc.y; g48Consumed = false; }
  if (tc.release) g48Consumed = false;
  if (tc.down && !g48Consumed) {
    int dx = tc.x - g48Ax, dy = tc.y - g48Ay;
    if (abs(dx) > 24 || abs(dy) > 24) {
      g48Consumed = true;
      int dir = (abs(dx) > abs(dy)) ? (dx > 0 ? 1 : 0) : (dy > 0 ? 3 : 2);
      if (g48Move(dir)) {
        beep(900, 20);
        g48Spawn(); g48All(); g48Side();
        if (!g48CanMove()) {
          g48Over = true;
          submitScore(5, g48Score); flushHi();
          beep(250, 400);
          overlay("Game Over", String("Score ") + g48Score, TH.bad);
        }
      }
    }
  }
}

// ---- 6: BREAKOUT ----
const int BR_COLS = 8, BR_ROWS = 5, BR_BW = 36, BR_BH = 12, BR_GAP = 2, BR_X0 = 8, BR_Y0 = 40, BR_PY = 222, BR_PW = 54;
const uint16_t BRC[5] = {C565(255,90,90), C565(255,160,60), C565(250,220,70), C565(90,220,120), C565(80,170,255)};
bool brBrick[BR_ROWS][BR_COLS];
float brX, brY, brVX, brVY, brPX, brSpeed;
int brLives, brScore, brLeft, brState, brLevel, brOldPX, brDrawX, brDrawY;
uint32_t brLast;

void brDrawBrick(int r, int c, bool on) {
  int x = BR_X0 + c * (BR_BW + BR_GAP), y = BR_Y0 + r * (BR_BH + BR_GAP);
  if (on) tft.fillRoundRect(x, y, BR_BW, BR_BH, 3, BRC[r]); else tft.fillRect(x, y, BR_BW, BR_BH, TH.bg);
}
void brBricks() {
  for (int r = 0; r < BR_ROWS; r++) for (int c = 0; c < BR_COLS; c++) { brBrick[r][c] = true; brDrawBrick(r, c, true); }
  brLeft = BR_ROWS * BR_COLS;
}
void brStatus() { gameScoreText(String(brScore) + "  x" + brLives); }
void brBallReady() {
  brState = 0;
  brX = brPX; brY = BR_PY - 5;
  txt("tap to launch", SW / 2, 150, 2, TH.dim, TH.bg, MC_DATUM);
}
void brPaddle() { tft.fillRoundRect((int)brPX - BR_PW / 2, BR_PY, BR_PW, 8, 4, TH.accent); }
void brInit() {
  clearContent();
  brLives = 3; brScore = 0; brLevel = 1; brSpeed = 3.4f; brPX = SW / 2; brOldPX = (int)brPX; brDrawX = -50; brDrawY = -50;
  brBricks();
  brPaddle();
  brBallReady();
  brStatus();
}
bool brHit(float px, float py) {
  int lx = (int)px - BR_X0, ly = (int)py - BR_Y0;
  if (lx < 0 || ly < 0) return false;
  int c = lx / (BR_BW + BR_GAP), r = ly / (BR_BH + BR_GAP);
  if (c >= BR_COLS || r >= BR_ROWS) return false;
  if (lx % (BR_BW + BR_GAP) >= BR_BW || ly % (BR_BH + BR_GAP) >= BR_BH) return false;
  if (!brBrick[r][c]) return false;
  brBrick[r][c] = false; brDrawBrick(r, c, false);
  brScore += 10; brLeft--; beep(1200 + r * 150, 25);
  brStatus();
  return true;
}
void brLoop() {
  if (brState == 2) { if (tapAfterOver()) brInit(); return; }
  uint32_t now = millis();
  if (now - brLast < 16) return;
  brLast = now;
  if (tc.down) brPX = constrain(tc.x, BR_PW / 2, SW - BR_PW / 2);
  if (brState == 0 && tc.press && tc.y > BAR) {
    brState = 1;
    tft.fillRect(0, 140, SW, 24, TH.bg);
    brVX = brSpeed * 0.45f * (random(2) ? 1 : -1);
    brVY = -brSpeed * 0.89f;
  }
  if (brDrawX > -40) tft.fillCircle(brDrawX, brDrawY, 4, TH.bg);
  if (brState == 0) { brX = brPX; brY = BR_PY - 5; }
  else {
    brX += brVX; brY += brVY;
    if (brX < 4) { brX = 4; brVX = -brVX; }
    if (brX > SW - 5) { brX = SW - 5; brVX = -brVX; }
    if (brY < BAR + 4) { brY = BAR + 4; brVY = -brVY; }
    if (brVY > 0 && brY + 4 >= BR_PY && brY + 4 <= BR_PY + 10 && brX >= brPX - BR_PW / 2 - 4 && brX <= brPX + BR_PW / 2 + 4) {
      float rel = constrain((brX - brPX) / (BR_PW / 2.0f), -1.0f, 1.0f);
      float sp = sqrtf(brVX * brVX + brVY * brVY);
      float ang = rel * 1.05f;
      brVX = sp * sinf(ang); brVY = -sp * cosf(ang);
      brY = BR_PY - 5; beep(500, 20);
    }
    float lpy = brY + (brVY > 0 ? 4 : -4);
    if (brHit(brX, lpy)) brVY = -brVY;
    else {
      float lpx = brX + (brVX > 0 ? 4 : -4);
      if (brHit(lpx, brY)) brVX = -brVX;
    }
    if (brLeft == 0) {
      brLevel++; brSpeed = min(6.0f, brSpeed + 0.5f);
      brBricks(); brBallReady(); beep(2000, 150);
    }
    if (brY > SH + 6) {
      brLives--; brStatus(); beep(250, 250);
      if (brLives <= 0) {
        brState = 2; submitScore(6, brScore); flushHi();
        brDrawX = -50;
        overlay("Game Over", String("Score ") + brScore + "   Best " + hi[6], TH.bad);
        return;
      }
      brBallReady();
    }
  }
  if ((int)brPX != brOldPX) {
    tft.fillRect(brOldPX - BR_PW / 2 - 1, BR_PY, BR_PW + 2, 8, TH.bg);
    brOldPX = (int)brPX;
  }
  brPaddle();
  brDrawX = (int)brX; brDrawY = (int)brY;
  tft.fillCircle(brDrawX, brDrawY, 4, TH.text);
}

// ---- 7: SIMON ----
const uint16_t SIMB[4] = {C565(60,220,100), C565(255,70,70), C565(255,220,60), C565(70,140,255)};
const uint16_t SIMD[4] = {C565(18,74,34), C565(84,24,24), C565(84,74,20), C565(24,46,84)};
const int SIMF[4] = {330, 262, 392, 523};
uint8_t simSeq[90];
int simLevel, simIdx, simIn, simState, simLit;
uint32_t simLitUntil, simGapUntil;

void simHub() {
  tft.fillCircle(160, 133, 27, TH.bg);
  txt(String(simLevel), 160, 133, 4, TH.text, TH.bg, MC_DATUM);
}
void simPad(int i, bool lit) {
  int x = (i % 2) ? 164 : 6, y = (i / 2) ? 134 : 32;
  tft.fillRoundRect(x, y, 150, 98, 14, lit ? SIMB[i] : SIMD[i]);
  simHub();
}
void simInit() {
  clearContent();
  simLevel = 1; simIdx = 0; simIn = 0; simState = 0; simLit = -1;
  simSeq[0] = random(4);
  for (int i = 0; i < 4; i++) simPad(i, false);
  gameScoreText("Level 1");
  txt("Tap", 160, 133, 2, TH.dim, TH.bg, MC_DATUM);
}
void simOver() {
  simState = 3;
  submitScore(7, simLevel - 1); flushHi();
  beep(150, 500);
  overlay("Wrong!", String("Rounds ") + (simLevel - 1) + "   Best " + hi[7], TH.bad);
}
void simLoop() {
  uint32_t now = millis();
  if (simState == 3) { if (tapAfterOver()) simInit(); return; }
  if (simLit >= 0 && (int32_t)(now - simLitUntil) >= 0) { simPad(simLit, false); simLit = -1; }
  if (simState == 0) {
    if (tc.press && tc.y > BAR) {
      simState = 1; simIdx = 0; simGapUntil = now + 500;
      simHub();
    }
    return;
  }
  if (simState == 1 && simLit < 0 && (int32_t)(now - simGapUntil) >= 0) {
    if (simIdx < simLevel) {
      simLit = simSeq[simIdx++];
      simPad(simLit, true); beep(SIMF[simLit], 250);
      int dur = max(200, 520 - simLevel * 14);
      simLitUntil = now + dur; simGapUntil = now + dur + 150;
    } else { simState = 2; simIn = 0; }
  }
  if (simState == 2 && tc.press && tc.y >= 32) {
    int pad = ((tc.y < 133) ? 0 : 2) + ((tc.x < 160) ? 0 : 1);
    if (simLit >= 0) { simPad(simLit, false); simLit = -1; }
    simLit = pad; simPad(pad, true); beep(SIMF[pad], 180);
    simLitUntil = now + 200;
    if (pad == simSeq[simIn]) {
      simIn++;
      if (simIn == simLevel) {
        simLevel++;
        simSeq[simLevel - 1] = random(4);
        submitScore(7, simLevel - 1);
        gameScoreText(String("Level ") + simLevel);
        simState = 1; simIdx = 0; simGapUntil = now + 900;
      }
    } else simOver();
  }
}

// ---- 8: TETRIS ----
#define TET_COLS 10
#define TET_ROWS 16
#define TET_SIZE 13
#define TET_X0   ((SW - TET_COLS * TET_SIZE) / 2)
#define TET_Y0   (BAR + 4)

const uint8_t TET_SHAPES[7][4][4] = {
  {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
  {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
  {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
  {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
  {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
  {{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
  {{0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0}}
};
const uint16_t TET_COL[7] = {
  C565(0,220,220), C565(240,220,60), C565(180,80,220),
  C565(60,210,90), C565(240,70,70), C565(70,110,240), C565(240,160,50)
};

int tetGrid[TET_ROWS][TET_COLS];
int tetPiece[4][4];
int tetPx, tetPy, tetPc;
int tetScore, tetLines, tetLevel;
int tetState;
uint32_t tetFall;

void tetDrawCell(int x, int y, uint16_t c, uint16_t bg) {
  tft.fillRect(TET_X0 + x * TET_SIZE, TET_Y0 + y * TET_SIZE,
               TET_SIZE - 1, TET_SIZE - 1, c);
  tft.drawRect(TET_X0 + x * TET_SIZE, TET_Y0 + y * TET_SIZE,
               TET_SIZE - 1, TET_SIZE - 1, bg);
}
void tetDrawBoard() {
  tft.fillRoundRect(TET_X0 - 2, TET_Y0 - 2,
                    TET_COLS * TET_SIZE + 4, TET_ROWS * TET_SIZE + 4,
                    4, TH.panel);
  for (int y = 0; y < TET_ROWS; y++)
    for (int x = 0; x < TET_COLS; x++) {
      if (tetGrid[y][x]) tetDrawCell(x, y, TET_COL[tetGrid[y][x] - 1], TH.bg);
      else               tetDrawCell(x, y, TH.bg, TH.bg);
    }
}
void tetDrawPiece() {
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      if (tetPiece[y][x]) {
        int gx = tetPx + x, gy = tetPy + y;
        if (gy >= 0 && gy < TET_ROWS && gx >= 0 && gx < TET_COLS)
          tetDrawCell(gx, gy, TET_COL[tetPc], TH.bg);
      }
}
void tetErasePiece() {
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      if (tetPiece[y][x]) {
        int gx = tetPx + x, gy = tetPy + y;
        if (gy >= 0 && gy < TET_ROWS && gx >= 0 && gx < TET_COLS) {
          if (tetGrid[gy][gx]) tetDrawCell(gx, gy, TET_COL[tetGrid[gy][gx]-1], TH.bg);
          else                 tetDrawCell(gx, gy, TH.bg, TH.bg);
        }
      }
}
void tetSpawn() {
  tetPc = random(7);
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      tetPiece[y][x] = TET_SHAPES[tetPc][y][x];
  tetPx = TET_COLS / 2 - 2;
  tetPy = -1;
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      if (tetPiece[y][x]) {
        int gy = tetPy + y, gx = tetPx + x;
        if (gy >= 0 && gy < TET_ROWS && (gx < 0 || gx >= TET_COLS || tetGrid[gy][gx])) {
          tetState = 1; return;
        }
      }
  tetDrawPiece();
}
bool tetCollideMove(int nx, int ny) {
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++) {
      if (!tetPiece[y][x]) continue;
      int gx = nx + x, gy = ny + y;
      if (gx < 0 || gx >= TET_COLS || gy >= TET_ROWS) return true;
      if (gy >= 0 && tetGrid[gy][gx]) return true;
    }
  return false;
}
bool tetCollideRotate() {
  int tmp[4][4];
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      tmp[x][3 - y] = tetPiece[y][x];
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++) {
      if (!tmp[y][x]) continue;
      int gx = tetPx + x, gy = tetPy + y;
      if (gx < 0 || gx >= TET_COLS || gy >= TET_ROWS) return true;
      if (gy >= 0 && tetGrid[gy][gx]) return true;
    }
  return false;
}
void tetLock() {
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      if (tetPiece[y][x]) {
        int gx = tetPx + x, gy = tetPy + y;
        if (gy >= 0 && gy < TET_ROWS && gx >= 0 && gx < TET_COLS)
          tetGrid[gy][gx] = tetPc + 1;
      }
  int cleared = 0;
  for (int y = TET_ROWS - 1; y >= 0; y--) {
    bool full = true;
    for (int x = 0; x < TET_COLS; x++) if (!tetGrid[y][x]) { full = false; break; }
    if (full) {
      cleared++;
      for (int yy = y; yy > 0; yy--)
        for (int x = 0; x < TET_COLS; x++) tetGrid[yy][x] = tetGrid[yy-1][x];
      for (int x = 0; x < TET_COLS; x++) tetGrid[0][x] = 0;
      y++;
    }
  }
  if (cleared) {
    tetLines += cleared;
    tetScore += (cleared == 1 ? 100 : cleared == 2 ? 300 : cleared == 3 ? 600 : 1000);
    tetLevel = 1 + tetLines / 10;
    gameScoreText(String(tetScore));
    beep(1800, 60);
  }
  tetDrawBoard();
  tetSpawn();
}
void tetMove(int dx) {
  tetErasePiece();
  if (!tetCollideMove(tetPx + dx, tetPy)) tetPx += dx;
  tetDrawPiece();
}
void tetRotate() {
  tetErasePiece();
  if (!tetCollideRotate()) {
    int tmp[4][4];
    for (int y = 0; y < 4; y++)
      for (int x = 0; x < 4; x++)
        tmp[x][3 - y] = tetPiece[y][x];
    for (int y = 0; y < 4; y++)
      for (int x = 0; x < 4; x++)
        tetPiece[y][x] = tmp[y][x];
  }
  tetDrawPiece();
}
void tetInit() {
  clearContent();
  memset(tetGrid, 0, sizeof(tetGrid));
  tetScore = 0; tetLines = 0; tetLevel = 1; tetState = 0;
  tetDrawBoard();
  gameScoreText("0");
  tetSpawn();
  tetFall = millis();
}
void tetLoop() {
  if (tetState == 1) {
    if (tapAfterOver()) tetInit();
    return;
  }
  uint32_t now = millis();
  if (tc.press) {
    if (tc.x < 80) tetMove(-1);
    else if (tc.x > SW - 80) tetMove(1);
    else tetRotate();
  }
  int fallMs = max(150, 700 - tetLevel * 50);
  if (now - tetFall >= (uint32_t)fallMs) {
    tetFall = now;
    tetErasePiece();
    if (!tetCollideMove(tetPx, tetPy + 1)) { tetPy++; tetDrawPiece(); }
    else { tetDrawPiece(); tetLock(); if (tetState == 1) {
      submitScore(8, tetScore); flushHi();
      overlay("Game Over", String("Score ") + tetScore, TH.bad);
    } }
  }
}

// ---- 9: PONG ----
float pgBallX, pgBallY, pgVX, pgVY;
int pgPlayerY, pgCpuY, pgScore, pgCpuScore, pgState;
int pgOldPlayerY, pgOldCpuY, pgOldBallX, pgOldBallY;
uint32_t pgLast = 0;
const int PG_PAD_W = 7, PG_PAD_H = 42, PG_BALL_R = 5;

void pgStatic() {
  tft.fillScreen(TH.bg);
  drawBar("Pong", "NEW", false);
  for (int y = BAR + 4; y < SH; y += 10) tft.fillRect(SW / 2 - 1, y, 2, 5, TH.dim);
  txt("YOU", 45, BAR + 18, 1, TH.accent, TH.bg, MC_DATUM);
  txt("CPU", SW - 45, BAR + 18, 1, TH.accent2, TH.bg, MC_DATUM);
  txt(String(pgScore), SW / 2 - 25, BAR + 18, 4, TH.accent, TH.bg, MC_DATUM);
  txt(String(pgCpuScore), SW / 2 + 25, BAR + 18, 4, TH.accent2, TH.bg, MC_DATUM);
}
void pgDrawPaddle(int x, int y, uint16_t c) { tft.fillRoundRect(x, y, PG_PAD_W, PG_PAD_H, 3, c); }
void pgDrawBall(int x, int y) { tft.fillCircle(x, y, PG_BALL_R, TH.text); }
void pgEraseBall(int x, int y) {
  tft.fillCircle(x, y, PG_BALL_R + 1, TH.bg);
  if (abs(x - SW / 2) < PG_BALL_R + 3) {
    int yy = y - 5;
    for (int dy = -10; dy <= 10; dy += 10) if (yy + dy > BAR + 3 && yy + dy < SH) tft.fillRect(SW/2-1, yy+dy, 2, 5, TH.dim);
  }
}
void pgErasePaddle(int x, int y) { tft.fillRect(x - 1, y - 1, PG_PAD_W + 2, PG_PAD_H + 2, TH.bg); }
void pgDrawDynamic() {
  if (pgOldPlayerY != pgPlayerY) { pgErasePaddle(12, pgOldPlayerY); pgDrawPaddle(12, pgPlayerY, TH.accent); }
  if (pgOldCpuY != pgCpuY) { pgErasePaddle(SW - 19, pgOldCpuY); pgDrawPaddle(SW - 19, pgCpuY, TH.accent2); }
  if (pgOldBallX >= 0) pgEraseBall(pgOldBallX, pgOldBallY);
  pgDrawBall((int)pgBallX, (int)pgBallY);
  pgOldPlayerY = pgPlayerY; pgOldCpuY = pgCpuY;
  pgOldBallX = (int)pgBallX; pgOldBallY = (int)pgBallY;
}
void pgDrawScene() {
  pgStatic();
  pgOldPlayerY = pgPlayerY; pgOldCpuY = pgCpuY; pgOldBallX = -1; pgOldBallY = -1;
  pgDrawPaddle(12, pgPlayerY, TH.accent);
  pgDrawPaddle(SW - 19, pgCpuY, TH.accent2);
  pgDrawBall((int)pgBallX, (int)pgBallY);
  if (pgState == 0) txt("Touch the screen to start", SW / 2, SH - 14, 1, TH.dim, TH.bg, MC_DATUM);
}
void pgResetBall(int dir) {
  pgBallX = SW / 2.0f;
  pgBallY = BAR + 65 + random(10, 110);
  pgVX = dir * 3.0f;
  pgVY = (random(-100, 101)) / 45.0f;
  if (fabsf(pgVY) < 1.0f) pgVY = pgVY < 0 ? -1.0f : 1.0f;
}
void pgInit() {
  pgScore = 0; pgCpuScore = 0; pgState = 0;
  pgPlayerY = 100; pgCpuY = 100;
  pgResetBall(random(0, 2) ? 1 : -1);
  pgLast = millis();
  pgDrawScene();
}
void pgLoop() {
  uint32_t now = millis();
  if (pgState == 2) { if (tc.press && tc.y > BAR) pgInit(); return; }
  if (tc.down && tc.y > BAR) {
    int ny = constrain(tc.y - PG_PAD_H / 2, BAR + 3, SH - PG_PAD_H - 2);
    if (abs(ny - pgPlayerY) > 1) pgPlayerY = ny;
    if (pgState == 0) pgState = 1;
  }
  if (pgState == 0) return;
  if (now - pgLast < 16) return;
  float dt = (now - pgLast) / 16.0f;
  pgLast = now;
  pgBallX += pgVX * dt;
  pgBallY += pgVY * dt;
  if (pgBallY <= BAR + PG_BALL_R) { pgBallY = BAR + PG_BALL_R; pgVY = fabsf(pgVY); }
  if (pgBallY >= SH - PG_BALL_R) { pgBallY = SH - PG_BALL_R; pgVY = -fabsf(pgVY); }
  float cpuCenter = pgCpuY + PG_PAD_H / 2.0f;
  int cpuStep = (fabsf(pgVX) > 4.0f) ? 3 : 2;
  if (cpuCenter < pgBallY - 3) pgCpuY += cpuStep;
  else if (cpuCenter > pgBallY + 3) pgCpuY -= cpuStep;
  pgCpuY = constrain(pgCpuY, BAR + 3, SH - PG_PAD_H - 2);

  if (pgVX < 0 && pgBallX - PG_BALL_R <= 19 && pgBallX > 8 && pgBallY >= pgPlayerY - PG_BALL_R && pgBallY <= pgPlayerY + PG_PAD_H + PG_BALL_R) {
    pgBallX = 19 + PG_BALL_R; pgVX = fabsf(pgVX) + 0.08f;
    pgVY += (pgBallY - (pgPlayerY + PG_PAD_H / 2.0f)) * 0.055f;
    pgVY = constrain(pgVY, -6.0f, 6.0f); beep(1300, 10);
  }
  if (pgVX > 0 && pgBallX + PG_BALL_R >= SW - 19 && pgBallX < SW - 8 && pgBallY >= pgCpuY - PG_BALL_R && pgBallY <= pgCpuY + PG_PAD_H + PG_BALL_R) {
    pgBallX = SW - 19 - PG_BALL_R; pgVX = -fabsf(pgVX) - 0.08f;
    pgVY += (pgBallY - (pgCpuY + PG_PAD_H / 2.0f)) * 0.055f;
    pgVY = constrain(pgVY, -6.0f, 6.0f); beep(1500, 10);
  }
  if (pgBallX < -10) {
    pgCpuScore++; pgResetBall(1);
    if (pgCpuScore >= 5) { pgState = 2; submitScore(9, pgScore); flushHi(); overlay("Pong Over", String("You ") + pgScore + " - " + pgCpuScore, TH.bad); return; }
    pgStatic(); pgDrawDynamic(); return;
  }
  if (pgBallX > SW + 10) {
    pgScore++; pgResetBall(-1);
    if (pgScore >= 5) { pgState = 2; submitScore(9, pgScore); flushHi(); overlay("You Win", String("Score ") + pgScore, TH.good); return; }
    pgStatic(); pgDrawDynamic(); return;
  }
  pgDrawDynamic();
}

// ---- GAME DISPATCHER ----
void gameInit() {
  clearContent();
  drawBar(GAME_TITLE[curGame], "NEW", false);
  switch (curGame) {
    case 0: tttInit(); break;
    case 1: snInit(); break;
    case 2: mmInit(); break;
    case 3: wkInit(); break;
    case 4: rxInit(); break;
    case 5: g48Init(); break;
    case 6: brInit(); break;
    case 7: simInit(); break;
    case 8: tetInit(); break;
    default: pgInit(); break;
  }
}
void gameStart(int g) {
  flushHi();
  curGame = g;
  screen = SCR_GAME;
  gameInit();
}
void gameLoop() {
  if (pressIn(SW - 70, 0, 66, BAR + 2)) { beep(1500, 20); gameStart(curGame); return; }
  switch (curGame) {
    case 0: tttLoop(); break;
    case 1: snLoop(); break;
    case 2: mmLoop(); break;
    case 3: wkLoop(); break;
    case 4: rxLoop(); break;
    case 5: g48Loop(); break;
    case 6: brLoop(); break;
    case 7: simLoop(); break;
    case 8: tetLoop(); break;
    default: pgLoop(); break;
  }
}

