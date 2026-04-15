const int mq3_analog_pin = A0;
const int relay_pin = 8;

bool relayLocked = false;  // Latch variable

void setup() {
  Serial.begin(9600);
  pinMode(relay_pin, OUTPUT);

  // Start with relay ON (active LOW)
  digitalWrite(relay_pin, LOW);
}

void loop() {
  int sensorValue = analogRead(mq3_analog_pin);

  Serial.print("MQ-3 Value: ");
  Serial.println(sensorValue);

  // If already locked, keep relay OFF forever
  if (relayLocked) {
    digitalWrite(relay_pin, HIGH); // Relay OFF
    Serial.println("Relay LOCKED OFF - Alcohol was detected earlier");
    delay(200);
    return;   // Skip rest of loop
  }

  // If alcohol crosses threshold, lock relay OFF
  if (sensorValue > 440) {
    relayLocked = true;
    digitalWrite(relay_pin, HIGH);  // Relay OFF
    Serial.println("Alcohol Detected! Relay OFF and LOCKED.");
  } 
  else {
    // Relay stays ON (only if not locked)
    digitalWrite(relay_pin, LOW);  // Relay ON
    Serial.println("Safe - Relay ON");
  }

  delay(200);
}
