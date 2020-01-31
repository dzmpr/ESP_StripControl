#ifndef OTAUpdater_h
#define OTAUpdater_h
    #include <Arduino.h>
    #include <ESP8266httpUpdate.h>

    #define VER "00008040"

    #ifndef DEBUG_BUILD
        #define VERSION VER "-light"
    #else
        #define VERSION VER "-light-d"
    #endif    

    //  Defined for debug (Serial)
    #ifndef DEBUG_BUILD
        #define DEBUG(...)
        #define DEBUG_S(...)
        #define DEBUG_F(...)
    #else
        #define DEBUG(...) Serial.println(__VA_ARGS__);
        #define DEBUG_S(...) Serial.print(__VA_ARGS__);
        #define DEBUG_F(...) Serial.printf(__VA_ARGS__);
    #endif

    bool OTAUpdate(String, BearSSL::WiFiClientSecure*,uint8_t);

#endif