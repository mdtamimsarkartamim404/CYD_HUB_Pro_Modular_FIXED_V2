// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================== USER SETTINGS ==============================
const char* WIFI_SSID = "";
const char* WIFI_PASS = "";
const char* CONTROL_AP_SSID = "CYD-HUB-CONTROL";
const char* CONTROL_AP_PASS = "CYDControl24";
const long  GMT_OFFSET_SEC = 6 * 3600;
const int   DST_OFFSET_SEC = 0;
const char* CITY_NAME = "Rajshahi";
const float LATITUDE  = 24.3745f;
const float LONGITUDE = 88.6042f;

#define TFT_ROT        1
#define TOUCH_FLIP     0
#define INVERT_COLORS  0

#define TS_X_MIN 440
#define TS_X_MAX 3568
#define TS_Y_MIN 504
#define TS_Y_MAX 3561
#define TS_Z_MIN 250

#define PIN_BL      21
#define PIN_SPK     26
#define TOUCH_CLK   25
#define TOUCH_MISO  39
#define TOUCH_MOSI  32
#define TOUCH_CS    33

#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
  #define CORE3 1
#else
  #define CORE3 0
#endif

#define SCR_HOME     0
#define SCR_CLOCK    1
#define SCR_TIMER    2
#define SCR_WEATHER  3
#define SCR_GAMES    4
#define SCR_SETTINGS 5
#define SCR_GAME     6
#define SCR_SETUP    7
#define SCR_ISLAMIC  8
#define SCR_ALARM    9
#define SCR_KNOWLEDGE 10

#define C565(r,g,b) ((uint16_t)((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)))

const int SW = 320, SH = 240, BAR = 26;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite clockSprite = TFT_eSprite(&tft);
bool clockSpriteReady = false;

SPIClass touchSPI(VSPI);
XPT2046_Touchscreen ts(TOUCH_CS);
Preferences prefs;

int screen = SCR_HOME;
int curGame = 0;
uint8_t setupQrStage = 0; // 0=phone setup, 1=AP control QR, 2=Home-WiFi IP QR, 3=mDNS control QR
bool setupQrClientSeen = false;
uint32_t setupQrPopupUntil = 0;

// Connection status popups
bool lastStaConnected = false;
bool lastApHasClient = false;
bool bootWaitingForWiFi = true;
uint32_t bootHomeAt = 0;
bool connPopupActive = false;
uint32_t connPopupUntil = 0;
String connPopupTitle = "";
String connPopupLine = "";

