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
    String _url;//Endpoint address
    BearSSL::WiFiClientSecure _client;
    HTTPClient _https;
    bool _isNewData = true;//Is new data were got
    uint16_t _errorCounter = 0;
    int16_t _responseCode;//Last request result
    String _response;//Stored response
    String _newResponse;//Buffer for new response
public:
    NetworkHandler() = delete;
    NetworkHandler(String, uint8_t*);
    void makeRequest();
    void changeURL(String);//Change endpoint address
    String getResponse();//Get response data
    int16_t getResponseCode();//Get response result
    uint16_t getErrorCount();
    void dataProcessed();//Mark data processed
    inline bool isConnected();//Check connection
    inline bool newDataStatus();//Get data status
};

inline bool NetworkHandler::isConnected() {
    return _https.connected();
}

inline bool NetworkHandler::newDataStatus() {
    return _isNewData;
}

#endif