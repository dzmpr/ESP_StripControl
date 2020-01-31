#ifndef SettingsServer_h
#define SettingsServer_h

#include <Arduino.h>
#include "GETParser.h"
#include <ESP8266WiFi.h>
#include "RGB_LED.h"
//  DNS Server
#include <DNSServer.h>

#ifndef DEBUG_BUILD
    #define DEBUG(...)
    #define DEBUG_S(...)
    #define DEBUG_F(...)
#else
    #define DEBUG(...) Serial.println(__VA_ARGS__);
    #define DEBUG_S(...) Serial.print(__VA_ARGS__);
    #define DEBUG_F(...) Serial.printf(__VA_ARGS__);
#endif

//Size of data
#define SSID_SIZE               (32+1)
#define PASS_SIZE               (64+1)
#define LINK_SIZE               (64+1)
#define TOKEN_SIZE              (8+1)

//  AP defined settings
#define AP_PASS "12341234"
#define AP_STATIC IPAddress(1,1,1,1),IPAddress(1,1,1,0),IPAddress(255,255,255,0)

// struct configuration;
// struct bootConfiguration;

//  Boot Configuration Structure
struct bootConfiguration {
    uint8_t isConf:1;       //Enter setup once, will be descended after enter
    uint8_t isSetup:1;      //Enter setup while flag wiil be not descended
    uint8_t isUpdate:1;     //Notes that device need update
    uint8_t isHttpError:1;  //HTTP error (response code not 200)
    uint8_t isWifiError:1;  //Error connecting to Wi-Fi
    uint8_t isCertError:1;  //Cert expired/not correct (server response "Connection refused")
    uint8_t field7:1;
    uint8_t field8:1;
};


struct configuration {
    char ssid[SSID_SIZE];
    char pass[PASS_SIZE];
    char link[LINK_SIZE];
    uint8_t certificate[20];
    char token[TOKEN_SIZE];
    uint8_t ledCount;
};

void startServer(configuration*, bootConfiguration*, RGB_LED*);
uint8_t parseConfiguration(String* response, configuration*);
uint8_t parseHexByte(char*);
bool parseCertificate(char*, uint8_t*);

#endif