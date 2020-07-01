/**
 * 
 * 
 */
 #ifndef StripControl_h
 #define StripControl_h
    #include <Arduino.h>
    #include <Adafruit_NeoPixel.h>
    #include "NetworkHandler.h"
    #include "GETParser.h"

    #define CHECK_DELAY 2000 //Delay for checking updates

    #ifndef DEBUG_BUILD
        #define DEBUG(...)
        #define DEBUG_S(...)
        #define DEBUG_F(...)
    #else
        #define DEBUG(...) Serial.println(__VA_ARGS__);
        #define DEBUG_S(...) Serial.print(__VA_ARGS__);
        #define DEBUG_F(...) Serial.printf(__VA_ARGS__);
    #endif

    class HSB;
    class RGB;

    class HSB {
        private:
            friend class RGB;
            uint16_t _hue;
            uint8_t _saturation;
            uint8_t _brightness;
        public:
            HSB();
            HSB(const RGB&);//Convert RGB to HSB
            HSB(uint16_t, uint8_t, uint8_t);//Set all components
            void fromRGB(const RGB&);//Convert RGB to HSB
            void setColor(uint16_t, uint8_t, uint8_t);//Set components (2160,255,255)
            void setBrightness(uint8_t);//Set brightness
            void setNormalizedColor(uint16_t, uint8_t, uint8_t);//Set normalized color (360,255,255)
            void cycleHue(uint16_t step = 1);//Change hue value
            uint16_t getHue();//Get hue value
            uint8_t getSaturation();//Get saturation value
            uint8_t getBrightness();//Get brightness value
    };

    class RGB {
        private:
            friend class HSB;
            uint8_t _red;
            uint8_t _green;
            uint8_t _blue;
        public:
            RGB();
            RGB(const HSB&);//Convert HSB to RGB
            RGB(uint8_t, uint8_t, uint8_t);//Set all components
            RGB(uint32_t);//Create from packed value
            void fromHSB(const HSB&);//Convert HSB to RGB
            void setColor(uint8_t, uint8_t, uint8_t);//Set all components
            void setPacked(uint32_t);//Set from packed value
            uint8_t getRed();//Get red component
            uint8_t getGreen();//Get green component
            uint8_t getBlue();//Get blue component
            uint32_t getPacked();//Get packed value
    };

    class StripControl {
        public:
            StripControl(uint16_t, NetworkHandler*);
            void modeSelection();
            inline void checkUpdates(uint32_t);
        private:
            ///[Service]
            Adafruit_NeoPixel _strip = Adafruit_NeoPixel(1,4,NEO_GRB + NEO_KHZ800);
            uint32_t _timer;//Updates checking timer
            uint16_t _pixels;//Number of LEDs in strip
            NetworkHandler* _NH;
            GETParser _responseParser;
            ///[Mode Parameters]
            uint8_t _mode = 1;
            uint8_t _speed = 255;
            uint8_t _brightness = 255;
            uint8_t _shift = 0;
            RGB _rgbColor = RGB(1,1,1);
            HSB _hsbColor = HSB(StripControl::_rgbColor);
            void _parseResponse();
            ///[Modes]
            void _fill();
            void _dispersion();
            void _shading();
    };

    inline void StripControl::checkUpdates(uint32_t latency) {
        if (millis() - _timer > CHECK_DELAY) {
            _NH->makeRequest();
            _timer = millis();
        } else {
            delay(latency);
        }
    }
 #endif