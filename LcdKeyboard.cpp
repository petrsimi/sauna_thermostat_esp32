#include  "LcdKeyboard.h"

#define KEY_WIDTH 28
#define KEY_HEIGHT 28


const int8_t LcdKeyboard::keymap_low[KEYS]  = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', KEY_ESC,
                                                    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', KEY_BACKSPACE,
                                                    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', KEY_ENTER,
                                                    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', KEY_SHIFT,
                                                    '`', '-', '=', '[', ']', '\\', '\'', ' ', KEY_CAPS};
                                            
const int8_t LcdKeyboard::keymap_high[KEYS] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', KEY_ESC,
                                                    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', KEY_BACKSPACE,
                                                    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', KEY_ENTER,
                                                    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', KEY_SHIFT,
                                                    '~', '_', '+', '{', '}', '|', '"', ' ', KEY_CAPS};




LcdKeyboard::LcdKeyboard(TFT_eSPI& tft)
{
    for (int i = 0; i < KEYS; i++) {
        key[i] = new ButtonWidget(&tft);
    }

    shift = false;
    capslock = false;

    // First row (numbers)
    
    key[ 0]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  0, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "1!", 1);
    key[ 1]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  1, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "2@", 1);
    key[ 2]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  2, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "3#", 1);
    key[ 3]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  3, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "4$", 1);
    key[ 4]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  4, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "5%", 1);
    key[ 5]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  5, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "6^", 1);
    key[ 6]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  6, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "7&", 1);
    key[ 7]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  7, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "8*", 1);
    key[ 8]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  8, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "9(", 1);
    key[ 9]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  9, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "0)", 1);
    key[10]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) * 10, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 0, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_RED, TFT_BLACK, "Esc", 1);

    // Second row (qwertyuiop)
    key[11]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  0, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Q", 1);
    key[12]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  1, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "W", 1);
    key[13]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  2, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "E", 1);
    key[14]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  3, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "R", 1);
    key[15]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  4, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "T", 1);
    key[16]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  5, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Y", 1);
    key[17]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  6, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "U", 1);
    key[18]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  7, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "I", 1);
    key[19]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  8, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "O", 1);
    key[20]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  9, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "P", 1);
    key[21]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) * 10, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 1, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_YELLOW, TFT_BLACK, "Back", 1);

    // Third row (asdfghjkl)
    key[22]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  0, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "A", 1);
    key[23]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  1, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "S", 1);
    key[24]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  2, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "D", 1);
    key[25]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  3, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "F", 1);
    key[26]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  4, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "G", 1);
    key[27]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  5, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "H", 1);
    key[28]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  6, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "J", 1);
    key[29]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  7, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "K", 1);
    key[30]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  8, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "L", 1);
    key[31]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  9, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, ";:", 1);
    key[32]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) * 10, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 2, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_GREEN, TFT_BLACK, "Ent", 1);
    // Fourth row (zxcvbnm,.)
    key[33]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  0, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Z", 1);
    key[34]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  1, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "X", 1);
    key[35]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  2, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "C", 1);
    key[36]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  3, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "V", 1);
    key[37]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  4, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "B", 1);
    key[38]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  5, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "N", 1);
    key[39]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  6, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "M", 1);
    key[40]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  7, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, ",<", 1);
    key[41]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  8, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, ".>", 1);
    key[42]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  9, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "/?", 1);
    key[43]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) * 10, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 3, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Shi", 1);

    // Fifth row (special chars)
    key[44]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  0, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "`~", 1);
    key[45]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  1, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "-_", 1);
    key[46]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  2, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "=+", 1);
    key[47]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  3, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "[{", 1);
    key[48]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  4, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "]}", 1);
    key[49]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  5, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "\\|", 1);
    key[50]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  6, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "'\"", 1);
    key[51]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) *  8, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH*3 + 2, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Space", 1);
    key[52]->initButton(KEY_WIDTH/2 + (KEY_WIDTH+1) * 10, 96 + KEY_HEIGHT/2 + (KEY_HEIGHT+1) * 4, KEY_WIDTH, KEY_HEIGHT, TFT_WHITE, TFT_BLACK, TFT_WHITE, "Caps", 1);
}


LcdKeyboard::~LcdKeyboard()
{
    for (int i = 0; i < KEYS; i++) {
        delete key[i];
    }
}


void LcdKeyboard::draw()
{
    for (int i=0; i<KEYS; i++) {
        key[i]->drawButton();
    }
}

int8_t LcdKeyboard::handlePress(int16_t x, int16_t y, bool pressed)
{
    int8_t ret = 0;

    for (int i=0; i<KEYS; i++) {
        // Update "press" status for all keys
        if (pressed && key[i]->contains(x, y)) {
            key[i]->press(true);
        } else {
            key[i]->press(false);
        }

        // Handle the press event of the key
        if (key[i]->justPressed()) {
            key[i]->drawButton(true);
            if (shift || capslock) {
                ret = keymap_high[i];
            } else {
                ret = keymap_low[i];
            }

            if (ret == KEY_SHIFT) {
                shift = !shift;
            } else if (shift) {
                shift = false;
                key[43]->drawButton(shift);
            }
            if (ret == KEY_CAPS) {
                capslock = !capslock;
            }
            if (ret == KEY_ESC || ret == KEY_ENTER) {
                ret = 0;
            }
        }

        // Handle the release event of the key
        if (key[i]->justReleased()) {
            switch(keymap_low[i]) {
                case KEY_SHIFT:
                    key[i]->drawButton(shift);
                    break;
                case KEY_CAPS:
                    key[i]->drawButton(capslock);
                    break;
                case KEY_ESC:
                    ret = KEY_ESC;
                    break;
                case KEY_ENTER:
                    ret = KEY_ENTER;
                default:
                    // Release the key
                    key[i]->drawButton(false);
                    break;
            }
        }

    }

    return ret;
}