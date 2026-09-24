// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ SCREEN ROUTER =================================
void goScreen(int s) {
  if (screen == SCR_GAME && s != SCR_GAME) flushHi();
  if (screen == SCR_CLOCK && s != SCR_CLOCK) clockSpriteEnd();

  screen = s;
  switch (s) {
    case SCR_HOME:     homeInit();     break;
    case SCR_CLOCK:    clockInit();    break;
    case SCR_TIMER:    timerInit();    break;
    case SCR_WEATHER:  weatherInit();  break;
    case SCR_GAMES:    gamesInit();    break;
    case SCR_SETTINGS: settingsInit(); break;
    case SCR_SETUP:    setupInit();    break;
    case SCR_ISLAMIC:  islamicInit();  break;
    case SCR_ALARM:    alarmInit();    break;
    case SCR_KNOWLEDGE: knowledgeInit(); break;
    default: break;
  }
}

