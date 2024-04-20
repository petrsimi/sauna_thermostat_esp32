#include "sauna.h"
#include "WifiWrap.h"
#include <WiFi.h>
#include <ESPmDNS.h>


static const char httpContent[] = "<html>\
<head>\
<title>Sauna</title>\
<style>\
button {font-size:40pt;width:90pt;height:90pt;margin:10pt;}\
.stisk {background-color: #555555; color:white;}\
</style>\
</head>\
<body style='text-align:center'>\
<p style='font-size:160pt;margin:30pt;color:%s'><b>%u.%u</b> &#176;C</p>\
<form action='.' method='post'>\
<button type='submit' name='btnMinus' id='btnMinus'>-</button>\
<span style='font-size:50pt;display:inline-block;width:200pt;'>%u &#176;C</span>\
<button type='submit' name='btnPlus' id='btnPlus'>+</button>\
<button type='submit' name='btnOnOff' id='btnOnOff' class='%s'>ON</button>\
<p>\
<button style='width:400pt;' onClick='window.location.href=window.location.href; return false;' id='btnAktual'>Aktualizovat</button>\
<button type='submit' name='btnVent' id='btnVent' class='%s'>Vent</button>\
</p>\
</form>\
</body>\
</html>";



void WifiWrap::handleRoot()
{
    Serial.print("HTTP client: ");
    Serial.println(server.client().remoteIP());

    // Handle POST arguments
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i) == "btnMinus") {
            state.setTarget(state.getTarget() - 1);
        }
        if (server.argName(i) == "btnPlus") {
            state.setTarget(state.getTarget() + 1);
        }
        if (server.argName(i) == "btnOnOff") {
            if (state.getState() == OFF) {
                state.setState(ON);
            } else {
                state.setState(OFF);
            }
        }
        if (server.argName(i) == "btnVent") {
            state.setVent(!state.getVent());
        }
        //Serial.println(server.argName(i) + ": " + server.arg(i));
    }

    char buffer[1024] = {0};
    int size = snprintf(buffer, 1024, httpContent,
        (state.getState() == WAITING) ? "limegreen" : (state.getState() == HEATING ? "red" : "black"),
        state.getTempWhole(),
        state.getTempDecimal(),
        state.getTarget(),
        state.getState() == OFF ? "" : "stisk",
        state.getVent() == false ?  "" : "stisk"
    );

    server.send(200, "text/html", buffer);
}


WifiWrap::WifiWrap(State& state)
    : server(80), state(state), mutexConnect(xSemaphoreCreateMutex())
{

}
/*

void WifiWrap::connect(String ssid, String pwd) {
    if(mutexConnect != NULL){ // Sanity check if the mutex exists
        // Try to take the mutex and wait indefintly if needed
        if (xSemaphoreTake(mutexConnect, portMAX_DELAY) == pdTRUE){
            // Mutex successfully taken
            this->ssid = ssid;
            this->pwd = pwd;
            this->reconnect = true;
            xSemaphoreGive(mutexConnect); // After accessing the shared resource give the mutex and allow other processes to access it
        } else {
          // We could not obtain the semaphore and can therefore not access the shared resource safely.
        }
    }
}
*/


void WifiWrap::connect(String ssid, String pwd)
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.disconnect();

    state.setWifiIp("");

    WiFi.setHostname("sauna");
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, pwd);
}

void WifiWrap::onWifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());

    state.setWifiIp(WiFi.localIP().toString());

    if (MDNS.begin("sauna")) {
      Serial.println("MDNS responder started");
    }

    server.on("/", std::bind(&WifiWrap::handleRoot, this));
    server.begin();
    Serial.println("HTTP server started");
}

void WifiWrap::onDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    Serial.println("Trying to Reconnect");
    WiFi.reconnect();
}

void WifiWrap::run()
{
    WiFi.onEvent(std::bind(&WifiWrap::onWifiGotIP, this, std::placeholders::_1, std::placeholders::_2), WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(std::bind(&WifiWrap::onDisconnected, this, std::placeholders::_1, std::placeholders::_2), WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    connect(state.getSsid(), state.getPwd());

    while (1) {
        state.setWifiState(WiFi.status());
        state.setWifiRssi(WiFi.RSSI());

        if (state.isWifiCredentialsChanged()) {
            connect(state.getSsid(), state.getPwd());
        }

        if (WiFi.status() == WL_CONNECTED) {
            server.handleClient();
        }
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}