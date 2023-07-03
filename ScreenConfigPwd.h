#pragma once

#include "sauna.h"

#include <Arduino.h>
#include "Screen.h"
#include "State.h"
#include <arduino-timer.h>
#include "LcdKeyboard.h"
#include <TFT_eSPI.h>
#include <TFT_eWidget.h>



class ScreenConfigPwd : public Screen {
    public:
        ScreenConfigPwd(TFT_eSPI& tft, State& state, LcdKeyboard& lcdKeyboard);

        void display();

        void handle_buttons(TSPoint& p);

        void tick();

        String pwd;
        bool pwdAvailable;

    private:
        State& state;

        void displayPwd(bool invert);

        LcdKeyboard& lcdKeyboard;
};