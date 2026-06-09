#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// =========================
// WIFI
// =========================
const char* ssid = "jiohouse2.4";
const char* password = "4561pahwa";

// =========================
// PINS
// =========================
#define IR_PIN      D1
#define GREEN_LED   D3
#define RED_LED     D4
#define FLAME_PIN   D5
#define RAIN_PIN    D6
#define TOUCH_PIN   D7
#define BUZZER_PIN  D8

ESP8266WebServer server(80);

// =========================
// JSON ENDPOINT
// =========================
void handleData()
{
  bool irDetected    = (digitalRead(IR_PIN) == LOW);
  bool fireDetected  = (digitalRead(FLAME_PIN) == LOW);
  bool rainDetected  = (digitalRead(RAIN_PIN) == LOW);
   bool touchDetected = (digitalRead(TOUCH_PIN) == HIGH);
  bool panicDetected = touchDetected;
 
  String json = "{";
  json += "\"fire\":" + String(fireDetected) + ",";
  json += "\"rain\":" + String(rainDetected) + ",";
  json += "\"ir\":" + String(irDetected) + ",";
  json += "\"touch\":" + String(touchDetected) + ",";
  json += "\"panic\":" + String(panicDetected);
  json += "}";

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", json);
}

// =========================
// SETUP
// =========================
void setup()
{
  Serial.begin(115200);

  pinMode(IR_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(TOUCH_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println();
  Serial.println("=================================");
  Serial.println("AEGIS-X AI DEFENSE HABITAT");
  Serial.println("=================================");

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/data", handleData);

  server.begin();

  Serial.println("Web Server Started");
}

// =========================
// LOOP
// =========================
void loop()
{
  server.handleClient();

  bool irDetected    = (digitalRead(IR_PIN) == LOW);
  bool fireDetected  = (digitalRead(FLAME_PIN) == LOW);
  bool rainDetected  = (digitalRead(RAIN_PIN) == LOW);
  bool touchDetected = (digitalRead(TOUCH_PIN) == HIGH);
  bool panicDetected=touchDetected;
  bool threat =
      irDetected ||
      fireDetected ||
      rainDetected ||
      touchDetected;

  if (threat)
  {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.print("IR:");
  Serial.print(irDetected);

  Serial.print(" Fire:");
  Serial.print(fireDetected);

  Serial.print(" Rain:");
  Serial.print(rainDetected);

  Serial.print(" Touch:");
  Serial.print(touchDetected);

  Serial.print(" Threat:");
  Serial.println(threat);

  delay(200);
}