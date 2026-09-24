// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ WEATHER SCREEN ================================
const char* wxModeName(uint8_t m) {
  static const char* n[] = {"NOW","7-DAY","DETAILS","RAIN + UV","WIND","SUN","AIR","WEEK VIEW"};
  return n[m % 8];
}
String wxShortCondition(int c) { return String(wmoText(c)); }
void wxCard(int x, int y, int w, int h) {
  tft.fillRoundRect(x,y,w,h,12,TH.panel); tft.drawRoundRect(x,y,w,h,12,TH.accent);
}
void wxLabelValue(int x,int y,const String& label,const String& value,bool accent=false) {
  txtp(label,x,y,1,TH.dim,TH.panel,TL_DATUM,1,90);
  txtp(value,x,y+16,2,accent?TH.accent:TH.text,TH.panel,TL_DATUM,1,90);
}
void wxMiniDay(int i,int x,int y,int w) {
  // Compact 7-day card: deliberately kept inside 72px so rows never overlap.
  wxCard(x,y,w,76);
  txt(i==0?"TOD":DOW3[wx.dow[i]],x+w/2,y+11,1,TH.accent,TH.panel,MC_DATUM);
  drawWxIcon(x+w/2,y+32,2,wmoIcon(wx.dcode[i]));
  txt(String(tempNum(wx.hi[i]))+"/"+String(tempNum(wx.lo[i])),x+w/2,y+52,1,TH.text,TH.panel,MC_DATUM);
  txt(String(wx.dailyRain[i])+"%",x+w/2,y+67,1,TH.dim,TH.panel,MC_DATUM);
}

void wxDraw() {
  tft.fillScreen(TH.bg);
  drawBar("WEATHER", "REF", true);
  txtp(savedCity(),8,39,1,TH.dim,TH.bg,TL_DATUM,1,185);
  txtp(wxModeName(wxMode),312,39,1,TH.accent,TH.bg,TR_DATUM,1,92);

  if (!wx.ok) {
    wxCard(10,56,300,136);
    drawWxIcon(160,87,5,2);
    txt("WEATHER OFFLINE",160,126,2,TH.text,TH.panel,MC_DATUM);
    txt(WiFi.status()==WL_CONNECTED?"Tap REFRESH":"CONNECT WI-FI",160,148,1,TH.dim,TH.panel,MC_DATUM);
    button(103,164,114,26,"REFRESH",TH.accent,TH.onacc,2);
    txt("TAP SCREEN • NEXT PAGE",160,221,1,TH.dim,TH.bg,MC_DATUM);
    return;
  }

  const String unit=cfg.fahr?" F":" C";
  if(wxMode==0){
    wxCard(8,52,304,86); drawWxIcon(43,91,5,wmoIcon(wx.code));
    drawTemp(86,58,wx.t,4,2,TH.text,TH.panel);
    txt(wxShortCondition(wx.code),86,112,1,TH.accent,TH.panel,TL_DATUM);
    txt(String("FEELS ")+tempNum(wx.feels)+unit,86,126,1,TH.dim,TH.panel,TL_DATUM);
    const int xs[4]={45,119,193,273}; const char* lab[4]={"H/L","HUM","WIND","UV"};
    for(int i=0;i<4;i++){ wxCard(xs[i]-33,146,66,48); txt(lab[i],xs[i],157,1,TH.dim,TH.panel,MC_DATUM); }
    txt(String(tempNum(wx.hi[0]))+"/"+String(tempNum(wx.lo[0])),45,179,1,TH.text,TH.panel,MC_DATUM);
    txt(String(wx.hum)+"%",119,179,1,TH.text,TH.panel,MC_DATUM);
    txt(String((int)lroundf(wx.wind)),193,179,1,TH.text,TH.panel,MC_DATUM);
    txt(String(wx.uv,1),273,179,1,TH.text,TH.panel,MC_DATUM);
  } else if(wxMode==1){
    // 7 cards in one clean row; no vertical overlap.
    for(int i=0;i<7;i++) wxMiniDay(i,4+i*45,55,42);
    wxCard(8,138,304,58);
    txt("7-DAY OUTLOOK",20,153,1,TH.accent,TH.panel,TL_DATUM);
    txt("HIGH/LOW • RAIN % • CONDITION",20,176,1,TH.text,TH.panel,TL_DATUM);
  } else if(wxMode==2){
    wxCard(8,52,304,148); txt("LIVE CONDITIONS",160,68,1,TH.accent,TH.panel,MC_DATUM);
    wxLabelValue(20,76,"TEMP",String(tempNum(wx.t))+unit,true); wxLabelValue(112,76,"FEELS",String(tempNum(wx.feels))+unit); wxLabelValue(206,76,"DEW",String(tempNum(wx.dew))+unit);
    wxLabelValue(20,128,"HUMIDITY",String(wx.hum)+"%"); wxLabelValue(112,128,"PRESSURE",String((int)lroundf(wx.pressure))+" hPa"); wxLabelValue(206,128,"CLOUD",String((int)lroundf(wx.cloud))+"%");
    txt("VISIBILITY",20,181,1,TH.dim,TH.panel,TL_DATUM); txt(String((int)lroundf(wx.visibility/1000.0f))+" km",110,181,1,TH.text,TH.panel,TL_DATUM);
    txt("CONDITION",20,194,1,TH.dim,TH.panel,TL_DATUM); txt(wxShortCondition(wx.code),110,194,1,TH.text,TH.panel,TL_DATUM);
  } else if(wxMode==3){
    wxCard(8,52,148,146); wxCard(164,52,148,146);
    txt("RAIN",82,74,1,TH.accent,TH.panel,MC_DATUM); txt(String(wx.rainProb)+"%",82,108,4,TH.text,TH.panel,MC_DATUM); txt(String(wx.precip,1)+" mm",82,134,1,TH.dim,TH.panel,MC_DATUM);
    tft.fillRoundRect(25,156,114,8,4,TH.bg); tft.fillRoundRect(25,156,min(114,wx.rainProb*114/100),8,4,TH.accent);
    txt("UV INDEX",238,74,1,TH.accent2,TH.panel,MC_DATUM); txt(String(wx.uv,1),238,108,4,TH.text,TH.panel,MC_DATUM); txt(wx.uv>=8?"VERY HIGH":wx.uv>=6?"HIGH":wx.uv>=3?"MODERATE":"LOW",238,134,1,TH.dim,TH.panel,MC_DATUM);
    tft.fillRoundRect(181,156,114,8,4,TH.bg); tft.fillRoundRect(181,156,min(114,(int)lroundf(wx.uv*114.0f/11.0f)),8,4,TH.accent2);
  } else if(wxMode==4){
    wxCard(8,52,304,146); txt("WIND & SKY",160,70,1,TH.accent,TH.panel,MC_DATUM);
    txt("SPEED",22,92,1,TH.dim,TH.panel,TL_DATUM); txt(String((int)lroundf(wx.wind))+" km/h",22,111,2,TH.text,TH.panel,TL_DATUM);
    txt("DIRECTION",22,140,1,TH.dim,TH.panel,TL_DATUM); txt(String(wx.windDir)+" deg",22,159,2,TH.text,TH.panel,TL_DATUM);
    txt("MAX TODAY",176,92,1,TH.dim,TH.panel,TL_DATUM); txt(String((int)lroundf(wx.windMax[0]))+" km/h",176,111,2,TH.text,TH.panel,TL_DATUM);
    txt("CLOUD COVER",176,140,1,TH.dim,TH.panel,TL_DATUM); txt(String((int)lroundf(wx.cloud))+"%",176,159,2,TH.text,TH.panel,TL_DATUM);
    txt("DEW "+String(tempNum(wx.dew))+unit,160,185,1,TH.dim,TH.panel,MC_DATUM);
  } else if(wxMode==5){
    wxCard(8,52,304,146); drawSun(74,110,18,COL_SUN); drawCloud(246,110,7,COL_CLOUD);
    txt("SUNRISE",74,149,1,TH.dim,TH.panel,MC_DATUM); txt(wx.sunrise[0]?wx.sunrise:"--:--",74,173,3,TH.text,TH.panel,MC_DATUM);
    txt("SUNSET",246,149,1,TH.dim,TH.panel,MC_DATUM); txt(wx.sunset[0]?wx.sunset:"--:--",246,173,3,TH.text,TH.panel,MC_DATUM);
    txt("TODAY",160,201,1,TH.dim,TH.bg,MC_DATUM); txt(String(tempNum(wx.hi[0]))+unit+" / "+String(tempNum(wx.lo[0]))+unit,160,218,1,TH.text,TH.bg,MC_DATUM);
  } else if(wxMode==6){
    wxCard(8,52,304,146); txt("AIR / COMFORT",160,70,1,TH.accent,TH.panel,MC_DATUM);
    wxLabelValue(22,78,"DEW",String(tempNum(wx.dew))+unit); wxLabelValue(112,78,"HUMIDITY",String(wx.hum)+"%"); wxLabelValue(208,78,"CLOUD",String((int)lroundf(wx.cloud))+"%");
    wxLabelValue(22,130,"PRESSURE",String((int)lroundf(wx.pressure))+" hPa"); wxLabelValue(142,130,"VISIBILITY",String((int)lroundf(wx.visibility/1000.0f))+" km");
    txt("APPARENT",22,181,1,TH.dim,TH.panel,TL_DATUM); txt(String(tempNum(wx.apparentHi[0]))+unit+" max / "+String(tempNum(wx.apparentLo[0]))+unit+" min",100,181,1,TH.text,TH.panel,TL_DATUM);
  } else {
    wxCard(8,52,304,146); txt("WEEK SUMMARY",160,68,1,TH.accent,TH.panel,MC_DATUM);
    for(int i=0;i<7;i++){
      int x=28+i*43;
      txt(i==0?"TOD":DOW3[wx.dow[i]],x,87,1,TH.dim,TH.panel,MC_DATUM);
      drawWxIcon(x,111,2,wmoIcon(wx.dcode[i]));
      txt(String(tempNum(wx.hi[i]))+"/"+String(tempNum(wx.lo[i])),x,139,1,TH.text,TH.panel,MC_DATUM);
      txt(String(wx.dailyRain[i])+"%",x,157,1,TH.accent,TH.panel,MC_DATUM);
      txt(String((int)lroundf(wx.windMax[i])),x,176,1,TH.dim,TH.panel,MC_DATUM);
    }
  }
  txt("TAP SCREEN  •  NEXT WEATHER PAGE",160,225,1,TH.dim,TH.bg,MC_DATUM);
}
void wxRefresh() {
  txtp("UPDATING...", SW - 74, BAR / 2 + 1, 1, TH.accent, TH.panel, MR_DATUM, 1, 70);
  wxTried=true; wxLastTry=millis(); fetchWeather(); wxDraw();
}
void weatherInit() { tft.fillScreen(TH.bg); wxDraw(); if(WiFi.status()==WL_CONNECTED && (!wx.ok || millis()-wx.updated>600000UL)) wxRefresh(); }
void weatherLoop() {
  if(!tc.press) return;
  if(pressIn(SW-70,0,66,BAR+2)){beep(1500,20);wxRefresh();return;}
  if(tc.y>BAR+2){wxMode=(wxMode+1)%8;beep(1300,18);wxDraw();}
}

