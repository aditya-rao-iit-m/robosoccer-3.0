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
motor_b_forward = Pin(21, Pin.OUT)
motor_b_backward = Pin(20, Pin.OUT)
motor_c_forward = Pin(13, Pin.OUT)
motor_c_backward = Pin(12, Pin.OUT)
motor_d_forward = Pin(10, Pin.OUT)
motor_d_backward = Pin(11, Pin.OUT)

# Wi-Fi credentials
ssid = 'robosoccer'
password = 'iitmadras'

# Motor A - Front Right
# Motor B - Back Right
# Motor C - Front Left
# Motor D - Back Left

# Movement functions
def move_forward():
    motor_a_forward.value(1)
    motor_b_forward.value(1)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(1)
    motor_d_forward.value(1)
    motor_c_backward.value(0)
    motor_d_backward.value(0)

def move_backward():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(1)
    motor_b_backward.value(1)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(1)
    motor_d_backward.value(1)

def move_stop():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(0)

def move_right():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(1)
    motor_b_backward.value(1)
    motor_c_forward.value(1)
    motor_d_forward.value(1)
    motor_c_backward.value(0)
    motor_d_backward.value(0)

def move_left():
    motor_a_forward.value(1)
    motor_b_forward.value(1)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(1)
    motor_d_backward.value(1)

def strafe_left():
    motor_a_forward.value(1)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(1)
    motor_c_forward.value(0)
    motor_d_forward.value(1)
    motor_c_backward.value(1)
    motor_d_backward.value(0)
    
def strafe_right():
    motor_a_forward.value(0)
    motor_b_forward.value(1)
    motor_a_backward.value(1)
    motor_b_backward.value(0)
    motor_c_forward.value(1)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(1)

def forward_left_diagonal():
    motor_a_forward.value(1)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(0)
    motor_d_forward.value(1)
    motor_c_backward.value(0)
    motor_d_backward.value(0)

def forward_right_diagonal():
    motor_a_forward.value(0)
    motor_b_forward.value(1)
    motor_a_backward.value(0)
    motor_b_backward.value(0)
    motor_c_forward.value(1)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(0)
    
def backward_left_diagonal():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(0)
    motor_b_backward.value(1)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(1)
    motor_d_backward.value(0)

def backward_right_diagonal():
    motor_a_forward.value(0)
    motor_b_forward.value(0)
    motor_a_backward.value(1)
    motor_b_backward.value(0)
    motor_c_forward.value(0)
    motor_d_forward.value(0)
    motor_c_backward.value(0)
    motor_d_backward.value(1)

move_stop()

# HTML page with control interface
def webpage(random_value, state):
    html = """
        <!DOCTYPE html>
        <html lang="en">
        <head>
          <meta charset="UTF-8">
          <title>ROBOSOCCER 3.0 - PICO WH Mecanum Robot Control</title>
          <style>
            /* Lock down all text selection everywhere */
            * {
              user-select: none;
              -webkit-user-select: none;
              -moz-user-select: none;
              -ms-user-select: none;
            }

            body {
              display: flex;
              flex-direction: column;
              align-items: center;
              justify-content: center;
              height: 80vh;
              margin: 0;
              background-color: white;
              font-family: sans-serif;
            }

            .main-grid {
              display: grid;
              grid-template-columns: 100px 100px 100px;
              grid-template-rows: 100px 100px 100px;
              gap: 20px;
              margin-bottom: 50px;
            }

            .button {
              width: 100px;
              height: 100px;
              border: none;
              border-radius: 20px;
              font-size: 14px;
              font-weight: bold;
              color: black;
              display: flex;
              align-items: center;
              justify-content: center;
              text-align: center;
              cursor: pointer;
              touch-action: manipulation;

              /* REMOVE tap highlight on mobile */
              -webkit-tap-highlight-color: transparent;

              /* REMOVE focus outline */
              outline: none;
            }

            /* Just in case focus appears later */
            .button:focus {
              outline: none;
            }

            .rotate-text {
              transform: rotate(90deg);
            }

            .red { background-color: #ff3b30; }
            .blue { background-color: #00cfff; }
            .green { background-color: #73e04d; }
            .orange { background-color: #fbbd3c; }

            .diagonal-grid {
              display: grid;
              grid-template-columns: repeat(3, 100px);
              grid-template-rows: repeat(2, 100px);
              gap: 20px;
            }
          </style>
        </head>
        <body>
        <h2 style="margin-top: 5px;">ROBOSOCCER 3.0</h2>
        <h4 style="color: #404040;">PICO WH Mecanum Robot - Aditya Rao</h4>

          <!-- T-Shaped Directional Controls -->
          <div class="main-grid">
            <div></div>
            <button class="button blue rotate-text" ontouchstart="sendCommand('left')" ontouchend="sendCommand('stop')">LEFT</button>
            <div></div>

            <button class="button red rotate-text" ontouchstart="sendCommand('backward')" ontouchend="sendCommand('stop')">BACKWARD</button>
            <div></div>
            <button class="button red rotate-text" ontouchstart="sendCommand('forward')" ontouchend="sendCommand('stop')">FORWARD</button>

            <div></div>
            <button class="button blue rotate-text" ontouchstart="sendCommand('right')" ontouchend="sendCommand('stop')">RIGHT</button>
            <div></div>
          </div>

          <!-- Diagonal and Strafe Controls -->
          <div class="diagonal-grid">
            <button class="button green rotate-text" ontouchstart="sendCommand('back_left')" ontouchend="sendCommand('stop')">BACK LEFT<br>DIAGONAL</button>
            <button class="button orange rotate-text" ontouchstart="sendCommand('strafe_left')" ontouchend="sendCommand('stop')">STRAFE LEFT</button>
            <button class="button green rotate-text" ontouchstart="sendCommand('front_left')" ontouchend="sendCommand('stop')">FRONT LEFT<br>DIAGONAL</button>

            <button class="button green rotate-text" ontouchstart="sendCommand('back_right')" ontouchend="sendCommand('stop')">BACK RIGHT<br>DIAGONAL</button>
            <button class="button orange rotate-text" ontouchstart="sendCommand('strafe_right')" ontouchend="sendCommand('stop')">STRAFE RIGHT</button>
            <button class="button green rotate-text" ontouchstart="sendCommand('front_right')" ontouchend="sendCommand('stop')">FRONT RIGHT<br>DIAGONAL</button>
          </div>

          <script>
            function sendCommand(cmd) {
              fetch("/" + cmd)
                .catch(error => console.error("Failed to send command:", error));
            }
          </script>

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
        elif path == '/strafe_left':
            strafe_left()
            state = 'Strafing Left'
        elif path == '/strafe_right':
            strafe_right()
            state = 'Strafing Right'
        elif path == '/front_left':
            forward_left_diagonal()
            state = 'FWD Left Diagonal'
        elif path == '/front_right':
            forward_right_diagonal()
            state = 'FWD Right Diagonal' 
        elif path == '/back_left':
            backward_left_diagonal()
            state = 'BKWD Left Diagonal'
        elif path == '/back_right':
            backward_right_diagonal()
            state = 'BKWD Right Diagonal' 
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
