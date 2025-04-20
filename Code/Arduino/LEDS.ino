#define SOUND_SENSOR_PIN 2  // KY-038 DO pin

int leds[] = {3, 4, 5, 6, 7, 8, 9, 10, 11};
int numLeds = 9;

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_SENSOR_PIN, INPUT);

  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() {
  int soundValue = digitalRead(SOUND_SENSOR_PIN);  // HIGH = sound detected

  if (soundValue == HIGH) {
    Serial.println("Sound detected!");

    // ====== Increasing pattern ======
    for (int speed = 50; speed <= 150; speed += 50) {  // Fast to slow
      for (int i = 0; i < numLeds; i++) {
        digitalWrite(leds[i], HIGH);
        delay(speed);
      }

      for (int i = 0; i < numLeds; i++) {
        digitalWrite(leds[i], LOW);
      }

      // ====== Decreasing pattern ======
      for (int i = numLeds - 1; i >= 0; i--) {
        digitalWrite(leds[i], HIGH);
        delay(speed);
      }

      for (int i = 0; i < numLeds; i++) {
        digitalWrite(leds[i], LOW);
      }
    }

    // ====== Intense and dull blinking (PWM fade) ======
    for (int fadeCycle = 0; fadeCycle < 2; fadeCycle++) {
      for (int brightness = 0; brightness <= 255; brightness += 15) {
        for (int i = 0; i < numLeds; i++) {
          analogWrite(leds[i], brightness);
        }
        delay(30);
      }
      for (int brightness = 255; brightness >= 0; brightness -= 15) {
        for (int i = 0; i < numLeds; i++) {
          analogWrite(leds[i], brightness);
        }
        delay(30);
      }
    }

    // Turn off all LEDs after effect
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(leds[i], LOW);
    }
  }

  delay(100);  // debounce and prevent retriggering too fast
}

