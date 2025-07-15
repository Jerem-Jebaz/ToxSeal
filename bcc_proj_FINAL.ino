#define SMOKE_SENSOR_PIN 34  // MQ-2/MQ-135 sensor pin
#define BUZZER_PIN 26        // Buzzer pin
#define FAN_RELAY_PIN 27     // Relay channel for the fan
#define THRESHOLD 2000        // Adjust based on testing

bool smokeDetected = false;  // Flag to track smoke detection

void setup() {
    Serial.begin(115200);
    pinMode(SMOKE_SENSOR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(FAN_RELAY_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(FAN_RELAY_PIN, HIGH);  // Ensure fan is OFF at startup (active-low logic)
    Serial.println("Initializing system...");
    Serial.println("Fan relay state: OFF");
}

void loop() {
    int smokeLevel = analogRead(SMOKE_SENSOR_PIN);
    Serial.print("Smoke Level: ");
    Serial.println(smokeLevel);

    // If smoke is detected and it was not previously detected
    if (smokeLevel > THRESHOLD && !smokeDetected) {
        smokeDetected = true;  // Set flag to prevent multiple triggers
        Serial.println("🚨 SMOKE DETECTED! Beeping & Running Fan...");

        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);

        digitalWrite(FAN_RELAY_PIN, LOW);  // Turn fan ON (active-low logic)
        Serial.println("Fan relay state: ON");
        delay(5000);  // Run fan for 5 seconds
        digitalWrite(FAN_RELAY_PIN, HIGH);  // Turn fan OFF (active-low logic)
        Serial.println("Fan relay state: OFF");

        Serial.println("✅ System waiting for smoke to clear...");
    }

    // Reset flag and turn off fan if smoke level drops significantly below threshold
    if (smokeLevel < (THRESHOLD - 50)) {
        smokeDetected = false;
        digitalWrite(FAN_RELAY_PIN, HIGH);  // Ensure fan is turned OFF (active-low logic)
        Serial.println("✅ Smoke cleared. System reset.");
        Serial.println("Fan relay state: OFF");
    }

    delay(1000);  // Regular monitoring interval
}