#pragma once

#include "sauna.h"

#include <Arduino.h>
#include "State.h"
#include "Screen.h"
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <arduino-timer.h>



class ScreenStatus : public Screen {
    public:

        ScreenStatus(TFT_eSPI& tft, State& state);

        void display();

        void tick();

        void handle_buttons(TSPoint& p);


    private:
        uint16_t getTempColor();
        void displayTemperature(uint16_t value, uint16_t color);
        void displayTarget(uint8_t value, uint16_t color);
        void displayTimer();

        State& state;

        uint16_t temp_last;
        uint8_t target_last;
        state_t state_last;
        bool vent_last;
        uint32_t seconds_last;

        ButtonWidget btnMinus;
        ButtonWidget btnPlus;
        ButtonWidget btnOn;
        ButtonWidget btnCfg;
        ButtonWidget btnVent;
        ButtonWidget btnTimer;
};
