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
    _responseCode = _https.GET();//Make request, get result code
    DEBUG("[NETWORK]Request sent.");
    if (_responseCode == 200) {//Request succeed
        _newResponse = _https.getString();//Save response
        DEBUG("[NETWORK]Response: " + _newResponse);
        if (_response == _newResponse) {
            return;
        } else {//When new data retrieved
            DEBUG("[NETWORK]Old response: " + _response);
            _response = _newResponse;//Replace old data
            _isNewData = true;//Mark that new data got
            return;
        }
    }
    //Error case
    _isNewData = false;//Mark that new data isn't retrieved
    _errorCounter++;//Increase error counter
    DEBUG_S("[NETWORK]Request error. Response code: ");
    DEBUG(_responseCode);
    return;
}

void NetworkHandler::changeURL(String url) {
    _url = url;
    _https.setURL(_url);
}

uint16_t NetworkHandler::getErrorCount() {
    return _errorCounter;
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
