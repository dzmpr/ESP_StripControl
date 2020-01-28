/**
 * 
 * 
 * 
 * 
 */

#include "NetworkHandler.h"

#ifndef DEBUG_NH
    #define DEBUG_N(x)
    #define DEBUG_S(x)
#else
    #define DEBUG_N(x) Serial.println(x);
    #define DEBUG_S(x) Serial.print(x);
#endif

NetworkHandler::NetworkHandler(String url, uint8_t* certificate) {
    _url = url;
    _client.setFingerprint(certificate);
    _https.setReuse(true);
    _https.setUserAgent("Light_" + String(ESP.getChipId()));
    _https.begin(_client, _url);
}

void NetworkHandler::makeRequest() {
    _responseCode = _https.GET();
    DEBUG_N("[DEBUG]Request sent.");
    if (_responseCode == 200) {//Request succeed 
        _newResponse = _https.getString();//Save response
        DEBUG_N("[DEBUG]Old response: " + _response);
        DEBUG_N("[DEBUG]New response: " + _newResponse);
        if (_response == _newResponse) {
            return;
        } else {//When new data retrieved
            _response = _newResponse;//Replace old data
            _isNewData = true;//Mark that new data got
            return;
        }
    }
    _isNewData = false;
    _errorCounter++;
    DEBUG_S("[DEBUG]Request error. Response code: ");
    DEBUG_N(_responseCode);
    return;
}

inline bool NetworkHandler::newDataStatus() {
    return _isNewData;
}

String NetworkHandler::getResponse() {
    return _response;
}

int16_t NetworkHandler::getResponseCode() {
    return _responseCode;
}

void NetworkHandler::dataProcessed() {
    _isNewData = false;
}
