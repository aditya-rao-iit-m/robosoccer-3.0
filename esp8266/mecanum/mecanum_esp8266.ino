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

// Motor Driver Pins - LEFT SIDE MOTOR DRIVER
#define IN1_FL 4    // GPIO4    Front Left
#define IN2_FL 5    // GPIO5    Front Left
#define IN1_FR 13   // GPIO13   Front Right
#define IN2_FR 15   // GPIO15   Front Right

// Motor Driver Pins - RIGHT SIDE MOTOR DRIVER
#define IN1_RL 14   // GPIO14   Rear Left
#define IN2_RL 12   // GPIO12   Rear Left
#define IN1_RR 2    // GPIO2    Rear Right
#define IN2_RR 0    // GPIO0    Rear Right

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
  int motorPins[] = {IN1_FL, IN2_FL, IN1_RL, IN2_RL, IN1_FR, IN2_FR, IN1_RR, IN2_RR};
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

// Movement Functions for Mecanum Drive
void moveForward() {
  digitalWrite(IN1_FL, HIGH); digitalWrite(IN2_FL, LOW);
  digitalWrite(IN1_RL, HIGH); digitalWrite(IN2_RL, LOW);
  digitalWrite(IN1_FR, HIGH); digitalWrite(IN2_FR, LOW);
  digitalWrite(IN1_RR, HIGH); digitalWrite(IN2_RR, LOW);
}

void moveBackward() {
  digitalWrite(IN1_FL, LOW); digitalWrite(IN2_FL, HIGH);
  digitalWrite(IN1_RL, LOW); digitalWrite(IN2_RL, HIGH);
  digitalWrite(IN1_FR, LOW); digitalWrite(IN2_FR, HIGH);
  digitalWrite(IN1_RR, LOW); digitalWrite(IN2_RR, HIGH);
}

void strafeLeft() {
  digitalWrite(IN1_FL, LOW); digitalWrite(IN2_FL, HIGH);
  digitalWrite(IN1_RL, HIGH); digitalWrite(IN2_RL, LOW);
  digitalWrite(IN1_FR, HIGH); digitalWrite(IN2_FR, LOW);
  digitalWrite(IN1_RR, LOW); digitalWrite(IN2_RR, HIGH);
}

void strafeRight() {
  digitalWrite(IN1_FL, HIGH); digitalWrite(IN2_FL, LOW);
  digitalWrite(IN1_RL, LOW); digitalWrite(IN2_RL, HIGH);
  digitalWrite(IN1_FR, LOW); digitalWrite(IN2_FR, HIGH);
  digitalWrite(IN1_RR, HIGH); digitalWrite(IN2_RR, LOW);
}

void rotateLeft() {
  digitalWrite(IN1_FL, LOW); digitalWrite(IN2_FL, HIGH);
  digitalWrite(IN1_RL, LOW); digitalWrite(IN2_RL, HIGH);
  digitalWrite(IN1_FR, HIGH); digitalWrite(IN2_FR, LOW);
  digitalWrite(IN1_RR, HIGH); digitalWrite(IN2_RR, LOW);
}

void rotateRight() {
  digitalWrite(IN1_FL, HIGH); digitalWrite(IN2_FL, LOW);
  digitalWrite(IN1_RL, HIGH); digitalWrite(IN2_RL, LOW);
  digitalWrite(IN1_FR, LOW); digitalWrite(IN2_FR, HIGH);
  digitalWrite(IN1_RR, LOW); digitalWrite(IN2_RR, HIGH);
}

void stop() {
  digitalWrite(IN1_FL, LOW); digitalWrite(IN2_FL, LOW);
  digitalWrite(IN1_RL, LOW); digitalWrite(IN2_RL, LOW);
  digitalWrite(IN1_FR, LOW); digitalWrite(IN2_FR, LOW);
  digitalWrite(IN1_RR, LOW); digitalWrite(IN2_RR, LOW);
}
