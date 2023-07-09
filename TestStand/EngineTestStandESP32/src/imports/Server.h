#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <functional>

class CustomAsyncWebServer {
public:
    using OxidizerValveCallback = std::function<int()>;
    // Thrust readingCallback returns an array of float
    using ThrustReadingCallback = std::function<String()>;
    using CommandSparkPlugCallback = std::function<String()>;

    using HandleLogCallback = std::function<String()>;

    CustomAsyncWebServer(int port): server(port) {
        server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            request->send(SPIFFS, "/favicon.ico", "image/avif"); 
        });
        // Root
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            request->send(SPIFFS, "/control-center.html", "text/html"); 
        });
        server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            request->send(SPIFFS, "/styles.css", "text/css"); 
        });
        server.on("/control-center.js", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            request->send(SPIFFS, "/control-center.js", "text/javascript"); 
        });
        server.on("/asciichart.js", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            request->send(SPIFFS, "/asciichart.js", "text/javascript"); 
        });
        
    }

    void begin() {
        server.begin();
    }

    void SetOxidizerTickOpenHandler(OxidizerValveCallback oxidizerValveCallback) 
    {
        server.on("/oxidizer-tick-open", HTTP_GET, [this, oxidizerValveCallback](AsyncWebServerRequest *request)
        {
            int encoderCount = oxidizerValveCallback();
            request->send(200, "text/plain", String(encoderCount)); 
        });
    }

    void SetOxidizerTickCloseHandler(OxidizerValveCallback oxidizerValveCallback) 
    {
        server.on("/oxidizer-tick-close", HTTP_GET, [this, oxidizerValveCallback](AsyncWebServerRequest *request)
        {
            int encoderCount = oxidizerValveCallback();
            request->send(200, "text/plain", String(encoderCount)); 
        });
    }

    void SetThrustReadingHandler(ThrustReadingCallback thrustReadingCallback) 
    {
        server.on("/thrust-load", HTTP_GET, [this, thrustReadingCallback](AsyncWebServerRequest *request)
        {
            String thrustReading = thrustReadingCallback();
            
            request->send(200, "text/plain", thrustReading);
        });
    }

    void CommandSparkPlug(CommandSparkPlugCallback commandSparkPlugCallback) 
    {
        server.on("/spark", HTTP_GET, [this, commandSparkPlugCallback](AsyncWebServerRequest *request)
        {
            String sparkPlugResponse = commandSparkPlugCallback();

            request->send(200, "text/plain", sparkPlugResponse);
        });
    }

    void HandleLog(HandleLogCallback handleLogCallback) 
    {
        server.on("/log", HTTP_GET, [this, handleLogCallback](AsyncWebServerRequest *request)
        {
            String loggingStatus = handleLogCallback();

            request->send(200, "text/plain", "Logging status: " + loggingStatus);
        });
    }

    private:
        AsyncWebServer server;
};