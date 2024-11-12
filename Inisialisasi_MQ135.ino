const int MQ135_PIN = 25;
int sensitivity = 500;  // Adjust this value based on your calibration
int MQ135_Value, airQuality;

void setup() {
  Serial.begin(115200);
  pinMode(MQ135_PIN, INPUT);
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
  MQ135_Value = analogRead(MQ135_PIN);
  airQuality = MQ135_Value * sensitivity / 4095;
  Serial.println("Air Quality Index: " + String(airQuality) + ", Status: " + airQualityStatus(airQuality));
  delay(1000);
}
