#include "sauna.h"
#include "ScreenStatus.h"
#include <ezBuzzer.h>



bool ScreenStatus::timer_cb(void* ptr)
{
    ScreenStatus *scr = (ScreenStatus*)ptr;

    bool cont = false;

    // Beep every 5 minutes
    if (scr->seconds == 5 * 60) {
        scr->state.beep(1);
    } else if (scr->seconds == 10 * 60) {
        scr->state.beep(2);
    } else if (scr->seconds == 15 * 60) {
        scr->state.beep(3);
    }

    if (scr->seconds < 15 * 60) {
        char buff[10];
        snprintf(buff, 9, "%02u:%02u", scr->seconds / 60, scr->seconds % 60);

        scr->tft.setFreeFont(FMB18);
        scr->btnTimer.initButton(160, 140, 140, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, buff, 1);
        scr->btnTimer.drawButton(true);

        cont = true;
    } else {
        scr->tft.setFreeFont(FSS12);
        scr->btnTimer.initButton(160, 140, 140, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Stopky", 1);
        scr->btnTimer.drawButton(false);
    }

    scr->seconds++;

    return cont;
}

ScreenStatus::ScreenStatus(TFT_eSPI &tft, State& state) :
    Screen(tft),
    state(state),
    btnMinus(&tft),
    btnPlus(&tft),
    btnOn(&tft),
    btnCfg(&tft),
    btnVent(&tft),
    btnTimer(&tft)
{
    btnMinus.initButton(40, 210, 60, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, "-", 1);
    btnMinus.setFreeFont(FSS24);

    btnPlus.initButton(200, 210, 60, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, "+", 1);
    btnOn.initButton(280, 210, 60, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, "ON", 1);

    btnCfg.initButton(40, 140, 60, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, "WiFi", 1);
    btnCfg.setFreeFont(FSS12);

    btnTimer.initButton(160, 140, 140, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Stopky", 1);

    btnVent.initButton(280, 140, 60, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Vent", 1);
}


void ScreenStatus::displayTemperature(uint16_t value, uint16_t color) {
    // convert raw temperature to fixed point Celsius
    uint8_t div = value / 128;
    uint8_t mod = (value % 128) * 10 / 128; // we care only about one digit

    // convert to string
    char buff[10];
    if (div < 100) {
        snprintf(buff, 10, "%u.%u", div, mod);
    } else {
        snprintf(buff, 10, "%u", div);
    }

    tft.setTextColor(color);
    //tft.setFreeFont(FSSB66);
    tft.setFreeFont(FMB76);
    tft.drawCentreString(buff, tft.width() / 2, -8, 1);
}



void ScreenStatus::displayTarget(uint8_t value, uint16_t color) {
    tft.setTextColor(color);
    tft.setFreeFont(FSS24);
    tft.drawCentreString(String(value), 120, 190, 1);
}



void ScreenStatus::display()
{
    tft.fillScreen(TFT_BLACK);

    tft.setFreeFont(FSS24);
    btnMinus.drawButton(false);
    btnPlus.drawButton(false);

    tft.setFreeFont(FSS12);
    btnOn.drawButton(false);
    btnCfg.drawButton(false);
    btnTimer.drawButton(false);
    btnVent.drawButton(false);

    displayTemperature(state.getTemp(), getTempColor());
    displayTarget(state.getTarget(), TFT_WHITE);
}


uint16_t ScreenStatus::getTempColor()
{
    switch (state.getState()) {
        case OFF:
            return TFT_WHITE;
        case HEATING:
            return TFT_RED;
        default:
            return TFT_GREEN;
    }
}


void ScreenStatus::tick()
{
    uint16_t curr_temp = state.getTemp();
    uint8_t curr_target = state.getTarget();
    state_t curr_state = state.getState();
    bool curr_vent = state.getVent();

    // Update displayed temperature
    if ((temp_last / 128) != (curr_temp / 128) ||
        ((temp_last % 128) * 10 / 128) != ((curr_temp % 128) * 10 / 128) ||
        (state_last != curr_state))
    {
        // Clear the old text
        displayTemperature(temp_last, TFT_BLACK);
        // and display new text
        displayTemperature(curr_temp, getTempColor());
    }

    // Update target temperature
    if (target_last != curr_target) {
        displayTarget(target_last, TFT_BLACK);
        displayTarget(curr_target, TFT_WHITE);
    }

    // Update Power button
    if (curr_state == OFF && state_last != OFF) {
        tft.setFreeFont(FSS12);
        btnOn.drawButton(false);
    } else if (curr_state != OFF && state_last == OFF) {
        tft.setFreeFont(FSS12);
        btnOn.drawButton(true);
    }

    // Update Ventilator button
    if (curr_vent == true && vent_last == false) {
        tft.setFreeFont(FSS12);
        btnVent.drawButton(true);
    } else if (curr_vent == false && vent_last == true) {
        tft.setFreeFont(FSS12);
        btnVent.drawButton(false);
    }

    timer.tick();


    temp_last = curr_temp;
    state_last = curr_state;
    target_last = curr_target;
    vent_last = curr_vent;
}


void ScreenStatus::handle_buttons(TSPoint& p)
{
    // Detect if a button was pressed
    if (p.z != 0) {
        if (btnMinus.contains(p.x, p.y)) {
            btnMinus.press(true);
        }
        if (btnPlus.contains(p.x, p.y)) {
            btnPlus.press(true);
        }
        if (btnOn.contains(p.x, p.y)) {
            btnOn.press(true);
        }
        if (btnCfg.contains(p.x, p.y)) {
            btnCfg.press(true);
        }
        if (btnTimer.contains(p.x, p.y)) {
            btnTimer.press(true);
        }
        if (btnVent.contains(p.x, p.y)) {
            btnVent.press(true);
        }
    } else {
        btnMinus.press(false);
        btnPlus.press(false);
        btnOn.press(false);
        btnCfg.press(false);
        btnTimer.press(false);
        btnVent.press(false);
    }

    // Handle Minus button
    if (btnMinus.justPressed()) {
        tft.setFreeFont(FSS24);
        btnMinus.drawButton(true);
        state.setTarget(state.getTarget() - 1);
    } else if (btnMinus.justReleased()) {
        tft.setFreeFont(FSS24);
        btnMinus.drawButton(false);
    }

    // Handle Plus button
    if (btnPlus.justPressed()) {
        tft.setFreeFont(FSS24);
        btnPlus.drawButton(true);
        state.setTarget(state.getTarget() + 1);
    } else if (btnPlus.justReleased()) {
        tft.setFreeFont(FSS24);
        btnPlus.drawButton(false);
    }

    // Handle Power button
    if (btnOn.justPressed()) {
        if (state.getState() == OFF) {
            state.setState(ON);
        } else {
            state.setState(OFF);
        }
    }

    // Handle Ventilator button
    if (btnVent.justPressed()) {
        if (state.getVent() == false) {
            state.setVent(true);
        } else {
            state.setVent(false);
        }
    }

    // Handle Config button
    if (btnCfg.justPressed()) {
        tft.setFreeFont(FSS12);
        btnCfg.drawButton(true);
    } else if (btnCfg.justReleased()) {
        tft.setFreeFont(FSS12);
        btnCfg.drawButton(false);
        state.setScreen(SCREEN_CONFIG);
    }


    // Handle Timer button
    if (btnTimer.justPressed()) {
        if (timer.empty()) { // timer is not running
            seconds = 0;
            timer_cb(this);
            timer.every(1000, timer_cb, this);
        } else {
            timer.cancel();
            tft.setFreeFont(FSS12);
            btnTimer.initButton(160, 140, 140, 60, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Stopky", 1);
            btnTimer.drawButton(false);
        }
    }
}
