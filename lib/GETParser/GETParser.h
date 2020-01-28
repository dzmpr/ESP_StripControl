/**
 * 
 * 
 */
#ifndef GETParser_h
#define GETParser_h
    #include <Arduino.h>

    #ifndef DEBUG_GP
        #define DEBUG_N(x)
        #define DEBUG_S(x)
    #else
        #define DEBUG_N(x) Serial.println(x);
        #define DEBUG_S(x) Serial.print(x);
    #endif

    class GETParser {
    private:
        String _target;
        const char* _targetPtr;
        bool _parseError = false;
    public:
        GETParser(String target): _target(target) {
            _targetPtr = _target.c_str();
        };

        template <typename T>
        T parseUint(const char*);

        GETParser();
        GETParser(String);
        void replaceSource(String);

        bool isErrorOccured();
        String parseString(const char*);
        uint8_t parseHexByte(char*);
    };

    template <typename T>
    T GETParser::parseUint(const char* key) {
    _parseError = false;
    char *ptr, *eq;
    T dest = 0;
    ptr = strstr(_targetPtr, key);
    if (ptr != nullptr) {
        eq = strstr(ptr, "=");
        if (eq != nullptr && atol(eq+1) != 0) {
            dest = atol(eq+1);
            DEBUG_N("[DEBUG]Parsed "+ String(key) +" as: " + String(dest));
            return dest;
        }
    }
    DEBUG_N("[DEBUG]Parsing"+ String(key) +"error. Wrong request.");
    _parseError = true;
    return 0;
}
#endif