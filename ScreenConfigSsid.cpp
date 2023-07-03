#include "sauna.h"
#include "ScreenConfigSsid.h"

ScreenConfigSsid::ScreenConfigSsid(TFT_eSPI& tft, State& state, LcdKeyboard &lcdKeyboard)
    : Screen(tft), state(state), lcdKeyboard(lcdKeyboard)
{
    
}



void ScreenConfigSsid::display()
{
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(NULL);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(TFT_CYAN);

    tft.print("Zadejte SSID:");

    displaySsid(false);

    lcdKeyboard.draw();
}


void ScreenConfigSsid::displaySsid(bool invert)
{
    tft.setTextSize(2);
    tft.setCursor(0, 20);
    tft.setTextColor(invert ? TFT_BLACK : TFT_WHITE);
    tft.print(ssid);
}

void ScreenConfigSsid::tick()
{

}


void ScreenConfigSsid::handle_buttons(TSPoint& p)
{
    int8_t key = lcdKeyboard.handlePress(p.x, p.y, p.z);

    if (key == LcdKeyboard::KEY_ESC) {
        state.setScreen(SCREEN_CONFIG);
        ssid = "";
    } else if (key == LcdKeyboard::KEY_ENTER) {
        state.setScreen(SCREEN_CONFIG_PWD);
    } else if (key == LcdKeyboard::KEY_BACKSPACE) {
        // clear the old text
        displaySsid(true);
        // remove the character
        ssid.remove(ssid.length()-1);
        // and display the SSID
        displaySsid(false);
    } else if (key > 0) {
        ssid += (char)key;
        displaySsid(false);
    }
}