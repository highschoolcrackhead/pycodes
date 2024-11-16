// version 1.2.1
// ldr map reversed for light%
// soil moisture expressed in % acc to min and max value
// added servo functionality and improved lcd

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <DHT.h>

#define DHTPIN 4
#define PIRPIN 5
#define RAINPIN 18
#define SOILPIN 35
#define MQ135PIN 32
#define SMOKEPIN 23
#define LDRPIN 34

DHT dht(DHTPIN, DHT11);
AsyncWebServer server(80);


const char* ssid = "ESP32_WebServer";
const char* password = "123456789";

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
	
    Serial.begin(115200);
    dht.begin();
    pinMode(PIRPIN, INPUT);
    pinMode(RAINPIN, INPUT);
    pinMode(SMOKEPIN, INPUT);
    
    WiFi.softAP(ssid, password);
    
    Serial.println("Access Point started");
    
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    Wire.begin();
    lcd.init();
    lcd.backlight();
	
    lcd.setCursor(0, 0);
    lcd.print("Plant Monitoring");
    lcd.setCursor(0, 1);
    lcd.print("System Init...");
    delay(1000);
 
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", generateHTML());
    });

    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "application/json", generateData());
    });

    server.begin();
}

String generateHTML() {
    String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'><style>"
                  "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; }"
                  "h1 { color: #333; }"
                  "table { width: 100%; border-collapse: collapse; }"
                  "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }"
                  "th { background-color: #f2f2f2; }"
                  "</style></head><body>";
    html += "<h1>Sensor Readings</h1>";
    html += "<table>";
    html += "<tr><th>Sensor</th><th>Value</th></tr>";
    html += "<tr><td>Temperature (°C)</td><td id='tempValue'>Loading...</td></tr>";
    html += "<tr><td>Humidity (%)</td><td id='humValue'>Loading...</td></tr>";
    html += "<tr><td>Soil Moisture (%)</td><td id='soilValue'>Loading...</td></tr>";
    html += "<tr><td>Rain Detected</td><td id='rainValue'>Loading...</td></tr>";
    html += "<tr><td>Motion Detected</td><td id='motionValue'>Loading...</td></tr>";
    html += "<tr><td>LDR Value</td><td id='ldrValue'>Loading...</td></tr>";
    html += "<tr><td>CO2 Level (ppm)</td><td id='co2Value'>Loading...</td></tr>";
	html += "<tr><td>Smoke Detected</td><td id='smokeValue'>Loading...</td></tr>";
    html += "</table>";
    html += "<script>"
            "async function fetchData() {"
            "  try {"
            "    const response = await fetch('/data');"
            "    const data = await response.json();"
            "    document.getElementById('tempValue').innerText = data.temperature;"
            "    document.getElementById('humValue').innerText = data.humidity;"
            "    document.getElementById('soilValue').innerText = data.soilMoisture;"
            "    document.getElementById('rainValue').innerText = data.rainDetected ? 'Yes' : 'No';"
            "    document.getElementById('motionValue').innerText = data.motionDetected ? 'Yes' : 'No';"
            "    document.getElementById('ldrValue').innerText = data.ldrValue;"
            "    document.getElementById('co2Value').innerText = data.co2Level;"
            "    document.getElementById('smokeValue').innerText = data.smokeValue ? 'Yes' : 'No';"
            "  } catch (error) {"
            "    console.error('Error fetching data:', error);"
            "  }"
            "}"
            "setInterval(fetchData, 2000);"
            "</script></body></html>";
    return html;
}

String generateData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (isnan(temperature) || isnan(humidity)) {
        temperature = 0;
        humidity = 0;
    }
    float soilMoisture = map(analogRead(SOILPIN), 4096, 800, 0, 100);
    bool rainDetected = digitalRead(RAINPIN) == LOW;
    bool motionDetected = digitalRead(PIRPIN) == HIGH;
    int ldrValue = analogRead(LDRPIN);
    int co2Level = analogRead(MQ135PIN);
    bool smokeValue = digitalRead(SMOKEPIN) == HIGH;
    float percentage = map(soilMoisture, 4095, 800, 0, 100);

    String json = "{";
    json += "\"temperature\":" + String(temperature, 1) + ",";
    json += "\"humidity\":" + String(humidity, 1) + ",";
    json += "\"soilMoisture\":" + String(soilMoisture) + ",";
    json += "\"rainDetected\":" + String(rainDetected ? "true" : "false") + ",";
    json += "\"motionDetected\":" + String(motionDetected ? "true" : "false") + ",";
    json += "\"ldrValue\":" + String(ldrValue) + ",";
    json += "\"co2Level\":" + String(co2Level) + ","; 
    json += "\"smokeValue\":" + String(smokeValue ? "true" : "false");
    json += "}";
    return json;
}

void loop() {
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IP: "); 
    lcd.print(WiFi.softAPIP());
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(dht.readTemperature(), 1);
    lcd.print("C H:");
    lcd.print(dht.readHumidity(), 1);
    lcd.print("%");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Motion: ");
    lcd.print(digitalRead(PIRPIN) == HIGH ? "Yes" : "No");
    lcd.setCursor(0, 1);
    lcd.print("Rain: ");
    lcd.print( digitalRead(RAINPIN) == LOW ? "Yes" : "No");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Light: ");
    lcd.print(map(analogRead(LDRPIN), 20, 4096, 0, 100));
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print(" Moisture: ");
    lcd.print(map(analogRead(SOILPIN), 4096, 800, 0, 100));
    lcd.print("%");
    delay(1500);
	
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CO2: ");
    lcd.print(analogRead(MQ135PIN));
    lcd.print(" ppm");
    lcd.setCursor(0, 1);
    lcd.print("Smoke: ");
    lcd.print(digitalRead(SMOKEPIN) == HIGH ? "Yes" : "No");
    delay(1500);
  
}
