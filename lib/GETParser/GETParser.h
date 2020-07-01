/**
 * 
 * 
 */
#ifndef GETParser_h
#define GETParser_h
    #include <Arduino.h>

    #ifndef DEBUG_BUILD
        #define DEBUG(...)
        #define DEBUG_S(...)
        #define DEBUG_F(...)
    #else
        #define DEBUG(...) Serial.println(__VA_ARGS__);
        #define DEBUG_S(...) Serial.print(__VA_ARGS__);
        #define DEBUG_F(...) Serial.printf(__VA_ARGS__);
    #endif

    class GETParser {
    private:
        String _target;
        const char* _targetPtr;
        bool _parseError = false;
    public:
        template <typename T>
        T parseUint(const char*);

        GETParser();
        GETParser(String);
        void replaceSource(String);

        bool isErrorOccured();
        String parseString(const char*);
    };

    template <typename T>
    T GETParser::parseUint(const char* key) {
        //TODO: do error check
        _parseError = false;
        char *ptr, *eq;
        T dest = 0;
        ptr = strstr(_targetPtr, key);//Check presence of target key
        if (ptr != nullptr) {//Key present
            eq = strstr(ptr, "=");//Get pointer to equal sign (start of value)
            if (eq != nullptr && atol(eq+1) != 0) {//Pointer is valid and 
                dest = atol(eq+1);
                DEBUG("[PARSER]Parsed "+ String(key) +" as \"" + String(dest) + "\".");
                return dest;
            }
        }
        DEBUG("[PARSER]Parsing key \"" + String(key) + "\" error.");
        _parseError = true;
        return 0;
    }

#endif