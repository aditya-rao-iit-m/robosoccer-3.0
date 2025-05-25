# Import necessary modules
import network
import socket
import time
import random
from machine import Pin

# Create an LED object on pin 'LED'
led = Pin('LED', Pin.OUT)

# Create four motor driver objects
motor_a_forward = Pin(18, Pin.OUT)
motor_a_backward = Pin(19, Pin.OUT)
motor_b_forward = Pin(20, Pin.OUT)
motor_b_backward = Pin(21, Pin.OUT)

# Wi-Fi credentials
ssid = 'robosoccer'
password = 'iitmadras'

# Movement functions
def move_forward():
    motor_a_forward.value(1)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(1)

def move_backward():
    motor_a_forward.value(0)
    motor_b_forward.value(1)
    motor_a_backward.value(1)
    motor_b_backward.value(0)

def move_stop():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(0)

def move_right():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(1)
    motor_b_backward.value(1)

def move_left():
    motor_a_forward.value(1)
    motor_b_forward.value(1)
    motor_a_backward.value(0)
    motor_b_backward.value(0)

move_stop()

# HTML page with control interface
def webpage(random_value, state):
    html = """
        <!DOCTYPE html>
        <html>
        <head>
          <title>ROBOSOCCER 3.0 - PICO WH Robot Control</title>
          <meta name="viewport" content="width=device-width, initial-scale=0.7, maximum-scale=1, user-scalable=no">
          <style>
            html, body {
              overflow: hidden;
              margin: 0;
              padding: 0;
              text-align: center;
              font-size: 24px;
              font-family: Arial, sans-serif;
            }
            .control-btn {
              position: absolute;
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
              user-select: none;
              transition: all 0.1s ease-in-out;
            }
            .control-btn.pressed {
              background-color: #BE7D03;
              transform: scale(0.95);
              box-shadow: 0 2px 4px rgba(0,0,0,0.3);
              transform: rotate(90deg);
            }
            #container {
              margin: auto;
              width: 400px;
              height: 870px;
              position: relative;
            }
            .up    { top: 35px; right: 0px; transform: rotate(90deg); }
            .down  { top: 35px; left: 0px; transform: rotate(90deg); }
            .left  { top: 420px; left: 100px; transform: rotate(90deg); }
            .right { top: 640px; left: 100px; transform: rotate(90deg); }

            button {
              font-size: 22px;
              padding: 10px 20px;
              margin: 10px;
              background-color: #E7A832;
              border: none;
              border-radius: 8px;
              font-weight: bold;
              box-shadow: 0 4px 8px rgba(0,0,0,0.2);
              cursor: pointer;
              transition: all 0.1s ease-in-out;
            }
            button:active {
              background-color: #BE7D03;
              transform: scale(0.95);
              box-shadow: 0 2px 4px rgba(0,0,0,0.3);
            }
          </style>
          <script>
            function sendCommand(cmd) {
              fetch("/" + cmd).catch(err => console.log("Fetch failed:", err));
            }

            function setupControl(id, cmd) {
              const el = document.getElementById(id);
              const addPressed = () => el.classList.add("pressed");
              const removePressed = () => el.classList.remove("pressed");

              el.addEventListener("mousedown", () => {
                sendCommand(cmd);
                addPressed();
              });
              el.addEventListener("mouseup", () => {
                sendCommand("stop");
                removePressed();
              });
              el.addEventListener("mouseleave", () => {
                sendCommand("stop");
                removePressed();
              });

              el.addEventListener("touchstart", (e) => {
                e.preventDefault();
                sendCommand(cmd);
                addPressed();
              }, { passive: false });

              el.addEventListener("touchend", () => {
                sendCommand("stop");
                removePressed();
              });
            }

            window.onload = () => {
              setupControl("btn_fwd", "forward");
              setupControl("btn_back", "backward");
              setupControl("btn_left", "left");
              setupControl("btn_right", "right");
            };
          </script>
        </head>
        <body>
          <h2 style="margin-top: 5px;">ROBOSOCCER 3.0</h2>
          <h4 style="color: #404040;">PICO WH Robot - Aditya Rao</h4>

          <div id="container">
            <div id="btn_fwd" class="control-btn up">FORWARD</div>
            <div id="btn_back" class="control-btn down">BACKWARD</div>
            <div id="btn_left" class="control-btn left">LEFT</div>
            <div id="btn_right" class="control-btn right">RIGHT</div>
          </div>

          <br><br>
          <button onclick="sendCommand('stop')">STOP</button>
          <button onclick="sendCommand('lighton')">LED ON</button>
          <button onclick="sendCommand('lightoff')">LED OFF</button>
          <button onclick="sendCommand('value')">GET RANDOM VALUE</button>
        </body>
        </html>
    """
    return html

# Connect to Wi-Fi
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)

# Wait for connection
timeout = 10
while timeout > 0:
    if wlan.status() >= 3:
        break
    timeout -= 1
    print('Waiting for RoboSoccer Wi-Fi connection...')
    time.sleep(1)

# Check status
if wlan.status() != 3:
    raise RuntimeError('Failed to connect to RoboSoccer WiFi')
else:
    print('Connected to WiFi')
    print('IP:', wlan.ifconfig()[0])

# Setup web server
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
s = socket.socket()
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(addr)
s.listen(1)
print('Listening on', addr)

state = "OFF"
random_value = 0

while True:
    try:
        conn, addr = s.accept()
        print('Got a connection from', addr)

        request = conn.recv(1024)
        request = str(request)
        print("Request:", request)

        try:
            path = request.split(' ')[1]
        except IndexError:
            path = '/'

        if path == '/lighton':
            led.value(1)
            state = 'LED ON'
        elif path == '/lightoff':
            led.value(0)
            state = 'LED OFF'
        elif path == '/forward':
            move_forward()
            state = 'Moving Forward'
        elif path == '/backward':
            move_backward()
            state = 'Moving Backward'
        elif path == '/left':
            move_left()
            state = 'Turning Left'
        elif path == '/right':
            move_right()
            state = 'Turning Right'
        elif path == '/stop':
            move_stop()
            state = 'Stopped'
        elif path == '/value':
            random_value = random.randint(10, 20)

        response = webpage(random_value, state)

        conn.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
        conn.send(response.encode())
        conn.close()

    except OSError as e:
        print("Error:", e)
        conn.close()