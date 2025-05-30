// Base Code for RoboSoccer 3.0
// ESP8266 Mecanum Wheels
// 23f3000019@es.study.iitm.ac.in - Aditya Rao
// Chief Mentor - Dr.Janakiraman Viraraghavan
// Electrical Engineering Department - IIT Madras

const char *HTML_CONTENT = R"=====(

<!DOCTYPE html>
<html>
<head>
<title>ROBOSOCCER 3.0 - ESP8266 Mecanum Control</title>

<meta name="viewport" content="width=device-width, initial-scale=0.7, maximum-scale=1, user-scalable=no">
<style type="text/css">
html, body {
  overflow: hidden;
  margin: 0;
  padding: 0;
  text-align: center;
  font-size: 20px;
  font-family: arial;
  background: #000000;
}

button {
  font-size: 20px;
  width: 90%;
  margin: 10px auto;
  padding: 20px;
  background-color: #909090;
  border: none;
  border-radius: 10px;
  box-shadow: 0 4px 8px rgba(0,0,0,0.2);
  font-weight: bold;
  display: block;
  -webkit-user-select: none; /* Safari */
  -ms-user-select: none;     /* IE 10 and IE 11 */
  user-select: none;         /* Standard syntax */
}

button:active {
  background-color: #dedede;
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
  document.getElementById("ws_state").innerHTML = "Connected";
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
    document.getElementById("ws_state").innerHTML = "Connecting...";
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
<h3 style="margin-top: 5px; margin-bottom: 4px; color: #ffff00;">ROBOSOCCER 3.0</h3>
<h4 style="margin-top: 1px; margin-bottom: 2px; color: #606060;">ESP8266 Wi-Fi Mecanum Robot</h4>
<h5 style="margin-top: 1px; margin-bottom: 2px; color: #909090;">Aditya Rao</h4>

<div style="width: 100%; max-width: 800px; margin: auto;">
<center>
<table>
<tr>
  <td>
  <button data-cmd="16" style="background-color:#4d4dff;">FORWARD LEFT</button>
  </td>
  <td>
  <button data-cmd="1" style="background-color:#00cc00;">FORWARD</button>
  </td>
  <td>
  <button data-cmd="32" style="background-color:#4d4dff;">FORWARD RIGHT</button>
  </td>
</tr>
<tr>
  <td>
  <button data-cmd="4" style="background-color:#e68a00;">STRAFE LEFT</button>
  </td>
  <td>
  <button data-cmd="0" style="background-color:#ff0000;">STOP</button>
  </td>
  <td>
  <button data-cmd="8" style="background-color:#aaaa55;">STRAFE RIGHT</button>
  </td>
</tr>
<tr>
  <td>
  <button data-cmd="64" style="background-color:#4d4dff;">BACKWARD LEFT</button>
  </td>
  <td>
  <button data-cmd="2" style="background-color:#990099;">BACKWARD</button>
  </td>
  <td>
  <button data-cmd="128" style="background-color:#4d4dff;">BACKWARD RIGHT</button>
  </td>
</tr>
</table>
</br>
<table>
<tr>
  <td>
  <button data-cmd="256" style="background-color:#e68a00;">SPIN LEFT</button>
  </td>
  <td>
  <button data-cmd="512" style="background-color:#aaaa55;">SPIN RIGHT</button>
  </td>
</tr>
</table>
</center>
</div>
<p font-color: #404040;>
<center>
Remote Control 
</br>
<span id="ws_state" style="color:blue">Not Connected</span>
</br>
</center>
</p>
<button id="wc_conn" type="button" onclick="wc_onclick();">Connect to Soccer Robot</button>
<br><br>
</body>
</html>

)=====";
