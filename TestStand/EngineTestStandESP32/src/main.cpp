/* #region Imports */

#include <Arduino.h>
// #include <ArduinoJson.h>

// #include <chrono>
// #include <ctime>
// #include <iomanip>
// #include <sstream>

#include <SD.h>
#include <SPI.h>
#include <FS.h>

#include <U8g2lib.h>
#include <WiFi.h>
#include "SPIFFS.h"

#include <heltec.h>
#include <ESP32Encoder.h>
#include <HX711.h>

#include "./imports/Motor.h"
#include "./imports/Server.h"
#include "./imports/LoadCell.h"
#include "./imports/local_wifi_cred.h"
#include "./imports/SDLog.h"

/* #endregion Imports */

#define OLED_CLOCK 15 // PINS FOR THE OLED DISPLAY
#define OLED_RESET 16

#define BUTTON 0

#define ENG_IGNITE_OUTPUT 21

#define HX711_CLK_PIN_INPUT 22
#define HX711_DATA_PIN_INPUT 4

#define OX_ANGLE_ENCODER_1_IN 34
#define OX_ANGLE_ENCODER_2_IN 35

#define ENABLE_A_OUT 14
#define INPUT_2 13
#define INPUT_1 12

#define MISO 19
#define MOSI 23
#define SCK 18
#define CS 2
#define DEFAULT_CS -1

const char *ssid = SSID;
const char *password = PASSWORD;



bool isLogging = false;

ESP32Encoder encoder;
ESP32Encoder encoder2;

CustomAsyncWebServer myServer(80);

Motor motor = Motor(ENABLE_A_OUT, INPUT_1, INPUT_2);
LoadCell loadCell = LoadCell(HX711_DATA_PIN_INPUT, HX711_CLK_PIN_INPUT, 71490);

File myFile;

SDLog sdLog(DEFAULT_CS);

static void initPins()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT);

  pinMode(ENG_IGNITE_OUTPUT, OUTPUT);
  pinMode(HX711_CLK_PIN_INPUT, INPUT);
  pinMode(HX711_DATA_PIN_INPUT, INPUT);

  pinMode(ENABLE_A_OUT, OUTPUT);
  pinMode(INPUT_2, OUTPUT);

  pinMode(INPUT_1, OUTPUT);
}

void DrawIPToOLED()
{
  Heltec.display->displayOn();
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Engine Test Stand v0.1");
  Heltec.display->drawString(0, 15, (WiFi.localIP().toString()));
  //Heltec.display->drawString(0, 50, "Count OFF");
  Heltec.display->display();
}

void DrawStringToOLED(String str)
{
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Engine Test Stand v0.1");
  //Heltec.display->drawString(0, 15, (WiFi.localIP().toString()));
  Heltec.display->drawString(0, 50, "New String: " + str);
  Heltec.display->display();
}

int MeasureMotorCurrent()
{
  //Placeholder
  int L298N_CURRENT_SENSE = 0;
  int current = 0;
  for (int i = 0; i < 10; i++)
  {
    current += analogRead(L298N_CURRENT_SENSE);
  }
  return current / 10;
}

int EstimateMotorTorqueWithCurrentFlow()
{
  //Placeholder
  int L298_PLACEHOLDER = 0;
  int torque = 0;
  int current = MeasureMotorCurrent();
  /**
   * 1. Get current flow
   * 2. If current flow is >= SOME_THRESHOLD
   * 2a. Then valve == closed, stop rotation, notify client
   * 2b. Else valve == open, continue rotation
   * 
   */
  return torque / 10;
}

void InitServerEndpoints() 
{
  
  myServer.SetOxidizerTickOpenHandler([](){
    motor.stop();
    for (int i = 0; i < 5000000; i++)
    {
      motor.RotateMotorForwards();
      
    }
    motor.stop();
    return encoder.getCount();
  });

  myServer.SetOxidizerTickCloseHandler([](){
    motor.stop();
    for (int i = 0; i < 5000000; i++)
    {
      motor.RotateMotorBackwards();
    }
      motor.stop();
    
    //delay(1000);
    motor.stop();
    return encoder.getCount();
  });

  myServer.SetThrustReadingHandler([](){
    String thrustReadings = "{\"thrust\": [";
    for (int i = 0; i < 10; i++)
    {
      Serial.println("Thrust Reading: " + thrustReadings);
      thrustReadings += i < 9 ? String(loadCell.getLoad()) + ", " : String(loadCell.getLoad());
    }

    return thrustReadings + "]}";
  });

  myServer.CommandSparkPlug([](){
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(ENG_IGNITE_OUTPUT, HIGH);
      
      long mili_start = millis();
      long mili_end = millis();

      while(mili_end - mili_start < 30)
      {
        //do nothing
        mili_end = millis();
      }
      digitalWrite(ENG_IGNITE_OUTPUT, LOW);

      mili_start = millis();
      mili_end = millis();
      while(mili_end - mili_start < 30)
      {
        //do nothing
        mili_end = millis();
      }
    }
    return "Spark Plug Ignited";
  });

  myServer.HandleLog([](){
    isLogging = !isLogging;
    return isLogging ? "true" : "false";
  });

}

void InitWifi()
{
  // Connect to WiFi network
  Serial.println("Connecting to WiFi network: " + String(ssid));

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("IP address: " + (WiFi.localIP().toString()));
  DrawIPToOLED();
  InitServerEndpoints();
}

void InitAngleEncoder()
{
  ESP32Encoder::useInternalWeakPullResistors=UP;

	// use pin 19 and 18 for the first encoder
	encoder.attachHalfQuad(OX_ANGLE_ENCODER_1_IN, OX_ANGLE_ENCODER_2_IN);
		
	// set starting count value after attaching
	//encoder.setCount(0);

	// clear the encoder's raw count and set the tracked count to zero
	encoder.clearCount();
	Serial.println("Encoder Start = " + String((int32_t)encoder.getCount()));
}

void setup()
{
  Serial.begin(115200);

  initPins();
  //DrawStringToOLED("Initializing...");
  // Mount SPIFFS
  String spiffsError = "An Error has occurred while mounting SPIFFS";
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    DrawStringToOLED(spiffsError);
    return;
  }

  Heltec.begin(true, false, true, true, 470E6);

  delay(2000);

  InitWifi();
  InitAngleEncoder();

  digitalWrite(LED_BUILTIN, HIGH);

  myServer.begin();
  loadCell.initLoadCell();

  
  sdLog.listDir(SD, "/", 0);
  sdLog.openLogFile("/Logging.txt");
  sdLog.writeFirstLines();
  DrawIPToOLED();
}

void checkEncoderState()
{
  // Loop and read the count
	//Serial.println("Encoder count = " + String((int32_t)encoder.getCount()));
	delay(100);
  encoder.resumeCount();
	delay(100);
  encoder.pauseCount();
}

// String time_point_to_string(const std::chrono::system_clock::time_point& tp) {
//     // Convert time_point to time_t.
//     std::time_t time_t = std::chrono::system_clock::to_time_t(tp);

//     // Convert time_t to struct tm.
//     std::tm tm = *std::localtime(&time_t);

//     // Use strftime to format struct tm as a string.
//     std::ostringstream oss;
//     oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

//     return String(oss.str().c_str());
// }


void loop()
{
  // get the current time
 // auto now = std::chrono::high_resolution_clock::now();

  // check if a second has passed
  if (isLogging) {
      // convert now to a string
     // String now_str = time_point_to_string(now);

      //Time,Load,Encoder Count
      String logData = "," + String(loadCell.getLoad()) + "," + String(encoder.getCount());
      sdLog.writeToLogFile(logData);
      Serial.println(logData);
  }
}
