//  General
#include <Arduino.h>
#include <ESP8266WiFi.h>
//  Network
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include "NetworkHandler.h"
//  Strip handling
#include <Adafruit_NeoPixel.h>
#include "StripControl.h"
//  Status led control
#include <RGB_LED.h>
//  EEPROM
#include <NVSettings.h>
//  DNS Server
#include <DNSServer.h>

//TODO: Rewrite update on new core
//TODO: Rewrite page to generate inputs by itself
//TODO: Test network errors on new core
//TODO: Comment code
//TODO: Split SC class to SC class and web interface class
//TODO: Add button
//TODO: Implement flash data encryption
//TODO: Move web settings page to SPIFFS


//  Update settings
#define VER "00008040"


//  Defined for debug (Serial)
#ifndef DEBUG
    #define DEBUG_N(x)
    #define DEBUG_S(x)
    #define FLUSH()
    #define CURR_VER VER "-light"
#else
    #define DEBUG_N(x) Serial.println(x);
    #define DEBUG_S(x) Serial.print(x);
    #define FLUSH() Serial.flush();
    #define CURR_VER VER "-light-d"
    #define DEBUG_DELIMETER "#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#"
#endif

#define forever for (;;)


//  LED config
#define LED_PIN_RED     14
#define LED_PIN_GREEN   12
#define LED_PIN_BLUE    13


//  AP defined settings
#define AP_PASS "12341234"
#define AP_STATIC IPAddress(1,1,1,1),IPAddress(1,1,1,0),IPAddress(255,255,255,0)


//  AP predef strings
String header_html = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String header_text = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
String page = R"=====(
<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="icon"
          href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA8AAAAPCAQAAACR313BAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QAAKqNIzIAAAAJcEhZcwAADdcAAA3XAUIom3gAAAAHdElNRQfjAwUSBDvmvxJQAAAAxElEQVQY06XRMUtCARQF4O+9pp6o0OaUTyFcgqe4NOqUP6af0G+IlramlhqktLHJUTcXlwqCRHQIoiGIHtgi8R64ec5yL+fAvededsFeris7lPrZZqx5lFpLDcRZoSDRtbL+51JHokCg51YpYx678YtQ033gRd2Hdwv7Iscqzjx7M8F1qIYDV849mZr7EltpgO/QKwJtF05UwNDIJ4gCp+4ys2cuN1WopQ+RRMcyt3lXIsrGiw02uR9Ut9+w5Ehxpy/k8AdFkTWK77PBMgAAACV0RVh0ZGF0ZTpjcmVhdGUAMjAxOS0wMy0wNVQxNzowNDo1OSswMTowMIFOLTYAAAAldEVYdGRhdGU6bW9kaWZ5ADIwMTktMDMtMDVUMTc6MDQ6NTkrMDE6MDDwE5WKAAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAAABJRU5ErkJggg==">
    <title>Web Settings</title>
</head>
<body>
<header>
    <svg xmlns="http://www.w3.org/2000/svg" width="48" height="48">
        <path d="M3.7 18c.2.9.6 1.7 1 2.5l-1.3 1.6c-.4.6-.3 1.3.1 1.8l2.2 2.2c.5.5 1.2.5 1.7.1L9 24.9c.8.5 1.7.9 2.6 1.1l.2 2.1c.1.6.7 1.1 1.3 1.1h3.1c.7 0 1.2-.5 1.3-1.1l.2-2c1-.3 1.9-.6 2.8-1.1l1.5 1.2c.5.4 1.3.4 1.7-.1l2.2-2.2c.5-.5.5-1.2.1-1.8l-1.2-1.5c.5-.8.9-1.8 1.2-2.7l1.8-.2c.7-.1 1.1-.7 1.1-1.3v-3.2c0-.6-.4-1.2-1.1-1.3l-1.8-.2c-.3-.9-.6-1.8-1.1-2.7L26 7.6c.4-.5.4-1.3-.1-1.8l-2.2-2.1c-.4-.5-1.2-.6-1.7-.1l-1.4 1c-.8-.5-1.8-.9-2.8-1.2l-.2-1.7C17.5 1 17 .5 16.3.5h-3.1C12.6.5 12 1 12 1.7l-.2 1.7c-1.1.3-2 .7-2.9 1.3L7.4 3.6c-.5-.5-1.2-.4-1.7.1L3.5 5.8c-.4.5-.5 1.3-.1 1.8l1.2 1.5c-.5.9-.8 1.8-1.1 2.8l-1.8.2c-.7 0-1.2.6-1.2 1.3v3.1c0 .6.5 1.2 1.2 1.3l2 .2zm11.1-8.4c2.8 0 5.1 2.3 5.1 5.2 0 2.8-2.3 5.1-5.1 5.1s-5.1-2.3-5.1-5.1c0-2.9 2.3-5.2 5.1-5.2zm30.4 9.2l-1.7-1.4c-.5-.4-1.2-.4-1.7 0l-.9.9c-.7-.4-1.5-.7-2.3-.8l-.3-1.3c-.1-.6-.7-1-1.4-1l-2.1.2c-.6 0-1.1.6-1.2 1.2v1.3c-.8.3-1.6.7-2.3 1.2l-1.1-.7c-.5-.4-1.2-.3-1.7.2l-1.3 1.7c-.4.5-.4 1.2 0 1.7l1 1c-.4.8-.6 1.5-.7 2.3l-1.4.3c-.6.1-1 .7-1 1.4l.2 2.1c0 .7.6 1.2 1.2 1.2l1.5.1c.2.7.6 1.3 1 2l-.8 1.2c-.4.5-.3 1.3.2 1.7l1.7 1.4c.5.4 1.2.4 1.7-.1l1-1c.7.4 1.4.6 2.2.8l.3 1.4c.1.7.7 1.1 1.4 1.1l2.1-.2c.6-.1 1.1-.6 1.2-1.3V36c.8-.3 1.5-.7 2.2-1.1l1.2.8c.5.3 1.2.2 1.6-.3l1.4-1.6c.4-.5.4-1.3 0-1.8l-1-1c.4-.7.6-1.5.8-2.3l1.3-.3c.6-.1 1-.7 1-1.3l-.2-2.2c-.1-.6-.6-1.1-1.2-1.2h-1.3c-.3-.8-.7-1.5-1.1-2.1l.7-1.1c.4-.6.3-1.3-.2-1.7zm-8 12.3c-2.3.2-4.3-1.5-4.5-3.8s1.5-4.4 3.8-4.6 4.3 1.6 4.5 3.9-1.5 4.3-3.8 4.5zm-26.1 5.8c-.7 0-1.2.6-1.2 1.3v1.3c0 .6.5 1.2 1.1 1.3l1 .1c.1.6.4 1.2.7 1.7l-.7.8c-.4.5-.3 1.2.1 1.7l.9.9c.5.5 1.2.6 1.7.1l.8-.6c.5.4 1.1.6 1.7.8l.1 1c.1.7.6 1.2 1.3 1.2h1.3c.6 0 1.2-.5 1.3-1.2l.1-.9c.6-.2 1.3-.4 1.9-.7l.7.6c.5.4 1.2.3 1.7-.1l.9-.9c.5-.5.5-1.2.1-1.8l-.5-.7c.3-.6.6-1.2.8-1.8l.8-.1c.7-.1 1.2-.6 1.2-1.3v-1.3c0-.7-.5-1.2-1.1-1.3l-.9-.1c-.1-.6-.4-1.3-.7-1.8l.5-.7c.5-.5.4-1.2 0-1.7l-.9-.9c-.5-.5-1.2-.6-1.7-.2l-.7.5-1.8-.8-.1-.8c-.1-.7-.6-1.2-1.3-1.2h-1.3c-.7 0-1.2.5-1.3 1.1l-.1.8c-.7.2-1.3.5-1.9.8l-.7-.5c-.5-.4-1.2-.4-1.7.1l-.9.9c-.5.5-.5 1.2-.1 1.7l.5.8c-.3.5-.6 1.1-.7 1.8l-.9.1zm8.4-1.5c1.8 0 3.3 1.6 3.3 3.5 0 1.8-1.5 3.4-3.4 3.3-1.9 0-3.4-1.5-3.4-3.4.1-1.9 1.6-3.4 3.5-3.4z"/>
    </svg>
    <p class="h-text">Settings</p>
</header>
<form name="data">
    <fieldset>
        <legend>Wi-Fi Settings</legend>
        <input type="text" id="ssid" placeholder="Network SSID" maxlength="32">
        <input type="text" id="pass" placeholder="Network password" maxlength="64">
    </fieldset>
    <fieldset>
        <legend>Server API Settings</legend>
        <input type="text" id="link" placeholder="Server API" maxlength="64">
        <input type="text" id="cert" placeholder="API certificate" maxlength="59">
        <input type="text" id="token" placeholder="Token" maxlength="8">
    </fieldset>
    <fieldset>
        <legend>LED Settings</legend>
        <input type="text" id="led" placeholder="Number of led's" min="1" max="255" maxlength="3">
    </fieldset>
    <div id="info" style="visibility: hidden">Settings updated. Restart ESP.</div>
    <div class="bt-wrapper">
        <input type="button" id="send" value="Update">
        <input type="button" id="rst" value="Restart">
    </div>
</form>
<style>
* {
    font-family: Roboto, "San Francisco", Arial, serif;
}

form, body {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
}

body {
    min-width: unset;
    min-height: unset;
}

header {
    display: flex;
    align-items: center;
    justify-content: center;
    margin: 20px 0 10px;
}

header svg {
    margin-right: 15px;
    fill: #273043;
}

.h-text {
    font-size: 23px;
    font-weight: bold;
    color: #273043;
}

/*  INPUTS */
input[type=text] {
    width: 76vw;
    height: 26px;
    outline: none;
    font-size: 16px;
    padding-left: 12px;
    border: 1.5px solid rgba(0, 0, 0, .40);
    border-radius: 6px;
    margin: 2px 0 10px;
    transition: border .35s;
}

input[type=text]:focus {
    border: 1.5px solid #008DD5;
}

input::placeholder {
    color: rgba(0, 0, 0, .35);
    font-style: italic;
}

fieldset {
    margin: 10px 0 10px;
    border: 2px solid #273043;
    border-radius: 7px;
    width: 70vw;
}

/* BUTTONS */
input[type=button] {
    width: 45%;
    height: 40px;
    background: white;
    border-radius: 8px;
    outline: none;
    font-weight: bold;
}

.bt-wrapper {
    margin: 1.75vh 0 2.5vh;
    width: 70vw;
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
}
#send {
    border: 2px solid #3EC300;
}

#send:active {
    background: #3EC300;
    color: white;
}

#rst {
    border: 2px solid #F21B3F;
}

#rst:active {
    background: #F21B3F;
    color: white;
}

/* RESPONSE INFO */
#info {
    visibility: hidden;
    text-align: center;
}

/* DESKTOP CASE */
@media screen and (min-width: 600px) {
    body {
        min-height: 480px;
        min-width: 640px;
    }

    input[type=text], .bt-wrapper, fieldset {
        width: 300px;
    }

    input[type=button] {
        width: 140px;
    }
}
</style>
<script type="application/javascript">
    document.getElementById('send').onclick = function () {
        let url = '/config?';
        let inputs = document.getElementsByTagName('input');
        for (let i = 0; i < inputs.length - 2; i++) {
            let input = inputs[i];
            if (input.value !== '') {
                url += input.id + '=' + input.value + '&';
            }
        }
        if (url.slice(-1) === '&') {
            url = url.slice(0, -1);
        }
        let xhr = new XMLHttpRequest();
        let info = document.getElementById('info');
        xhr.open("GET", url, true);
        xhr.onreadystatechange = function () {
            if (xhr.readyState !== 4) return;
            if (xhr.status === 200) {
                if (xhr.responseText === "OK") {
                    info.innerHTML = "Settings updated. Restart ESP.";
                    info.style.color = "#5BBA6F";
                    info.style.visibility = "visible";
                }
            } else {
                info.innerHTML = "[" + xhr.status + "]Error. Please try again.";
                info.style.color = "#F21B3F";
                info.style.visibility = "visible";
            }
        };
        xhr.send();
    };

    document.getElementById('rst').onclick = function () {
        let xhr = new XMLHttpRequest();
        let info = document.getElementById('info');
        xhr.open('GET', '/restart', true);
        xhr.onreadystatechange = function () {
            if (xhr.readyState !== 4) return;
            if (xhr.status === 200) {
                if (xhr.responseText === "OK") {
                    info.innerHTML = "ESP will be restarted.";
                    info.style.color = "#5BBA6F";
                    info.style.visibility = "visible";
                }
            } else {
                info.innerHTML = "[" + xhr.status + "]Error. Please try again.";
                info.style.color = "#F21B3F";
                info.style.visibility = "visible";
            }
        };
        xhr.send();
    }
</script>
</body>
</html>
)=====";


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


//  Strip configuration
#define STRIP_PIN       4       //Pin where strip is connected (old 13)
#define CHASE_TIMING    80      //Delay of chase mode
#define TIMING          80      //Delay of rainbow mods
#define FILL_REPEATS    2       //Number of fill repeats for avoid random lighting
#define FADE_STEP       4       //Step for rising colors in fade mode


//  Boot Configuration Structure
struct bc_set {
    uint8_t isConf:1;       //Enter setup once, will be descended after enter
    uint8_t isSetup:1;      //Enter setup while flag wiil be not descended
    uint8_t isUpdate:1;     //Notes that device need update
    uint8_t isHttpError:1;  //HTTP error (response code not 200)
    uint8_t isWifiError:1;  //Error connecting to Wi-Fi
    uint8_t isCertError:1;  //Cert expired/not correct (server response "Connection refused")
    uint8_t field7:1;
    uint8_t field8:1;
};


//  Global variables
NVSettings settings(ALLOCATED_EEPROM_SIZE);
char* ssid = new char[SSID_SIZE];
char* pass = new char[PASS_SIZE];
char* link = new char[LINK_SIZE];
uint8_t* cert = new uint8_t[20];
char* token = new char[TOKEN_SIZE];
uint8_t* led_count = new uint8_t;
bc_set marks;
String device_id = "Light_" + String(ESP.getChipId());
RGB_LED state_led(LED_PIN_RED, LED_PIN_GREEN, LED_PIN_BLUE);

/* Rebooting ESP */
inline void reboot() {
    DEBUG_N("[DEBUG]Rebooting device.");
    ESP.restart();
}

//Strip control class
class Strip_Control_deprecated {
private: 
    // ***[Fields]***
    BearSSL::WiFiClientSecure _client;
    HTTPClient _https;
    Adafruit_NeoPixel _strip = Adafruit_NeoPixel(1, STRIP_PIN, NEO_GRB + NEO_KHZ800);
    bool _isNewData = true;//Flag that displays whether new data recieved
    float _brightness = 0.5;
    uint8_t _error_counter = 0;//How many errors was occured
    uint16_t _mode;
    uint32_t _color;
    int16_t _response_code;//Last HTTP code was recieved
    String _response;//Last response body
    uint32_t _timer;//Timer for color modes
    // ***[Methods]***
    uint8_t _parseUint32(const char*, uint32_t*, const char*);
    uint8_t _parseUint16(const char*, uint16_t*, const char*);
    void _getData();
    void _parseData();
    void _modeSelect();
    inline void _checkUpdates();
    // ***[Modes]***
    void _fillColor();
    void _rainbow();
    void _rainbowCycle();
    void _theaterChaseRainbow();
    void _theaterChase();
    void _fading();
    void _randomLight();
    uint32_t _wheel(byte);
    int _randgen(int);
    void _breathe();
public:
    Strip_Control_deprecated();//Constructor
    void start();
};


Strip_Control_deprecated::Strip_Control_deprecated() {
    //  Creating instance of strip class and configure brightness
    _strip.updateLength((uint16_t)(*(led_count)));
    _strip.setBrightness(255);
    _strip.begin();
    _strip.fill(_strip.Color(1,1,1)); //Fill all led's with low white color
    _strip.show();
    //  Setting up connection to the API
    String url = link;
    url += "/light_api/ans.php?rm=";
    url += token;
    DEBUG_N("[DEBUG]Final API request link:");
    DEBUG_N(url);
    _client.setFingerprint(cert);
    _https.setReuse(true);
    _https.setUserAgent(device_id.c_str());
    _https.begin(_client, url);
    //Make request to server for figure out is connection established
    DEBUG_S("[DEBUG]Attempting connect to API");
    do {//Check http connection to server
        _response_code = _https.GET();
        _error_counter++;
        if (_error_counter >= 10) {//Check is ereors exceeded limit
            DEBUG_N("\n[DEBUG]Connect to API failed.");
            if (_response_code == -1) {//Connection refused server response
                marks.isCertError = 1;
            } else {
                marks.isHttpError = 1;
            }
            marks.isConf = 1;
            settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
            reboot();
        }
        DEBUG_S(".");
        // FLUSH(); //FIXME: m-b not needed
        delay(750);//FIXME: check delay timings
    } while (!_https.connected());
    DEBUG_N("\n[DEBUG]Connect to API successful.");
    _error_counter = 0;
    //  Indicate that we ready to work with strip
    state_led.led_set(LED_BLUE);
}


void Strip_Control_deprecated::_modeSelect() {
    switch (_mode) {
        case 1: {
            _isNewData = false;
            _fillColor();
            break;
        }
        case 2: {
            _isNewData = false;
            _rainbow();
            break;
        }
        case 3: {
            _isNewData = false;
            _rainbowCycle();
            break;
        }
        case 4: {
            _isNewData = false;
            _theaterChaseRainbow();
            break;
        }
        case 5: {
            _isNewData = false;
            _theaterChase();
            break;
        }
        case 6: {
            _isNewData = false;
            _fading();
            break;
        }
        case 7: {
            _isNewData = false;
            _randomLight();
            break;
        }
        case 8: {
            _isNewData = false;
            _breathe();
            break;
        }
    }
}


uint8_t Strip_Control_deprecated::_parseUint32(const char* raw, uint32_t* dest, const char* key) {
    char *ptr, *eq;
    ptr = strstr(raw, key);
    if (ptr != nullptr) {
        eq = strstr(ptr, "=");
        if (eq == nullptr || atol(eq+1) == 0) {
            DEBUG_N("[DEBUG]Parsing"+ String(key) +"error. Wrong request.");
            return 0;
        } else {
            *dest = atol(eq+1);
            DEBUG_N("[DEBUG]Parsed "+ String(key) +" as: " + String(*dest));
            return 1;
        }
    }
    return 0;
}


uint8_t Strip_Control_deprecated::_parseUint16(const char* raw, uint16_t* dest, const char* key) {
    char *ptr, *eq;
    ptr = strstr(raw, key);
    if (ptr != nullptr) {
        eq = strstr(ptr, "=");
        if (eq == nullptr || atoi(eq+1) == 0) {
            DEBUG_N("[DEBUG]Parsing"+ String(key) +"error. Wrong request.");
            return 0;
        } else {
            *dest = atoi(eq+1);
            DEBUG_N("[DEBUG]Parsed "+ String(key) +" as: " + String(*dest));
            return 1;
        }
    }
    return 0;
}


void Strip_Control_deprecated::start() {
    uint32_t loop_timer = 0;//FIXME: May be possibe use common timer
    forever {
        yield();
        if (loop_timer + POLLING_TIME < millis()) {
            if (_isNewData) {
                loop_timer = millis();//FIXME: Simplify code
                _parseData();
                _modeSelect();
            } else {
                loop_timer = millis();
            }
            _getData();
        }
    }
}


uint8_t parse_elem_uint(const char* raw, uint8_t* dest, const char* key) {
    char *ptr, *eq;
    ptr = strstr(raw, key);
    if (ptr != nullptr) {
        eq = strstr(ptr, "=");
        if (eq == nullptr || atoi(eq+1) == 0) {
            DEBUG_N("[DEBUG]Parsing"+ String(key) +"error. Wrong request.");
            return 0;
        } else {
            *dest = atoi(eq+1);
            DEBUG_N("[DEBUG]Parsed "+ String(key) +" as: " + String(*dest));
            return 1;
        }
    }
    return 0;
}


uint8_t parse_elem_str(const char* raw, char* dest, const char* key) {
    char *ptr, *eq, *amp;
    size_t len;
    ptr = strstr(raw, key);
    if (ptr != nullptr) {
        eq = strstr(ptr, "=");
        amp = strstr(ptr, "&");
        if (amp == nullptr) {
            amp = strstr(ptr, " ");
        }
        len = strlen(eq+1) - strlen(amp);
        if (eq == nullptr || amp == nullptr || len > 128 || len == 0) {
            DEBUG_N("[DEBUG]Parsing " + String(key) + " error. Wrong request.");
            return 0;
        } else {
            strncpy(dest, eq+1, len);
            dest[len] = '\0';
            DEBUG_N("[DEBUG]Parsed " + String(key) + " as: " + String(dest));
            return 1;
        }
    }
    return 0;
}


void readSettings() {
    settings.readString(SSID_ADDR, SSID_SIZE, ssid);
    settings.readString(PASS_ADDR, PASS_SIZE, pass);
    settings.readString(LINK_ADDR, LINK_SIZE, link);
    settings.readByteArray(CERT_ADDR, CERT_SIZE, cert);
    settings.readString(TOKEN_ADDR, TOKEN_SIZE, token);
    settings.readByte(LED_ADDR, led_count);
    settings.readByte(BCS_ADDR, (uint8_t*)(&marks));
    DEBUG_N("[DEBUG]Settings successfuly readed.");
}


uint8_t parseHexByte(char* hexnum) {
    uint8_t result = 0;
    for (int i = 0; i < 2; i++) {
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


uint8_t parseCert(char* temp, uint8_t* cert) {
    char* ptr = temp - 1;
    for (int i = 0; i < 19; i++) {
        ptr = strstr(ptr+1, ":");
        if (ptr != nullptr) {
            cert[i] = parseHexByte(ptr-2);
        } else return 0;
    }
    if (ptr != nullptr) {
        cert[19] = parseHexByte(ptr+1);
    } else return 0;
    DEBUG_S("[DEBUG]Certificate parsed as: ");
    for (uint8_t i = 0; i < 19; i++) DEBUG_S(String(*(cert+i)) + ":");
    DEBUG_N(String(*(cert+19)));
    return 1;
}


void parse_config(const char* raw) {
    DEBUG_N("[DEBUG]Request parsing started.");
    DEBUG_N(raw);
    uint8_t count = 0;
    count += parse_elem_str(raw, ssid, "ssid");
    count += parse_elem_str(raw, pass, "pass");
    count += parse_elem_str(raw, link, "link");
    char* cert_temp = new char[60];
    if (parse_elem_str(raw, cert_temp, "cert")) {
        count += parseCert(cert_temp,cert);
    }
    delete [] cert_temp;
    count += parse_elem_str(raw, token, "token");
    count += parse_elem_uint(raw, led_count, "led");
    if (count > 0) {
        marks.isSetup = 0;
        settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
    }
}


void writeSettings() {
    settings.writeString(SSID_ADDR, SSID_SIZE, ssid);
    settings.writeString(PASS_ADDR, PASS_SIZE, pass);
    settings.writeString(LINK_ADDR, LINK_SIZE, link);
    settings.writeByteArray(CERT_ADDR, CERT_SIZE, cert);
    settings.writeString(TOKEN_ADDR, TOKEN_SIZE, token);
    settings.writeByte(LED_ADDR, led_count);
    settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
    DEBUG_N("[DEBUG]Settings successfully written.");
}


void config_server() {
    state_led.led_set(LED_YELLOW);
    if (marks.isConf) {
        marks.isConf = 0;
        settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
    }
    DNSServer dns;//DNS server object
    WiFiServer server(80);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(device_id.c_str(), AP_PASS);
    WiFi.softAPConfig(AP_STATIC);
    DEBUG_S("[DEBUG]AP started at: http://");
    DEBUG_N(WiFi.softAPIP());
    dns.setTTL(60);
    dns.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dns.start(53, "*", WiFi.softAPIP());//Start server for all requests with redirect to local web-server
    server.begin();
    uint32_t timer = 0, blink_timer = millis();
    String request;
    const char* req;
    while (true) {
        yield();
        dns.processNextRequest();
        WiFiClient AP_client = server.available();
        if (AP_client) {
            request = AP_client.readStringUntil('\r');
            AP_client.flush();
            req = request.c_str();
            DEBUG_N("[DEBUG]Client connected.");
            DEBUG_N("[DEBUG]Client request:\n" + request);
            if (strstr(req, "GET /config") != nullptr) {
                DEBUG_N("[DEBUG]Config data recieved.");
                AP_client.print(header_text);
                AP_client.print("OK");
                parse_config(request.c_str());
                writeSettings();
            } else if (strstr(req, "GET /restart") != nullptr) {
                AP_client.print(header_text);
                AP_client.print("OK");
                DEBUG_N("[DEBUG]Restarting ESP.");
                timer = millis();
            } else if (strstr(req, "GET /update") != nullptr) {
                AP_client.print(header_text);
                AP_client.print("OK");
                DEBUG_N("[DEBUG]Rise update flag.");
                marks.isUpdate = 1;
                settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
            } else {
                DEBUG_N("[DEBUG]Sending configuration page.");
                AP_client.print(header_html);
                AP_client.print(page);
            }
        }
        if (timer != 0 && timer + 400 < millis()) reboot();
        if(blink_timer + 1200 < millis()) {
            state_led.invert();
            blink_timer = millis();
        }
    }
}


void ota_update() {
    // TODO: rewrite update on new core
    BearSSL::WiFiClientSecure client;
    client.setFingerprint(cert);//Set HTTPS certificate
    state_led.led_set(LED_MARINE);
    ESP8266HTTPUpdate ESPUpdate;
    ESPUpdate.rebootOnUpdate(false);
    String update_url = link;
    update_url += "/update_center/update_handler.php";
    DEBUG_N("[DEBUG]Composed update link: " + update_url);
    ESPUpdate.setLedPin(state_led.get_pin(LED_BLUE));
    HTTPUpdateResult result = ESPUpdate.update(client, update_url, CURR_VER);
    switch (result) {
        case HTTP_UPDATE_FAILED:
            #ifdef DEBUG
                Serial.printf("[DEBUG]Update error (%d): %s\n", ESPUpdate.getLastError(), ESPUpdate.getLastErrorString().c_str());
            #endif
            break;
        case HTTP_UPDATE_NO_UPDATES:
            DEBUG_N("[DEBUG]No updates available.");
            //FIXME: When button will be adeed, need to reset update flag
            // marks.isUpdate = 0;
            // settings.writeByte(BCS_ADDR,(uint8_t*)(&marks));
            break;
        case HTTP_UPDATE_OK:
            DEBUG_N("[DEBUG]Successfully updated.");
            marks.isUpdate = 0;
            settings.writeByte(BCS_ADDR,(uint8_t*)(&marks));
            reboot();
            break;
    }
}


void setup() {
    uint32_t wf_connect_timer;
    state_led.led_set(LED_PURPLE);
    #ifdef DEBUG
        Serial.begin(115200);
        Serial.println();
    #endif
    DEBUG_N(DEBUG_DELIMETER);
    DEBUG_N("[DEBUG]Basic device info:");
    DEBUG_N("[DEBUG]Sketch version: " + String(CURR_VER));
    DEBUG_N("[DEBUG]Device ID: " + String(ESP.getChipId()));
    DEBUG_N("[DEBUG]Core version: " + ESP.getCoreVersion());
    DEBUG_N("[DEBUG]SDK version: " + String(ESP.getSdkVersion()));
    DEBUG_N("[DEBUG]Memory chip ID: " + String(ESP.getFlashChipId()));
    DEBUG_N("[DEBUG]Memory size: " + String(ESP.getFlashChipRealSize()));
    DEBUG_N("[DEBUG]ESP STA MAC addres: " + WiFi.macAddress());
    DEBUG_N("[DEBUG]ESP AP MAC addres: " + WiFi.softAPmacAddress());
    DEBUG_N("[DEBUG]Memory free space: " + String(ESP.getFreeSketchSpace()));
    DEBUG_N("[DEBUG]Sketch size: " + String(ESP.getSketchSize()));
    DEBUG_N("[DEBUG]Sketch MD5: " + String(ESP.getSketchMD5()));
    DEBUG_N(DEBUG_DELIMETER);
    readSettings();
    if (marks.isSetup || marks.isConf) {
        DEBUG_N("[DEBUG]Entering setup.");
        config_server();
    }
    DEBUG_N("[DEBUG]NV settings info:");
    DEBUG_N(String("[DEBUG]Network ssid: ") + ssid);
    DEBUG_N(String("[DEBUG]Network password: ") + pass);
    DEBUG_N(String("[DEBUG]API link: ") + link);
    DEBUG_S("[DEBUG]API certificate: ");
    for (uint8_t i = 0; i < 19; i++) DEBUG_S(String(*(cert+i)) + ":");
    DEBUG_N(String(*(cert+19)));
    DEBUG_N(String("[DEBUG]API token: ") + token);
    DEBUG_N(String("[DEBUG]LED count: ") + *led_count);
    DEBUG_N(DEBUG_DELIMETER);
    WiFi.mode(WIFI_STA);
    if (WiFi.SSID() == ssid) {
        DEBUG_N("[DEBUG]Reconnecting to last beacon: " + WiFi.SSID());
        WiFi.begin();
    } else {
        DEBUG_N("[DEBUG]Beacon info wasn't saved.");
        WiFi.begin(ssid,pass);
    }
    DEBUG_S("[DEBUG]Connecting to WiFi.");
    wf_connect_timer = millis();
    //TODO: Add info about wi-fi not connected status
    while (WiFi.status() != WL_CONNECTED) {
        if (wf_connect_timer + 15000 >= millis()) {
            DEBUG_S(".");
            state_led.invert();
            delay(500);
        } else {
            DEBUG_N();
            marks.isSetup = 1;
            marks.isWifiError = 1;
            settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
            WiFi.disconnect();
            reboot();
        }
    }
    DEBUG_N();
    if (marks.isUpdate) {
        DEBUG_N("[DEBUG]Update initialized.");
        ota_update();
        DEBUG_N("[DEBUG]Update ended.");
    } else {
        //TODO: deprecate when update will be fixed
        marks.isUpdate = 1;
        settings.writeByte(BCS_ADDR, (uint8_t*)(&marks));
    }
    state_led.led_set(LED_GREEN);
}

void loop() {
    #ifdef DEBUG
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
    NetworkHandler neth(link, cert);
    StripControl strip(*led_count, &neth);

    forever {
        yield();
        
    }
}
