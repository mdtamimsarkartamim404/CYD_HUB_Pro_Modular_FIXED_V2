// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ KNOWLEDGE QUIZ ================================
// A simple offline Yes/No quiz: General Knowledge, Islamic facts and Bangladesh facts.
struct KnowledgeQ { const char* cat; const char* q; bool yes; };
const KnowledgeQ KQ[] = {
  {"GENERAL", "The Earth goes around the Sun.", true},
  {"GENERAL", "Water freezes at 0 C at sea level.", true},
  {"GENERAL", "The Pacific Ocean is larger than the Atlantic.", true},
  {"GENERAL", "The Moon is a star.", false},
  {"ISLAMIC", "Muslims pray five obligatory prayers each day.", true},
  {"ISLAMIC", "Ramadan is the ninth month of the Islamic calendar.", true},
  {"ISLAMIC", "The Quran is the holy book of Islam.", true},
  {"ISLAMIC", "Fajr is one of the five daily obligatory prayers.", true},
  {"BANGLADESH", "Dhaka is the capital of Bangladesh.", true},
  {"BANGLADESH", "The national currency of Bangladesh is the taka.", true},
  {"BANGLADESH", "The Sundarbans is shared by Bangladesh and India.", true},
  {"BANGLADESH", "Cox's Bazar is known for its long natural sea beach.", true},
  {"BANGLADESH", "Bangladesh is located in South Asia.", true},
  {"GENERAL", "The Sun is a planet.", false},
  {"ISLAMIC", "Zakat is one of the Five Pillars of Islam.", true},
  {"BANGLADESH", "The national language of Bangladesh is Bangla.", true}
};
const int KQ_COUNT = sizeof(KQ) / sizeof(KQ[0]);
int kqIndex = 0, kqScore = 0, kqAnswered = 0;
bool kqAnsweredThis = false, kqLastCorrect = false;
uint32_t kqMsgUntil = 0;

void knowledgeDraw() {
  tft.fillScreen(TH.bg);
  drawBar("Knowledge", "NEXT", true);
  const KnowledgeQ &q = KQ[kqIndex];
  uint16_t catCol = !strcmp(q.cat, "ISLAMIC") ? TH.good :
                    (!strcmp(q.cat, "BANGLADESH") ? TH.accent2 : TH.accent);

  // Header cards
  tft.fillRoundRect(10, 38, 300, 30, 10, TH.panel);
  txt(q.cat, 24, 53, 2, catCol, TH.panel, ML_DATUM);
  txt(String("Q ") + String(kqIndex + 1) + "/" + String(KQ_COUNT),
      286, 53, 1, TH.dim, TH.panel, MR_DATUM);
  txt(String("Score ") + String(kqScore), 160, 77, 1, TH.dim, TH.bg, MC_DATUM);

  // Question card
  tft.fillRoundRect(10, 86, 300, 82, 12, TH.panel);
  tft.drawRoundRect(10, 86, 300, 82, 12, catCol);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TH.text, TH.panel);
  tft.setTextSize(1);
  String text = q.q;
  int y = 98;
  int pos = 0;
  while (pos < (int)text.length() && y < 155) {
    int cut = pos, last = -1;
    while (cut < (int)text.length()) {
      if (text[cut] == ' ') last = cut;
      String part = text.substring(pos, cut + 1);
      if (tft.textWidth(part, 1) > 278) break;
      cut++;
    }
    if (cut >= (int)text.length()) cut = text.length();
    else if (last > pos) cut = last;
    String line = text.substring(pos, cut);
    tft.drawString(line, 18, y, 1);
    y += 13;
    pos = cut;
    while (pos < (int)text.length() && text[pos] == ' ') pos++;
  }

  // Answer state
  if (!kqAnsweredThis) {
    button(12, 176, 140, 34, "YES", TH.good, TH.onacc, 2);
    button(168, 176, 140, 34, "NO", TH.bad, C565(255,255,255), 2);
    txt("Tap YES or NO", 160, 218, 1, TH.dim, TH.bg, MC_DATUM);
  } else {
    uint16_t resultCol = kqLastCorrect ? TH.good : TH.bad;
    const char* result = kqLastCorrect ? "RIGHT!" : "WRONG!";
    tft.fillRoundRect(12, 176, 296, 34, 10, resultCol);
    tft.drawRoundRect(12, 176, 296, 34, 10, resultCol);
    txt(result, 160, 193, 2, TH.onacc, resultCol, MC_DATUM);
    txt(kqLastCorrect ? "Correct answer" : "Try the next one",
        160, 218, 1, resultCol, TH.bg, MC_DATUM);
  }
}
void knowledgeInit() {
  int old = kqIndex;
  int tries = 0;
  do { kqIndex = random(KQ_COUNT); tries++; }
  while (kqIndex == old && KQ_COUNT > 1 && tries < 20);
  kqAnsweredThis = false; kqLastCorrect = false;
  knowledgeDraw();
}
void knowledgeAnswer(bool answer) {
  if (kqAnsweredThis) return;
  kqLastCorrect = (answer == KQ[kqIndex].yes);
  if (kqLastCorrect) kqScore++;
  kqAnswered++; kqAnsweredThis = true; kqMsgUntil = millis() + 1200;
  beep(kqLastCorrect ? 1800 : 700, 35);
  knowledgeDraw();
}
void knowledgeNext() {
  int old = kqIndex, tries = 0;
  do { kqIndex = random(KQ_COUNT); tries++; }
  while (kqIndex == old && KQ_COUNT > 1 && tries < 20);
  kqAnsweredThis = false; kqLastCorrect = false;
  knowledgeDraw();
}
void knowledgeLoop() {
  if (!tc.press) return;
  if (pressIn(SW - 70, 0, 66, BAR + 2)) { beep(1500, 20); knowledgeNext(); return; }
  if (pressIn(12, 170, 140, 42)) { knowledgeAnswer(true); return; }
  if (pressIn(168, 170, 140, 42)) { knowledgeAnswer(false); return; }
  if (kqAnsweredThis && (int32_t)(millis() - kqMsgUntil) > 0) knowledgeNext();
}

