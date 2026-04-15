#define BLYNK_TEMPLATE_ID           "TMPL3dKEQisZi"
#define BLYNK_TEMPLATE_NAME         "MOTOR CONTROL"
#define BLYNK_AUTH_TOKEN            "XJqzZkfXrNYrqG7j5aecWGJiHZA3fMH5"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "HF-R1104T-4G-W6";
char pass[] = "1234567890";

#define RELAY_PIN 4
bool relayState = false;
unsigned long relayOnTime = 0;
const unsigned long AUTO_OFF_TIME = 5 * 60 * 1000;

BlynkTimer timer;

BLYNK_WRITE(V0)
{
  int value = param.asInt();

  if (value == 1) {
    digitalWrite(RELAY_PIN, LOW);
    relayState = true;
    relayOnTime = millis();
    Serial.println("Relay turned ON");
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    relayState = false;
    Serial.println("Relay turned OFF");
  }
}

void checkAutoOff() {
  if (relayState && (millis() - relayOnTime >= AUTO_OFF_TIME)) {
    digitalWrite(RELAY_PIN, HIGH);
    relayState = false;
    Blynk.virtualWrite(V0, 0);
    Serial.println("Relay auto-turned OFF after 5 minutes");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Connecting to WiFi and Blynk...");

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, checkAutoOff);
}

void loop() {
  Blynk.run();
  timer.run();
}