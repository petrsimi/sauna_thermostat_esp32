#pragma once

#include "sauna.h"

#include <Arduino.h>
#include "State.h"
#include <WiFi.h>
#include <WebServer.h>


class WifiWrap {
    public:
        WifiWrap(State& state);

        void run();

    private:
        void handleRoot();

        void onWifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);

        void connect(String ssid, String pwd);

        WebServer server;
        State& state;

        SemaphoreHandle_t mutexConnect;

};