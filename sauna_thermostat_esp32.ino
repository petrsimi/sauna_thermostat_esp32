#include <Arduino.h>
#include <Preferences.h>

#include <arduino-timer.h>
#include <ezBuzzer.h>


#include <OneWire.h>
#include <DallasTemperature.h>

#include <TFT_eSPI.h>
#include <TFT_eWidget.h>
#include <ESP32_TouchScreen.h>

#include "Filter.h"

#include "WifiWrap.h"

#include "LcdKeyboard.h"
#include "State.h"
#include "ScreenStatus.h"
#include "ScreenConfig.h"
#include "ScreenConfigSsid.h"
#include "ScreenConfigPwd.h"

// Temperature hysteresis in degrees
#define HYSTERESIS 0.5

// Output to drive Heater
#define HEATING_OUT     19

// Output to drive LED
#define LED_OUT       23

// Output to drive Heater
#define VENTILATOR_OUT     18

// Data wire is connected to the Arduino digital pin
#define ONE_WIRE_BUS 21

#define BUZZER_PIN  22
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
DeviceAddress sensors_addr;

ezBuzzer buzzer(BUZZER_PIN);

Preferences preferences;



TFT_eSPI tft;


// adjust pressure sensitivity - note works 'backwards'
#define MINPRESSURE 200
#define MAXPRESSURE 10000


#define TOUCH_PIN_YP TFT_WR
#define TOUCH_PIN_XM TFT_DC
#define TOUCH_PIN_YM TFT_D7
#define TOUCH_PIN_XP TFT_D6

TouchScreen ts(TOUCH_PIN_XP, TOUCH_PIN_YP, TOUCH_PIN_XM, TOUCH_PIN_YM, 300);



LcdKeyboard lcdKeyboard(tft);

State state(buzzer);

ScreenStatus screenStatus(tft, state);
ScreenConfig screenConfig(tft, state);
ScreenConfigSsid screenConfigSsid(tft, state, lcdKeyboard);
ScreenConfigPwd screenConfigPwd(tft, state, lcdKeyboard);


Screen* currScreen;

screen_t screen_last;


Timer<1, millis> timer_sensor;


Filter filter_temp(5);

// Read out current temperature
bool timer_sensor_cb(void* temp)
{
    uint16_t current = sensors.getTemp(sensors_addr);
    sensors.requestTemperatures();

    // multiply by 9% as the sensor was showing 70 degree while the real temperature was 76
    uint32_t aaa = current;
    current = aaa * 109/100;

    filter_temp.add(current);

    state.setTemp(filter_temp.getAvg());

    return true; // to repeat the action - false to stop
}


void TaskWifi(void* params)
{
    WifiWrap wifiWrap(state);
    // run infinite process
    wifiWrap.run();
}


void setup() {
    pinMode(HEATING_OUT, OUTPUT);
    digitalWrite(HEATING_OUT, LOW);

    pinMode(LED_OUT, OUTPUT);
    digitalWrite(LED_OUT, LOW);

    pinMode(VENTILATOR_OUT, OUTPUT);
    digitalWrite(VENTILATOR_OUT, LOW);

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.begin(115200);

    state.init();

    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    ts.enableRestore();

    // Init temperature sensor
    sensors.begin();
    sensors.setWaitForConversion(false);
    if (!sensors.getAddress(sensors_addr, 0)) {
        Serial.println(F("ERROR: temperature sensor was not found"));
    }

    // monitor temperature every 1000 ms
    timer_sensor.every(1000, timer_sensor_cb, NULL);


    xTaskCreate(
        TaskWifi,
        "Task WiFi",
        1024*32,
        NULL,
        1,
        NULL
    );

    currScreen = &screenStatus;

    currScreen->display();
}



Filter touch_x(64);
Filter touch_y(64);

// new display which is not used
/*
#define TS_MINX 10
#define TS_MINY -10
#define TS_MAXX 470
#define TS_MAXY 310
*/

#define TS_MINX 130
#define TS_MINY 140
#define TS_MAXX 963
#define TS_MAXY 900

bool pressed_last = false;
bool pressed_curr = false;

TSPoint handleTouch()
{
    uint16_t x, y, z;
    ts.getTouchRaw(x, y, z);

    TSPoint p, ret;
    p.x = x;
    p.y = y;
    p.z = z;

    ret.z = 0;

    // swap axis
    int16_t tmp = p.x;
    p.x = p.y;
    p.y = tmp;

    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
/*
        Serial.print("X = "); Serial.print(p.x);
        Serial.print("\tY = "); Serial.print(p.y);
        Serial.print("\tPressure = "); Serial.println(p.z);
*/
        p.x = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
        p.y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());
        
        ret.x = touch_x.add(p.x);
        ret.y = touch_y.add(p.y);

        if (touch_x.isFull() && touch_y.isFull()) {
            ret.z = 1;
            //tft.drawPixel(ret.x, ret.y, TFT_YELLOW);
        }


    } else {
        p.z = 0;
        touch_x.clear();
        touch_y.clear();
    }


    return ret;
}

void loop() {
    // put your main code here, to run repeatedly:
    TSPoint p = handleTouch();

    state.tick();
    timer_sensor.tick();
    currScreen->tick();

    currScreen->handle_buttons(p);

    // State machine
    switch (state.getState()) {
        case ON:
        case HEATING:
            if (state.getTemp() >= state.getTarget() * 128) {
                state.setState(WAITING);
            } else {
                state.setState(HEATING);
            }
            break;
        case WAITING:
            if (state.getTemp() < state.getTarget() * 128 - HYSTERESIS * 128) {
                state.setState(HEATING);
            }
            break;
        case OFF:

            break;
    }

    // Drive the output signal to the heater
    digitalWrite(HEATING_OUT, state.getState() == HEATING ? HIGH : LOW);
    digitalWrite(LED_OUT, state.getState() == HEATING ? HIGH : LOW);


    digitalWrite(VENTILATOR_OUT, state.getVent() ? HIGH : LOW);

    // Display the whole screen
    if (screen_last != state.getScreen()) {
        switch(state.getScreen()) {
            case SCREEN_STATUS:
                currScreen = &screenStatus;
                break;
            case SCREEN_CONFIG:
                currScreen = &screenConfig;
                if (screenConfigPwd.pwdAvailable) {
                    screenConfigPwd.pwdAvailable = false;
                    state.setSsid(screenConfigSsid.ssid);
                    state.setPwd(screenConfigPwd.pwd);
                }
                break;
            case SCREEN_CONFIG_SSID:
                currScreen = &screenConfigSsid;
                break;
            case SCREEN_CONFIG_PWD:
                currScreen = &screenConfigPwd;
                break;
        }
        currScreen->display();
        screen_last = state.getScreen();
    }


    //usleep(1000);
}
