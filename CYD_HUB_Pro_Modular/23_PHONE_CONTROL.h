// CYD HUB Pro modular section.
// Included by CYD_HUB_Pro_Modular.ino; do not compile this file alone.

// ============================ PHONE CONTROL =================================
WebServer server(80);
const char* SCREEN_NAMES[11] = {"Home", "Clock", "Timer", "Weather", "Games", "Settings", "Game", "Wi-Fi Setup", "Islamic", "Alarms", "Knowledge"};

String jsonEscape(const String &in) {
  String out;
  out.reserve(in.length() + 8);
  for (size_t i = 0; i < in.length(); i++) {
    char c = in[i];
    if (c == '\\') out += "\\\\";
    else if (c == '"') out += "\\\"";
    else if (c == '\n') out += "\\n";
    else if (c == '\r') out += "\\r";
    else out += c;
  }
  return out;
}

void wsRefreshTimerUI() {
  if (screen != SCR_TIMER) return;
  tmLastA = -1; tmLastB = -1;
  tmDrawMain(); tmDrawSub(); tmDrawButtons();
}

void handleWebRoot() {
  String html = R"rawliteral(
<!doctype html><html><head>
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#0b1020">
<title>CYD HUB Pro</title>
<link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;600;800&display=swap" rel="stylesheet">
<style>
:root{--bg:#070b14;--card:#101827;--card2:#151f31;--line:#26344a;--text:#f5f7fb;--muted:#91a0b7;--accent:#29c7ff;--accent2:#b070ff;--good:#36df8a;--bad:#ff5d6c;--gold:#f7c948}
*{box-sizing:border-box}body{margin:0;background:radial-gradient(1200px 600px at 20% -10%,#101c3a 0%,#070b14 60%),#070b14;color:var(--text);font-family:Inter,system-ui,sans-serif;min-height:100vh}
.wrap{max-width:1100px;margin:auto;padding:16px}
.hero{background:linear-gradient(135deg,rgba(41,199,255,.15),rgba(176,112,255,.15));border:1px solid var(--line);border-radius:22px;padding:20px;margin-bottom:16px}
.hero-top{display:flex;align-items:center;justify-content:space-between;gap:12px;flex-wrap:wrap}
.brand{font-size:26px;font-weight:800;background:linear-gradient(90deg,var(--accent),var(--accent2));-webkit-background-clip:text;background-clip:text;color:transparent}
.sub{color:var(--muted);font-size:12px;margin-top:4px}
.pill{padding:10px 14px;border:1px solid var(--line);background:rgba(13,21,35,.85);border-radius:999px;font-size:12px;font-weight:600}
.hero-nav{display:flex;flex-wrap:wrap;gap:8px;margin-top:14px}
.hero-nav button{padding:10px 14px;font-size:13px}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(340px,1fr));gap:14px}
.card{background:linear-gradient(180deg,rgba(21,31,49,.95),rgba(16,24,39,.95));border:1px solid var(--line);border-radius:20px;padding:16px;box-shadow:0 18px 40px rgba(0,0,0,.35);position:relative;overflow:hidden}
.card::before{content:"";position:absolute;top:0;left:0;right:0;height:2px;background:linear-gradient(90deg,var(--accent),var(--accent2))}
.card h2{font-weight:800;font-size:15px;margin:0 0 4px;display:flex;align-items:center;gap:8px;color:var(--text)}
.card .subh{font-size:11px;color:var(--muted);margin-bottom:12px;font-weight:500}
.icon{width:22px;height:22px;border-radius:6px;background:linear-gradient(135deg,var(--accent),var(--accent2));display:inline-flex;align-items:center;justify-content:center;font-size:12px}
.muted{color:var(--muted);font-size:12px}
.row{display:flex;flex-wrap:wrap;gap:8px;margin:8px 0}
button,select,input{font:inherit}
button{border:1px solid var(--line);background:var(--card2);color:var(--text);border-radius:12px;padding:10px 14px;cursor:pointer;transition:.15s;font-weight:600}
button:hover{border-color:var(--accent);transform:translateY(-1px)}
button.primary{background:linear-gradient(135deg,var(--accent),#1e8fd0);color:#04101a;border-color:transparent;font-weight:800}
button.primary2{background:linear-gradient(135deg,var(--accent2),#7d44d6);color:#fff;border-color:transparent;font-weight:800}
button.good{background:linear-gradient(135deg,#36df8a,#159f61);color:#04120a;border-color:transparent;font-weight:800}
button.danger{background:linear-gradient(135deg,#ff5d6c,#c2323f);color:#fff;border-color:transparent;font-weight:800}
button.state{min-width:150px}
.on{border-color:#2e9d68!important;background:rgba(18,59,42,.9)!important;color:#7affb1!important}
.off{border-color:#6e3945!important;background:rgba(38,21,27,.9)!important;color:#ff9aa6!important}
.input,select{width:100%;padding:10px 12px;background:#0b1320;border:1px solid var(--line);border-radius:12px;color:var(--text);outline:none}
.input:focus,select:focus{border-color:var(--accent)}
.two{display:grid;grid-template-columns:1fr 1fr;gap:8px}
.status{padding:12px 14px;background:rgba(11,19,32,.85);border:1px solid var(--line);border-radius:12px;font-size:12px;color:var(--muted);margin-top:10px;line-height:1.5}
.network{display:flex;align-items:center;justify-content:space-between;gap:10px;padding:12px;border:1px solid var(--line);border-radius:14px;margin-top:8px;background:rgba(12,20,33,.85)}
.network b{font-size:13px;display:block}
.netmeta{font-size:11px;color:var(--muted);margin-top:3px}
.range{width:100%;accent-color:var(--accent)}
.games button{flex:1 1 105px;font-size:12px;padding:8px 10px}
.badge{display:inline-block;font-size:10px;padding:3px 8px;border-radius:6px;background:rgba(41,199,255,.15);color:var(--accent);font-weight:700;margin-left:6px}
.badge.gold{background:rgba(247,201,72,.15);color:var(--gold)}
.badge.green{background:rgba(54,223,138,.15);color:var(--good)}
.badge.pink{background:rgba(255,90,200,.15);color:#ff5ac8}
.islam-grid{display:grid;grid-template-columns:repeat(3,1fr);gap:6px;margin:8px 0}
.islam-cat{padding:10px;border-radius:12px;border:1px solid var(--line);background:var(--card2);color:var(--text);font-weight:700;font-size:12px;cursor:pointer;text-align:center}
.islam-cat.ayat{background:rgba(41,199,255,.15);color:var(--accent);border-color:rgba(41,199,255,.4)}
.islam-cat.hadith{background:rgba(54,223,138,.15);color:var(--good);border-color:rgba(54,223,138,.4)}
.islam-cat.tip{background:rgba(255,90,200,.15);color:#ff5ac8;border-color:rgba(255,90,200,.4)}
.toast{position:fixed;left:50%;bottom:24px;transform:translate(-50%,20px);background:linear-gradient(180deg,#101827,#0b1320);border:1px solid var(--line);padding:12px 18px;border-radius:14px;opacity:0;pointer-events:none;transition:.25s;z-index:5;font-weight:600}
.toast.show{opacity:1;transform:translate(-50%,0)}
.section-title{font-size:11px;font-weight:800;letter-spacing:.8px;color:var(--muted);text-transform:uppercase;margin:20px 0 10px;padding-left:4px}
@media(max-width:520px){.wrap{padding:11px}.brand{font-size:20px}.two{grid-template-columns:1fr}}
</style></head><body><div class="wrap">

<div class="hero">
  <div class="hero-top">
    <div>
      <div class="brand">CYD HUB Pro Control Center</div>
      <div class="sub">Wi-Fi | Display | Weather | Islamic | Alarm | Games | Timer</div>
    </div>
    <div id="connection" class="pill">Connecting...</div>
  </div>
  <div class="hero-nav">
    <button onclick="go('home')">Home</button>
    <button onclick="go('clock')">Clock</button>
    <button onclick="go('timer')">Timer</button>
    <button onclick="go('alarm')">Alarm</button>
    <button onclick="go('weather')">Weather</button>
    <button onclick="go('islamic')">Islamic</button>
    <button onclick="go('games')">Games</button>
    <button onclick="go('settings')">Settings</button><button onclick="go('knowledge')">Knowledge</button>
    <button class="primary" onclick="go('setup')">Wi-Fi Setup</button>
  </div>
</div>

<div class="section-title">Device Status</div>
<div class="card">
  <h2><span class="icon">i</span> Live Device Status</h2>
  <div class="subh">Real-time information from your CYD display</div>
  <div id="device" class="status">Loading device status...</div>
</div>

<div class="section-title">Wi-Fi & Network</div>
<div class="grid">
  <section class="card">
    <h2><span class="icon">W</span> Wi-Fi Scanner & Connection</h2>
    <div class="subh">Scan nearby networks and connect the CYD</div>
    <div class="row"><button class="primary" onclick="scanWiFi()">Scan Networks</button></div>
    <div id="wifiList"><div class="status">Tap Scan to see nearby Wi-Fi.</div></div>
    <div class="status">
      <b>Control AP:</b> <span id="apInfo">-</span><br>
      <b>Home Wi-Fi:</b> <span id="staInfo">-</span><br>
      <b>Signal:</b> <span id="rssi">-</span>
    </div>
  </section>

  <section class="card">
    <h2><span class="icon">L</span> Live Settings <span class="badge" id="themeBadge">-</span></h2>
    <div class="subh">Quick toggles for common preferences</div>
    <div class="row">
      <button id="h24Btn" class="state" onclick="act('toggle','h24')">24-hour: --</button>
      <button id="soundBtn" class="state" onclick="act('toggle','sound')">Sound: --</button>
      <button id="fahrBtn" class="state" onclick="act('toggle','fahr')">Temp: --</button>
    </div>
    <div class="status" id="stateText">Loading...</div>
  </section>
</div>

<div class="section-title">Display & Appearance</div>
<div class="grid">
  <section class="card">
    <h2><span class="icon">D</span> Display Settings</h2>
    <div class="subh">Theme, brightness and clock style</div>
    <div class="row">
      <button onclick="act('theme','prev')">Prev Theme</button>
      <button class="primary2" onclick="act('theme','next')">Next Theme</button>
    </div>
    <div class="muted" style="margin-top:10px">Brightness: <span id="brightVal">-</span></div>
    <input class="range" type="range" min="10" max="255" id="bright" oninput="setBright(this.value)">
    <div class="two" style="margin-top:10px">
      <div><label class="muted">Clock Style</label>
        <select id="clockStyle" onchange="setClockStyle(this.value)"><option value="0">0: Digital</option><option value="1">1: Analog</option><option value="2">2: Big Digital</option><option value="3">3: Minimal</option><option value="4">4: Ring</option><option value="5">5: Neon</option><option value="6">6: Dashboard</option><option value="7">7: Split</option><option value="8">8: Clean</option><option value="9">9: Seconds</option><option value="10">10: Gear Train</option><option value="11">11: Pendulum</option><option value="12">12: Roman</option><option value="13">13: Moon Phase</option><option value="14">14: Compass</option><option value="15">15: Industrial</option><option value="16">16: Concentric</option><option value="17">17: Binary LED</option><option value="18">18: Sun Dial</option><option value="19">19: Balance</option></select>
      </div>
      <div><label class="muted">Temperature</label>
        <select id="tempUnit" onchange="setTemp(this.value)">
          <option value="C">Celsius</option><option value="F">Fahrenheit</option>
        </select>
      </div>
    </div>
  </section>
</div>

<div class="section-title">Alarm System</div>
<div class="card">
  <h2><span class="icon">A</span> Alarms <span class="badge" id="alarmBadge">Ready</span></h2>
  <div class="subh">Set up to 3 daily alarms. Times are local to the device.</div>
  <div id="alarmList"></div>
  <div class="row">
    <button class="danger" onclick="act('alarm','stop')">Stop Ringing</button>
    <button class="primary2" onclick="act('alarm','snooze')">Snooze 5m</button>
  </div>
</div>

<div class="section-title">Islamic Content</div>
<div class="card">
  <h2><span class="icon">I</span> Daily Islamic <span class="badge gold">Ayat | Hadith | Tip</span></h2>
  <div class="subh">Quranic Ayat, Hadith, and Poramorso. English / Bangla toggle on device.</div>
  <div class="islam-grid">
    <div class="islam-cat ayat" onclick="act('islamic','cat_ayat')">Quran Ayat</div>
    <div class="islam-cat hadith" onclick="act('islamic','cat_hadith')">Hadith</div>
    <div class="islam-cat tip" onclick="act('islamic','cat_tip')">Poramorso</div>
  </div>
  <div class="row">
    <button class="primary2" onclick="act('islamic','next')">Next Item</button>
    <button onclick="act('islamic','prev')">Previous</button>
    <button onclick="act('islamic','random')">Random</button>
    <button class="primary" onclick="act('islamic','cat_all')">Show All</button>
    <button class="good" onclick="go('islamic')">Show on CYD</button>
  </div>
  <div class="status" id="islamicInfo">Tap a category to filter.</div>
</div>

<div class="section-title">Time & Weather</div>
<div class="grid">
  <section class="card">
    <h2><span class="icon">T</span> Time Control</h2>
    <div class="subh">Works with NTP when online, and manual time from this Control AP when offline.</div>
    <div style="margin-bottom:10px">
      <label class="muted">Quick country / timezone preset</label>
      <select id="countryPreset" onchange="applyCountryPreset(this.value)">
        <option value="">Choose preset...</option>
        <option value="dhaka">Bangladesh - Dhaka (UTC+6)</option>
        <option value="kolkata">India - Kolkata (UTC+5:30)</option>
        <option value="karachi">Pakistan - Karachi (UTC+5)</option>
        <option value="kathmandu">Nepal - Kathmandu (UTC+5:45)</option>
        <option value="dubai">UAE - Dubai (UTC+4)</option>
        <option value="riyadh">Saudi Arabia - Riyadh (UTC+3)</option>
        <option value="singapore">Singapore (UTC+8)</option>
        <option value="tokyo">Japan - Tokyo (UTC+9)</option>
        <option value="shanghai">China - Shanghai (UTC+8)</option>
      </select>
    </div>
    <div class="two">
      <div><label class="muted">Format</label>
        <select id="timeFormat" onchange="setTimeFormat(this.value)">
          <option value="12">12-hour</option><option value="24">24-hour</option>
        </select>
      </div>
      <div><label class="muted">Manual Time</label>
        <input id="manualTime" class="input" type="datetime-local">
      </div>
    </div>
    <div class="row">
      <button class="primary" onclick="setManualTime()">Set Time</button>
      <button onclick="syncNtp()">Sync NTP</button>
    </div>
    <div class="status" id="timeStatus">NTP sync uses the saved UTC offset.</div>
  </section>

  <section class="card">
    <h2><span class="icon">Wx</span> Easy Weather Location</h2>
    <div class="subh">Pick a place, or type a city and let Open-Meteo find its coordinates. Wi-Fi is only needed to fetch live weather.</div>
    <label class="muted">Quick location</label>
    <select id="weatherPreset" onchange="applyWeatherPreset(this.value)" style="margin-top:6px">
      <option value="">Choose a city...</option>
      <option value="dhaka">Dhaka, Bangladesh</option><option value="rajshahi">Rajshahi, Bangladesh</option>
      <option value="chittagong">Chattogram, Bangladesh</option><option value="sylhet">Sylhet, Bangladesh</option>
      <option value="kolkata">Kolkata, India</option><option value="karachi">Karachi, Pakistan</option>
      <option value="kathmandu">Kathmandu, Nepal</option><option value="dubai">Dubai, UAE</option>
      <option value="riyadh">Riyadh, Saudi Arabia</option><option value="singapore">Singapore</option>
      <option value="tokyo">Tokyo, Japan</option><option value="shanghai">Shanghai, China</option>
    </select>
    <div class="two" style="margin-top:8px"><input id="citySearch" class="input" placeholder="Type city name"><button class="primary" onclick="findCity()">Find City</button></div>
    <input id="city" class="input" style="margin-top:8px" placeholder="Selected city">
    <div class="two" style="margin-top:8px"><input id="lat" class="input" type="number" step="0.0001" placeholder="Latitude"><input id="lon" class="input" type="number" step="0.0001" placeholder="Longitude"></div>
    <div class="two" style="margin-top:8px"><input id="tzH" class="input" type="number" step="0.25" placeholder="UTC Hours"><input id="dst" class="input" type="number" value="0" placeholder="DST"></div>
    <div class="row"><button class="primary" onclick="saveLocation()">Save Location</button><button onclick="act('refresh','weather')">Refresh Weather</button></div>
    <div class="status" id="geoStatus">Select a preset or find a city.</div>
  </section>
</div>

<div class="section-title">Timer & Games</div>
<div class="grid">
  <section class="card">
    <h2><span class="icon">Tr</span> Timer + Stopwatch</h2>
    <div class="subh">Both are inside the same CYD time tool. Choose the mode here.</div>
    <div class="row"><button class="primary" onclick="act('timer','countdown')">Countdown</button><button class="primary2" onclick="act('timer','stopwatch')">Stopwatch</button></div>
    <div class="row"><button onclick="act('timer','-1m')">-1 min</button><button onclick="act('timer','+1m')">+1 min</button><button onclick="act('timer','-10s')">-10 sec</button><button onclick="act('timer','+10s')">+10 sec</button></div>
    <div class="row"><button class="good" onclick="act('timer','start')">Start / Pause</button><button onclick="act('timer','reset')">Reset</button></div>
  </section>

  <section class="card">
    <h2><span class="icon">G</span> Games Library</h2>
    <div class="subh">10 built-in games. Tap to launch on the CYD.</div>
    <div id="games" class="row games"></div>
  </section>
</div>

<div class="section-title">Device Actions</div>
<div class="card">
  <h2><span class="icon">A</span> System Actions</h2>
  <div class="subh">Refresh, disconnect and utility actions</div>
  <div class="row">
    <button class="primary" onclick="act('refresh','screen')">Refresh Display</button>
    <button onclick="act('refresh','weather')">Refresh Weather</button>
    <button onclick="act('time','sync')">Sync NTP</button>
    <button class="danger" onclick="act('wifi','disconnect')">Disconnect Wi-Fi</button>
  </div>
</div>

<div id="toast" class="toast"></div>

<script>
const games=['X & O','Snake','Memory','Whack','Reflex','2048','Bricks','Simon','Tetris','Pong'];
const ge=document.getElementById('games');
games.forEach((n,i)=>{const b=document.createElement('button');b.textContent=n;b.onclick=()=>act('game',i);ge.appendChild(b);});
function toast(m){const e=document.getElementById('toast');e.textContent=m;e.classList.add('show');setTimeout(()=>e.classList.remove('show'),2200);}
function go(s){fetch('/goto?screen='+encodeURIComponent(s)).then(r=>r.text()).then(()=>{toast('Screen changed');refresh();}).catch(()=>toast('Connection error'));}
function act(k,v){fetch('/act?k='+encodeURIComponent(k)+'&v='+encodeURIComponent(v)).then(r=>r.text()).then(x=>{toast(x||'Done');refresh();}).catch(()=>toast('Connection error'));}
function setBright(v){document.getElementById('brightVal').textContent=v;fetch('/act?k=bright&v='+v).catch(()=>toast('Connection error'));}
function setClockStyle(v){act('clock',v);}
function setTemp(v){act('temp',v);}
function setTimeFormat(v){act('format',v);}
function syncNtp(){act('time','sync');}
const countryPresets={
 dhaka:{city:'Dhaka',lat:23.8103,lon:90.4125,tz:6},
 kolkata:{city:'Kolkata',lat:22.5726,lon:88.3639,tz:5.5},
 karachi:{city:'Karachi',lat:24.8607,lon:67.0011,tz:5},
 kathmandu:{city:'Kathmandu',lat:27.7172,lon:85.3240,tz:5.75},
 dubai:{city:'Dubai',lat:25.2048,lon:55.2708,tz:4},
 riyadh:{city:'Riyadh',lat:24.7136,lon:46.6753,tz:3},
 singapore:{city:'Singapore',lat:1.3521,lon:103.8198,tz:8},
 tokyo:{city:'Tokyo',lat:35.6762,lon:139.6503,tz:9},
 shanghai:{city:'Shanghai',lat:31.2304,lon:121.4737,tz:8}
};
const weatherPresets={
 dhaka:{city:'Dhaka',lat:23.8103,lon:90.4125,tz:6},rajshahi:{city:'Rajshahi',lat:24.3745,lon:88.6042,tz:6},
 chittagong:{city:'Chattogram',lat:22.3569,lon:91.7832,tz:6},sylhet:{city:'Sylhet',lat:24.8949,lon:91.8687,tz:6},
 kolkata:{city:'Kolkata',lat:22.5726,lon:88.3639,tz:5.5},karachi:{city:'Karachi',lat:24.8607,lon:67.0011,tz:5},
 kathmandu:{city:'Kathmandu',lat:27.7172,lon:85.3240,tz:5.75},dubai:{city:'Dubai',lat:25.2048,lon:55.2708,tz:4},
 riyadh:{city:'Riyadh',lat:24.7136,lon:46.6753,tz:3},singapore:{city:'Singapore',lat:1.3521,lon:103.8198,tz:8},
 tokyo:{city:'Tokyo',lat:35.6762,lon:139.6503,tz:9},shanghai:{city:'Shanghai',lat:31.2304,lon:121.4737,tz:8}
};
function applyWeatherPreset(k){const p=weatherPresets[k];if(!p)return;document.getElementById('city').value=p.city;document.getElementById('lat').value=p.lat;document.getElementById('lon').value=p.lon;document.getElementById('tzH').value=p.tz;document.getElementById('geoStatus').textContent=p.city+' selected. Tap Save Location.';}
function findCity(){const q=document.getElementById('citySearch').value.trim();if(!q){toast('Type a city first');return;}document.getElementById('geoStatus').textContent='Searching...';fetch('/geo?q='+encodeURIComponent(q)).then(r=>r.json()).then(d=>{if(!d.ok){document.getElementById('geoStatus').textContent=d.message||'City not found';return;}document.getElementById('city').value=d.city;document.getElementById('lat').value=d.lat;document.getElementById('lon').value=d.lon;document.getElementById('tzH').value=d.tz;document.getElementById('geoStatus').textContent=d.city+' found. Tap Save Location.';}).catch(()=>document.getElementById('geoStatus').textContent='Search failed. Check Wi-Fi.');}

function applyCountryPreset(k){
 const p=countryPresets[k]; if(!p)return;
 document.getElementById('city').value=p.city;
 document.getElementById('lat').value=p.lat;
 document.getElementById('lon').value=p.lon;
 document.getElementById('tzH').value=p.tz;
 saveLocation();
}
function saveLocation(){
 const q='city='+encodeURIComponent(document.getElementById('city').value)+'&lat='+encodeURIComponent(document.getElementById('lat').value)+'&lon='+encodeURIComponent(document.getElementById('lon').value)+'&tz='+encodeURIComponent(document.getElementById('tzH').value)+'&dst='+encodeURIComponent(document.getElementById('dst').value||0);
 fetch('/act?k=location&'+q).then(r=>r.text()).then(x=>{toast(x);refresh();}).catch(()=>toast('Location save failed'));
}
function setManualTime(){
 const v=document.getElementById('manualTime').value;
 if(!v){toast('Select date and time first');return;}
 fetch('/act?k=timeSet&v='+encodeURIComponent(v)).then(r=>r.text()).then(x=>{toast(x);refresh();});
}
function setState(id,on,onText,offText){const e=document.getElementById(id);e.textContent=on?onText:offText;e.classList.toggle('on',on);e.classList.toggle('off',!on);}
function alarmDaysLabel(d){return d===127?'Every day':d===62?'Weekdays':d===65?'Weekends':'Custom';}
let alarmUiReady=false;
let lastAlarmJson='';
function buildAlarmUi(){
 const box=document.getElementById('alarmList');
 if(!box||alarmUiReady)return;
 box.innerHTML='';
 for(let i=0;i<3;i++){
  const row=document.createElement('div'); row.className='network'; row.id='alarmRow'+i;
  const left=document.createElement('div');
  const title=document.createElement('b'); title.textContent='Alarm '+(i+1);
  const meta=document.createElement('div'); meta.className='netmeta'; meta.id='alarmDays'+i;
  const inp=document.createElement('input');
  inp.type='time'; inp.className='input'; inp.id='alarmTime'+i;
  inp.style.width='120px'; inp.style.marginTop='6px';
  inp.onchange=()=>saveAlarmTime(i);
  left.appendChild(title); left.appendChild(meta); left.appendChild(inp);
  const right=document.createElement('div');
  const b=document.createElement('button'); b.id='alarmToggle'+i;
  b.onclick=()=>fetch('/act?k=alarm&v=toggle&i='+i).then(()=>refresh());
  right.appendChild(b);
  row.appendChild(left); row.appendChild(right); box.appendChild(row);
 }
 alarmUiReady=true;
}
function saveAlarmTime(i){
 const inp=document.getElementById('alarmTime'+i);
 const p=(inp.value||'').split(':'); if(p.length!==2)return;
 const days=Number(inp.dataset.days||127), enabled=inp.dataset.enabled==='1';
 fetch('/act?k=alarm&v=set&i='+i+'&h='+encodeURIComponent(p[0])+'&m='+encodeURIComponent(p[1])+'&d='+days+'&e='+(enabled?1:0))
  .then(r=>r.text()).then(x=>{toast(x);refresh();}).catch(()=>toast('Alarm save failed'));
}
function renderAlarms(list){
 if(!list)return;
 buildAlarmUi();
 const sig=JSON.stringify(list);
 if(sig===lastAlarmJson)return;
 list.forEach((a,i)=>{
  const inp=document.getElementById('alarmTime'+i);
  const meta=document.getElementById('alarmDays'+i);
  const b=document.getElementById('alarmToggle'+i);
  meta.textContent=alarmDaysLabel(a.d);
  inp.dataset.days=a.d; inp.dataset.enabled=a.e?'1':'0';
  // Never overwrite a time field while the user is editing/opening its picker.
  if(document.activeElement!==inp){
    const v=String(a.h).padStart(2,'0')+':'+String(a.m).padStart(2,'0');
    if(inp.value!==v) inp.value=v;
  }
  b.textContent=a.e?'ON':'OFF'; b.className=a.e?'good':'';
 });
 lastAlarmJson=sig;
}
function safeValue(id,value){
 const e=document.getElementById(id);
 if(e && document.activeElement!==e) e.value=value;
}
function refresh(){fetch('/status').then(r=>r.json()).then(d=>{
 document.getElementById('connection').textContent=d.sta?'Home Wi-Fi Connected':'Control AP Active';
 document.getElementById('connection').style.color=d.sta?'#36df8a':'#29c7ff';
 document.getElementById('device').innerHTML='<b>Screen:</b> '+d.screen+' &nbsp; <b>Time:</b> '+d.time+'<br><b>Location:</b> '+escapeHtml(d.city)+' &nbsp; <b>TZ:</b> UTC'+escapeHtml(d.tz)+'<br><b>Brightness:</b> '+d.bright+' &nbsp; <b>Clock:</b> '+escapeHtml(d.clockStyle);
 document.getElementById('apInfo').textContent=d.ap+' / '+d.apip;
 document.getElementById('staInfo').textContent=d.sta?(d.ssid+' / '+d.staip):'Not connected';
 document.getElementById('rssi').textContent=d.sta?(d.rssi+' dBm'):'-';
 document.getElementById('bright').value=d.bright;document.getElementById('brightVal').textContent=d.bright;
 document.getElementById('clockStyle').value=d.clockIndex;document.getElementById('tempUnit').value=d.fahr?'F':'C';document.getElementById('timeFormat').value=d.h24?'24':'12';
 safeValue('city',d.city);safeValue('lat',d.lat);safeValue('lon',d.lon);safeValue('tzH',d.tzHours);
 setState('h24Btn',d.h24,'24-hour: ON','24-hour: OFF');setState('soundBtn',d.sound,'Sound: ON','Sound: OFF');setState('fahrBtn',d.fahr,'Fahrenheit: ON','Celsius: ON');
 document.getElementById('themeBadge').textContent='Theme: '+d.theme;
 document.getElementById('stateText').textContent='Clock: '+d.clockStyle+' | Wi-Fi: '+(d.sta?'ON':'OFF')+' | Weather: '+(!d.sta?'OFFLINE':(d.weather?'ACTIVE':'WAITING'))+' | Islamic: '+(d.islamicCat||'All');
 document.getElementById('islamicInfo').textContent='Category: '+(d.islamicCat||'All')+' | Current: '+escapeHtml(d.islamicRef||'-');
 if(d.alarms) renderAlarms(d.alarms);
 document.getElementById('alarmBadge').textContent=d.alarmRinging?'RINGING!':'Ready';
 document.getElementById('alarmBadge').style.color=d.alarmRinging?'#ff5d6c':'';
}).catch(()=>{document.getElementById('connection').textContent='Offline';});}
function scanWiFi(){const box=document.getElementById('wifiList');box.innerHTML='<div class="status">Scanning...</div>';fetch('/wifi/scan').then(r=>r.json()).then(d=>{
 if(!d.networks||!d.networks.length){box.innerHTML='<div class="status">No networks found.</div>';return;}
 box.innerHTML='';d.networks.forEach(n=>{const row=document.createElement('div');row.className='network';
 const left=document.createElement('div');left.innerHTML='<b>'+escapeHtml(n.ssid||'(Hidden)')+'</b><div class="netmeta">'+n.rssi+' dBm - '+(n.secure?'Secured':'Open')+'</div>';
 const actions=document.createElement('div');const b=document.createElement('button');b.textContent='Connect';b.className='primary';b.onclick=()=>connectWiFi(n.ssid,n.secure);actions.appendChild(b);row.appendChild(left);row.appendChild(actions);box.appendChild(row);});
 toast(d.networks.length+' networks found');}).catch(()=>{box.innerHTML='<div class="status">Scan failed.</div>';});}
function connectWiFi(ssid,secure){const pass=secure?prompt('Password for '+ssid):'';if(pass===null)return;
 const body='ssid='+encodeURIComponent(ssid)+'&pass='+encodeURIComponent(pass);fetch('/wifi/connect',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:body}).then(r=>r.text()).then(x=>{toast(x);setTimeout(refresh,1500);}).catch(()=>toast('Connection failed'));}
function escapeHtml(s){return String(s).replace(/[&<>'"]/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;',"'":'&#39;','"':'&quot;'}[c]));}
refresh();setInterval(refresh,2500);
</script></body></html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleWebGoto() {
  String s = server.arg("screen");
  int target = -1;
  if (s == "home") target = SCR_HOME;
  else if (s == "clock") target = SCR_CLOCK;
  else if (s == "timer") target = SCR_TIMER;
  else if (s == "weather") target = SCR_WEATHER;
  else if (s == "games") target = SCR_GAMES;
  else if (s == "settings") target = SCR_SETTINGS;
  else if (s == "setup") target = SCR_SETUP;
  else if (s == "islamic") target = SCR_ISLAMIC;
  else if (s == "alarm") target = SCR_ALARM;
  else if (s == "knowledge") target = SCR_KNOWLEDGE;
  if (target >= 0) { flushHi(); goScreen(target); }
  server.send(200, "text/plain", target >= 0 ? "Navigation updated" : "Unknown screen");
}

long daysFromCivil(int y, unsigned m, unsigned d) {
  y -= m <= 2;
  const long era = (y >= 0 ? y : y - 399) / 400;
  const unsigned yoe = (unsigned)(y - era * 400);
  const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
  return era * 146097L + (long)doe - 719468L;
}
bool setManualLocalTime(const String &v) {
  if (v.length() < 16) return false;
  int y = v.substring(0,4).toInt(), mo = v.substring(5,7).toInt();
  int d = v.substring(8,10).toInt(), h = v.substring(11,13).toInt();
  int mi = v.substring(14,16).toInt(), s = v.length() >= 19 ? v.substring(17,19).toInt() : 0;
  if (y < 2020 || mo < 1 || mo > 12 || d < 1 || d > 31 || h > 23 || mi > 59 || s > 59) return false;
  time_t epoch = (time_t)(daysFromCivil(y, mo, d) * 86400L + h * 3600L + mi * 60L + s - savedTzOffset());
  struct timeval tv = { epoch, 0 };
  settimeofday(&tv, nullptr);
  return true;
}
void applyTimeSettings() {
  configTime(savedTzOffset(), savedDstOffset(), "pool.ntp.org", "time.google.com", "time.cloudflare.com");
  ntpStarted = true;
}

const char* islamicCatName(uint8_t c) {
  switch (c) { case 1: return "Ayat"; case 2: return "Hadith"; case 3: return "Tip"; default: return "All"; }
}

void handleWebAct() {
  String k = server.arg("k");
  String v = server.arg("v");
  String msg = "Command completed";

  if (k == "game") {
    int idx = v.toInt();
    if (idx >= 0 && idx < 10) { gameStart(idx); msg = "Game started"; }
  } else if (k == "timer") {
    if (v == "countdown") { if(swRun){swMs += millis()-swLast; swRun=false;} tmTab=0; cdRun=false; if(screen==SCR_TIMER) timerInit(); msg="Countdown mode"; wsRefreshTimerUI(); return; }
    if (v == "stopwatch") { cdRun=false; tmTab=1; if(screen==SCR_TIMER) timerInit(); msg="Stopwatch mode"; wsRefreshTimerUI(); return; }
    if (v == "start" && tmTab==1) { if(swRun){swMs += millis()-swLast; swRun=false;} else {swLast=millis(); swRun=true;} msg=swRun?"Stopwatch started":"Stopwatch paused"; wsRefreshTimerUI(); return; }
    if (v == "reset" && tmTab==1) { swRun=false; swMs=0; msg="Stopwatch reset"; wsRefreshTimerUI(); return; }
    int delta = 0;
    if (v == "-1m") delta = -60;
    else if (v == "+1m") delta = 60;
    else if (v == "-10s") delta = -10;
    else if (v == "+10s") delta = 10;
    if (delta && tmTab==0 && !cdRun && !cdDone) {
      int t = (int)(cdTotal / 1000) + delta;
      t = constrain(t, 10, 5990);
      cdTotal = (uint32_t)t * 1000UL; cdLeft = cdTotal; msg = "Timer updated";
    } else if (v == "start") {
      if (cdDone) { cdDone = false; cdLeft = cdTotal; }
      else if (cdRun) cdRun = false;
      else { if (cdLeft == 0) cdLeft = cdTotal; cdRun = true; cdLast = millis(); }
      msg = cdRun ? "Timer started" : "Timer paused";
    } else if (v == "reset") {
      cdRun = false; cdDone = false; cdLeft = cdTotal; msg = "Timer reset";
    }
    wsRefreshTimerUI();
  } else if (k == "theme") {
    if (v == "prev") cfg.theme = (cfg.theme + NTHEMES - 1) % NTHEMES;
    else cfg.theme = (cfg.theme + 1) % NTHEMES;
    saveCfg();
    goScreen(screen); msg = "Theme updated";
  } else if (k == "toggle") {
    if (v == "h24") cfg.h24 = !cfg.h24;
    else if (v == "sound") cfg.sound = !cfg.sound;
    else if (v == "fahr") cfg.fahr = !cfg.fahr;
    saveCfg(); goScreen(screen);
    msg = "Preference updated";
  } else if (k == "bright") {
    int b = constrain(v.toInt(), 10, 255);
    cfg.bright = b; setBacklight(b); saveCfg();
    if (screen == SCR_SETTINGS) setRowBright();
    msg = "Brightness updated";
  } else if (k == "clock") {
    int st = constrain(v.toInt(), 0, 19);
    cfg.clockStyle = st; saveCfg();
    if (screen == SCR_CLOCK) clockInit(); else if (screen == SCR_SETTINGS) setRowClockStyle();
    msg = "Clock style set to " + String(CLOCK_STYLE_NAMES[st]);
  } else if (k == "temp") {
    cfg.fahr = (v == "F"); saveCfg();
    if (screen == SCR_SETTINGS) setRowTemp();
    else if (screen == SCR_HOME) homeInit();
    msg = cfg.fahr ? "Fahrenheit enabled" : "Celsius enabled";
  } else if (k == "format") {
    cfg.h24 = (v == "24"); saveCfg();
    if (screen == SCR_SETTINGS) setRowH24();
    else if (screen == SCR_CLOCK || screen == SCR_HOME) goScreen(screen);
    msg = cfg.h24 ? "24-hour format enabled" : "12-hour format enabled";
  } else if (k == "location") {
    String city = server.arg("city"); city.trim();
    float lat = server.arg("lat").toFloat(), lon = server.arg("lon").toFloat();
    float tzH = server.arg("tz").toFloat();
    if (!city.length() || lat < -90 || lat > 90 || lon < -180 || lon > 180 || tzH < -14 || tzH > 14) {
      msg = "Invalid location or timezone";
    } else {
      saveLocationConfig(city, lat, lon, (long)lroundf(tzH * 3600.0f), (long)server.arg("dst").toInt());
      applyTimeSettings(); wx.ok = false; wxTried = false;
      if (screen == SCR_WEATHER) wxRefresh();
      msg = "Location and timezone saved";
    }
  } else if (k == "time") {
    if (v == "sync") {
      ntpStarted = false; applyTimeSettings(); msg = "NTP sync requested";
    }
  } else if (k == "timeSet") {
    if (setManualLocalTime(v)) { alarmFiredKey = -1; alarmSuppress = false; msg = "Device time updated"; }
    else msg = "Invalid date/time";
  } else if (k == "refresh") {
    if (v == "screen") { goScreen(screen); msg = "Display refreshed"; }
    else if (v == "weather") { wx.ok = false; wxTried = false; if (WiFi.status() == WL_CONNECTED) wxRefresh(); msg = "Weather refreshed"; }
  } else if (k == "islamic") {
    if (v == "next") {
      islamicIdx = islamicNext(islamicIdx, 1, islamicCat);
      msg = "Next Islamic item";
    } else if (v == "prev") {
      islamicIdx = islamicNext(islamicIdx, -1, islamicCat);
      msg = "Previous Islamic item";
    } else if (v == "random") {
      int tries = 0, n = islamicIdx;
      do { n = random(ISLAMIC_COUNT); tries++; }
      while (islamicCat != 0 && ISLAMIC_DB[n].category != islamicCat && tries < 50);
      islamicIdx = n;
      msg = "Random Islamic item";
    } else if (v == "cat_ayat") {
      islamicCat = 1;
      for (int j = 0; j < ISLAMIC_COUNT; j++) if (ISLAMIC_DB[j].category == 1) { islamicIdx = j; break; }
      msg = "Filter: Quran Ayat";
    } else if (v == "cat_hadith") {
      islamicCat = 2;
      for (int j = 0; j < ISLAMIC_COUNT; j++) if (ISLAMIC_DB[j].category == 2) { islamicIdx = j; break; }
      msg = "Filter: Hadith";
    } else if (v == "cat_tip") {
      islamicCat = 3;
      for (int j = 0; j < ISLAMIC_COUNT; j++) if (ISLAMIC_DB[j].category == 3) { islamicIdx = j; break; }
      msg = "Filter: Poramorso (Tips)";
    } else if (v == "cat_all") {
      islamicCat = 0;
      msg = "Showing all categories";
    }
    if (screen == SCR_ISLAMIC) islamicDraw();
  } else if (k == "alarm") {
    if (v == "stop") {
      if (alarmRinging) { dismissAlarm(); msg = "Alarm stopped"; }
      else msg = "No alarm ringing";
    } else if (v == "snooze") {
      if (alarmRinging) { snoozeAlarm(); msg = "Snoozed 5 minutes"; }
      else msg = "No alarm ringing";
    } else if (v == "toggle") {
      int i = server.arg("i").toInt();
      if (i >= 0 && i < 3) {
        alarms[i].enabled = !alarms[i].enabled; alarmFiredKey = -1; saveAlarms();
        if (screen == SCR_ALARM) alarmDrawRow(i);
        msg = String("Alarm ") + (i + 1) + (alarms[i].enabled ? " enabled" : " disabled");
      }
    } else if (v == "set") {
      int i = server.arg("i").toInt();
      int h = server.arg("h").toInt();
      int m = server.arg("m").toInt();
      int d = server.arg("d").toInt();
      bool e = server.arg("e").toInt() != 0;
      if (i >= 0 && i < 3 && h >= 0 && h < 24 && m >= 0 && m < 60) {
        alarms[i].hour = h; alarms[i].minute = m;
        alarms[i].days = d; alarms[i].enabled = e;
        alarmFiredKey = -1;
        saveAlarms();
        if (screen == SCR_ALARM) alarmDrawRow(i);
        msg = String("Alarm ") + (i + 1) + " updated";
      } else msg = "Invalid alarm";
    }
  } else if (k == "wifi" && v == "disconnect") {
    WiFi.disconnect(); ntpStarted = false; msg = "Wi-Fi disconnected";
  }
  server.send(200, "text/plain", msg);
}

void handleWebStatus() {
  String t = "--:--";
  if (timeValid()) {
    if (cfg.h24) {
      char b[12]; snprintf(b, sizeof(b), "%02d:%02d:%02d", nowTm.tm_hour, nowTm.tm_min, nowTm.tm_sec); t = String(b);
    } else {
      char b[16]; snprintf(b, sizeof(b), "%d:%02d:%02d %s", hour12(nowTm.tm_hour), nowTm.tm_min, nowTm.tm_sec, nowTm.tm_hour >= 12 ? "PM" : "AM"); t = String(b);
    }
  }
  String islamicRef = ISLAMIC_DB[islamicIdx].ref;
  if (!islamicRef.length()) islamicRef = String(ISLAMIC_DB[islamicIdx].translation).substring(0, 40) + "...";

  String json = "{";
  json += "\"screen\":\"" + jsonEscape(String(SCREEN_NAMES[screen])) + "\",";
  json += "\"time\":\"" + jsonEscape(t) + "\",";
  json += "\"city\":\"" + jsonEscape(savedCity()) + "\",";
  json += "\"lat\":" + String(savedLat(), 4) + ",";
  json += "\"lon\":" + String(savedLon(), 4) + ",";
  json += "\"tz\":\"" + jsonEscape(tzLabel()) + "\",";
  json += "\"tzHours\":" + String(savedTzOffset() / 3600.0f, 2) + ",";
  json += "\"bright\":" + String(cfg.bright) + ",";
  json += "\"h24\":" + String(cfg.h24 ? "true" : "false") + ",";
  json += "\"sound\":" + String(cfg.sound ? "true" : "false") + ",";
  json += "\"fahr\":" + String(cfg.fahr ? "true" : "false") + ",";
  json += "\"clockIndex\":" + String(cfg.clockStyle) + ",";
  json += "\"clockStyle\":\"" + jsonEscape(String(CLOCK_STYLE_NAMES[cfg.clockStyle])) + "\",";
  json += "\"theme\":\"" + jsonEscape(String(TH.name)) + "\",";
  json += "\"weather\":" + String(wx.ok ? "true" : "false") + ",";
  json += "\"islamicCat\":\"" + jsonEscape(String(islamicCatName(islamicCat))) + "\",";
  json += "\"islamicRef\":\"" + jsonEscape(islamicRef) + "\",";
  json += "\"sta\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
  json += "\"ssid\":\"" + jsonEscape(WiFi.status() == WL_CONNECTED ? WiFi.SSID() : savedWifiSSID()) + "\",";
  json += "\"staip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"rssi\":" + String(WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0) + ",";
  json += "\"ap\":\"" + jsonEscape(String(CONTROL_AP_SSID)) + "\",";
  json += "\"apip\":\"" + WiFi.softAPIP().toString() + "\",";
  json += "\"alarmRinging\":" + String(alarmRinging ? "true" : "false") + ",";
  json += "\"alarms\":[";
  for (int i = 0; i < 3; i++) {
    if (i) json += ",";
    json += "{\"h\":" + String(alarms[i].hour) +
            ",\"m\":" + String(alarms[i].minute) +
            ",\"e\":" + String(alarms[i].enabled ? "true" : "false") +
            ",\"d\":" + String(alarms[i].days) + "}";
  }
  json += "],";
  json += "\"message\":\"" + jsonEscape(wifiMessage) + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handleWiFiScan() {
  int n = WiFi.scanComplete();
  if (n != WIFI_SCAN_FAILED) WiFi.scanDelete();
  n = WiFi.scanNetworks(false, true);
  String json = "{\"networks\":[";
  if (n > 0) {
    int idx[40];
    int count = n > 40 ? 40 : n;
    for (int i = 0; i < count; i++) idx[i] = i;
    for (int a = 0; a < count - 1; a++) {
      for (int b = a + 1; b < count; b++) {
        if (WiFi.RSSI(idx[b]) > WiFi.RSSI(idx[a])) {
          int tmp = idx[a]; idx[a] = idx[b]; idx[b] = tmp;
        }
      }
    }
    for (int p = 0; p < count; p++) {
      int i = idx[p];
      if (p) json += ",";
      String ssid = jsonEscape(WiFi.SSID(i));
      json += "{\"ssid\":\"" + ssid + "\",\"rssi\":" + String(WiFi.RSSI(i)) +
              ",\"secure\":" + String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? "true" : "false") + "}";
    }
  }
  json += "]}";
  WiFi.scanDelete();
  server.send(200, "application/json", json);
}

void handleWiFiConnect() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  ssid.trim();
  if (!ssid.length()) {
    server.send(400, "text/plain", "SSID is required");
    return;
  }
  saveWifiCredentials(ssid, pass);
  WiFi.disconnect(false, false);
  delay(100);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());
  lastWifiTry = millis();
  wifiConnectingFromWeb = true;
  ntpStarted = false;
  setWifiMessage("Connecting to " + ssid);
  server.send(200, "text/plain", "Wi-Fi credentials saved. Connecting to " + ssid + "...");
}

void handleWebGeo() {
  String q = server.arg("q"); q.trim();
  if (!q.length()) { server.send(400,"application/json","{\"ok\":false,\"message\":\"City required\"}"); return; }
  if (WiFi.status() != WL_CONNECTED) { server.send(200,"application/json","{\"ok\":false,\"message\":\"Connect Wi-Fi first for city search\"}"); return; }
  WiFiClientSecure client; client.setInsecure(); HTTPClient http; http.setTimeout(8000);
  String url = String("https://geocoding-api.open-meteo.com/v1/search?name=") + q + "&count=1&language=en&format=json";
  if (!http.begin(client,url)) { server.send(200,"application/json","{\"ok\":false,\"message\":\"Geocoding unavailable\"}"); return; }
  int code=http.GET(); if(code!=HTTP_CODE_OK){http.end();server.send(200,"application/json","{\"ok\":false,\"message\":\"City search failed\"}");return;}
  String body=http.getString(); http.end(); JsonDocument doc; if(deserializeJson(doc,body)){server.send(200,"application/json","{\"ok\":false,\"message\":\"Invalid geocoding response\"}");return;}
  JsonObject r=doc["results"][0]; if(r.isNull()){server.send(200,"application/json","{\"ok\":false,\"message\":\"City not found\"}");return;}
  String name=String((const char*)(r["name"]|"")); String country=String((const char*)(r["country"]|""));
  float lat=r["latitude"]|0.0f, lon=r["longitude"]|0.0f; String tzs=String((const char*)(r["timezone"]|""));
  long tz=0; if(tzs=="Asia/Dhaka")tz=21600; else if(tzs=="Asia/Kolkata")tz=19800; else if(tzs=="Asia/Kathmandu")tz=20700; else if(tzs=="Asia/Karachi")tz=18000; else if(tzs=="Asia/Dubai")tz=14400; else if(tzs=="Asia/Riyadh")tz=10800; else if(tzs=="Asia/Singapore")tz=28800; else if(tzs=="Asia/Tokyo")tz=32400; else if(tzs=="Asia/Shanghai")tz=28800;
  String tzH=String(tz/3600.0f,2); String json="{\"ok\":true,\"city\":\""+jsonEscape(name+", "+country)+"\",\"lat\":"+String(lat,5)+",\"lon\":"+String(lon,5)+",\"tz\":"+tzH+"}"; server.send(200,"application/json",json);
}

void setupWebServer() {
  server.on("/", HTTP_GET, handleWebRoot);
  server.on("/goto", HTTP_GET, handleWebGoto);
  server.on("/act", HTTP_GET, handleWebAct);
  server.on("/status", HTTP_GET, handleWebStatus);
  server.on("/wifi/scan", HTTP_GET, handleWiFiScan);
  server.on("/wifi/connect", HTTP_POST, handleWiFiConnect);
  server.on("/geo", HTTP_GET, handleWebGeo);
  server.begin();
}

