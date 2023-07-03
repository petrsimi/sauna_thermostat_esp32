#pragma once

#include "sauna.h"

#include <Arduino.h>
#include <ESP32_TouchScreen.h>


class Screen {
    public:
        Screen(TFT_eSPI& tft) : tft(tft) {
        };
        virtual void display() = 0;
        virtual void tick() = 0;
        virtual void handle_buttons(TSPoint& p) = 0;


    protected:
        TFT_eSPI &tft;
};