#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
const char* ssid= "";
const char* password="";
DHT dht(4,DHT22);
AsyncWebServer server(80);
String getTemp(){-
float t=dht.readTemperature();
return isnan(t) ? "--" : String(t,1);
}
String getHumidity(){
float h=dht.readHumidity();
return isnan(h) ? "--" : String(h,1);
}

const char index_html[] PROGMEM=R"skibidi(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css">
<style>
body {font-family:Arial,text-align:center;background-color:#121212;color:#ffffff;margin:0;}
h2 {font-size:2.5rem; margin-top:50px;}
p {font-size:2rem;margin:20px}
.units {font-size:1.2rem;}
.labels {font-size:1.5rem;vertical-align:middle;}
</style>
</head><body>
<h2>ESP32 DHT22 Sensor</h2>
<p><i class="fas fa-thermometer-half" style="color:#059e8a;"></i> <span class="labels">Temperature</span> <span id="t">%TEMP%</span><sup class="units">&deg;C</sup></p>
<p><i class="fas fa-tint" style="color:#00add6;"></i> <span class="labels">Humidity</span> <span id="h">%HUM%</span><sup class="units">&percnt;</sup></p>
<script>
setInterval(()=>{
fetch('/t').then(r=>r.text()).then(v=>document.getElementById("t").innerHTML=v);
fetch('/h').then(r=>r.text()).then(v=>document.getElementById("h").innerHTML=v);
},10000);
</script></body></html>)skibidi";
String processor(const String& var){
    if(var=="TEMP"){
        return getTemp();
    }
    else if(var=="HUM"){
        return getHumidity();
    }
    return String();
}
void setup();
Serial.begin(115200);
dht.begin();
WiFi.begin(ssid,password);
while (WiFi.status!=WL_CONNECTED){
    delay(1000);
    Serial.print(".");
    Serial.println("IP address is"+WiFi.localIP().toString());
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);});
    server.on("/t", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(200, "text/plain", getTemp()); });
    server.on("/h", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(200, "text/plain", getHum()); });
    server.begin();
}

void loop() {}
