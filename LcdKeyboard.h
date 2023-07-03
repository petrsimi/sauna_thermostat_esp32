#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>


class LcdKeyboard {
    private:
        static const int  KEYS = 53;

        static const int8_t keymap_low[KEYS];
                                            
        static const int8_t keymap_high[KEYS];

    public:
        LcdKeyboard(TFT_eSPI& tft);
        ~LcdKeyboard();
        void draw();
        int8_t handlePress(int16_t x, int16_t y, bool pressed);

        static const int8_t KEY_ESC = -1;
        static const int8_t KEY_BACKSPACE = -2;
        static const int8_t KEY_ENTER = -3;
        static const int8_t KEY_SHIFT = -4;
        static const int8_t KEY_CAPS = -5;

    private:
        ButtonWidget* key[KEYS];
        bool shift;
        bool capslock;
};
