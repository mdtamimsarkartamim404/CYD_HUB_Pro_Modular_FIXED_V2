// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ TIME ==========================================
struct tm nowTm;
bool ntpStarted = false;
uint32_t lastWifiTry = 0;
bool wifiConnectingFromWeb = false;
String wifiMessage = "";
uint32_t wifiMessageUntil = 0;
bool staQrActive = false;
uint32_t staQrUntil = 0;
bool staQrShown = false;
bool wxTried = false;

void setWifiMessage(const String &m) { wifiMessage = m; wifiMessageUntil = millis() + 5000; }
String savedWifiSSID() { return prefs.getString("wifi_ssid", String(WIFI_SSID)); }
String savedWifiPASS() { return prefs.getString("wifi_pass", String(WIFI_PASS)); }
String savedCity()     { return prefs.getString("city", String(CITY_NAME)); }
float savedLat()       { return prefs.getFloat("lat", LATITUDE); }
float savedLon()       { return prefs.getFloat("lon", LONGITUDE); }
long savedTzOffset()   { return prefs.getLong("tz", GMT_OFFSET_SEC); }
long savedDstOffset()  { return prefs.getLong("dst", DST_OFFSET_SEC); }

void saveLocationConfig(const String &city, float lat, float lon, long tz, long dst) {
  prefs.putString("city", city);
  prefs.putFloat("lat", lat);
  prefs.putFloat("lon", lon);
  prefs.putLong("tz", tz);
  prefs.putLong("dst", dst);
  ntpStarted = false;
  wxTried = false;
}
String tzLabel() {
  long sec = savedTzOffset();
  char b[12];
  long mins = sec / 60;
  long ah = labs(mins) / 60, am = labs(mins) % 60;
  snprintf(b, sizeof(b), "%s%02ld:%02ld", mins >= 0 ? "+" : "-", ah, am);
  return String(b);
}
void saveWifiCredentials(const String &ssid, const String &pass) {
  prefs.putString("wifi_ssid", ssid);
  prefs.putString("wifi_pass", pass);
}
void beginSavedWiFi() {
  String ssid = savedWifiSSID();
  String pass = savedWifiPASS();
  if (ssid.length()) {
    WiFi.begin(ssid.c_str(), pass.c_str());
    lastWifiTry = millis();
  }
}
const char* DOW3[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* MON3[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

bool timeValid() {
  time_t n = time(nullptr);
  if (n < 1700000000L) return false;
  localtime_r(&n, &nowTm);
  return true;
}
int hour12(int h) { int x = h % 12; return x == 0 ? 12 : x; }
int dowOf(int y, int m, int d) {
  static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  if (m < 3) y -= 1;
  return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}
void wifiTick() {
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnectingFromWeb = false;
    if (!ntpStarted) {
      configTime(savedTzOffset(), savedDstOffset(), "pool.ntp.org", "time.google.com", "time.cloudflare.com");
      ntpStarted = true;
    }
  } else if (millis() - lastWifiTry > 20000) {
    beginSavedWiFi();
  }
}

void redrawCurrentScreen() {
  switch (screen) {
    case SCR_HOME: homeInit(); break;
    case SCR_CLOCK: clockInit(); break;
    case SCR_TIMER: timerInit(); break;
    case SCR_WEATHER: weatherInit(); break;
    case SCR_GAMES: gamesInit(); break;
    case SCR_SETTINGS: settingsInit(); break;
    case SCR_SETUP: setupInit(); break;
    case SCR_ISLAMIC: islamicInit(); break;
    case SCR_ALARM: alarmInit(); break;
    case SCR_KNOWLEDGE: knowledgeInit(); break;
    default: break;
  }
}

void drawConnectionPopup() {
  tft.fillRoundRect(18, 70, 284, 104, 18, TH.panel);
  tft.drawRoundRect(18, 70, 284, 104, 18, TH.accent);
  uint16_t c = (connPopupTitle.indexOf("DISCONNECTED") >= 0) ? TH.bad : TH.good;
  tft.fillCircle(52, 96, 12, c);
  txt(connPopupTitle, 160, 92, 2, c, TH.panel, MC_DATUM);
  txtp(connPopupLine, 160, 121, 1, TH.text, TH.panel, MC_DATUM, 1, 248);
  txt("CYD HUB", 160, 148, 1, TH.dim, TH.panel, MC_DATUM);
}


static void drawStaQrCallback(esp_qrcode_handle_t qr) {
  int size = esp_qrcode_get_size(qr);
  if (size <= 0) return;

  const int border = 2;
  const int maxQrW = SW - 28;
  const int maxQrH = 154;
  int module = min(maxQrW / (size + border * 2), maxQrH / (size + border * 2));
  if (module < 2) module = 2;

  int total = (size + border * 2) * module;
  int x = (SW - total) / 2;
  int y = 40;

  tft.fillRect(x, y, total, total, TFT_WHITE);
  for (int r = -border; r < size + border; r++) {
    for (int c = -border; c < size + border; c++) {
      bool black = (c >= 0 && r >= 0 && c < size && r < size) &&
                   esp_qrcode_get_module(qr, c, r);
      if (black) {
        tft.fillRect(x + (c + border) * module,
                     y + (r + border) * module,
                     module, module, TFT_BLACK);
      }
    }
  }
}

void drawStaIpQrScreen() {
  // QR #2: current Home-WiFi DHCP IP -> Control Center
  setupQrStage = 2;
  tft.fillScreen(TH.bg);
  drawBar("WIFI CONTROL", "NEXT", true);

  String ip = WiFi.localIP().toString();
  if (ip == "0.0.0.0") {
    tft.fillRoundRect(20, 72, 280, 76, 12, TH.panel);
    txt("WIFI NOT READY", 160, 98, 2, TH.bad, TH.panel, MC_DATUM);
    txt("Connect to Home Wi-Fi", 160, 124, 1, TH.text, TH.panel, MC_DATUM);
    return;
  }
  String url = String("http://") + ip + "/";

  esp_qrcode_config_t qrCfg = {};
  qrCfg.display_func = drawStaQrCallback;
  qrCfg.max_qrcode_version = 6;
  qrCfg.qrcode_ecc_level = ESP_QRCODE_ECC_LOW;
  esp_err_t qrErr = esp_qrcode_generate(&qrCfg, url.c_str());

  if (qrErr != ESP_OK) {
    tft.fillRoundRect(20, 72, 280, 76, 12, TH.panel);
    txt("QR GENERATION FAILED", 160, 98, 2, TH.bad, TH.panel, MC_DATUM);
    txt("Use the IP below", 160, 124, 1, TH.dim, TH.panel, MC_DATUM);
  }

  txt("QR 2 / 3  •  HOME WI-FI IP", SW/2, 190, 1, TH.text, TH.bg, MC_DATUM);
  txt(ip, SW/2, 211, 2, TH.accent, TH.bg, MC_DATUM);
  txt("SCAN: http://" + ip + "/", SW/2, 231, 1, TH.dim, TH.bg, MC_DATUM);
}

void drawMdnsQrScreen() {
  // QR #3: mDNS hostname -> Control Center. Works on the same LAN when
  // the phone/device supports multicast DNS.
  setupQrStage = 3;
  tft.fillScreen(TH.bg);
  drawBar("WIFI CONTROL", "BACK", true);

  const char* host = "cydhub.local";
  String url = String("http://") + host + "/";

  esp_qrcode_config_t qrCfg = {};
  qrCfg.display_func = drawStaQrCallback;
  qrCfg.max_qrcode_version = 6;
  qrCfg.qrcode_ecc_level = ESP_QRCODE_ECC_LOW;
  esp_err_t qrErr = esp_qrcode_generate(&qrCfg, url.c_str());

  if (qrErr != ESP_OK) {
    tft.fillRoundRect(20, 72, 280, 76, 12, TH.panel);
    txt("QR GENERATION FAILED", 160, 98, 2, TH.bad, TH.panel, MC_DATUM);
  }

  txt("QR 3 / 3  •  HOSTNAME", SW/2, 190, 1, TH.text, TH.bg, MC_DATUM);
  txt("http://cydhub.local/", SW/2, 211, 1, TH.accent, TH.bg, MC_DATUM);
  txt("SAME WI-FI NETWORK", SW/2, 231, 1, TH.dim, TH.bg, MC_DATUM);
}

void connectionPopupTick() {
  bool sta = (WiFi.status() == WL_CONNECTED);
  bool ap = (WiFi.softAPgetStationNum() > 0);
  static bool initialized = false;
  if (!initialized) { lastStaConnected = sta; lastApHasClient = ap; initialized = true; }

  if (sta != lastStaConnected) {
    lastStaConnected = sta;
    connPopupActive = true;
    connPopupUntil = millis() + 2600UL;
    connPopupTitle = sta ? "WI-FI CONNECTED" : "WI-FI DISCONNECTED";
    connPopupLine = sta ? (String("IP: ") + WiFi.localIP().toString()) : "Home Wi-Fi link lost";
    drawConnectionPopup();
    if (sta) {
      ntpStarted = false;
      // Third Control QR: mDNS hostname. It remains valid even if DHCP IP changes.
      if (MDNS.begin("cydhub")) {
        MDNS.addService("http", "tcp", 80);
      }

      // IMPORTANT: always build a NEW Control Center QR from the CURRENT
      // Home-Wi-Fi DHCP address. Never reuse the AP IP/static QR here.
      // The QR will be shown automatically after the connection popup.
      // Home-WiFi gets its OWN dynamic Control Center QR.
      // AP QR remains available separately through PHONE WIFI SETUP.
      staQrActive = true;
      staQrShown = false;
      staQrUntil = millis() + 9000UL;
      bootHomeAt = 0;
    } else {
      staQrActive = false;
      staQrShown = false;
    }
  }

  // After EVERY new Home-WiFi connection, regenerate the Control Center QR
  // from the freshly assigned DHCP address. This means changing routers/networks
  // automatically produces a QR for the NEW IP instead of reusing an old one.
  if (staQrActive && !staQrShown && !connPopupActive && WiFi.status() == WL_CONNECTED) {
    staQrShown = true;
    delay(120); // allow the DHCP address/server state to settle before QR generation
    drawStaIpQrScreen();
  }
  if (staQrActive && (int32_t)(millis() - staQrUntil) >= 0) {
    staQrActive = false;
    staQrShown = false;
    if (bootWaitingForWiFi) { bootWaitingForWiFi = false; goScreen(SCR_HOME); }
    else redrawCurrentScreen();
  }
  if (bootHomeAt && (int32_t)(millis() - bootHomeAt) >= 0) {
    bootHomeAt = 0;
    bootWaitingForWiFi = false;
    connPopupActive = false;
    goScreen(SCR_HOME);
  }

  if (ap != lastApHasClient) {
    lastApHasClient = ap;
    if (screen == SCR_SETUP && setupQrStage == 0 && ap) {
      setupQrClientSeen = true;
      setupQrPopupUntil = millis() + 1200UL;
      drawQrConnectedPopup();
    }
    if (screen != SCR_SETUP || !ap) {
      connPopupActive = true;
      connPopupUntil = millis() + 2200UL;
      connPopupTitle = ap ? "DEVICE CONNECTED" : "DEVICE DISCONNECTED";
      connPopupLine = ap ? "Phone joined CYD Control AP" : "Phone left CYD Control AP";
      drawConnectionPopup();
    }
  }

  if (connPopupActive && (int32_t)(millis() - connPopupUntil) >= 0) {
    connPopupActive = false;
    redrawCurrentScreen();
  }
}

