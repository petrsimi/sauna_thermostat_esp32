#include "ScreenConfigPwd.h"



ScreenConfigPwd::ScreenConfigPwd(TFT_eSPI& tft, State& state, LcdKeyboard &lcdKeyboard)
    : Screen(tft), state(state), lcdKeyboard(lcdKeyboard)
{
    
}


void ScreenConfigPwd::display()
{
    tft.fillScreen(TFT_BLACK);
    tft.setFreeFont(NULL);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.setTextColor(TFT_CYAN);

    tft.print("Zadejte heslo:");

    displayPwd(false);

    lcdKeyboard.draw();
}


void ScreenConfigPwd::displayPwd(bool invert)
{
    tft.setTextSize(2);
    tft.setCursor(0, 20);
    tft.setTextColor(invert ? TFT_BLACK : TFT_WHITE);
    tft.print(pwd);
}

void ScreenConfigPwd::tick()
{

}

void ScreenConfigPwd::handle_buttons(TSPoint& p)
{
    int8_t key = lcdKeyboard.handlePress(p.x, p.y, p.z);

    if (key == LcdKeyboard::KEY_ESC) {
        state.setScreen(SCREEN_CONFIG);
        pwd = "";
    } else if (key == LcdKeyboard::KEY_ENTER) {
        state.setScreen(SCREEN_CONFIG);
        pwdAvailable = true;
    } else if (key == LcdKeyboard::KEY_BACKSPACE) {
        // clear the old text
        displayPwd(true);
        // remove the character
        pwd.remove(pwd.length()-1);
        // and display the SSID
        displayPwd(false);
    } else if (key > 0) {
        pwd += (char)key;
        displayPwd(false);
    }
}
