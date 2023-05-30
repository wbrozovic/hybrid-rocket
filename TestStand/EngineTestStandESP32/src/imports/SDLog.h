#ifndef SDLog_h
#define SDLog_h

#include "Arduino.h"
#include <SD.h>

class SDLog {
public:
    SDLog(int chipSelectPin);
    ~SDLog();
    void printCardInfo();
    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void openLogFile(const String& filename);
    void writeToLogFile(const String& data);
    void closeLogFile();
    void writeFirstLines();

private:
    int _csPin;
    File _logFile;
};

#endif
