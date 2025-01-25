#include "sauna.h"
#include "WifiWrap.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Update.h>


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
<button style='width:400pt;' type='submit' name='btnTimer' id='btnTimer' class='%s'>%02u:%02u</button>\
<button type='submit' name='btnVent' id='btnVent' class='%s'>Vent</button>\
</p>\
<p>\
<button style='width:510pt;' onClick='window.location.href=window.location.href; return false;' id='btnAktual'>Aktualizovat</button>\
</p>\
</form>\
</body>\
</html>";


// ***************************************************************************
// OTA HTTP interface
// ***************************************************************************

/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

 
/* Server Index Page */
String httpUpdate = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/upload',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;




void WifiWrap::handleRoot()
{
    Serial.print("HTTP client: ");
    Serial.println(server.client().remoteIP());
    Serial.println("URL: /");

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
        if (server.argName(i) == "btnTimer") {
            state.toggleTimer();
        }
        //Serial.println(server.argName(i) + ": " + server.arg(i));
    }

    int32_t sec = state.seconds;
    if (sec < 0) {
        sec = 0;
    }

    char buffer[1024] = {0};
    snprintf(buffer, 1024, httpContent,
        (state.getState() == WAITING) ? "limegreen" : (state.getState() == HEATING ? "red" : "black"),
        state.getTempWhole(),
        state.getTempDecimal(),
        state.getTarget(),
        // button ON
        state.getState() == OFF ? "" : "stisk",
        // button timer
        state.seconds < 0 ? "" : "stisk",
        sec / 60,
        sec % 60,
        // button ventilator
        state.getVent() == false ?  "" : "stisk"
    );

    server.send(200, "text/html", buffer);
}


void WifiWrap::handleUpdate()
{
    Serial.print("HTTP client: ");
    Serial.println(server.client().remoteIP());
    Serial.println("URL: /update");

    server.sendHeader("Connection", "close");
    server.send(200, "text/html", httpUpdate);
}


void WifiWrap::handleUploadDone()
{
    Serial.print("HTTP client: ");
    Serial.println(server.client().remoteIP());
    Serial.println("URL: /upload");

    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
}


void WifiWrap::handleUpload()
{
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        // flashing firmware to ESP
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
            Update.printError(Serial);
        }
    }
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
    server.on("/update", std::bind(&WifiWrap::handleUpdate, this));
    server.on("/upload", HTTP_POST, std::bind(&WifiWrap::handleUploadDone, this), 
                                    std::bind(&WifiWrap::handleUpload, this)
    );
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