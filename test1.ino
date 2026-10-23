#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "MbugtiDeco";
const char* password = "1552mehboob";

const int LED_PIN = 4;
bool ledOn = false;

WebServer server(80);

void sendPage() {
  String html = "<!DOCTYPE html><html><head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<style>body{font-family:sans-serif;text-align:center;margin-top:60px}"
                "a{display:inline-block;padding:20px 40px;font-size:24px;"
                "background:#2d7;color:#fff;text-decoration:none;border-radius:10px}</style>"
                "</head><body><h1>ESP32-S3 LED</h1>";
  html += ledOn ? "<p>LED is ON</p><a href='/off'>Turn OFF</a>"
                : "<p>LED is OFF</p><a href='/on'>Turn ON</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Open this in your browser: http://");
  Serial.println(WiFi.localIP());

  server.on("/", sendPage);
  server.on("/on", []() {
    ledOn = true;
    digitalWrite(LED_PIN, HIGH);
    sendPage();
  });
  server.on("/off", []() {
    ledOn = false;
    digitalWrite(LED_PIN, LOW);
    sendPage();
  });
  server.begin();
}

void loop() {
  server.handleClient();
}