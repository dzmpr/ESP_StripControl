#include <Arduino.h>
#include <RGB_LED.h>

RGB_LED::RGB_LED(uint8_t pin_red, uint8_t pin_green, uint8_t pin_blue) {
    _pin_red = pin_red;
    _pin_green = pin_green;
    _pin_blue = pin_blue;
    pinMode(pin_red, OUTPUT);
    pinMode(pin_green, OUTPUT);
    pinMode(pin_blue, OUTPUT);
}

void RGB_LED::_led_set(colors color) { //todo: write led through reg manip
    switch (color)
    {
        case LED_RED: {
            digitalWrite(_pin_blue, 0);
            digitalWrite(_pin_red, 1);
            digitalWrite(_pin_green, 0);
            break;
        }
        case LED_BLUE: {
            digitalWrite(_pin_blue, 1);
            digitalWrite(_pin_red, 0);
            digitalWrite(_pin_green, 0);
            break;
        }
        case LED_GREEN: {
            digitalWrite(_pin_blue, 0);
            digitalWrite(_pin_red, 0);
            digitalWrite(_pin_green, 1);
            break;
        }
        case LED_WHITE: {
            digitalWrite(_pin_blue, 1);
            digitalWrite(_pin_red, 1);
            digitalWrite(_pin_green, 1);
            break;
        }
        case LED_MARINE: {
            digitalWrite(_pin_blue, 1);
            digitalWrite(_pin_red, 0);
            digitalWrite(_pin_green, 1);
            break;
        }
        case LED_PURPLE: {
            digitalWrite(_pin_blue, 1);
            digitalWrite(_pin_red, 1);
            digitalWrite(_pin_green, 0);
            break;
        }
        case LED_YELLOW: {
            digitalWrite(_pin_blue, 0);
            digitalWrite(_pin_red, 1);
            digitalWrite(_pin_green, 1);
            break;
        }
        case LED_OFF:
        default: {
            digitalWrite(_pin_blue, 0);
            digitalWrite(_pin_red, 0);
            digitalWrite(_pin_green, 0);
            break;
        }
    }
}

void RGB_LED::led_set(colors color) {
    _color = color;
    if (color != LED_OFF) {
        _state = 1;
    } else {
        _state = 0;
    }
    _led_set(color);
}

void RGB_LED::invert() {
    if (_state) {
        _state = 0;
        _led_set(LED_OFF);
    } else {
        _state = 1;
        _led_set(_color);
    }
}

uint8_t RGB_LED::get_pin(colors color) {
    switch (color)
    {   
        case LED_RED:
            return _pin_red;
        case LED_GREEN:
            return _pin_green;
        case LED_BLUE:
            return _pin_blue;
        default:
            break;
    }
    return 0;
}

