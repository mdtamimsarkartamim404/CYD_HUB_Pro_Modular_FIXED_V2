/* CYD HUB Professional Control v3
 * ESP32-2432S028R (2.8" Cheap Yellow Display, ST7789)
 * Clock | Timer | Alarm | Weather | 10 Games | Islamic (Bangla) | Web Control
 *
 * FIX: islamicText() now takes an int index (not const IslamicItem&) so the
 *      Arduino auto-generated prototype doesn't reference IslamicItem before
 *      the struct is defined.
 */

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <time.h>
#include <sys/time.h>
#include <qrcode.h>


// ============================================================
// MODULAR PROJECT FILES — original section order preserved
// ============================================================
#include "01_USER_SETTINGS.h"
#include "02_FORWARD_DECLARATIONS.h"
#include "03_AP_QR_CODES.h"
#include "04_THEMES.h"
#include "05_STORAGE.h"
#include "06_SOUND_BACKLIGHT.h"
#include "07_TOUCH.h"
#include "08_DRAW_HELPERS.h"
#include "09_TIME.h"
#include "10_WEATHER.h"
#include "11_ICONS.h"
#include "12_ALARM.h"
#include "13_HOME.h"
#include "14_CLOCK.h"
#include "15_TIMER.h"
#include "16_WEATHER_SCREEN.h"
#include "17_SETTINGS.h"
#include "18_SETUP_SCANNER.h"
#include "19_ISLAMIC_Bangla.h"
#include "20_KNOWLEDGE_QUIZ.h"
#include "21_GAME_COMMON.h"
#include "22_GAMES_MENU.h"
#include "23_PHONE_CONTROL.h"
#include "24_SCREEN_ROUTER.h"

// ============================ SETUP / LOOP ==================================
// ============================ SETUP / LOOP ==================================
void splash() {
  tft.fillScreen(TH.bg);
  txtp("CYD HUB Pro", SW / 2, 96, 4, TH.accent, TH.bg, MC_DATUM, 2, 0);
  txt("Clock  Timer  Alarm  Islamic  Games", SW / 2, 140, 2, TH.dim, TH.bg, MC_DATUM);
  tft.drawRoundRect(60, 170, 200, 10, 5, TH.dim);
  for (int i = 0; i <= 192; i += 6) { tft.fillRoundRect(64, 172, i, 6, 3, TH.accent); delay(6); }
}
void setup() {
  Serial.begin(115200);
  Serial.println("CYD HUB: alarm engine starting");
  loadCfg();
  tft.init();
  tft.setRotation(3);
  tft.invertDisplay(INVERT_COLORS);
  tft.fillScreen(TH.bg);
  hwPwmInit();
  touchSPI.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
  ts.begin(touchSPI);
  ts.setRotation(3);
  randomSeed(esp_random());

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(CONTROL_AP_SSID, CONTROL_AP_PASS);
  lastApHasClient = false;
  // Start disconnected so a saved Home-Wi-Fi connection is treated as a real
  // connection event. This guarantees the CONNECTED popup and fresh DHCP-IP
  // Control Center QR also appear after a reboot.
  lastStaConnected = false;
  bootWaitingForWiFi = true;
  beginSavedWiFi();
  setupWebServer();
  splash();
  goScreen(SCR_SETUP);
}
void loop() {
  updateTouch();
  beepTick();
  wifiTick();
  connectionPopupTick();
  timerBackground();
  alarmTick();
  weatherTick();
  server.handleClient();

  if (!alarmRinging && screen != SCR_HOME && tc.press && tc.x < 52 && tc.y < BAR + 2) {
    beep(1000, 20);
    goScreen(screen == SCR_GAME ? SCR_GAMES : SCR_HOME);
    return;
  }
  switch (screen) {
    case SCR_HOME:     homeLoop();     break;
    case SCR_CLOCK:    clockLoop();    break;
    case SCR_TIMER:    timerLoop();    break;
    case SCR_WEATHER:  weatherLoop();  break;
    case SCR_GAMES:    gamesLoop();    break;
    case SCR_SETTINGS: settingsLoop(); break;
    case SCR_SETUP:    setupLoop();    break;
    case SCR_GAME:     gameLoop();     break;
    case SCR_ISLAMIC:  islamicLoop();  break;
    case SCR_ALARM:    alarmLoop();    break;
    case SCR_KNOWLEDGE: knowledgeLoop(); break;
  }
  delay(1);
}
