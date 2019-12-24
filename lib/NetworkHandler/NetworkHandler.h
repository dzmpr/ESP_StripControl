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
    uint8_t parseUint32(const char*, uint32_t*, const char*);
    uint8_t parseUint16(const char*, uint16_t*, const char*);
    uint8_t parseUint8(const char*, uint8_t*, const char*);
public:
    NetworkHandler(String, uint8_t*);
    void makeRequest();
    void parseResponse();
    inline bool newDataStatus();
    int16_t checkConnection();
};

#endif