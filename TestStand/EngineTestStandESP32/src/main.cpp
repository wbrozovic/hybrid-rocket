#include <Arduino.h>
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

#define LEVEL_SHIFT_1_INPUT 34
#define LEVEL_SHIFT_2_INPUT 35

#define LEVEL_SHIFT_3_OUTPUT 14
#define LEVEL_SHIFT_4_OUTPUT 13
#define LEVEL_SHIFT_5_OUTPUT 12

// Replace with your network credentials
const char *ssid = "HottyToddy";//"DMS Member";
const char *password = "104Tanglewood";  //"dms--109238";

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

  pinMode(LEVEL_SHIFT_1_INPUT, INPUT);
  pinMode(LEVEL_SHIFT_2_INPUT, INPUT);
  pinMode(LEVEL_SHIFT_3_OUTPUT, OUTPUT);
  pinMode(LEVEL_SHIFT_4_OUTPUT, OUTPUT);
  // pinMode(LEVEL_SHIFT_5, OUTPUT);
  pinMode(LEVEL_SHIFT_5_OUTPUT, OUTPUT);
}

void configureWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  g_OLED.setCursor(3, g_lineHeight * 2 + 10);
  Serial.println("Connected to the WiFi network");

  g_OLED.setCursor(3, g_lineHeight * 4 + 10);
  g_OLED.printf("Connected to\n %s", WiFi.localIP().toString().c_str());
  // g_OLED.sendBuffer();
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

  // GET Oxidizer value change
  server.on("/oxidizer?oxVal=", HTTP_GET, [](AsyncWebServerRequest *request) {
    String newOxidizerValue = request->arg("oxVal");
    Serial.println("Oxidizer Value: " + newOxidizerValue);
    request->send(200, "text/plain", "Oxidizer Value: " + newOxidizerValue);
    DrawStringToOLED(newOxidizerValue);
    //request->method() == HTTP_POST;
  });

  server.begin();
  DrawIPToOLED();
}

void setup()
{
  Serial.begin(115200);
  
  initPins();


  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
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
  buttonState = digitalRead(BUTTON);
  if (buttonState == HIGH)
  {
    //Serial.println("Button Pressed");
    digitalWrite(ENG_IGNITE_OUTPUT, LOW);
  }
  else
  {
    //Serial.println("Button Not Pressed");
    digitalWrite(ENG_IGNITE_OUTPUT, HIGH);
  }
  delay(10);

  // digitalWrite(ENG_IGNITE_OUTPUT, HIGH);
  // delay(10);
  // digitalWrite(ENG_IGNITE_OUTPUT, LOW);
  

  //digitalWrite(LED_BUILTIN, HIGH);
}


// const char html[] PROGMEM = R"(
// <!DOCTYPE html>
// <html>
// <head>
//   <title>Geiger Counter</title>
//   <style>
//     body {
//       font-family: Arial, sans-serif;
//       margin: 0;
//       padding: 0;
//       background-color: #f2f2f2;
//     }

//     h1 {
//       background-color: #333;
//       color: white;
//       padding: 10px;
//       text-align: center;
//       margin: 0;
//     }

//     form {
//       margin-top: 20px;
//       display: flex;
//       flex-direction: column;
//       align-items: center;
//     }

//     label {
//       font-size: 18px;
//       margin-bottom: 10px;
//     }

//     input[type=number] {
//       padding: 10px;
//       border: none;
//       border-radius: 5px;
//       box-shadow: 0px 0px 5px rgba(0, 0, 0, 0.3);
//       width: 100%;
//       max-width: 200px;
//       margin-bottom: 20px;
//       font-size: 16px;
//       text-align: center;
//     }

//     #counterValue {
//       font-size: 48px;
//       text-align: center;
//       margin-top: 50px;
//     }

//     #switchButton {
//       background-color: #4CAF50;
//       color: white;
//       padding: 10px;
//       border: none;
//       border-radius: 5px;
//       box-shadow: 0px 0px 5px rgba(0, 0, 0, 0.3);
//       width: 100%;
//       max-width: 200px;
//       cursor: pointer;
//       transition: background-color 0.3s ease;
//       font-size: 18px;
//     }

//     #switchButton:hover {
//       background-color: #3e8e41;
//     }

//         /* Style the container */
//     .container {
//       text-align: center;
//       margin: 20px auto;
//       max-width: 500px;
//       background-color: white;
//       border-radius: 5px;
//       box-shadow: 0px 0px 5px rgba(0, 0, 0, 0.3);
//       padding: 20px;
//     }
//   </style>
//   <script>
//     var refreshInterval = 1;

//     function updateCounterValue() {
//       fetch('/counter')
//         .then(response => response.text())
//         .then(data => {
//           document.getElementById('counterValue').innerText = data;
//         });
//     }

//     setInterval(updateCounterValue, refreshInterval * 1000);

//     function handleRefreshIntervalChange() {
//       refreshInterval = document.getElementById('refreshInterval').value;
//       clearInterval();
//       setInterval(updateCounterValue, refreshInterval * 1000);
//     }

//     function switchCounter() {
//       fetch('/switchCounter');
//     }
//   </script>
// </head>
// <body>
//   <h1>Geiger Counter</h1>
//   <div class='container'>
//     <form>
//       <label for='refreshInterval'>Refresh Interval (in seconds):</label>
//       <input type='number' id='refreshInterval' value='1' onchange='handleRefreshIntervalChange()'>
//     </form>
//        <div>
//    <button id='switchButton' onclick='switchCounter()'>Switch (ON/OFF)</button>
//  </div>
//     <div id='counterValue'>0</div>
//   </div>
// </html>
//   )";