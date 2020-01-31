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
            DEBUG("[PARSER]Parsed " + String(key) + " as \"" + result + "\"");
            return result;
        } else if (eq != nullptr) {
            result = _target.substring(strlen(_targetPtr) - strlen(eq+1),strlen(_targetPtr));
            DEBUG("[PARSER]Parsed " + String(key) + " as \"" + result + "\"");
            return result;
        }
    }
    DEBUG("[PARSER]Parsing \"" + String(key) + "\" error. Wrong request.");
    _parseError = true;
    return result;
}

