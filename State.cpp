#include <sys/_stdint.h>
#include <Preferences.h>

#include "State.h"


const unsigned long shutdown_interval = 3l * 60 * 60 * 1000; // 3 hours in miliseconds


static bool timer_shutdown_cb(void* ptr)
{
    State *state = (State*)ptr;

    Serial.println("Timeout expired. Turning off the heating.");
    state->setState(OFF);
    state->setVent(false);
    return false;
}


State::State(ezBuzzer& buzzer)
   : buzzer(buzzer)
{
    temp = 65535;
    target = 75;
    state = OFF;
    screen = SCREEN_STATUS;
    vent = false;
    reconnect = false;
}


void State::init()
{
    extern Preferences preferences;
    preferences.begin("sauna", false);

    ssid = preferences.getString("ssid", "");
    Serial.print("SSID: ");
    Serial.println(ssid);

    pwd = preferences.getString("pwd", "");
    Serial.print("PWD: ");
    Serial.println(pwd);

    target = preferences.getUChar("target");
    Serial.print("Target temperature: ");
    Serial.println(target);

    //preferences.end();
}


void State::tick()
{
    timer_shutdown.tick();
    buzzer.loop();
}


void State::beep(uint8_t count)
{
    if (count > 3) {
        count = 3;
    }
    static int melody[] = {NOTE_E6, 0, NOTE_E6, 0, NOTE_E6, 0};
    static int duration[] = {3, 8, 3, 8, 3, 8};
    buzzer.playMelody(melody, duration, count * 2);
}

void State::setTemp(uint16_t value) {
    temp = value;

    uint8_t div = value / 128;
    uint8_t mod = (value % 128) * 10 / 128; // we care only about one digit

    Serial.print("Temperature: ");
    Serial.print(div);
    Serial.print(".");
    Serial.println(mod);
}


uint16_t State::getTemp() {
    return temp;
}

uint8_t State::getTempWhole() {
    return temp / 128;
}

uint8_t State::getTempDecimal() {
    return (temp % 128) * 10 / 128;
}


void  State::setTarget(uint8_t target) {
    this->target = target;
}


uint8_t  State::getTarget() {
    return target;
}


void State::setState(state_t value)
{
    state = value;

    switch (value) {
        case ON:
            timer_shutdown.in(shutdown_interval, timer_shutdown_cb, this);
            if (getTemp() >= getTarget() * 128) {
                state = WAITING;
            } else {
                state = HEATING;
            }
            break;
        case OFF:
            // Store current target
            extern Preferences preferences;
            preferences.putUChar("target", target);
            // Cancel Shutdown timer
            timer_shutdown.cancel();
            break;
    }
}


state_t State::getState() {
    return state;
}


void State::setScreen(screen_t screen) {
    Serial.print("Screen: ");
    Serial.println(screen);
    this->screen = screen;
}


screen_t State::getScreen() {
    return screen;
}


void State::setVent(bool enable) {
    this->vent = enable;
}


bool State::getVent() {
    return vent;
}


void State::setWifiState(wl_status_t wlst) {
    wlstate = wlst;
}

wl_status_t State::getWifiState() {
    return wlstate;
}

void State::setSsid(String ssid) {
    this->ssid = ssid;
    Serial.print("New SSID: ");
    Serial.println(ssid);

    extern Preferences preferences;
    //preferences.begin("sauna", false);
    preferences.putString("ssid", ssid);
    //preferences.end();
}

String State::getSsid() {
    return ssid;
}


void State::setPwd(String pwd) {
    this->pwd = pwd;
    this->reconnect = true;
    Serial.print("New password: ");
    Serial.println(pwd);

    extern Preferences preferences;
    //preferences.begin("sauna", false);
    preferences.putString("pwd", pwd);
    //preferences.end();
}

String State::getPwd() {
    return pwd;
}


void State::setWifiIp(String ip) {
    Serial.print("IP address: ");
    Serial.println(ip);
    this->ip = ip;
}


void State::setWifiRssi(int8_t rssi) {
    this->rssi = rssi;
}

int8_t State::getWifiRssi()
{
    return rssi;
}


String State::getWifiIp()
{
    return ip;
}

bool State::isWifiCredentialsChanged() {
    bool tmp = reconnect;
    reconnect = false;
    return tmp;
}
