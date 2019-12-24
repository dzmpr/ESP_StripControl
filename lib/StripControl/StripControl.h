/**
 * 
 * 
 */
 #ifndef StripControl_h
 #define StripControl_h
    #include <Arduino.h>
    #include <Adafruit_NeoPixel.h>
    #include "NetworkHandler.h"

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
            HSB(const RGB&);
            HSB(uint16_t, uint8_t, uint8_t);
            void fromRGB(const RGB&);
            void setColor(uint16_t, uint8_t, uint8_t);
            void setBrightness(uint8_t);
            void setNormalizedColor(uint16_t, uint8_t, uint8_t);
            void cycleHue(uint16_t step = 1);
            uint16_t getHue();
            uint8_t getSaturation();
            uint8_t getBrightness();
    };

    class RGB {
        private:
            friend class HSB;
            uint8_t _red;
            uint8_t _green;
            uint8_t _blue;
        public:
            RGB();
            RGB(const HSB&);
            RGB(uint8_t, uint8_t, uint8_t);
            void fromHSB(const HSB&);
            void setColor(uint8_t, uint8_t, uint8_t);
            uint8_t getRed();
            uint8_t getGreen();
            uint8_t getBlue();
            uint32_t getPacked();
    };

    class StripControl {
        public:
            StripControl(uint16_t, NetworkHandler*);
            void modeSelection();
        private:
            ///[Service]
            Adafruit_NeoPixel _strip = Adafruit_NeoPixel(1,4,NEO_GRB + NEO_KHZ800);
            uint32_t _timer;
            uint16_t _pixels;
            NetworkHandler* _NH;
            ///[Mode Parameters]
            uint8_t _mode = 1;
            uint8_t _speed = 255;
            uint8_t _brightness = 255;
            uint8_t _shift = 0;
            RGB _rgbColor = RGB(1,1,1);
            HSB _hsbColor = HSB(StripControl::_rgbColor);
            inline void _checkUpdates(uint32_t);
            ///[Modes]
            void _fill();
            void _dispersion();
            void _shading();
    };
 #endif