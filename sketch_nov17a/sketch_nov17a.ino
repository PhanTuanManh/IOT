#define BLYNK_TEMPLATE_ID "TMPL6Z8uY0O5k"
#define BLYNK_TEMPLATE_NAME "Temperature fan control"
#define BLYNK_AUTH_TOKEN "0ui7gmpzpm_MFrOEiqjtQElKmnLpuwai"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

#define DHTPIN D5     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11

#define relayPin D2    // GPIO pin connected to the relay

DHT dht(DHTPIN, DHTTYPE);

// Ngưỡng nhiệt độ để bật quạt
float thresholdTemperature = 25.0;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(relayPin, OUTPUT);
  dht.begin();
}

void loop() {
  Blynk.run();

  // Reading temperature or humidity takes about 250 milliseconds!
  float humDHT = dht.readHumidity();
  float tempDHT = dht.readTemperature();

  if (isnan(humDHT) || isnan(tempDHT)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print(F("Temperature: "));
  Serial.print(tempDHT);
  Serial.print(F("°C "));
  Serial.println();
  Serial.print(F("Humidity: "));
  Serial.print(humDHT);
  Serial.print(F("%"));
  Serial.println();
  Serial.println("***********************");
  Serial.println();

  // Compare Threshold value from Blynk and DHT Temperature value.
  if (tempDHT > thresholdTemperature && Blynk.connected()) {
    Blynk.virtualWrite(V0, HIGH);
  } else {
    Blynk.virtualWrite(V0, LOW);
  }

  Blynk.virtualWrite(V1, tempDHT);
  Blynk.virtualWrite(V2, humDHT);
}

BLYNK_WRITE(V0) {
  int switchState = param.asInt();

  if (switchState == HIGH) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
}
