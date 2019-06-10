# ESP_StripControl
Firmware which allows ESP8266 control ws2812b LED strip over Internet.

Project based on ESP8266 microcontroller to control address LED-strip (on ws2812b). Has 7 color modes (in dev), allows to control strip brightness.
Device interact with server by HTTPS protocol (can be rewrited to WebSockets).
Control of device is from web-site (in further will be availaible from Android app and Telegram bot).
Firmware has possibility for upgrade by OTA, has web-page to configure device settings from AP.

In plans:
* Add button for direct control.
* Maybe adding light sensor for dynamic brightness control.
* Changing server polling way from HTTPS to WS to reduce number of requests.
* Technical improvments (depends from uc core).
* Unexpected updates.
