#include <Arduino.h>
#include <U8g2lib.h>

/**
 * U8g2 Constructor for ESP32
 * U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/
// U8X8_PIN_NONE);

#define OLED_CLOCK 15 // PINS FOR THE OLED DISPLAY
#define OLED_DATA 4
#define OLED_RESET 16

#define BUTTON 0

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);
int g_lineHeight = 0;
int buttonState = 0;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT);

  g_OLED.begin();
  g_OLED.clear();

  g_OLED.setFont(u8g2_font_profont15_tf);

  g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent(); // Calculate the height of the font -- Descent is a negative number, so add it to the ascent

  digitalWrite(LED_BUILTIN, HIGH);
}

void drawFrame(int fps)
{
  g_OLED.clearBuffer();
  g_OLED.home();

  g_OLED.drawFrame(0, 0, g_OLED.getDisplayWidth(), g_OLED.getDisplayHeight()); // Draw a frame around the display

  for (int x = 0; x < g_OLED.getDisplayWidth(); x += 4)
  {
    g_OLED.drawLine(x, 0, g_OLED.getDisplayWidth() - x, g_OLED.getHeight());
    ///delay(1);
  }

  g_OLED.setCursor(3, g_lineHeight * 2 + 10);
  g_OLED.printf("FPS: %d", fps);
  g_OLED.sendBuffer();
}

double FramesPerSecond(double seconds)
{
  static double framesPerSecond;
  framesPerSecond = (framesPerSecond * 0.9) + (1.0 / seconds * 0.1);

  return framesPerSecond;
}

void loop()
{
  static bool bLED = LOW;
  int fps = 0;

  for (;;)
  {
    buttonState = digitalRead(BUTTON);
    Serial.printf("Button State: %d", buttonState);

    if (buttonState == HIGH)
    {
      Serial.println("Button Pressed");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      Serial.println("Button Released");
      digitalWrite(LED_BUILTIN, LOW);
    }

    //bLED = !bLED;
    digitalWrite(LED_BUILTIN, bLED);

    double dStart = millis() / 1000.0;
    drawFrame(fps);
    double dEnd = millis() / 1000.0;
    fps = FramesPerSecond(dEnd - dStart);
  }
}