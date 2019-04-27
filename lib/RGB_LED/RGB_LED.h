

#ifndef RGB_LED_h
#define RGB_LED_h

#include <Arduino.h>

//  Color enum type
typedef enum {
    LED_OFF,
    LED_RED,
    LED_BLUE,
    LED_WHITE,
    LED_GREEN,
    LED_PURPLE,
    LED_MARINE,
    LED_YELLOW
} colors;

//  RGB class
class RGB_LED {
    private:
        uint8_t _state = 0;
        colors _color = LED_OFF;
        uint8_t _pin_red;
        uint8_t _pin_green;
        uint8_t _pin_blue;
        void _led_set(colors);
    public:
        RGB_LED(uint8_t, uint8_t, uint8_t);
        void led_set(colors);
        void invert();
        uint8_t get_pin(colors);
};
#endif