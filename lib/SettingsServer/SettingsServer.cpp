#include "SettingsServer.h"

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

bool parseCertificate(char* temp, uint8_t* cert) {
    char* ptr = temp - 1;
    for (int i = 0; i < 19; i++) {
        ptr = strstr(ptr+1, ":");
        if (ptr != nullptr) {
            cert[i] = parseHexByte(ptr-2);
        } else return false;
    }
    if (ptr != nullptr) {
        cert[19] = parseHexByte(ptr+1);
    } else return false;
    DEBUG_S("[PARSER]Certificate parsed as: ");
    for (uint8_t i = 0; i < 19; i++) DEBUG_S(String(*(cert+i)) + ":");
    DEBUG(String(*(cert+19)));
    return true;
}

uint8_t parseConfiguration(String* response, configuration* deviceConfig) {
    uint8_t parsedItems = 0;
    GETParser parser(*response);
    String temp;
    uint8_t tempInt;
    //Trying to parse Wi-Fi SSID
    temp = parser.parseString("ssid");
    if (!parser.isErrorOccured()) {
        temp.toCharArray(&(deviceConfig->ssid[0]), SSID_SIZE);
        parsedItems++;
    }
    //Trying to parse Wi-Fi password
    temp = parser.parseString("pass");
    if (!parser.isErrorOccured()) {
        temp.toCharArray(&(deviceConfig->pass[0]), PASS_SIZE);
        parsedItems++;
    }
    //Trying to parse API link
    temp = parser.parseString("link");
    if (!parser.isErrorOccured()) {
        temp.toCharArray(&(deviceConfig->link[0]), LINK_SIZE);
        parsedItems++;
    }
    //Trying to parse device token
    temp = parser.parseString("token");
    if (!parser.isErrorOccured()) {
        temp.toCharArray(&(deviceConfig->token[0]), TOKEN_SIZE);
        parsedItems++;
    }
    //Trying to parse SSL certificate fingerprint
    temp = parser.parseString("cert");
    if (parseCertificate(const_cast<char*>(temp.c_str()), &(deviceConfig->certificate[0]))) {
        parsedItems++;
    }
    //Trying to parse LED count
    tempInt = parser.parseUint<uint8_t>("led");
    if (!parser.isErrorOccured()) {
        deviceConfig->ledCount = tempInt;
        parsedItems++;
    }
    return parsedItems;
}

void startServer(configuration* deviceConfig, bootConfiguration* marks, RGB_LED* stateLED) {
    bool running = true;
    stateLED->led_set(LED_YELLOW);
    if (marks->isConf) {
        marks->isConf = 0;
    }
    DNSServer dns;//DNS server object
    WiFiServer server(80);
    WiFi.mode(WIFI_AP);//Set Wi-Fi mode to access point
    WiFi.softAP("Light_" + String(ESP.getChipId()), AP_PASS);
    WiFi.softAPConfig(AP_STATIC);
    DEBUG_S("[APSERVER]AP started at: http://");
    DEBUG(WiFi.softAPIP());
    dns.setTTL(60);
    dns.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dns.start(53, "*", WiFi.softAPIP());//Start server for all requests with redirect to local web-server
    server.begin();
    uint32_t blink_timer = millis();
    String request;
    const char* req;
    while (running) {
        yield();
        dns.processNextRequest();
        WiFiClient AP_client = server.available();
        if (AP_client) {
            request = AP_client.readStringUntil('\r');
            AP_client.flush();
            req = request.c_str();
            DEBUG("[APSERVER]Client connected.");
            DEBUG("[APSERVER]Client request:\n" + request);
            if (strstr(req, "GET /config") != nullptr) {
                DEBUG("[APSERVER]Config data recieved. Start parsing.");
                AP_client.print(header_text);
                AP_client.print("OK");
                if (parseConfiguration(&request, deviceConfig)) {
                    marks->isSetup = 0;
                    marks->isCertError = 0;
                    marks->isHttpError = 0;
                    marks->isWifiError = 0;
                }
            } else if (strstr(req, "GET /restart") != nullptr) {
                //TODO: restart -> exit
                AP_client.print(header_text);
                AP_client.print("OK");
                running = false;
            } else if (strstr(req, "GET /update") != nullptr) {
                AP_client.print(header_text);
                AP_client.print("OK");
                DEBUG("[APSERVER]Rise update flag.");
                marks->isUpdate = 1;
            } else {
                DEBUG("[APSERVER]Sending configuration page.");
                AP_client.print(header_html);
                AP_client.print(page);
            }
        }
        if(blink_timer + 1200 < millis()) {
            stateLED->invert();
            blink_timer = millis();
        }
    }
}