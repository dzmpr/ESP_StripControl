//  General
#include <Arduino.h>
#include <ESP8266WiFi.h>
//  Network
#include <ESP8266HTTPClient.h>
#include "NetworkHandler.h"
//  Strip handling
#include "StripControl.h"
//  Status led control
#include "RGB_LED.h"
//  EEPROM
#include "NVSettings.h"
//Settings server
#include "SettingsServer.h"
//Update handler
#include "OTAUpdater.h"

//TODO: Rewrite page to generate inputs by itself
//TODO: Test network errors on new core
//TODO: Add button
//TODO: Implement flash data encryption
//TODO: Move web settings page to SPIFFS
//TODO: Wi-Fi error explanation
//TODO: HTTP error explanation


//  Defined for debug (Serial)
#ifndef DEBUG_BUILD
    #define DEBUG(...)
    #define DEBUG_S(...)
    #define DEBUG_F(...)
    #define FLUSH()
#else
    #define DEBUG(...) Serial.println(__VA_ARGS__);
    #define DEBUG_S(...) Serial.print(__VA_ARGS__);
    #define DEBUG_F(...) Serial.printf(__VA_ARGS__);
    #define FLUSH() Serial.flush();
    #define DEBUG_DELIMETER "#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#"
#endif

#define forever for (;;)


//  LED config
#define LED_PIN_RED     14
#define LED_PIN_GREEN   12
#define LED_PIN_BLUE    13


//  EEPROM Configuration
#define CS_SIZE                 (1)
#define BYTE_SIZE               (1)
#define SSID_SIZE               (32+1)
#define PASS_SIZE               (64+1)
#define LINK_SIZE               (64+1)
#define CERT_SIZE               (BYTE_SIZE*20)
#define TOKEN_SIZE              (8+1)
#define ALLOCATED_EEPROM_SIZE   (SSID_SIZE+PASS_SIZE+TOKEN_SIZE+LINK_SIZE+\
                                CERT_SIZE+2*(BYTE_SIZE)+7*CS_SIZE)


//  EEPROM addresses
#define SSID_ADDR   (0)
#define PASS_ADDR   (SSID_ADDR+SSID_SIZE+CS_SIZE)
#define LINK_ADDR   (PASS_ADDR+PASS_SIZE+CS_SIZE)
#define CERT_ADDR   (LINK_ADDR+LINK_SIZE+CS_SIZE)
#define TOKEN_ADDR  (CERT_ADDR+CERT_SIZE+CS_SIZE)
#define LED_ADDR    (TOKEN_ADDR+TOKEN_SIZE+CS_SIZE)
#define BCS_ADDR    (LED_ADDR+BYTE_SIZE+CS_SIZE)


//  Global variables
NVSettings settings(ALLOCATED_EEPROM_SIZE);
configuration deviceConfig;
bootConfiguration marks;
RGB_LED stateLED(LED_PIN_RED, LED_PIN_GREEN, LED_PIN_BLUE);

/* Rebooting ESP */
inline void reboot() {
    DEBUG("[DEBUG]Rebooting device.");
    ESP.restart();
}

// //Strip control class
// class Strip_Control_deprecated {
// private: 
//     // ***[Fields]***
//     BearSSL::WiFiClientSecure _client;
//     HTTPClient _https;
//     Adafruit_NeoPixel _strip = Adafruit_NeoPixel(1, STRIP_PIN, NEO_GRB + NEO_KHZ800);
//     bool _isNewData = true;//Flag that displays whether new data recieved
//     float _brightness = 0.5;
//     uint8_t _error_counter = 0;//How many errors was occured
//     uint16_t _mode;
//     uint32_t _color;
//     int16_t _response_code;//Last HTTP code was recieved
//     String _response;//Last response body
//     uint32_t _timer;//Timer for color modes
//     // ***[Methods]***
//     uint8_t _parseUint32(const char*, uint32_t*, const char*);
//     uint8_t _parseUint16(const char*, uint16_t*, const char*);
//     void _getData();
//     void _parseData();
//     void _modeSelect();
//     inline void _checkUpdates();
//     // ***[Modes]***
//     void _fillColor();
//     void _rainbow();
//     void _rainbowCycle();
//     void _theaterChaseRainbow();
//     void _theaterChase();
//     void _fading();
//     void _randomLight();
//     uint32_t _wheel(byte);
//     int _randgen(int);
//     void _breathe();
// public:
//     Strip_Control_deprecated();//Constructor
//     void start();
// };


void readSettings() {
    settings.readString(SSID_ADDR, SSID_SIZE, &(deviceConfig.ssid[0]));
    settings.readString(PASS_ADDR, PASS_SIZE, &(deviceConfig.pass[0]));
    settings.readString(LINK_ADDR, LINK_SIZE, &(deviceConfig.link[0]));
    settings.readByteArray(CERT_ADDR, CERT_SIZE, &(deviceConfig.certificate[0]));
    settings.readString(TOKEN_ADDR, TOKEN_SIZE, &(deviceConfig.token[0]));
    settings.readByte(LED_ADDR, &(deviceConfig.ledCount));
    settings.readByte(BCS_ADDR, (uint8_t*)(&marks));
    DEBUG("[DEBUG]Settings successfuly readed.");
}


void writeSettings() {
    settings.writeString(SSID_ADDR, SSID_SIZE, &(deviceConfig.ssid[0]));
    settings.writeString(PASS_ADDR, PASS_SIZE, &(deviceConfig.pass[0]));
    settings.writeString(LINK_ADDR, LINK_SIZE, &(deviceConfig.link[0]));
    settings.writeByteArray(CERT_ADDR, CERT_SIZE, &(deviceConfig.certificate[0]));
    settings.writeString(TOKEN_ADDR, TOKEN_SIZE, &(deviceConfig.token[0]));
    settings.writeByte(LED_ADDR, &(deviceConfig.ledCount));
    settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
    DEBUG("[DEBUG]Settings successfully written.");
}


void setup() {
    uint32_t wf_connect_timer;
    stateLED.led_set(LED_PURPLE);
    #ifdef DEBUG_BUILD
        Serial.begin(115200);
        Serial.println();
    #endif
    DEBUG(DEBUG_DELIMETER);
    DEBUG("[DEBUG]Basic device info:");
    DEBUG("[DEBUG]Sketch version: " VERSION);
    DEBUG("[DEBUG]Device ID: " + String(ESP.getChipId()));
    DEBUG("[DEBUG]Core version: " + ESP.getCoreVersion());
    DEBUG("[DEBUG]SDK version: " + String(ESP.getSdkVersion()));
    DEBUG("[DEBUG]Memory chip ID: " + String(ESP.getFlashChipId()));
    DEBUG("[DEBUG]Memory size: " + String(ESP.getFlashChipRealSize()));
    DEBUG("[DEBUG]ESP STA MAC addres: " + WiFi.macAddress());
    DEBUG("[DEBUG]ESP AP MAC addres: " + WiFi.softAPmacAddress());
    DEBUG("[DEBUG]Memory free space: " + String(ESP.getFreeSketchSpace()));
    DEBUG("[DEBUG]Sketch size: " + String(ESP.getSketchSize()));
    DEBUG("[DEBUG]Sketch MD5: " + String(ESP.getSketchMD5()));
    DEBUG(DEBUG_DELIMETER);


    readSettings();
    DEBUG("[DEBUG]Boot state: " + String(*((uint8_t*)(&marks))));
    if (marks.isSetup || marks.isConf) {
        DEBUG("[DEBUG]Entering setup.");
        startServer(&deviceConfig, &marks, &stateLED);
        writeSettings();
    }


    DEBUG("[DEBUG]NV settings info:");
    DEBUG(String("[DEBUG]Network ssid: ") + deviceConfig.ssid);
    DEBUG(String("[DEBUG]Network password: ") + deviceConfig.pass);
    DEBUG(String("[DEBUG]API link: ") + deviceConfig.link);
    DEBUG_S("[DEBUG]API certificate: ");
    for (uint8_t i = 0; i < 19; i++) DEBUG_S(String(deviceConfig.certificate[i]) + ":");
    DEBUG(String(deviceConfig.certificate[19]));
    DEBUG(String("[DEBUG]API token: ") + deviceConfig.token);
    DEBUG(String("[DEBUG]LED count: ") + deviceConfig.ledCount);
    DEBUG(DEBUG_DELIMETER);


    WiFi.mode(WIFI_STA);
    if (WiFi.SSID() == deviceConfig.ssid) {
        DEBUG("[DEBUG]Reconnecting to last beacon: " + WiFi.SSID());
        WiFi.begin();
    } else {
        DEBUG("[DEBUG]Beacon info wasn't saved.");
        WiFi.begin(deviceConfig.ssid, deviceConfig.pass);
    }
    DEBUG_S("[DEBUG]Connecting to WiFi.");
    wf_connect_timer = millis();
    //TODO: Add info about wi-fi not connected status
    while (WiFi.status() != WL_CONNECTED) {
        if (wf_connect_timer + 15000 >= millis()) {
            DEBUG_S(".");
            stateLED.invert();
            delay(500);
        } else {
            DEBUG();
            marks.isSetup = 1;
            marks.isWifiError = 1;
            settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
            WiFi.disconnect();
            reboot();
        }
    }
    DEBUG();


    if (marks.isUpdate) {//Update required
        DEBUG("[DEBUG]Update initialized.");
        BearSSL::WiFiClientSecure updateClient;
        updateClient.setFingerprint(&(deviceConfig.certificate[0]));
        if (OTAUpdate(String(deviceConfig.link), &updateClient, stateLED.get_pin(LED_BLUE))) {
            reboot();
        }
    } else {//Update not required
        //TODO: deprecate when update will be fixed
        marks.isUpdate = 1;
        settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
    }

    stateLED.led_set(LED_GREEN);
}

void loop() {
    #ifdef DEBUG_BUILD
        Serial.println("HeHe");
        if (Serial.available()) {
            String rd;
            rd = Serial.readString();
            Serial.println(rd);
            if (rd == "CONF") {
                marks.isConf = 1;
                settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
            }
        }
    #endif

    String url = deviceConfig.link;
    url += "/light_api/connection_test.php";
    NetworkHandler networkHandler(url, &(deviceConfig.certificate[0]));

    //Test connection to API FIXME: prettify
    DEBUG("[DEBUG]Checking connection.");
    do {
        networkHandler.makeRequest();
        if (networkHandler.getErrorCount() >= 5) {
             if (networkHandler.getResponseCode() == -1) {//Connection refused server response
                marks.isCertError = 1;
            } else {
                marks.isHttpError = 1;
            }
            marks.isConf = 1;
            settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
            reboot();
        }
    } while (!networkHandler.isConnected());
    DEBUG("[DEBUG]Connected to API successfull.");

    url = deviceConfig.link;
    url += "/light_api/ans.php?rm=";
    url += deviceConfig.token;
    networkHandler.changeURL(url);

    StripControl strip(deviceConfig.ledCount, &networkHandler);
    DEBUG(millis());
    forever {
        yield();
        if (networkHandler.newDataStatus()) {
            strip.modeSelection();
        } else {
            strip.checkUpdates(2000);
        }
    }
}
