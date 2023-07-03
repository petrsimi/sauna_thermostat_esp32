#include "TFT_eSPI.h"
#include "sauna.h"
#include "ScreenConfig.h"

static bool timerWifiStatus_cb(void* ptr)
{
    ScreenConfig* screenConfig = (ScreenConfig*)ptr;
    screenConfig->displayWifiStatus();

    return true;
}


void ScreenConfig::displayWifiStatus()
{
    tft.fillRect(0, 0, 320, 120, TFT_BLACK);
    tft.setCursor(0, 17);
    tft.setFreeFont(FSS12);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);

    tft.print("Stav: ");
    switch (state.getWifiState()) {
        case WL_NO_SHIELD:
            tft.println("Chyba HW");
            break;
        case WL_IDLE_STATUS:
            tft.println("Idle");
            break;
        case WL_NO_SSID_AVAIL:
            tft.println("No SSID available");
            break;
        case WL_SCAN_COMPLETED:
            tft.println("Scan completed");
            break;
        case WL_CONNECTED:
            tft.println("Pripojeno");
            break;
        case WL_CONNECT_FAILED:
            tft.println("Chyba pripojeni");
            break;
        case WL_CONNECTION_LOST:
            tft.println("Ztrata signalu");
            break;
        case WL_DISCONNECTED:
            tft.println("Odpojeno");
            break;
    }
    tft.print("SSID: ");
    tft.println(state.getSsid());
    tft.print("IP: ");
    tft.println(state.getWifiIp());
    tft.print("Signal: ");
    tft.print(state.getWifiRssi());
    tft.println(" dbm");


}


ScreenConfig::ScreenConfig(TFT_eSPI &tft, State& state) :
    Screen(tft),
    state(state),
    btnConnect(&tft),
    btnExit(&tft)
{
    btnConnect.initButton(160, 160, 250, 40, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Nastavit WiFi", 1);
    btnExit.initButton(160, 210, 250, 40, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Zpet", 1);

    timerWifiStatus.every(1000, timerWifiStatus_cb, this);
}


void ScreenConfig::display()
{
    tft.fillScreen(TFT_BLACK);

    tft.setFreeFont(FSS12);
    btnConnect.drawButton();
    btnExit.drawButton();
}


void ScreenConfig::tick()
{
    timerWifiStatus.tick();
}


void ScreenConfig::handle_buttons(TSPoint& p)
{
    // Detect if a button was pressed
    if (p.z != 0) {
        if (btnConnect.contains(p.x, p.y)) {
            btnConnect.press(true);
        }
        if (btnExit.contains(p.x, p.y)) {
            btnExit.press(true);
        }
    } else {
        btnConnect.press(false);
        btnExit.press(false);
    }

    // Handle Connect button
    if (btnConnect.justPressed()) {
        tft.setFreeFont(FSS12);
        btnConnect.drawButton(true);
    } else if (btnConnect.justReleased()) {
        tft.setFreeFont(FSS12);
        btnConnect.drawButton(false);
        state.setScreen(SCREEN_CONFIG_SSID);
    }

    // Handle Exit button
    if (btnExit.justPressed()) {
        tft.setFreeFont(FSS12);
        btnExit.drawButton(true);
    } else if (btnExit.justReleased()) {
        tft.setFreeFont(FSS12);
        btnExit.drawButton(false);
        state.setScreen(SCREEN_STATUS);
    }
}