// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ GAMES MENU ====================================
const char* GAME_NAME[10] = {"X & O", "Snake", "Memory", "Whack", "Reflex", "2048", "Bricks", "Simon", "Tetris", "Pong"};
int gtx(int i) { return 4 + (i % 5) * 63; }
int gty(int i) { return 32 + (i / 5) * 102; }

String hiText(int g) {
  if (hi[g] == 0) return String("-");
  if (g == 0) return String("Wins ") + hi[0];
  if (g == 2) return String(hi[2]) + " moves";
  if (g == 4) return String(hi[4]) + " ms";
  if (g == 7) return String("Lv ") + hi[7];
  return String("Best ") + hi[g];
}
void gameIcon(int g, int cx, int cy, uint16_t bg) {
  switch (g) {
    case 0:
      tft.drawLine(cx - 6, cy - 18, cx - 6, cy + 18, TH.dim); tft.drawLine(cx + 6, cy - 18, cx + 6, cy + 18, TH.dim);
      tft.drawLine(cx - 18, cy - 6, cx + 18, cy - 6, TH.dim); tft.drawLine(cx - 18, cy + 6, cx + 18, cy + 6, TH.dim);
      thickLine(cx - 16, cy - 16, cx - 8, cy - 8, 3, TH.accent); thickLine(cx - 16, cy - 8, cx - 8, cy - 16, 3, TH.accent);
      for (int r = 4; r <= 5; r++) tft.drawCircle(cx, cy, r + 2, TH.accent2);
      break;
    case 1:
      for (int i = 0; i < 5; i++) tft.fillRoundRect(cx - 20 + i * 8, cy + 6, 7, 7, 2, TH.good);
      for (int i = 0; i < 3; i++) tft.fillRoundRect(cx + 12, cy - 10 + i * 8 - 2, 7, 7, 2, TH.good);
      tft.fillRoundRect(cx + 12, cy - 18, 7, 7, 2, TH.accent);
      tft.fillCircle(cx - 12, cy - 10, 4, TH.bad);
      break;
    case 2:
      tft.fillRoundRect(cx - 24, cy - 15, 22, 30, 4, TH.accent);
      tft.fillRoundRect(cx + 2, cy - 15, 22, 30, 4, TH.accent2);
      txt("?", cx - 13, cy, 2, TH.onacc, TH.accent, MC_DATUM);
      txt("?", cx + 13, cy, 2, TH.onacc, TH.accent2, MC_DATUM);
      break;
    case 3:
      tft.fillEllipse(cx, cy + 12, 20, 7, C565(55, 34, 22));
      tft.fillCircle(cx, cy - 2, 13, C565(166, 112, 70));
      tft.fillCircle(cx - 5, cy - 5, 2, C565(20, 14, 10)); tft.fillCircle(cx + 5, cy - 5, 2, C565(20, 14, 10));
      tft.fillCircle(cx, cy + 1, 3, C565(255, 150, 160));
      break;
    case 4:
      tft.fillTriangle(cx + 4, cy - 20, cx - 10, cy + 3, cx + 2, cy + 3, COL_SUN);
      tft.fillTriangle(cx - 4, cy + 20, cx + 10, cy - 3, cx - 2, cy - 3, COL_SUN);
      break;
    case 5:
      tft.fillRoundRect(cx - 22, cy - 18, 44, 36, 7, C565(237, 194, 46));
      txt("2048", cx, cy + 1, 2, C565(255, 255, 255), C565(237, 194, 46), MC_DATUM);
      break;
    case 6:
      for (int r = 0; r < 3; r++) for (int c = 0; c < 4; c++) tft.fillRect(cx - 22 + c * 12, cy - 20 + r * 7, 10, 5, BRC[r]);
      tft.fillCircle(cx + 6, cy + 4, 3, TH.text);
      tft.fillRoundRect(cx - 12, cy + 16, 24, 5, 2, TH.accent);
      break;
    case 7:
      tft.fillRoundRect(cx - 20, cy - 20, 19, 19, 5, SIMB[0]); tft.fillRoundRect(cx + 1, cy - 20, 19, 19, 5, SIMB[1]);
      tft.fillRoundRect(cx - 20, cy + 1, 19, 19, 5, SIMB[2]); tft.fillRoundRect(cx + 1, cy + 1, 19, 19, 5, SIMB[3]);
      break;
    case 8:
      tft.fillRoundRect(cx - 20, cy - 20, 12, 12, 2, C565(0,220,220));
      tft.fillRoundRect(cx - 8, cy - 20, 12, 12, 2, C565(240,220,60));
      tft.fillRoundRect(cx - 20, cy - 8, 12, 12, 2, C565(180,80,220));
      tft.fillRoundRect(cx - 8, cy - 8, 12, 12, 2, C565(60,210,90));
      tft.fillRoundRect(cx + 4, cy - 8, 12, 12, 2, C565(240,70,70));
      tft.fillRoundRect(cx - 20, cy + 4, 12, 12, 2, C565(70,110,240));
      tft.fillRoundRect(cx - 8, cy + 4, 12, 12, 2, C565(240,160,50));
      break;
    default:
      tft.fillRoundRect(cx - 25, cy - 20, 6, 40, 3, TH.accent);
      tft.fillRoundRect(cx + 19, cy - 20, 6, 40, 3, TH.accent2);
      tft.fillCircle(cx, cy, 5, TH.text);
      tft.drawFastHLine(cx - 11, cy, 11, TH.dim);
      break;
  }
}
void gamesInit() {
  tft.fillScreen(TH.bg);
  drawBar("Games", nullptr, false);
  for (int i = 0; i < 10; i++) {
    int x = gtx(i), y = gty(i);
    tft.fillRoundRect(x, y, 58, 94, 10, TH.panel);
    gameIcon(i, x + 29, y + 36, TH.panel);
    txt(GAME_NAME[i], x + 29, y + 68, 2, TH.text, TH.panel, MC_DATUM);
    txt(hiText(i), x + 29, y + 84, 1, TH.dim, TH.panel, MC_DATUM);
  }
}
void gamesLoop() {
  if (!tc.press) return;
  for (int i = 0; i < 10; i++) {
    if (pressIn(gtx(i), gty(i), 58, 94)) { beep(1500, 20); gameStart(i); return; }
  }
}

