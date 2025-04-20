#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "DHT.h"

// DFPlayer setup
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
char command;
int pausa = 0;

// KY-038 Sound Sensor setup
const int soundSensorPin = 2;
unsigned long soundHighStartTime = 0;
bool isSoundDetected = false;
bool musicPlayed = false;

// DHT22 setup
#define DHTPIN 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float temperature = 0.0;

// Fan setup (controlled via NPN transistor)
const int fanPin = 4;

void setup() {
  // Initialize Serial communication
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  pinMode(soundSensorPin, INPUT);
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW); // Fan OFF initially

  dht.begin(); // Start DHT sensor

  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Initializing DFPlayer module ... Wait!"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true);
  }

  Serial.println(F("DFPlayer Mini module initialized!"));
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(5);
  myDFPlayer.EQ(0);
  menu_opcoes();
}

void loop() {
  // 🌡️ DHT22 temperature reading and fan control
  temperature = dht.readTemperature();
  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");

    if (temperature > 27.0) {
      digitalWrite(fanPin, HIGH); // Fan ON
      Serial.println("Fan ON");
    } else {
      digitalWrite(fanPin, LOW); // Fan OFF
      Serial.println("Fan OFF");
    }
  } else {
    Serial.println("Temperature read failed!");
  }

  // 🎤 KY-038 sound detection logic
  if (digitalRead(soundSensorPin) == HIGH) {
    if (!isSoundDetected) {
      soundHighStartTime = millis();
      isSoundDetected = true;
    } else if ((millis() - soundHighStartTime >= 5000) && !musicPlayed) {
      Serial.println("Sound HIGH for 5 seconds – Playing song...");
      myDFPlayer.play(1); // Play song 1
      musicPlayed = true;
      delay(1000); // Delay to prevent immediate retrigger
    }
  } else {
    isSoundDetected = false;
    musicPlayed = false;
  }

  // 🎵 DFPlayer manual control via Serial input
  while (Serial.available() > 0) {
    command = Serial.read();

    if ((command >= '1') && (command <= '9')) {
      Serial.print("Playing track ");
      Serial.println(command);
      command = command - '0';
      myDFPlayer.play(command);
      menu_opcoes();
    }

    if (command == 's') {
      myDFPlayer.stop();
      Serial.println("Music stopped!");
      menu_opcoes();
    }

    if (command == 'p') {
      pausa = !pausa;
      if (pausa == 0) {
        Serial.println("Resumed music");
        myDFPlayer.start();
      } else {
        Serial.println("Music paused");
        myDFPlayer.pause();
      }
      menu_opcoes();
    }

    if (command == '+') {
      myDFPlayer.volumeUp();
      Serial.print("Current volume: ");
      Serial.println(myDFPlayer.readVolume());
      menu_opcoes();
    }

    if (command == '-') {
      myDFPlayer.volumeDown();
      Serial.print("Current volume: ");
      Serial.println(myDFPlayer.readVolume());
      menu_opcoes();
    }

    if (command == '<') {
      myDFPlayer.previous();
      Serial.println("Previous track");
      Serial.print("Current track: ");
      Serial.println(myDFPlayer.readCurrentFileNumber());
      menu_opcoes();
    }

    if (command == '>') {
      myDFPlayer.next();
      Serial.println("Next track");
      Serial.print("Current track: ");
      Serial.println(myDFPlayer.readCurrentFileNumber());
      menu_opcoes();
    }
  }

  delay(1000); // Sensor stability
}

void menu_opcoes() {
  Serial.println();
  Serial.println(F("==============================================================================="));
  Serial.println(F("Commands:"));
  Serial.println(F(" [1-9] Select track number"));
  Serial.println(F(" [s]   Stop playback"));
  Serial.println(F(" [p]   Pause/Continue music"));
  Serial.println(F(" [+/-] Volume Up/Down"));
  Serial.println(F(" [</>] Previous/Next track"));
  Serial.println(F(" [AUTO] Plays song if sound HIGH for 5 sec"));
  Serial.println(F(" [AUTO] Fan ON if temperature > 30°C"));
  Serial.println(F("==============================================================================="));
}


