#include <DHT.h>

#define DHTPIN 4       // DHT22 sensor pin
#define DHTTYPE DHT22  // DHT Type
#define FAN_PIN 5      // NPN transistor control pin

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);
    dht.begin();
    Serial.begin(115200);
}

void loop() {
    float temperature = dht.readTemperature();  // Read temperature in Celsius
    
    if (isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Temperature: ");
    Serial.println(temperature);

    if (temperature > 31.0) {  // Change 25.0 to your desired threshold
        digitalWrite(FAN_PIN, HIGH);  // Turn on fan
        Serial.println("Fan ON");
    } else {
        digitalWrite(FAN_PIN, LOW);   // Turn off fan
        Serial.println("Fan OFF");
    }

    delay(2000);  // Delay 2 seconds
}
