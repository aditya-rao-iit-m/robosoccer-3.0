// Base Code for RoboSoccer 3.0
// ESP8266 Mecanum Wheels
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

button {
  font-size: 20px;
  width: 90%;
  margin: 10px auto;
  padding: 20px;
  background-color: #E7A832;
  border: none;
  border-radius: 10px;
  box-shadow: 0 4px 8px rgba(0,0,0,0.2);
  font-weight: bold;
  display: block;
}

button:active {
  background-color: #BE7D03;
  transform: scale(0.97);
}
</style>
<script>
var CMD_STOP = 0;
var CMD_FORWARD = 1;
var CMD_BACKWARD = 2;
var CMD_LEFT = 4;
var CMD_RIGHT = 8;
var CMD_FORWARD_LEFT = 16;
var CMD_FORWARD_RIGHT = 32;
var CMD_BACKWARD_LEFT = 64;
var CMD_BACKWARD_RIGHT = 128;
var CMD_ROTATE_LEFT = 256;
var CMD_ROTATE_RIGHT = 512;

var ws = null;

function init() {
  document.querySelectorAll("button[data-cmd]").forEach(btn => {
    btn.addEventListener("touchstart", () => send_command(btn.dataset.cmd));
    btn.addEventListener("mousedown", () => send_command(btn.dataset.cmd));
    btn.addEventListener("touchend", () => send_command(CMD_STOP));
    btn.addEventListener("mouseup", () => send_command(CMD_STOP));
    btn.addEventListener("mouseout", () => send_command(CMD_STOP));
  });
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
<h4 style="margin-top: 1px; color: #404040;">ESP8266 Wi-Fi Mecanum Robot - Aditya Rao</h4>
<div style="width: 100%; max-width: 420px; margin: auto;">
  <button data-cmd="1">FORWARD</button>
  <button data-cmd="2">BACKWARD</button>
  <button data-cmd="4">STRAFE LEFT</button>
  <button data-cmd="8">STRAFE RIGHT</button>
  <button data-cmd="16">FORWARD LEFT</button>
  <button data-cmd="32">FORWARD RIGHT</button>
  <button data-cmd="64">BACKWARD LEFT</button>
  <button data-cmd="128">BACKWARD RIGHT</button>
  <button data-cmd="256">ROTATE LEFT</button>
  <button data-cmd="512">ROTATE RIGHT</button>
</div>
<p>
Remote Control : <span id="ws_state" style="color:blue">Not Connected</span><br>
</p>
<button id="wc_conn" type="button" onclick="wc_onclick();">Connect to Soccer Robot</button>
<br><br>
</body>
</html>

)=====";