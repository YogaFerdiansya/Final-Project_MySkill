#include <DHT.h>

#define DHTPIN 4      
#define DHTTYPE DHT22       

DHT dht(DHTPIN, DHTTYPE);
float temperature, humidity;
String tempStatus, humiStatus;

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Suhu " + String(temperature) + " *C");
  Serial.println("Kelembapan " + String(humidity) + " %");

  if (temperature >= 22 && temperature <= 24){
    tempStatus = "Good";
  }else{
    tempStatus = "Bad";
  }

  if (humidity >= 30 && humidity <= 50){
    humiStatus = "Good";
  } else{
    humiStatus = "Bad";
  }

  Serial.println("Suhu: " + tempStatus);
  Serial.println("Kelembapan: " + humiStatus);
  Serial.println(" ");
  delay(1000);
}
