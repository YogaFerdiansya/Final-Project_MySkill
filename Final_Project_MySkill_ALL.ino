#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6UcqAqiPi"
#define BLYNK_TEMPLATE_NAME "Ruang 4"
#define BLYNK_AUTH_TOKEN "0WVpVNr49RtzF-z0IFVADySVtcMQasSu"

#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define DHTPIN 4      
#define DHTTYPE DHT22
#define MQ135_PIN 36
#define buzzerPin 15
#define red 25
#define green 33
#define blue 32      

DHT dht(DHTPIN, DHTTYPE);
float temp, humi;
String tempStatus, humiStatus, AQIStatus;

int sensitivity = 500;  // Adjust this value based on your calibration
int MQ135_Value, AQI;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

BlynkTimer timer;

void myTimer(){
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, humi);
  Blynk.virtualWrite(V0, AQI);
  Blynk.virtualWrite(V3, tempStatus);
  Blynk.virtualWrite(V4, humiStatus);
  Blynk.virtualWrite(V5, AQIStatus);
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(MQ135_PIN, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, "POCO M4 Pro", "rrrrrrrr");
  timer.setInterval(2000L, myTimer);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(500);
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("MYSKILL");
  display.setCursor(0, 20);
  display.print("YOGA FERDIANSYA");
  display.display();
  delay(3000);
}

String temperatureStatus(float temperature) {
  if (temperature >= 32 && temperature <= 34){
    return "Good";
  }
  else if ((temperature >= 30 && temperature < 32) || (temperature > 34 && temperature <= 36)){
    return "Moderate";
  }
  else{
    return "Bad";
  }
}

String humidityStatus(float humidity) {
  if (humidity >= 40 && humidity <= 60){
    return "Good";
  } 
  else if ((humidity >= 35 && humidity < 40) || (humidity > 60 && humidity <= 65)){
    return "Moderate";
  }
  else{
    return "Bad";
  }
}

String airQualityStatus(int airQuality) {
  if (airQuality <= 50) {
    return "Good";
  } else if (airQuality <= 100) {
    return "Moderate";
  } else {
    return "Bad";
  }
}

void loop() {
  temp = dht.readTemperature();
  humi = dht.readHumidity();

  if (isnan(temp) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  tempStatus = temperatureStatus(temp);
  humiStatus = humidityStatus(humi);

  MQ135_Value = analogRead(MQ135_PIN);

  if (isnan(MQ135_Value)) {
    Serial.println("Failed to read from MQ135 sensor!");
    return;
  }

  AQI = MQ135_Value * sensitivity / 4095;
  AQIStatus = airQualityStatus(AQI);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: " + String(temp) + "*C, " + tempStatus);
  display.setCursor(0, 20);
  display.print("Humi: " + String(humi) + " %, " + humiStatus);
  display.setCursor(0, 40);
  display.print("AQI: " + String(AQI) + "  , " + AQIStatus);
  display.display();

  if (AQIStatus == "Good"){
    digitalWrite(buzzerPin, LOW);
  }else{
    digitalWrite(buzzerPin, HIGH);
  }

  if (humiStatus == "Bad" && tempStatus == "Bad"){
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    digitalWrite(blue, HIGH);
  }else if (humiStatus == "Moderate" || tempStatus == "Moderate") {
    digitalWrite(blue, LOW);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
  }else if (humiStatus == "Good" && tempStatus == "Good") {
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
    digitalWrite(blue, HIGH);
  }

  Blynk.run();
  timer.run();

  delay(1000);
}
