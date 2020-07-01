/**
 * 
 * 
 */

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "StripControl.h"

#define forever for (;;)

///     HSB class

//Constructor from separate parameters
HSB::HSB(uint16_t hue, uint8_t saturation, uint8_t brightness) : _hue(hue), _saturation(saturation), _brightness(brightness) {}

//Constructor from RGB color
HSB::HSB(const RGB &source) {
    fromRGB(source);
}

HSB::HSB() : _hue(0), _saturation(0), _brightness(0) {}

void HSB::fromRGB(const RGB &source) {
    uint8_t max = source._red;
    uint8_t min = source._red;

    if (max < source._green) max = source._green;
    if (max < source._blue) max = source._blue;
    if (min > source._green) min = source._green;
    if (min > source._blue) min = source._blue;

    uint8_t delta = max - min;

    _brightness = max;

    if (max == 0) {
        _saturation = 0;
    } else {
        _saturation = delta * 255 / max;
    }

    if (max == min) {
        _hue = 0;
    } else if (max == source._red) {
        if (source._green >= source._blue) {
            _hue = ((255 * (source._green - source._blue) / delta)) % 1530;
        } else {
            _hue = ((255 * (source._green - source._blue) / delta) + 1530) % 1530;
        }
    } else if (max == source._green) {
        _hue = ((255 * (source._blue - source._red) / delta) + 510) % 1530;
    } else {
        _hue = ((255 * (source._red - source._green) / delta) + 1020) % 1530;
    }
}

void HSB::setBrightness(uint8_t b) {
    _brightness = b;
}

void HSB::setColor(uint16_t h, uint8_t s, uint8_t b) {
    _hue = h;
    _saturation = s;
    _brightness = b;
}

void HSB::setNormalizedColor(uint16_t h, uint8_t s, uint8_t b) {
    _hue = h * 4.25;
    _saturation = s * 2.55;
    _brightness = b * 2.55;
}

void HSB::cycleHue(uint16_t step) {
    _hue = (_hue + step) % 1530;
}

uint16_t HSB::getHue() {
    return _hue;
}

uint8_t HSB::getSaturation() {
    return _saturation;
}

uint8_t HSB::getBrightness() {
    return _brightness;
}

///     RGB class

//Constructor from separate parameters
RGB::RGB(uint8_t red, uint8_t green, uint8_t blue) : _red(red), _green(green), _blue(blue) {}

//Constructor from HSB color
RGB::RGB(const HSB &source) {
    fromHSB(source);
}

RGB::RGB(): _red(0), _green(0), _blue(0) {}

RGB::RGB(uint32_t packed) {
    setPacked(packed);
}

void RGB::fromHSB(const HSB &source) {
    uint8_t part = (source._hue / 255) % 6;
    uint8_t min = ((255 - source._saturation) * source._brightness) / 255;
    uint8_t a = ((source._brightness - min) * (source._hue % 255) / 255);
    uint8_t inc = min + a;
    uint8_t dec = source._brightness - a;

    switch (part) {
        case 0: {
            _red = source._brightness;
            _green = inc;
            _blue = min;
            break;
        }
        case 1: {
            _red = dec;
            _green = source._brightness;
            _blue = min;
            break;
        }
        case 2: {
            _red = min;
            _green = source._brightness;
            _blue = inc;
            break;
        }
        case 3: {
            _red = min;
            _green = dec;
            _blue = source._brightness;
            break;
        }
        case 4: {
            _red = inc;
            _green = min;
            _blue = source._brightness;
            break;
        }
        case 5: {
            _red = source._brightness;
            _green = min;
            _blue = dec;
            break;
        }
    }
}

void RGB::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _red = r;
    _green = g;
    _blue = b;
}

void RGB::setPacked(uint32_t packed) {
    _blue = packed & 0xFF;
    _green = (packed >> 8) & 0xFF;
    _red = (packed >> 16) & 0xFF;
}

uint8_t RGB::getRed() {
    return _red;
}

uint8_t RGB::getGreen() {
    return _green;
}

uint8_t RGB::getBlue() {
    return _blue;
}

uint32_t RGB::getPacked() {
    return ((uint32_t)_red << 16) | ((uint32_t)_green <<  8) | _blue;
}

///     StripControl class

StripControl::StripControl(uint16_t pixels, NetworkHandler* NH): _pixels(pixels), _NH(NH) {
    _strip.updateLength(_pixels);
    _strip.setBrightness(255);
    _strip.begin();
    //TODO: test
    _strip.fill(65793);
    _strip.show();
}

void StripControl::modeSelection() {
    DEBUG("[STRIP]Select mode.");
    _parseResponse();
    DEBUG("[STRIP]Mode: " + String(_mode));
    DEBUG("[STRIP]Speed: " + String(_speed));
    DEBUG("[STRIP]Shift: " + String(_shift));
    DEBUG("[STRIP]Brightness: " + String(_brightness));
    switch (_mode) {
        case 1: {
            _fill();
            break;
        }
        case 2: {
            _dispersion();
            break;
        }
        case 3: {
            _shading();
            break;
        }
    }
}



void StripControl::_parseResponse() {
    /** Data to parse:
     * Mode, speed, shift, brightness - uint8_t
     * Color - uint32_t
     */
    DEBUG("[STRIP]Getting new mode data.");
    _responseParser.replaceSource(_NH->getResponse());
    uint8_t temp_8bit;
    uint32_t temp_32bit;
    temp_8bit = _responseParser.parseUint<uint8_t>("mode");
    if (!_responseParser.isErrorOccured()) {
        _mode = temp_8bit;
    }
    temp_8bit = _responseParser.parseUint<uint8_t>("speed");
    if (!_responseParser.isErrorOccured()) {
        _speed = temp_8bit;
    }
    temp_8bit = _responseParser.parseUint<uint8_t>("shift");
    if (!_responseParser.isErrorOccured()) {
        _shift = temp_8bit;
    }
    temp_8bit = _responseParser.parseUint<uint8_t>("bright");
    if (!_responseParser.isErrorOccured()) {
        _brightness = temp_8bit;
    }
    temp_32bit = _responseParser.parseUint<uint32_t>("color");
    if (!_responseParser.isErrorOccured()) {
        _rgbColor.setPacked(temp_32bit);
    }
    _NH->dataProcessed();
}

///[Modes]

void StripControl::_fill() {
    _strip.fill(_rgbColor.getPacked());
    _strip.show();
}

//TODO: explain "magic formulas"
void StripControl::_dispersion() {//TODO: finish
    uint32_t speed = 70 + 70*_speed/255;//TODO: variable latency mb needed
    uint16_t shiftedPixels = _pixels * _shift / 255;
    uint16_t currentPixels = 0;
    if (shiftedPixels == 0) shiftedPixels = 1;
    HSB currentColor;

    forever {
        yield();
        for (uint16_t i = 0; i < shiftedPixels; i++) {
            currentColor = _hsbColor;
            _rgbColor.fromHSB(currentColor);
            currentPixels = 0;
            for (uint16_t j = 0; j < _pixels; j++) {
                if ((currentPixels + i) % shiftedPixels == 0) {
                    currentColor.cycleHue(4);
                    _rgbColor.fromHSB(currentColor);
                }
                _strip.setPixelColor(j, _rgbColor.getPacked());
                currentPixels++;
            }
            _strip.show();
            if (i == 0) _hsbColor.cycleHue(4);
            checkUpdates(speed);
            if (_NH->newDataStatus()) return;
        }
    }
}

void StripControl::_shading() {
    randomSeed(millis());

    forever {
        yield();
        _hsbColor.setNormalizedColor(random(361), 255, 3);
        for (int16_t i = 4; i <= _brightness; i++) {
            _hsbColor.setBrightness(i);
            _rgbColor.fromHSB(_hsbColor);
            _strip.fill(_rgbColor.getPacked());
            _strip.show();

            // delay(20 + 50*_speed/255);
            checkUpdates(20 + 50*_speed/255);
            if (_NH->newDataStatus()) return;
        }

        for (int16_t i = _brightness; i > 4; i--) {
            _hsbColor.setBrightness(i);
            _rgbColor.fromHSB(_hsbColor);
            _strip.fill(_rgbColor.getPacked());
            _strip.show();

            // delay(20 + 50*_speed/255);
            checkUpdates(20 + 50*_speed/255);
            if (_NH->newDataStatus()) return;
        }
    }
}
