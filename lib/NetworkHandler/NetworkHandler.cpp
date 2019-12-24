/**
 * 
 * 
 * 
 * 
 */

#include "NetworkHandler.h"

#ifndef DEBUG_NM
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
    _https.setUserAgent(""); //TODO:
    _https.begin(_client, _url);
}

void NetworkHandler::makeRequest() {
    _responseCode = _https.GET();
    DEBUG_N("[DEBUG]Request sent.");
    if (_responseCode == 200) {
        _newResponse = _https.getString();
        DEBUG_N("[DEBUG]Old response: " + _response);
        DEBUG_N("[DEBUG]New response: " + _newResponse);
        if (_response == _newResponse) {
            return;
        } else {
            _response = _newResponse;
            _isNewData = true;
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

void NetworkHandler::parseResponse() {
    
}

int16_t NetworkHandler::checkConnection() {
    return 0; //FIXME:
}