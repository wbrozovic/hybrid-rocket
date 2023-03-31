#include <Arduino.h>
// #include <heltec.h>
// #include <ESP32Encoder.h>

/**
 * @brief Class for controlling a motor
 * @param enablePin The pin that controls the enable pin of the motor driver
 * @param input1Pin The pin that controls the input 1 pin of the motor driver
 * @param input2Pin The pin that controls the input 2 pin of the motor driver
 * 
 */
class Motor 
{
    public: 
        Motor(int enablePin, int input1Pin, int input2Pin) : 
            enablePin(enablePin), 
            input1Pin(input1Pin), 
            input2Pin(input2Pin) 
    {
        pinMode(enablePin, OUTPUT);
        pinMode(input1Pin, OUTPUT);
        pinMode(input2Pin, OUTPUT);
    }

    void RotateMotorForwards()
    {
        digitalWrite(enablePin, HIGH);
        digitalWrite(input2Pin, LOW);
        digitalWrite(input1Pin, HIGH);
    }

    void RotateMotorBackwards()
    {
        digitalWrite(enablePin, HIGH);
        digitalWrite(input2Pin, HIGH);
        digitalWrite(input1Pin, LOW);
    }

    void stop()
    {
        digitalWrite(input2Pin, LOW);
        digitalWrite(input1Pin, LOW);
    }

    private:
        int enablePin;
        int input1Pin;
        int input2Pin;
};