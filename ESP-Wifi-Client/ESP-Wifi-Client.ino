#include <WiFi.h>
#include <WebServer.h>

/*  SSID & Password */
const char* ssid = "ESP32";  //  SSID 
const char* password = "12345678";  // Password 

/* IP Address untuk ESP32 */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

uint8_t pin_LED = LED_BUILTIN;
bool LED_status = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(pin_LED, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/led/on", handle_ledOn);
  server.on("/led/off", handle_ledOff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server aktif");
}
void loop() {
  server.handleClient();
  if (LED_status)
  {
    digitalWrite(pin_LED, HIGH);
  }
  else
  {
    digitalWrite(pin_LED, LOW);
  }

}

void handle_OnConnect() {
  LED_status = LOW;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", KirimHTML(LED_status));
}

void handle_ledOn() {
  LED_status = HIGH;
  Serial.println("LED Status: ON");
  server.send(200, "text/html", KirimHTML(true));
}

void handle_ledOff() {
  LED_status = LOW;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", KirimHTML(false));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String KirimHTML(uint8_t led_status) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Kontrol</h1>\n";
  ptr += "<h3>Akses Point</h3>\n";

  if (led_status)
  {
    ptr += "<p>LED Status: ON</p><a class=\"button button-off\" href=\"/led/on\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/led/off\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
