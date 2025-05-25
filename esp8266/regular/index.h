// Base Code for RoboSoccer 3.0
// ESP8266 Regular Wheels
// 23f3000019@es.study.iitm.ac.in - Aditya Rao
// Chief Mentor - Dr.Janakiraman Viraraghavan
// Electrical Engineering Department - IIT Madras

const char *HTML_CONTENT = R"=====(

<!DOCTYPE html>
<html>
<head>
<title>ROBOSOCCER 3.0 - ESP8266 Robot Control</title>

<meta name="viewport" content="width=device-width, initial-scale=0.7, maximum-scale=1, user-scalable=no">
<style type="text/css">

html, body {
  overflow: hidden;
  margin: 0;
  padding: 0;
  text-align: center;
  font-size: 24px;
}

button { text-align: center; font-size: 24px;}

#container {
  margin: auto;
  width: 400px;
  height: 870px;
  position: relative;
  margin-bottom: 10px;
}

div[class^='button'] {
  position: absolute;
  transition: background-color 0.2s ease, transform 0.1s ease;
  width: 180px;
  height: 180px;
  background-color: #E7A832;
  color: black;
  border-radius: 10px;
  font-size: 20px;
  font-weight: bold;
  display: flex;
  align-items: center;
  justify-content: center;
  box-shadow: 0 4px 8px rgba(0,0,0,0.2);
}

.button_up {
  top: 35px;
  right: 0px;
  transform: rotate(90deg);
}

.button_down {
  top: 35px;
  left: 0px;
  transform: rotate(90deg);
}

.button_left {
  top: 420px;
  left: 100px;
  transform: rotate(90deg);
}

.button_right {
  top: 640px;
  left: 100px;
  transform: rotate(90deg);
}

/* Button Pressed Animations */

.button_up.pressed {
  background-color: #BE7D03;
  transform: rotate(90deg) scale(0.67);
}

.button_down.pressed {
  background-color: #BE7D03;
  transform: rotate(90deg) scale(0.67);
}

.button_left.pressed {
  background-color: #BE7D03;
  transform: rotate(90deg) scale(0.67);
}

.button_right.pressed {
  background-color: #BE7D03;
  transform: rotate(90deg) scale(0.67);
}

</style>
<script>
var CMD_STOP     = 0;
var CMD_FORWARD  = 1;
var CMD_BACKWARD = 2;
var CMD_LEFT     = 4;
var CMD_RIGHT    = 8;

var ws = null;

function init() {
  var container = document.querySelector("#container");
  container.addEventListener("touchstart", mouse_down);
  container.addEventListener("touchend", mouse_up);
  container.addEventListener("touchcancel", mouse_up);
  container.addEventListener("mousedown", mouse_down);
  container.addEventListener("mouseup", mouse_up);
  container.addEventListener("mouseout", mouse_up);    
}

function ws_onmessage(e_msg) {
  e_msg = e_msg || window.event;
}

function ws_onopen() {
  document.getElementById("ws_state").innerHTML = "CONNECTED";
  document.getElementById("wc_conn").innerHTML = "Disconnect";
}

function ws_onclose() {
  document.getElementById("ws_state").innerHTML = "Not Connected";
  document.getElementById("wc_conn").innerHTML = "Connect";
  ws.onopen = null;
  ws.onclose = null;
  ws.onmessage = null;
  ws = null;
}

function wc_onclick() {
  if(ws == null) {
    ws = new WebSocket("ws://" + window.location.host + ":81");
    document.getElementById("ws_state").innerHTML = "CONNECTING";
    ws.onopen = ws_onopen;
    ws.onclose = ws_onclose;
    ws.onmessage = ws_onmessage;
  } else {
    ws.close();
  }
}

function mouse_down(event) {
  if (event.target !== event.currentTarget) {
    var id = event.target.id;
    event.target.classList.add('pressed');
    send_command(id);
  }
  event.stopPropagation();
  event.preventDefault();
}

function mouse_up(event) {
  if (event.target !== event.currentTarget) {
    var id = event.target.id;
    event.target.classList.remove('pressed');
    send_command(CMD_STOP);
  }
  event.stopPropagation();
  event.preventDefault();
}

function send_command(cmd) {
  if(ws != null && ws.readyState == 1) {
    ws.send(cmd + "\r\n");
  }
}

window.onload = init;
</script>
</head>
<body>
<h3 style="margin-top: 5px; margin-bottom: 4px;">ROBOSOCCER 3.0</h3>
<h4 style="margin-top: 1px; color: #404040;">ESP8266 Wi-Fi Robot - Aditya Rao</h4>
<div id="container">
  <div id="1" class="button_up">FORWARD</div>
  <div id="2" class="button_down">BACKWARD</div>
  <div id="4" class="button_left">LEFT</div>
  <div id="8" class="button_right">RIGHT</div>
</div>
<p>
Remote Control : <span id="ws_state" style="color:blue">Not Connected</span><br>
</p>
<button id="wc_conn" type="button" onclick="wc_onclick();">Connect to Soccer Robot</button>
<br><br>
</body>
</html>

)=====";
