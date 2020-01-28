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
    String getResponse();
    int16_t getResponseCode();
    inline bool newDataStatus();
    void dataProcessed();
};

#endif