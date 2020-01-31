/**
 * 
 * 
 * 
 */
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#ifndef NetworkHandler_h
#define NetworkHandler_h


#ifndef DEBUG_BUILD
    #define DEBUG(...)
    #define DEBUG_S(...)
    #define DEBUG_F(...)
#else
    #define DEBUG(...) Serial.println(__VA_ARGS__);
    #define DEBUG_S(...) Serial.print(__VA_ARGS__);
    #define DEBUG_F(...) Serial.printf(__VA_ARGS__);
#endif

class NetworkHandler {
private:
    String _url;
    BearSSL::WiFiClientSecure _client;
    HTTPClient _https;
    bool _isNewData = true;
    uint16_t _errorCounter = 0;
    int16_t _responseCode;
    String _response;
    String _newResponse;
public:
    NetworkHandler(String, uint8_t*);
    void makeRequest();
    void changeURL(String);
    String getResponse();
    int16_t getResponseCode();
    inline bool newDataStatus();
    void dataProcessed();
};

#endif