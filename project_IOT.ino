#define BLYNK_TEMPLATE_ID "TMPL6hN0ytqUd"
#define BLYNK_TEMPLATE_NAME "Plant Watering"
#define BLYNK_AUTH_TOKEN "2_vUm_CgJaHvOh3SvvGhpDWYPYqvs9z5"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "2_vUm_CgJaHvOh3SvvGhpDWYPYqvs9z5";
char ssid[] = "ThuThu";
char pass[] = "23062003";

BlynkTimer timer;
bool Relay = false;
int moistureThreshold = 50;  // Đặt ngưỡng độ ẩm cần thiết để kích hoạt van tưới (đơn vị %)
int wateringDuration = 30;   // Đặt thời gian mặc định tưới (đơn vị giây)

#define sensor A0
#define waterPump D5

bool isWatering = false;
unsigned long wateringStartTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  timer.setInterval(100L, soilMoistureSensor);
}

BLYNK_WRITE(V1) {
  Relay = param.asInt();

  if (Relay == 1) {
    startWateringIfRequired();
  } else {
    stopWatering();
  }
}

void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  Serial.print("Moisture: ");
  Serial.println(value);

  if (value < moistureThreshold && !isWatering) {
    startWatering();
  } else if (value >= moistureThreshold && isWatering) {
    stopWatering();
  }
}

void startWateringIfRequired() {
  int currentMoisture = analogRead(sensor);
  currentMoisture = map(currentMoisture, 0, 1024, 0, 100);
  currentMoisture = (currentMoisture - 100) * -1;

  if (currentMoisture < moistureThreshold && !isWatering) {
    startWatering();
  }
}

void startWatering() {
  digitalWrite(waterPump, LOW);
  Serial.println("Motor is ON");

  isWatering = true;
  wateringStartTime = millis();
}

void stopWatering() {
  digitalWrite(waterPump, HIGH);
  Serial.println("Motor is OFF");

  isWatering = false;
  wateringStartTime = 0;
}

void loop() {
  Blynk.run();
  timer.run();

  // Kiểm tra xem thời gian tưới đã đủ hay chưa
  if (isWatering && millis() - wateringStartTime >= wateringDuration * 1000) {
    stopWatering();
  }
}
