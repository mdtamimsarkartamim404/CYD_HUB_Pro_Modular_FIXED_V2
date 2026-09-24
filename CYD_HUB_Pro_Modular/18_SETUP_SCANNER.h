// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ SETUP / SCANNER ===============================
void setupWifiScan() {
  tft.fillRect(0, BAR, SW, SH - BAR, TH.bg);
  txt("Scanning nearby Wi-Fi...", 160, 105, 2, TH.accent, TH.bg, MC_DATUM);
  WiFi.scanNetworks(false, true);
  WiFi.scanDelete();
}
void setupInit() {
  tft.fillScreen(TH.bg);
  drawBar("PHONE WIFI SETUP", "NEXT", true);
  drawQR(CYD_AP_QR, (SW - ((33 + 4) * 3)) / 2, 28, 3);
  txt("SCAN TO CONNECT PHONE", 160, 152, 1, TH.text, TH.bg, MC_DATUM);
  txt("AP: " + String(CONTROL_AP_SSID), 160, 169, 1, TH.accent, TH.bg, MC_DATUM);
  txt("Password: " + String(CONTROL_AP_PASS), 160, 185, 1, TH.text, TH.bg, MC_DATUM);
  txt("TOUCH SCREEN FOR NEXT QR", 160, 202, 1, TH.dim, TH.bg, MC_DATUM);
  if (WiFi.status() == WL_CONNECTED) {
    tft.fillRoundRect(72, 214, 176, 22, 7, TH.panel);
    tft.drawRoundRect(72, 214, 176, 22, 7, TH.accent);
    txt("TOUCH SCREEN  •  3 CONTROL QR", 160, 225, 1, TH.accent, TH.panel, MC_DATUM);
  }
}
void drawNextQrScreen() {
  setupQrStage = 1;
  tft.fillScreen(TH.bg);
  drawBar("PHONE WIFI SETUP", "BACK", true);

  // Dynamic AP Control Center QR: always use the current AP IP.
  String ip = WiFi.softAPIP().toString();
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

  txt("QR 1 / 3  •  AP CONTROL", 160, 190, 1, TH.text, TH.bg, MC_DATUM);
  txt(ip, 160, 212, 2, TH.accent, TH.bg, MC_DATUM);
  txt("SCAN CURRENT AP IP", 160, 232, 1, TH.dim, TH.bg, MC_DATUM);
}

void drawQrConnectedPopup() {
  tft.fillRoundRect(18, 76, 284, 88, 16, TH.panel);
  tft.drawRoundRect(18, 76, 284, 88, 16, TH.accent);
  txt("PHONE CONNECTED", 160, 99, 2, TH.good, TH.panel, MC_DATUM);
  txt("Opening next QR...", 160, 124, 1, TH.text, TH.panel, MC_DATUM);
  txt("Wi-Fi link ready", 160, 146, 1, TH.dim, TH.panel, MC_DATUM);
}

void setupLoop() {
  // Touch anywhere on the setup/QR screen to cycle through all 3 Control QRs.
  // No NEXT/BACK button is required.
  // 0 = setup/AP screen, 1 = AP Control QR, 2 = Home Wi-Fi IP QR, 3 = hostname QR.

  if (setupQrStage == 0) {
    int clients = WiFi.softAPgetStationNum();
    if (clients > 0 && !setupQrClientSeen) {
      setupQrClientSeen = true;
      setupQrPopupUntil = millis() + 1200UL;
      drawQrConnectedPopup();
    }

    // Keep the connection popup, but a touch immediately advances to QR #1.
    if (!tc.release) {
      if (setupQrPopupUntil && (int32_t)(millis() - setupQrPopupUntil) >= 0) {
        setupQrPopupUntil = 0;
        setupInit();
      }
      return;
    }

    setupQrPopupUntil = 0;
    drawNextQrScreen();                 // Touch #1 -> AP Control QR
    return;
  }

  if (!tc.release) return;

  if (setupQrStage == 1) {
    // Touch #2 -> Home Wi-Fi IP QR.
    if (WiFi.status() == WL_CONNECTED) {
      drawStaIpQrScreen();
    } else {
      // If Home Wi-Fi is not connected, still show a clear page instead of
      // silently returning to the setup screen.
      tft.fillScreen(TH.bg);
      drawBar("WIFI CONTROL", "TOUCH", true);
      tft.fillRoundRect(18, 72, 284, 88, 16, TH.panel);
      tft.drawRoundRect(18, 72, 284, 88, 16, TH.accent);
      txt("HOME WI-FI OFFLINE", 160, 100, 2, TH.bad, TH.panel, MC_DATUM);
      txt("Connect Home Wi-Fi", 160, 126, 1, TH.text, TH.panel, MC_DATUM);
      txt("Touch again for QR #3", 160, 149, 1, TH.dim, TH.panel, MC_DATUM);
      setupQrStage = 2;
    }
    return;
  }

  if (setupQrStage == 2) {
    // Touch #3 -> hostname QR.
    drawMdnsQrScreen();
    return;
  }

  // Touch after QR #3 -> back to the setup/AP QR page.
  setupQrStage = 0;
  setupQrClientSeen = (WiFi.softAPgetStationNum() > 0);
  setupInit();
}

