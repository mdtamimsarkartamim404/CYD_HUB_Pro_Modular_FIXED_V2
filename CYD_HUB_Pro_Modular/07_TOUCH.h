// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ TOUCH =========================================
struct TouchState { bool down, was, press, release; int x, y; uint32_t lastSeen; };
TouchState tc = {false, false, false, false, 0, 0, 0};

bool readTouchRaw(int &x, int &y) {
  if (!ts.touched()) return false;
  TS_Point p = ts.getPoint();
  if (p.z < TS_Z_MIN) return false;
  long mx = map(p.x, TS_X_MIN, TS_X_MAX, 0, SW - 1);
  long my = map(p.y, TS_Y_MIN, TS_Y_MAX, 0, SH - 1);
  mx = constrain(mx, 0, SW - 1);
  my = constrain(my, 0, SH - 1);
  if (TOUCH_FLIP) { mx = SW - 1 - mx; my = SH - 1 - my; }
  x = (int)mx; y = (int)my;
  return true;
}
void updateTouch() {
  int x = 0, y = 0;
  bool raw = readTouchRaw(x, y);
  uint32_t now = millis();
  if (raw) { tc.lastSeen = now; tc.x = x; tc.y = y; }
  bool d = raw || (tc.was && (now - tc.lastSeen) < 40);
  tc.press = d && !tc.was;
  tc.release = !d && tc.was;
  tc.down = d;
  tc.was = d;
}

