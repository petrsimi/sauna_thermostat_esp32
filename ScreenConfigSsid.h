#pragma once

#include "sauna.h"

#include <Arduino.h>
#include "Screen.h"
#include "State.h"
#include <arduino-timer.h>
#include "LcdKeyboard.h"
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>



class ScreenConfigSsid : public Screen {
    public:
        ScreenConfigSsid(TFT_eSPI& tft, State& state, LcdKeyboard& lcdKeyboard);

        void display();

        void handle_buttons(TSPoint& p);

        void tick();

        String ssid;

    private:
        State& state;

        void displaySsid(bool invert);

        LcdKeyboard& lcdKeyboard;
};