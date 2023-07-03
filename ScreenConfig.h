#pragma once

#include "sauna.h"

#include <Arduino.h>
#include "State.h"
#include "Screen.h"
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <arduino-timer.h>


class ScreenConfig : public Screen {
    public:
        ScreenConfig(TFT_eSPI& tft, State& state);

        void display();
        void displayWifiStatus();

        void tick();

        void handle_buttons(TSPoint& p);

    private:
        State& state;

        ButtonWidget btnConnect;
        ButtonWidget btnExit;

        Timer<1, millis> timerWifiStatus;
};