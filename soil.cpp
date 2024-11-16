// version 1.3
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
    String html = R"(
    <html>
    <head>
      <meta name='viewport' content='width=device-width, initial-scale=1'>
      <style>
        body {
          font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
          margin: 0;
          padding: 20px;
          background-color: #f0f8ff;
          color: #333;
        }
        .container {
          max-width: 800px;
          margin: 0 auto;
          background-color: #ffffff;
          border-radius: 10px;
          box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);
          padding: 20px;
        }
        h1 {
          color: #2c3e50;
          text-align: center;
          margin-bottom: 30px;
          font-size: 2.5em;
          text-shadow: 2px 2px 4px rgba(0,0,0,0.1);
        }
        table {
          width: 100%;
          border-collapse: separate;
          border-spacing: 0 10px;
        }
        th, td {
          padding: 15px;
          text-align: left;
          border-radius: 5px;
        }
        th {
          background-color: #3498db;
          color: white;
          font-weight: bold;
        }
        tr:nth-child(even) {
          background-color: #f2f2f2;
        }
        tr:hover {
          background-color: #e0f7fa;
          transition: background-color 0.3s ease;
        }
        .sensor-icon {
          margin-right: 10px;
          font-size: 1.2em;
        }
        @keyframes fadeIn {
          from { opacity: 0; }
          to { opacity: 1; }
        }
        .fade-in {
          animation: fadeIn 0.5s ease-in;
        }
      </style>
    </head>
    <body>
      <div class="container">
        <h1>🌱 Plant Monitoring System 🌱</h1>
        <table>
          <tr>
            <th>Sensor</th>
            <th>Value</th>
          </tr>
          <tr>
            <td><span class="sensor-icon">🌡️</span>Temperature (°C)</td>
            <td id='tempValue' class="fade-in">Loading...</td>
          </tr>
          <tr>
            <td><span class="sensor-icon">💧</span>Humidity (%)</td>
            <td id='humValue' class="fade-in">Loading...</td>
          </tr>
          <tr>
            <td><span class="sensor-icon">🌿</span>Soil Moisture (%)</td>
            <td id='soilValue' class="fade-in">Loading...</td>
          </tr>
          <tr>
            <td><span class="sensor-icon">☔</span>Rain Detected</td>
            <td id='rainValue' class="fade-in">Loading...</td>
          </tr>
          <tr>
            <td><span class="sensor-icon">🚶</span>Motion Detected</td>
            <td id='motionValue' class="fade-in">Loading...</td>
          </tr>
          <tr>
            <td><span class="sensor-icon">💡</span>Light Level</td>
            <td id='ldrValue' class="fade-in">Loading...</td>
          </tr>
          <tr>
            <td><span class="sensor-icon">🌫️</span>CO2 Level (ppm)</td>
            <td id='co2Value' class="fade-in">Loading...</td>
          </tr>
          <tr>
            <td><span class="sensor-icon">🚬</span>Smoke Detected</td>
            <td id='smokeValue' class="fade-in">Loading...</td>
          </tr>
        </table>
      </div>
      <script>
        async function fetchData() {
          try {
            const response = await fetch('/data');
            const data = await response.json();
            updateValue('tempValue', data.temperature);
            updateValue('humValue', data.humidity);
            updateValue('soilValue', data.soilMoisture);
            updateValue('rainValue', data.rainDetected ? 'Yes' : 'No');
            updateValue('motionValue', data.motionDetected ? 'Yes' : 'No');
            updateValue('ldrValue', data.ldrValue);
            updateValue('co2Value', data.co2Level);
            updateValue('smokeValue', data.smokeValue ? 'Yes' : 'No');
          } catch (error) {
            console.error('Error fetching data:', error);
          }
        }

        function updateValue(id, value) {
          const element = document.getElementById(id);
          element.textContent = value;
          element.classList.remove('fade-in');
          void element.offsetWidth; // Trigger reflow
          element.classList.add('fade-in');
        }

        setInterval(fetchData, 2000);
      </script>
    </body>
    </html>
    )";
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
    float percentage = map(soilMoisture, 0, 4096, 0, 100);

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
    lcd.print(digitalRead(RAINPIN) == HIGH ? "Yes" : "No");
    delay(1500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Light: ");
    lcd.print(map(analogRead(LDRPIN), 4096, 0, 0, 100));
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Moisture: ");
    lcd.print(map(analogRead(SOILPIN), 0, 4096, 0, 100));
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
