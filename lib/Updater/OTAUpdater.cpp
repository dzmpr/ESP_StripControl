#include "OTAUpdater.h"

bool OTAUpdate(String updateUrl, BearSSL::WiFiClientSecure* client, uint8_t ledPin) {
    ESP8266HTTPUpdate ESPUpdate;
    ESPUpdate.rebootOnUpdate(false);
    ESPUpdate.setLedPin(ledPin);
    updateUrl += "/update_center/update_handler.php";
    DEBUG("[UPDATER]Update started.");
    HTTPUpdateResult result = ESPUpdate.update(*client, updateUrl, VERSION);
    switch (result) {
        case HTTP_UPDATE_FAILED: {
            DEBUG_F("[UPDATER]Update error (%d): %s\n", ESPUpdate.getLastError(), ESPUpdate.getLastErrorString().c_str());
            return false;
        }
        case HTTP_UPDATE_NO_UPDATES: {
            DEBUG("[UPDATER]No updates availiable.");
            return false;
        }
        case HTTP_UPDATE_OK: {
            DEBUG("[UPDATER]Succsessfully updated.");
            return true;
        }
    }
    DEBUG("[UPDATER]Update ended.");
    return false;
}