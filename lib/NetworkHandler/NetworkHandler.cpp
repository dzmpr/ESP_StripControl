/**
 * 
 * 
 * 
 * 
 */

#include "NetworkHandler.h"

NetworkHandler::NetworkHandler(String url, uint8_t* certificate) {
    _url = url;
    _client.setFingerprint(certificate);
    _https.setReuse(true);
    _https.setUserAgent("Light_" + String(ESP.getChipId()));
    _https.begin(_client, _url);
}

void NetworkHandler::makeRequest() {
    _responseCode = _https.GET();
    DEBUG("[DEBUG]Request sent.");
    if (_responseCode == 200) {//Request succeed 
        _newResponse = _https.getString();//Save response
        DEBUG("[DEBUG]Old response: " + _response);
        DEBUG("[DEBUG]New response: " + _newResponse);
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
    DEBUG(_responseCode);
    return;
}

void NetworkHandler::changeURL(String url) {
    _url = url;
    _https.setURL(_url);
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
