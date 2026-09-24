// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ ISLAMIC (Bangla) ==============================
struct IslamicItem {
  const char* arabic;
  const char* translation;   // English
  const char* bangla;        // Banglish (romanised Bangla)
  const char* ref;
  uint8_t category;          // 1=Ayat, 2=Hadith, 3=Tip
};

const IslamicItem ISLAMIC_DB[] = {
  // ---- QURAN AYATS ----
  {"بِسْمِ اللَّهِ الرَّحْمَٰنِ الرَّحِيمِ",
   "In the name of Allah, the Entirely Merciful, the Especially Merciful.",
   "Porom korunamoy, maha dayalu Allah er name shuru korchi.",
   "Al-Fatihah 1:1", 1},
  {"إِنَّ مَعَ الْعُسْرِ يُسْرًا",
   "Indeed, with hardship [will be] ease.",
   "Nishchoi koshter sathe sohojota ache.",
   "Ash-Sharh 94:6", 1},
  {"وَمَن يَتَوَكَّلْ عَلَى اللَّهِ فَهُوَ حَسْبُهُ",
   "And whoever relies upon Allah - then He is sufficient for him.",
   "Je byakti Allah er upor bhorosa kore, tar jonno Allahi jotheshto.",
   "At-Talaq 65:3", 1},
  {"إِنَّ اللَّهَ مَعَ الصَّابِرِينَ",
   "Indeed, Allah is with the patient.",
   "Nishchoi Allah dhairjosholider sathe achen.",
   "Al-Baqarah 2:153", 1},
  {"وَاذْكُر رَّبَّكَ إِذَا نَسِيتَ",
   "And remember your Lord when you forget.",
   "Jokhon bhule jao, tokhon tomader Rob er smoron koro.",
   "Al-Kahf 18:24", 1},
  {"رَبِّ زِدْنِي عِلْمًا",
   "My Lord, increase me in knowledge.",
   "He amar Rob! Amar gyan bariye dao.",
   "Ta-Ha 20:114", 1},
  {"وَقُل رَّبِّ ارْحَمْهُمَا كَمَا رَبَّيَانِي صَغِيرًا",
   "And say: My Lord, have mercy upon them as they brought me up when I was small.",
   "Bolo: He amar Rob! Tader prati doya koro, jemon tara choto belay amake protipalon koreche.",
   "Al-Isra 17:24", 1},
  {"فَاذْكُرُونِي أَذْكُرْكُمْ وَاشْكُرُوا لِي وَلَا تَكْفُرُونِ",
   "So remember Me; I will remember you. And be grateful to Me and do not deny Me.",
   "Tomra amake smoron koro, ami tomader smoron korbo. Amar krito gyata hao, kufuri koro na.",
   "Al-Baqarah 2:152", 1},
  {"وَعَسَىٰ أَن تَكْرَهُوا شَيْئًا وَهُوَ خَيْرٌ لَّكُمْ",
   "Perhaps you hate a thing and it is good for you.",
   "Hote pare tumi kono jinis opochondo koro, athocho ta tomader jonno mongol.",
   "Al-Baqarah 2:216", 1},
  {"إِنَّ اللَّهَ لَا يُغَيِّرُ مَا بِقَوْمٍ حَتَّىٰ يُغَيِّرُوا مَا بِأَنفُسِهِمْ",
   "Indeed, Allah will not change the condition of a people until they change what is in themselves.",
   "Nishchoi Allah kono jati'r obostha poriborton koren na, jotokhon na tara nijedero poriborton kore.",
   "Ar-Ra'd 13:11", 1},
  {"وَاللَّهُ يُحِبُّ الْمُحْسِنِينَ",
   "And Allah loves the doers of good.",
   "Allah mohsin (uttom kormosholi) der bhalobasen.",
   "Al-Imran 3:134", 1},
  {"إِنَّ الصَّلَاةَ كَانَتْ عَلَى الْمُؤْمِنِينَ كِتَابًا مَّوْقُوتًا",
   "Indeed, prayer has been decreed upon the believers at specified times.",
   "Nishchoi namaj mumin der upor nirdisto somoye farj kora hoyeche.",
   "An-Nisa 4:103", 1},
  {"وَبَشِّرِ الصَّابِرِينَ",
   "And give good tidings to the patient.",
   "Aro shubhokhobor dao dhairjosholider.",
   "Al-Baqarah 2:155", 1},
  {"وَلَا تَقْرَبُوا الْفَوَاحِشَ مَا ظَهَرَ مِنْهَا وَمَا بَطَنَ",
   "And do not approach immoralities - what is apparent of them and what is concealed.",
   "Aslila (kharap kaj) er kache jao na - ja prokashyo ar ja gopon.",
   "Al-An'am 6:151", 1},
  {"يَا أَيُّهَا الَّذِينَ آمَنُوا اتَّقُوا اللَّهَ وَقُولُوا قَوْلًا سَدِيدًا",
   "O you who have believed, fear Allah and speak words of appropriate justice.",
   "He imandaro! Allah ke bhoyo koro ar sojha-sothik kotha bolo.",
   "Al-Ahzab 33:70", 1},
  {"وَقُلْ جَاءَ الْحَقُّ وَزَهَقَ الْبَاطِلُ ۚ إِنَّ الْبَاطِلَ كَانَ زَهُوقًا",
   "And say: Truth has come, and falsehood has departed. Indeed is falsehood ever bound to depart.",
   "Bolo: Sotyo eshe geche, mithya dur hoye geche. Nishchoi mithya dur hobar-i.",
   "Al-Isra 17:81", 1},
  {"فَإِنَّ مَعَ الْعُسْرِ يُسْرًا",
   "For indeed, with hardship [will be] ease.",
   "Nishchoi koshter sathe sohojota ache.",
   "Ash-Sharh 94:5", 1},
  {"وَاللَّهُ غَفُورٌ رَّحِيمٌ",
   "And Allah is Forgiving and Merciful.",
   "Allah khomashil, doyamoy.",
   "Al-Baqarah 2:218", 1},
  {"إِنَّ اللَّهَ يَأْمُرُ بِالْعَدْلِ وَالْإِحْسَانِ",
   "Indeed, Allah orders justice and good conduct.",
   "Nishchoi Allah bichar ar uttom achoron er adesh den.",
   "An-Nahl 16:90", 1},

  // ---- HADITH ----
  {"إِنَّمَا الأَعْمَالُ بِالنِّيَّاتِ",
   "Actions are judged by intentions.",
   "Niyot onujayi amol porigonyito hoy.",
   "Bukhari & Muslim", 2},
  {"الْمُسْلِمُ مَنْ سَلِمَ الْمُسْلِمُونَ مِنْ لِسَانِهِ وَيَدِهِ",
   "A Muslim is one from whose tongue and hand other Muslims are safe.",
   "Musolman shei byakti, jahar jibha ar hat theke onno musolman nirapod thake.",
   "Bukhari", 2},
  {"لَا يُؤْمِنُ أَحَدُكُمْ حَتَّى يُحِبَّ لِأَخِيهِ مَا يُحِبُّ لِنَفْسِهِ",
   "None of you truly believes until he loves for his brother what he loves for himself.",
   "Tomader keu prokrito mumin hobe na, jotokhon na se nijer jonno ja bhalobase ta nijer bhaiyer jonno bhalobase.",
   "Bukhari & Muslim", 2},
  {"مَنْ كَانَ يُؤْمِنُ بِاللَّهِ وَالْيَوْمِ الآخِرِ فَلْيَقُلْ خَيْرًا أَوْ لِيَصْمُتْ",
   "Whoever believes in Allah and the Last Day, let him speak good or remain silent.",
   "Je byakti Allah ar sesh dine iman rakhe, se jeno bhalo kotha bole othoba chup thake.",
   "Bukhari", 2},
  {"الدُّعَاءُ مُخُّ الْعِبَادَةِ",
   "Supplication is the essence of worship.",
   "Dua holo ibadat er morgo.",
   "Tirmidhi", 2},
  {"خَيْرُكُمْ مَنْ تَعَلَّمَ الْقُرْآنَ وَعَلَّمَهُ",
   "The best among you are those who learn the Quran and teach it.",
   "Tomader modhye shei byakti shrestho, je Quran shikhe ar onnoder shikhay.",
   "Bukhari", 2},
  {"اتَّقِ اللَّهَ حَيْثُمَا كُنْتَ",
   "Fear Allah wherever you are.",
   "Jekhanei thako, Allah ke bhoyo koro.",
   "Tirmidhi", 2},
  {"الطُّهُورُ شَطْرُ الإِيمَانِ",
   "Cleanliness is half of faith.",
   "Pobitrotai iman er ordhek.",
   "Muslim", 2},
  {"الْجَنَّةُ تَحْتَ أَقْدَامِ الأُمَّهَاتِ",
   "Paradise lies beneath the feet of mothers.",
   "Jannat ma er pa er niche.",
   "Nasa'i", 2},
  {"مَنْ لَا يَرْحَمُ لَا يُرْحَمُ",
   "Whoever shows no mercy will be shown no mercy.",
   "Je byakti doya kore na, takeo doya kora hoy na.",
   "Bukhari", 2},
  {"أَفْضَلُ الذِّكْرِ لَا إِلَهَ إِلَّا اللَّهُ",
   "The best remembrance is: There is no god but Allah.",
   "Shrestho zikir holo: La ilaha illallah.",
   "Tirmidhi", 2},
  {"الْمُؤْمِنُ لِلْمُؤْمِنِ كَالْبُنْيَانِ يَشُدُّ بَعْضُهُ بَعْضًا",
   "A believer to another believer is like a building whose parts support one another.",
   "Mumin onno mumin er jonno emon, jemon ekoti imarot - ek ongsho onno ongshoke shokti joge.",
   "Bukhari & Muslim", 2},
  {"مَنْ صَلَّى عَلَيَّ وَاحِدَةً صَلَّى اللَّهُ عَلَيْهِ عَشْرًا",
   "Whoever sends blessings upon me once, Allah will send blessings upon him ten times.",
   "Je byakti amar upor ekbar durood pore, Allah tar upor doshbar durood poren.",
   "Muslim", 2},

  // ---- PORAMORSO / TIPS ----
  {"", "Begin every task with Bismillah and end with Alhamdulillah.",
   "Prottek kaj Bismillah diye shuru koro ar Alhamdulillah diye shesh koro.", "", 3},
  {"", "Say Salam before speaking - it spreads love and peace.",
   "Kotha bolar age Salam dao - eta bhalobasha ar shanti choriye dey.", "", 3},
  {"", "Smiling at your brother is charity (Sadaqah).",
   "Tomar bhaiyer dike hese taka dekhano o sadakah.", "", 3},
  {"", "Read at least one page of the Quran every day.",
   "Prottek din kompokkhe ek pata Quran poro.", "", 3},
  {"", "Pray on time - the first question on the Day of Judgement is Salah.",
   "Somoye namaj poro - kiyamot er dine prothom proshno hobe namaj somporke.", "", 3},
  {"", "Forgive others so that Allah may forgive you.",
   "Onnoder khoma koro, jate Allah tomake khoma koren.", "", 3},
  {"", "Control your tongue - silence is better than useless talk.",
   "Jibha songjoto rakho - nirbobota opokarok kothar cheye bhalo.", "", 3},
  {"", "Be kind to your parents - Jannah lies beneath their feet.",
   "Pita-matar sathe sadaoy bhalo byabohar koro - Jannat tader pa er niche.", "", 3},
  {"", "Give charity daily - even a date or a glass of water.",
   "Prottek din sadakah dao - ekoti khejur othoba ek glass pani o hole.", "", 3},
  {"", "Seek knowledge from cradle to grave.",
   "Dol theke kobor porjonto gyan onusondhan koro.", "", 3},
  {"", "Say Alhamdulillah in every situation.",
   "Prottek obosthay Alhamdulillah bolo.", "", 3},
  {"", "Sleep in Wudu - angels make dua for you all night.",
   "Oju soho ghumiye poro - sararaat ferestara tomader jonno dua kore.", "", 3},
  {"", "Lower your gaze - it purifies the heart.",
   "Dristi nichu rakho - eta hridoy ke pobitro kore.", "", 3},
  {"", "Keep good company - a friend influences your deen.",
   "Bhalo songo rakho - bondhu tomar deen ke probhabito kore.", "", 3},
  {"", "Do not waste food - it is a blessing from Allah.",
   "Khabar nosto koro na - eta Allah er neamat.", "", 3},
  {"", "Say Bismillah before eating and Alhamdulillah after.",
   "Khawar age Bismillah ar pore Alhamdulillah bolo.", "", 3},
  {"", "Visit the sick - it brings Barakah in your life.",
   "Oshustho byaktike dekhote jao - eta tomader jibone borokot aniye dey.", "", 3},
  {"", "Repent daily - Allah loves those who turn to Him.",
   "Prottek din toba koro - Allah tader bhalobasen, jara tar dike phire ashe.", "", 3},
  {"", "Be patient in trials - they erase your sins.",
   "Bipode dhairjo dhoron koro - eta tomader gonah moche dey.", "", 3},
  {"", "Remember death often - it keeps you on the right path.",
   "Mrittur kotha proshongsha koro - eta tomake sojha pothe rakhe.", "", 3},
};
const int ISLAMIC_COUNT = sizeof(ISLAMIC_DB) / sizeof(ISLAMIC_DB[0]);

int islamicIdx = 0;
int islamicLastDay = -1;
uint8_t islamicCat = 0;
uint8_t islamicLang = 0;   // 0 = English, 1 = Bangla (Banglish)

const char* ISLAMIC_CATS[4] = {"All", "Ayat", "Hadith", "Tip"};
const uint16_t ISLAMIC_CAT_COLS[4] = {C565(120,180,255), C565(0,200,255), C565(70,225,140), C565(255,90,200)};

// >>> FIX: takes int index (not const IslamicItem&) so the Arduino
//     auto-generated prototype never references IslamicItem.
const char* islamicText(int idx) {
  const IslamicItem &it = ISLAMIC_DB[idx];
  if (islamicLang == 1 && it.bangla[0]) return it.bangla;
  return it.translation;
}

int islamicNext(int cur, int dir, uint8_t cat) {
  for (int i = 1; i <= ISLAMIC_COUNT; i++) {
    int n = (cur + dir * i + ISLAMIC_COUNT * 2) % ISLAMIC_COUNT;
    if (cat == 0 || ISLAMIC_DB[n].category == cat) return n;
  }
  return cur;
}

void islamicAutoPick() {
  time_t n = time(nullptr);
  struct tm tmv;
  localtime_r(&n, &tmv);
  int doy = tmv.tm_yday;
  if (doy != islamicLastDay) {
    islamicLastDay = doy;
    islamicIdx = (int)((doy * 7 + tmv.tm_year) % ISLAMIC_COUNT);
    if (islamicIdx < 0) islamicIdx = 0;
  }
}

void islamicDraw() {
  islamicAutoPick();
  tft.fillScreen(TH.bg);

  tft.fillRect(0, 0, SW, BAR, TH.panel);
  tft.fillRoundRect(4, 3, 42, 20, 6, TH.accent2);
  tft.fillTriangle(16, 13, 25, 7, 25, 19, TH.onacc);
  tft.fillRect(25, 11, 10, 5, TH.onacc);
  txtp("Daily Islamic", 54, BAR / 2 + 1, 2, TH.text, TH.panel, ML_DATUM, 1, 0);

  uint16_t lc = islamicLang ? TH.accent2 : TH.accent;
  tft.fillRoundRect(SW - 42, 4, 36, 18, 5, lc);
  txt(islamicLang ? "BN" : "EN", SW - 24, 13, 1, TH.onacc, lc, MC_DATUM);

  int tabY = BAR + 3, tabH = 22, tabW = 74;
  for (int i = 0; i < 4; i++) {
    int tx = 6 + i * (tabW + 3);
    bool active = (islamicCat == i);
    uint16_t fill = active ? ISLAMIC_CAT_COLS[i] : TH.panel;
    uint16_t fg   = active ? TH.onacc : TH.dim;
    tft.fillRoundRect(tx, tabY, tabW, tabH, 7, fill);
    txt(ISLAMIC_CATS[i], tx + tabW / 2, tabY + tabH / 2 + 1, 2, fg, fill, MC_DATUM);
  }

  const IslamicItem &it = ISLAMIC_DB[islamicIdx];
  int cardY = tabY + tabH + 4;
  int cardH = SH - cardY - 34;
  tft.fillRoundRect(8, cardY, SW - 16, cardH, 12, TH.panel);

  uint16_t kc = ISLAMIC_CAT_COLS[it.category];
  const char* kname = ISLAMIC_CATS[it.category];
  tft.fillRoundRect(14, cardY + 4, 56, 16, 5, kc);
  txt(kname, 14 + 28, cardY + 4 + 8, 1, TH.onacc, kc, MC_DATUM);

  int ty;
  if (it.arabic[0]) {
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TH.accent, TH.panel);
    tft.setTextSize(2);
    tft.drawString(it.arabic, SW / 2, cardY + 36, 4);
    tft.setTextSize(1);
    tft.drawFastHLine(30, cardY + 60, SW - 60, TH.dim);
    ty = cardY + 68;
  } else {
    ty = cardY + 26;
  }

  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TH.text, TH.panel);
  tft.setTextSize(1);
  String text = String(islamicText(islamicIdx));   // <<< FIX: index, not struct
  int lineStart = 0;
  int maxW = SW - 40;
  while (lineStart < (int)text.length()) {
    int lineEnd = lineStart;
    int lastSpace = -1;
    while (lineEnd < (int)text.length()) {
      if (text[lineEnd] == ' ') lastSpace = lineEnd;
      String probe = text.substring(lineStart, lineEnd + 1);
      if (tft.textWidth(probe, 2) > maxW) break;
      lineEnd++;
    }
    if (lineEnd >= (int)text.length()) lineEnd = text.length();
    else if (lastSpace > lineStart) lineEnd = lastSpace;
    tft.drawString(text.substring(lineStart, lineEnd), 20, ty, 2);
    ty += 18;
    lineStart = lineEnd;
    while (lineStart < (int)text.length() && text[lineStart] == ' ') lineStart++;
    if (ty > cardY + cardH - 20) break;
  }

  if (it.ref[0]) {
    tft.setTextDatum(BR_DATUM);
    tft.setTextColor(TH.accent, TH.panel);
    tft.setTextSize(1);
    tft.drawString(String("- ") + it.ref, SW - 20, cardY + cardH - 8, 2);
  }

  button(8,   SH - 30, 76, 26, "Prev",   TH.panel,  TH.text,  2);
  button(88,  SH - 30, 76, 26, "Next",   TH.accent, TH.onacc, 2);
  button(168, SH - 30, 76, 26, "Random", TH.accent2,TH.onacc, 2);
  button(248, SH - 30, 64, 26, islamicLang ? "BN" : "EN", TH.good, TH.onacc, 2);
}

void islamicInit() {
  int old = islamicIdx;
  int tries = 0;
  do { islamicIdx = random(ISLAMIC_COUNT); tries++; }
  while (islamicIdx == old && ISLAMIC_COUNT > 1 && tries < 20);
  islamicLastDay = -1;
  islamicDraw();
}

void islamicLoop() {
  if (!tc.press) return;

  int tabY = BAR + 3, tabH = 22, tabW = 74;
  for (int i = 0; i < 4; i++) {
    int tx = 6 + i * (tabW + 3);
    if (pressIn(tx, tabY, tabW, tabH)) {
      islamicCat = i;
      beep(1400, 20);
      if (islamicCat != 0 && ISLAMIC_DB[islamicIdx].category != islamicCat) {
        for (int j = 0; j < ISLAMIC_COUNT; j++) {
          if (ISLAMIC_DB[j].category == islamicCat) { islamicIdx = j; break; }
        }
      }
      islamicDraw();
      return;
    }
  }

  if (pressIn(8, SH - 30, 76, 26)) {
    islamicIdx = islamicNext(islamicIdx, -1, islamicCat);
    beep(1200, 20); islamicDraw();
  } else if (pressIn(88, SH - 30, 76, 26)) {
    islamicIdx = islamicNext(islamicIdx, 1, islamicCat);
    beep(1400, 20); islamicDraw();
  } else if (pressIn(168, SH - 30, 76, 26)) {
    int tries = 0, n = islamicIdx;
    do { n = random(ISLAMIC_COUNT); tries++; }
    while (islamicCat != 0 && ISLAMIC_DB[n].category != islamicCat && tries < 50);
    islamicIdx = n;
    beep(1700, 20); islamicDraw();
  } else if (pressIn(248, SH - 30, 64, 26)) {
    islamicLang = !islamicLang;
    beep(1500, 20); islamicDraw();
  }
}

