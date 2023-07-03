#pragma once

#include <Arduino.h>
#include <ezBuzzer.h>
#include <Preferences.h>
#include <arduino-timer.h>
#include <WiFiType.h>
#include "sauna.h"

class State {
    public:
        State(ezBuzzer& buzzer);

        void init();

        void tick();

        void beep(uint8_t count);

        void setTemp(uint16_t temp);
        uint16_t getTemp();
        uint8_t getTempWhole();
        uint8_t getTempDecimal();

        void setTarget(uint8_t target);
        uint8_t getTarget();

        void setState(state_t state);
        state_t getState();

        void setScreen(screen_t screen);
        screen_t getScreen();

        void setVent(bool enable);
        bool getVent();



        void setWifiState(wl_status_t wlst);
        wl_status_t getWifiState();

        void setWifiIp(String ip);
        String getWifiIp();

        void setWifiRssi(int8_t rssi);
        int8_t getWifiRssi();

        void setSsid(String ssid);
        String getSsid();

        void setPwd(String pwd);
        String getPwd();

        bool isWifiCredentialsChanged();

    private:
        ezBuzzer& buzzer;

        Timer<1, millis> timer_shutdown;


        uint16_t temp;
        uint8_t target;
        state_t state;
        screen_t screen;
        bool vent;

        wl_status_t wlstate;
        String ssid;
        String pwd;
        String ip;
        int8_t rssi;
        bool reconnect;

};