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

    ptr = strstr(_targetPtr, key);//Check key presence in string
    if (ptr != nullptr) {
        eq = strstr(ptr, "=");//Get pointer to equal sign (start of value)
        amp = strstr(ptr, "&");//Get pointer to ampersand sign (end of value)
        if (amp == nullptr) {//Unless ampersand found (last parameter)
            amp = strstr(ptr, " ");//Get pointer to space (alternative end of value)
        }
        if (eq != nullptr && amp != nullptr) {//Equal sign and ampersand was found
            result = _target.substring(strlen(_targetPtr) - strlen(eq+1),strlen(_targetPtr) - strlen(amp));
            DEBUG("[PARSER]Parsed " + String(key) + " as \"" + result + "\".");
            return result;
        } else if (eq != nullptr) {//Has found only equal sign
            result = _target.substring(strlen(_targetPtr) - strlen(eq+1),strlen(_targetPtr));
            DEBUG("[PARSER]Parsed " + String(key) + " as \"" + result + "\".");
            return result;
        }
    }
    DEBUG("[PARSER]Parsing \"" + String(key) + "\" error.");
    _parseError = true;
    return result;
}

