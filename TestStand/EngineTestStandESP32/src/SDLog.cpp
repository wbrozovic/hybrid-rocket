#include "./imports/SDLog.h"
#include <SD.h>
#include <SPI.h>

SDLog::SDLog(int chipSelectPin) : _csPin(chipSelectPin) {
    switch (chipSelectPin >= 0) {
        case true:
            if (!SD.begin(_csPin)) {
                Serial.println("Card failed, or not present");
            } else {
                Serial.println("Card initialized.");
                printCardInfo();
            }
            break;
        case false:
            if (!SD.begin()) {
                Serial.println("Card failed, or not present");
            } else {
                Serial.println("Card initialized.");
                printCardInfo();
            }
            break;
    }
}

SDLog::~SDLog() {
    SD.end();
}

void SDLog::printCardInfo() {
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
}

void SDLog::listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void SDLog::writeFirstLines() {
    String timeStamp = String(__DATE__) + " " + String(__TIME__);
    String header = "Log Session started at " + timeStamp;
    String header2 = "----------------------------------------------------------";
    String columns = "Time,Load,Encoder Count";
    writeToLogFile(header2);
    writeToLogFile(header);
    writeToLogFile(header2);
    writeToLogFile(columns);
}

void SDLog::openLogFile(const String& filename) {
    if (!SD.exists(filename)) {
        // If the file doesn't exist, create it.
        _logFile = SD.open(filename, FILE_WRITE);
    } else {
        // If the file exists, open it in append mode.
        _logFile = SD.open(filename, FILE_APPEND);
    }
}

// TODO: Modify this to accept a vector of data according to columns
void SDLog::writeToLogFile(const String& data) {
    if (_logFile) {
        _logFile.println(data);
        _logFile.flush();
    }
}

void SDLog::closeLogFile() {
    if (_logFile) {
        _logFile.close();
    }
}
