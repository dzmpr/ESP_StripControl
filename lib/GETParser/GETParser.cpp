/**
 * 
 * 
 * 
 */

#include "GETParser.h"

GETParser::GETParser(String target): _target(target) {
    _targetPtr = _target.c_str();
};

GETParser::GETParser() {
    _targetPtr = nullptr;
}

void GETParser::replaceSource(String newTarget) {
    _target = newTarget;
    _targetPtr = _target.c_str();
}

bool GETParser::isErrorOccured() {
    return _parseError;
}

String GETParser::parseString(const char* key) {
    _parseError = false;
    char *ptr, *eq, *amp;
    String result;
    ptr = strstr(_targetPtr, key);
    if (ptr != nullptr) {
        eq = strstr(ptr, "=");
        amp = strstr(ptr, "&");
        if (amp == nullptr) {
            amp = strstr(ptr, " ");
        }
        if (eq != nullptr && amp != nullptr) {
            result = _target.substring(strlen(_targetPtr) - strlen(eq+1),strlen(_targetPtr) - strlen(amp));
            DEBUG_N("[DEBUG]Parsed " + String(key) + " as: " + result);
            return result;
        } else if (eq != nullptr) {
            result = _target.substring(strlen(_targetPtr) - strlen(eq+1),strlen(_targetPtr));
            DEBUG_N("[DEBUG]Parsed " + String(key) + " as: " + result);
            return result;
        }
    }
    DEBUG_N("[DEBUG]Parsing " + String(key) + " error. Wrong request.");
    _parseError = true;
    return result;
}

uint8_t GETParser::parseHexByte(char* hexnum) {
    _parseError = false;
    uint8_t result = 0;
    for (uint8_t i = 0; i < 2; i++) {
        uint8_t num = *(hexnum+i) - '0';
        if (num > 16 && num < 23) {
            result += (num-7) << 4*(1-i);
            continue;
        }
        if (num < 10) {
            result += num << 4*(1-i);
        }
    }
    return result;
}
