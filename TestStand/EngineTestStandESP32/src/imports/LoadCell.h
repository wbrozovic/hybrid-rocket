#include "HX711.h" //This library can be obtained here http://librarymanager/All#Avia_HX711

#define LOADCELL_DOUT_PIN  23
#define LOADCELL_SCK_PIN  22

HX711 scale;

bool RunCalibrationLoop = true;
float calibration_factor_hard_coded = 71490; 

class LoadCell 
{
    public:
        LoadCell(int dout_pin, int loadcell_sck_pin, float calibration_factor) : 
            dout_pin(dout_pin), 
            loadcell_sck_pin(loadcell_sck_pin),
            calibration_factor(calibration_factor)
        {
            // StartCalibration();
        }
    
    void initLoadCell()
    {
        scale.begin(dout_pin, loadcell_sck_pin);
        scale.set_scale(calibration_factor);
        scale.tare();
    }

    // Calibration functions courtesy of Spark Fun's implementation
    float StartCalibration()
    {
        Serial.println("HX711 calibration sketch");
        Serial.println("Remove all weight from scale");
        Serial.println("After readings begin, place known weight on scale");
        Serial.println("Press + or a to increase calibration factor");
        Serial.println("Press - or z to decrease calibration factor");

        initLoadCell();

        long zero_factor = scale.read_average();    //Get a baseline reading
        
        Serial.print("Zero factor: ");              //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
        Serial.println(zero_factor);
        
        if (!RunCalibrationLoop)
            RunCalibrationLoop = true;
        
        CalibrationLoop();
        
        return calibration_factor;
    }


    void StopCalibration()
    {
        RunCalibrationLoop = false;
    }

    float getLoad() 
    {
        return scale.get_units();
    }

    private:
        int dout_pin;
        int loadcell_sck_pin;
        float calibration_factor;

    void CalibrationLoop()
    {
        while (RunCalibrationLoop)
        {
            scale.set_scale(calibration_factor); //Adjust to this calibration factor

            Serial.print("Reading: ");
            Serial.print(scale.get_units(), 1);
            Serial.print(" lbs"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
            Serial.print(" calibration_factor: ");
            Serial.print(calibration_factor);
            Serial.println();

            if (Serial.available())
            {
                char temp = Serial.read();
                String inputString = String(Serial.read());

                if(temp == '+' || temp == 'a') 
                    calibration_factor += 10 * inputString.length();
                else if(temp == '-' || temp == 'z')
                    calibration_factor -= 10 * inputString.length();
            }
        }
    }

};