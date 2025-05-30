// Base Code for RoboSoccer 3.0
// ESP8266 Mecanum Wheels
// 23f3000019@es.study.iitm.ac.in - Aditya Rao
// Chief Mentor - Dr.Janakiraman Viraraghavan
// Electrical Engineering Department - IIT Madras

#include <SocketIOclient.h>
#include <WebSockets.h>
#include <WebSockets4WebServer.h>
#include <WebSocketsClient.h>
#include <WebSocketsVersion.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include "index.h"

#define CMD_STOP            0
#define CMD_FORWARD         1
#define CMD_BACKWARD        2
#define CMD_LEFT            4   // strafe left
#define CMD_RIGHT           8   // strafe right
#define CMD_FORWARD_LEFT    16
#define CMD_FORWARD_RIGHT   32
#define CMD_BACKWARD_LEFT   64
#define CMD_BACKWARD_RIGHT  128
#define CMD_ROTATE_LEFT     256
#define CMD_ROTATE_RIGHT    512

// LEFT Motor Driver Pins - LEFT SIDE WHEELS
#define IN1_LEFT_FRONT_WHEEL_FORWARDS 4    // GPIO4    Front Left    D2   Forwards   l_driver_in1
#define IN2_LEFT_FRONT_WHEEL_BACKWARDS 5   // GPIO5    Front Left    D1   Backwards  l_driver_in2
#define IN3_LEFT_REAR_WHEEL_BACKWARDS 12   // GPIO12   Rear Left     D6   Backwards  l_driver_in3
#define IN4_LEFT_REAR_WHEEL_FORWARDS 14    // GPIO14   Rear Left     D5   Forwards   l_driver_in4


// RIGHT Motor Driver Pins - RIGHT SIDE WHEELS
#define IN1_RIGHT_REAR_WHEEL_BACKWARDS 2    // GPIO2    Rear Right    D4  Backwards  r_driver_in1
#define IN2_RIGHT_REAR_WHEEL_FORWARDS 0     // GPIO0    Rear Right    D3  Forwards   r_driver_in2
#define IN3_RIGHT_FRONT_WHEEL_FORWARDS 15   // GPIO15   Front Right   D8  Forwards   r_driver_in3
#define IN4_RIGHT_FRONT_WHEEL_BACKWARDS 13  // GPIO13   Front Right   D7  Backwards  r_driver_in4

const char* ssid = "robosoccer";
const char* password = "iitmadras";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Soccer Robot Disconnected\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
      }
      break;
    case WStype_TEXT:
      String angle = String((char*)payload);
      int command = angle.toInt();
      Serial.print("command: ");
      Serial.println(command);

      switch (command) {
        case CMD_STOP: stop(); break;
        case CMD_FORWARD: moveForward(); break;
        case CMD_BACKWARD: moveBackward(); break;
        case CMD_LEFT: strafeLeft(); break;
        case CMD_RIGHT: strafeRight(); break;
        case CMD_ROTATE_LEFT: rotateLeft(); break;
        case CMD_ROTATE_RIGHT: rotateRight(); break;
        default: Serial.println("Unknown Command"); break;
      }
      break;
  }
}

void setup() {
  Serial.begin(9600);

  // Set all motor pins as OUTPUT
  int motorPins[] = {IN1_LEFT_FRONT_WHEEL_FORWARDS, IN2_LEFT_FRONT_WHEEL_BACKWARDS, IN4_LEFT_REAR_WHEEL_FORWARDS, IN3_LEFT_REAR_WHEEL_BACKWARDS, IN4_RIGHT_FRONT_WHEEL_BACKWARDS, IN3_RIGHT_FRONT_WHEEL_FORWARDS, IN1_RIGHT_REAR_WHEEL_BACKWARDS, IN2_RIGHT_REAR_WHEEL_FORWARDS};
  for (int i = 0; i < 8; i++) {
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], LOW);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi Hotspot...");
  }
  Serial.println("Connected to WiFi Hotspot");

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  server.on("/", HTTP_GET, []() {
    String html = HTML_CONTENT;
    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.print("ESP8266 Soccer Robot Web Server's IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
  webSocket.loop();
}

// Fully functional movement functions for Mecanum Wheel Drive
// Important : Note that the physical position of the motors on the chassis influence HIGH and LOW
// If your FRONT AND REAR BO motors are installed physically opposed, these values will work
// For other positions of the motors, flip the HIGH and LOW of the rear motors accordingly
// Aditya Rao - Thursday, 29th May 2025. RoboSoccer 3.0

void moveForward() {
  digitalWrite(IN1_LEFT_FRONT_WHEEL_FORWARDS, HIGH); digitalWrite(IN2_LEFT_FRONT_WHEEL_BACKWARDS, LOW);
  digitalWrite(IN4_LEFT_REAR_WHEEL_FORWARDS, HIGH); digitalWrite(IN3_LEFT_REAR_WHEEL_BACKWARDS, LOW);
  digitalWrite(IN4_RIGHT_FRONT_WHEEL_BACKWARDS, HIGH); digitalWrite(IN3_RIGHT_FRONT_WHEEL_FORWARDS, LOW);
  digitalWrite(IN1_RIGHT_REAR_WHEEL_BACKWARDS, HIGH); digitalWrite(IN2_RIGHT_REAR_WHEEL_FORWARDS, LOW);
}

void moveBackward() {
  digitalWrite(IN1_LEFT_FRONT_WHEEL_FORWARDS, LOW); digitalWrite(IN2_LEFT_FRONT_WHEEL_BACKWARDS, HIGH);
  digitalWrite(IN4_LEFT_REAR_WHEEL_FORWARDS, LOW); digitalWrite(IN3_LEFT_REAR_WHEEL_BACKWARDS, HIGH);
  digitalWrite(IN4_RIGHT_FRONT_WHEEL_BACKWARDS, LOW); digitalWrite(IN3_RIGHT_FRONT_WHEEL_FORWARDS, HIGH);
  digitalWrite(IN1_RIGHT_REAR_WHEEL_BACKWARDS, LOW); digitalWrite(IN2_RIGHT_REAR_WHEEL_FORWARDS, HIGH);
}

void strafeLeft() {
  digitalWrite(IN1_LEFT_FRONT_WHEEL_FORWARDS, LOW); digitalWrite(IN2_LEFT_FRONT_WHEEL_BACKWARDS, HIGH);
  digitalWrite(IN4_LEFT_REAR_WHEEL_FORWARDS, HIGH); digitalWrite(IN3_LEFT_REAR_WHEEL_BACKWARDS, LOW);
  digitalWrite(IN4_RIGHT_FRONT_WHEEL_BACKWARDS, HIGH); digitalWrite(IN3_RIGHT_FRONT_WHEEL_FORWARDS, LOW);
  digitalWrite(IN1_RIGHT_REAR_WHEEL_BACKWARDS, LOW); digitalWrite(IN2_RIGHT_REAR_WHEEL_FORWARDS, HIGH);
}

void strafeRight() {
  digitalWrite(IN1_LEFT_FRONT_WHEEL_FORWARDS, HIGH); digitalWrite(IN2_LEFT_FRONT_WHEEL_BACKWARDS, LOW);
  digitalWrite(IN4_LEFT_REAR_WHEEL_FORWARDS, LOW); digitalWrite(IN3_LEFT_REAR_WHEEL_BACKWARDS, HIGH);
  digitalWrite(IN4_RIGHT_FRONT_WHEEL_BACKWARDS, LOW); digitalWrite(IN3_RIGHT_FRONT_WHEEL_FORWARDS, HIGH);
  digitalWrite(IN1_RIGHT_REAR_WHEEL_BACKWARDS, HIGH); digitalWrite(IN2_RIGHT_REAR_WHEEL_FORWARDS, LOW);
}

void rotateLeft() {
  digitalWrite(IN1_LEFT_FRONT_WHEEL_FORWARDS, LOW); digitalWrite(IN2_LEFT_FRONT_WHEEL_BACKWARDS, HIGH);
  digitalWrite(IN4_LEFT_REAR_WHEEL_FORWARDS, LOW); digitalWrite(IN3_LEFT_REAR_WHEEL_BACKWARDS, HIGH);
  digitalWrite(IN4_RIGHT_FRONT_WHEEL_BACKWARDS, HIGH); digitalWrite(IN3_RIGHT_FRONT_WHEEL_FORWARDS, LOW);
  digitalWrite(IN1_RIGHT_REAR_WHEEL_BACKWARDS, HIGH); digitalWrite(IN2_RIGHT_REAR_WHEEL_FORWARDS, LOW);
}

void rotateRight() {
  digitalWrite(IN1_LEFT_FRONT_WHEEL_FORWARDS, HIGH); digitalWrite(IN2_LEFT_FRONT_WHEEL_BACKWARDS, LOW);
  digitalWrite(IN4_LEFT_REAR_WHEEL_FORWARDS, HIGH); digitalWrite(IN3_LEFT_REAR_WHEEL_BACKWARDS, LOW);
  digitalWrite(IN4_RIGHT_FRONT_WHEEL_BACKWARDS, LOW); digitalWrite(IN3_RIGHT_FRONT_WHEEL_FORWARDS, HIGH);
  digitalWrite(IN1_RIGHT_REAR_WHEEL_BACKWARDS, LOW); digitalWrite(IN2_RIGHT_REAR_WHEEL_FORWARDS, HIGH);
}

void stop() {
  digitalWrite(IN1_LEFT_FRONT_WHEEL_FORWARDS, LOW); digitalWrite(IN2_LEFT_FRONT_WHEEL_BACKWARDS, LOW);
  digitalWrite(IN4_LEFT_REAR_WHEEL_FORWARDS, LOW); digitalWrite(IN3_LEFT_REAR_WHEEL_BACKWARDS, LOW);
  digitalWrite(IN4_RIGHT_FRONT_WHEEL_BACKWARDS, LOW); digitalWrite(IN3_RIGHT_FRONT_WHEEL_FORWARDS, LOW);
  digitalWrite(IN1_RIGHT_REAR_WHEEL_BACKWARDS, LOW); digitalWrite(IN2_RIGHT_REAR_WHEEL_FORWARDS, LOW);
}
