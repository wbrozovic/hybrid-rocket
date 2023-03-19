#include <Arduino.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <heltec.h>

/**
 * U8g2 Constructor for ESP32
 * U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/
// U8X8_PIN_NONE);

#define OLED_CLOCK 15 // PINS FOR THE OLED DISPLAY
#define OLED_DATA 4
#define OLED_RESET 16

#define BUTTON 0

#define ENG_IGNITE_OUTPUT 21

#define HX711_CLK_PIN_INPUT 22
#define HX711_DATA_PIN_INPUT 23

#define OX_LEVEL_SHIFT_1_INPUT 34
#define OX_LEVEL_SHIFT_2_INPUT 35

#define LEVEL_SHIFT_3_OUTPUT 14
#define LEVEL_SHIFT_4_OUTPUT 13
#define LEVEL_SHIFT_5_OUTPUT 12

// Replace with your network credentials
const char ***REMOVED*** = "DMS Member"; //"HottyToddy"
const char ***REMOVED*** = "dms--109238"; //"104Tanglewood"

AsyncWebServer server(80);

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(LED_BUILTIN)){
      return "ON";
    } else {
      return "OFF";
    }
  }
  return String();
}

// OLED Constructor
U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);
int g_lineHeight = 0;
int buttonState = 0;

static void initPins()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT);

  pinMode(ENG_IGNITE_OUTPUT, OUTPUT);
  pinMode(HX711_CLK_PIN_INPUT, INPUT);
  pinMode(HX711_DATA_PIN_INPUT, INPUT);

  pinMode(OX_LEVEL_SHIFT_1_INPUT, INPUT);
  pinMode(OX_LEVEL_SHIFT_2_INPUT, INPUT);
  pinMode(LEVEL_SHIFT_3_OUTPUT, OUTPUT);
  pinMode(LEVEL_SHIFT_4_OUTPUT, OUTPUT);
  // pinMode(LEVEL_SHIFT_5, OUTPUT);
  pinMode(LEVEL_SHIFT_5_OUTPUT, OUTPUT);
}

  
void DrawIPToOLED()
{
  Heltec.display -> clear();
  Heltec.display -> drawString(0, 0, "Engine Test Stand v0.1");
  Heltec.display -> drawString(0, 15, (WiFi.localIP().toString()));
  Heltec.display -> drawString(0, 50, "Count OFF");
  Heltec.display -> display();
}

void DrawStringToOLED(String str)
{
  Heltec.display -> clear();
  Heltec.display -> drawString(0, 0, "Engine Test Stand v0.1");
  Heltec.display -> drawString(0, 15, (WiFi.localIP().toString()));
  Heltec.display -> drawString(0, 50, "New String: " + str);
  Heltec.display -> display();
}

void initWifi() 
{
  //Connect to WiFi network
  Serial.println("Connecting to WiFi network: " + String(ssid));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.printf("Connected to the WiFi network %s", WiFi.localIP().toString());
  
  // Route for favicon.ico
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/favicon.ico", "image/avif");
  });

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/control-center.html", "text/html");
  });

  // Route for CSS
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/styles.css", "text/css");
  });

  // Route for JavaScript
  server.on("/control-center.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/control-center.js", "text/javascript");
  });

  // GET Oxidizer value
  server.on("/oxidizer", HTTP_GET, [](AsyncWebServerRequest *request) {
    String oxidizerValue = String(analogRead(OX_LEVEL_SHIFT_1_INPUT));
    Serial.printf("Oxidizer Value: %s", oxidizerValue);
    Serial.println();
    request->send(200, "text/plain", oxidizerValue);
  });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("control-center.html");
  server.begin();
  DrawIPToOLED();
}

void setup()
{
  Serial.begin(115200);
  
  initPins();

  // Mount SPIFFS
  String spiffsError = "An Error has occurred while mounting SPIFFS";
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    DrawStringToOLED(spiffsError);
    return;
  }
  digitalWrite(LED_BUILTIN, HIGH);

  //Display IP Address on Heltec OLED
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, 470E6 /**/);
  
  delay(2000);
  initWifi();
  digitalWrite(BUTTON, LOW);
  
}


void loop()
{

}

