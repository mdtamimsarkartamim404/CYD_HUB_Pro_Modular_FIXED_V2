// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ WEATHER =======================================
struct WxData {
  bool ok; float t, feels, wind; int hum, code;
  float pressure, precip, visibility, uv, dew, cloud;
  int windDir, rainProb;
  float hi[7], lo[7], apparentHi[7], apparentLo[7], windMax[7];
  int dcode[7], dow[7];
  int dailyRain[7];
  char sunrise[6], sunset[6];
  uint32_t updated; char stamp[8];
};
WxData wx;
uint32_t wxLastTry = 0;
uint8_t wxMode = 0; // NOW, FORECAST, DETAILS, RAIN, WIND, SUN, AIR, WEEK

const uint16_t COL_SUN = C565(255, 200, 50);
const uint16_t COL_CLOUD = C565(150, 168, 200);
const uint16_t COL_RAIN = C565(70, 150, 255);
const uint16_t COL_SNOW = C565(150, 190, 240);

const char* wmoText(int c) {
  if (c == 0) return "Clear sky";
  if (c == 1) return "Mainly clear";
  if (c == 2) return "Partly cloudy";
  if (c == 3) return "Overcast";
  if (c == 45 || c == 48) return "Fog";
  if (c >= 51 && c <= 57) return "Drizzle";
  if (c >= 61 && c <= 67) return "Rain";
  if (c >= 71 && c <= 77) return "Snow";
  if (c >= 80 && c <= 82) return "Rain showers";
  if (c == 85 || c == 86) return "Snow showers";
  if (c >= 95) return "Thunderstorm";
  return "Cloudy";
}
int wmoIcon(int c) {
  if (c <= 1) return 0;
  if (c == 2) return 1;
  if (c == 3) return 2;
  if (c == 45 || c == 48) return 6;
  if ((c >= 51 && c <= 67) || (c >= 80 && c <= 82)) return 3;
  if ((c >= 71 && c <= 77) || c == 85 || c == 86) return 5;
  if (c >= 95) return 4;
  return 2;
}
void drawSun(int cx, int cy, int r, uint16_t col) {
  tft.fillCircle(cx, cy, r, col);
  for (int i = 0; i < 8; i++) {
    float a = i * PI / 4.0f;
    int x0 = cx + (int)(cosf(a) * (r + r * 0.45f));
    int y0 = cy + (int)(sinf(a) * (r + r * 0.45f));
    int x1 = cx + (int)(cosf(a) * (r + r * 0.95f));
    int y1 = cy + (int)(sinf(a) * (r + r * 0.95f));
    thickLine(x0, y0, x1, y1, r > 9 ? 3 : 1, col);
  }
}
void drawCloud(int cx, int cy, int u, uint16_t col) {
  int a = (5 * u) / 2, b = u / 2, c = (9 * u) / 2;
  tft.fillCircle(cx - a, cy + u, 2 * u, col);
  tft.fillCircle(cx + b, cy, 3 * u, col);
  tft.fillCircle(cx + a, cy + u, 2 * u, col);
  tft.fillRect(cx - c, cy + u, 2 * c, 2 * u, col);
}
void drawWxIcon(int cx, int cy, int u, int type) {
  switch (type) {
    case 0: drawSun(cx, cy, u * 3, COL_SUN); break;
    case 1:
      drawSun(cx - u * 2, cy - u * 2, u * 2 + u / 2, COL_SUN);
      drawCloud(cx + u / 2, cy + u, u, COL_CLOUD);
      break;
    case 2: drawCloud(cx, cy, u, COL_CLOUD); break;
    case 3:
      drawCloud(cx, cy - u, u, COL_CLOUD);
      for (int k = -1; k <= 1; k++) {
        int x = cx + k * 2 * u;
        thickLine(x, cy + 2 * u + u / 2, x - u / 2, cy + 4 * u, 2, COL_RAIN);
      }
      break;
    case 4:
      drawCloud(cx, cy - u, u, COL_CLOUD);
      tft.fillTriangle(cx + u, cy + u, cx - u, cy + 3 * u, cx + u / 3, cy + 3 * u, COL_SUN);
      tft.fillTriangle(cx - u / 2, cy + 3 * u, cx + u, cy + 3 * u, cx - u, cy + 5 * u, COL_SUN);
      break;
    case 5:
      drawCloud(cx, cy - u, u, COL_CLOUD);
      for (int k = -1; k <= 1; k++) tft.fillCircle(cx + k * 2 * u, cy + 3 * u + (k == 0 ? u / 2 : 0), max(2, u / 2), COL_SNOW);
      break;
    default:
      for (int i = 0; i < 3; i++)
        tft.fillRoundRect(cx - 4 * u + (i % 2) * u, cy - 2 * u + i * 2 * u, 7 * u, max(2, u / 2 + 1), 2, COL_CLOUD);
      break;
  }
}
int tempNum(float c) { return (int)lroundf(cfg.fahr ? c * 9.0f / 5.0f + 32.0f : c); }

uint16_t hexTo565(String h) {
  h.trim();
  if (h.startsWith("#")) h.remove(0, 1);
  if (h.length() != 6) return C565(255,255,255);
  long v = strtol(h.c_str(), nullptr, 16);
  return C565((v >> 16) & 255, (v >> 8) & 255, v & 255);
}
String colorHex(uint16_t c) {
  uint8_t r = ((c >> 11) & 0x1F) * 255 / 31;
  uint8_t g = ((c >> 5) & 0x3F) * 255 / 63;
  uint8_t b = (c & 0x1F) * 255 / 31;
  char s[8]; snprintf(s, sizeof(s), "#%02X%02X%02X", r, g, b);
  return String(s);
}
int drawTemp(int x, int y, float c, int font, int size, uint16_t fg, uint16_t bg) {
  String s = String(tempNum(c));
  txtp(s, x, y, font, fg, bg, TL_DATUM, size, 0);
  tft.setTextSize(size);
  int w = tft.textWidth(s, font);
  tft.setTextSize(1);
  int cx = x + w + 3 * size + 2, cy = y + 3 * size + 2;
  tft.drawCircle(cx, cy, size + 1, fg);
  if (size > 1) tft.drawCircle(cx, cy, size, fg);
  txtp(cfg.fahr ? "F" : "C", cx + size + 3, y, font, fg, bg, TL_DATUM, size, 0);
  return w;
}

bool fetchWeather() {
  if (WiFi.status() != WL_CONNECTED) return false;
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  http.setTimeout(8000);
  String url = String("https://api.open-meteo.com/v1/forecast?latitude=") + String(savedLat(), 4) +
               "&longitude=" + String(savedLon(), 4) +
               "&current=temperature_2m,relative_humidity_2m,apparent_temperature,weather_code,wind_speed_10m,wind_direction_10m,pressure_msl,precipitation,visibility,uv_index,dew_point_2m,cloud_cover"
               "&daily=weather_code,temperature_2m_max,temperature_2m_min,apparent_temperature_max,apparent_temperature_min,wind_speed_10m_max,precipitation_probability_max,sunrise,sunset&forecast_days=7&timezone=auto";
  if (!http.begin(client, url)) return false;
  int code = http.GET();
  if (code != HTTP_CODE_OK) { http.end(); return false; }
  String body = http.getString();
  http.end();
  JsonDocument doc;
  if (deserializeJson(doc, body)) return false;
  JsonObject cur = doc["current"];
  if (cur.isNull()) return false;
  wx.t     = cur["temperature_2m"] | 0.0f;
  wx.feels = cur["apparent_temperature"] | 0.0f;
  wx.hum   = cur["relative_humidity_2m"] | 0;
  wx.wind  = cur["wind_speed_10m"] | 0.0f;
  wx.pressure = cur["pressure_msl"] | 0.0f;
  wx.precip = cur["precipitation"] | 0.0f;
  wx.visibility = cur["visibility"] | 0.0f;
  wx.uv = cur["uv_index"] | 0.0f;
  wx.dew = cur["dew_point_2m"] | 0.0f;
  wx.cloud = cur["cloud_cover"] | 0.0f;
  wx.windDir = cur["wind_direction_10m"] | 0;
  wx.code  = cur["weather_code"] | 0;
  JsonObject d = doc["daily"];
  for (int i = 0; i < 7; i++) {
    wx.hi[i]    = (d["temperature_2m_max"][i] | 0.0f);
    wx.lo[i]    = (d["temperature_2m_min"][i] | 0.0f);
    wx.apparentHi[i] = (d["apparent_temperature_max"][i] | 0.0f);
    wx.apparentLo[i] = (d["apparent_temperature_min"][i] | 0.0f);
    wx.windMax[i] = (d["wind_speed_10m_max"][i] | 0.0f);
    wx.dailyRain[i] = d["precipitation_probability_max"][i] | 0;
    wx.dcode[i] = (d["weather_code"][i] | 0);
    if (i == 0) wx.rainProb = wx.dailyRain[i];
    const char* ds = d["time"][i] | "2000-01-01";
    wx.dow[i] = dowOf(atoi(ds), atoi(ds + 5), atoi(ds + 8));
  }
  const char* sr = d["sunrise"][0] | "";
  const char* ss = d["sunset"][0] | "";
  strncpy(wx.sunrise, strlen(sr) >= 16 ? sr + 11 : sr, 5); wx.sunrise[5] = 0;
  strncpy(wx.sunset,  strlen(ss) >= 16 ? ss + 11 : ss, 5); wx.sunset[5] = 0;
  wx.ok = true;
  wx.updated = millis();
  if (timeValid()) snprintf(wx.stamp, sizeof(wx.stamp), "%02d:%02d", nowTm.tm_hour, nowTm.tm_min);
  else wx.stamp[0] = 0;
  return true;
}
void weatherTick() {
  if (screen == SCR_GAME) return;
  if (WiFi.status() != WL_CONNECTED) {
    if (wx.ok) {
      wx.ok = false;
      if (screen == SCR_WEATHER) wxDraw();
      else if (screen == SCR_HOME) homeHeader(true);
    }
    return;
  }
  uint32_t now = millis();
  if (wxTried && (now - wxLastTry) < (wx.ok ? 1200000UL : 30000UL)) return;
  wxTried = true;
  wxLastTry = now;
  fetchWeather();
  if (screen == SCR_WEATHER) wxDraw();
  else if (screen == SCR_HOME) homeHeader(true);
}

