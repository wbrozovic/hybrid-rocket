#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
// Font files are stored in SPIFFS, so load the library
#include <FS.h>


#define AA_FONT_SMALL "NotoSansBold15"
#define AA_FONT_LARGE "NotoSansBold36"
#define AA_FONT_MONO  "NotoSansMonoSCB20" // NotoSansMono-SemiCondensedBold 20pt

// Define Screen Dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

// Define Font
#define FONT_WIDTH 12
#define FONT_HEIGHT 18

#define OX_ANGLE_ENCODER_1_OUT 37
#define OX_ANGLE_ENCODER_2_OUT 38

#define THRUST_MEASURE_OUT_1 35
#define THRUST_MEASURE_OUT_2 32

#define IGNITION_IN 33

#define OX_VALVE_1_IN 15
#define OX_VALVE_2_IN 13
#define OX_VALVE_3_IN 12

#define oxVal 28
#define oxVal2 25

TFT_eSPI tft = TFT_eSPI();
//TFT_eSprite spr = TFT_eSprite(&tft); // Sprite class needs to be invoked

int AngleEncoder1Val;
int AngleEncoder2Val;

int IgnitionVal;

int OxValve1Val1;
int OxValve1Val2;
int OxValve1Val3;

void initPins() 
{
    pinMode(OX_ANGLE_ENCODER_1_OUT, OUTPUT);
    pinMode(OX_ANGLE_ENCODER_2_OUT, OUTPUT);

    pinMode(THRUST_MEASURE_OUT_1, OUTPUT);
    pinMode(THRUST_MEASURE_OUT_2, OUTPUT);

    pinMode(IGNITION_IN, INPUT);

    pinMode(OX_VALVE_1_IN, INPUT);
    pinMode(OX_VALVE_2_IN, INPUT);
    pinMode(OX_VALVE_3_IN, INPUT);
}

void setup()
{
    Serial.begin(115200);
    
    initPins();
}

void loop()
{
    analogWrite(OX_ANGLE_ENCODER_1_OUT, 128);
    analogWrite(OX_ANGLE_ENCODER_2_OUT, 128);
    // Serial.printf("oxVal_1: %d, oxVal_2: %d\n", oxVal, oxVal2);
    // Serial.println();

    delay(50);
}