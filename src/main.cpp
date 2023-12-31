#include <SPI.h>
#include <MFRC522.h>
#include <Ultrasonic.h>

// PIR Sensor
constexpr uint8_t PIRPin = 5;
// RFID Sensor
/* Other Pins:
MOSI 11,
MISO 12, 
SCK 13
*/
constexpr uint8_t SDAPin = 10;
constexpr uint8_t RSTPin = 9;
// Ultrasonic Sensor
constexpr uint8_t trigPin = 3;
constexpr uint8_t echoPin = 4;
constexpr uint8_t ledPin = 6;

MFRC522 mfrc522(SDAPin, RSTPin);
Ultrasonic ultrasonic(trigPin, echoPin);

void ultrasonicRead();
void rfidRead();
void pirRead();

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(PIRPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  delay(2000);
  digitalWrite(ledPin, LOW);
  rfidRead();
  ultrasonicRead();
  pirRead();
}

void ultrasonicRead() {
  int distance = ultrasonic.read(CM);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance < 20) {
    digitalWrite(ledPin, HIGH);
  }
}

void rfidRead() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("Nenhuma Etiqueta Detectada");
    return;
  }
  
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Falha na leitura");
    return;
  }

  Serial.print("UID tag :");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  } 
  Serial.println();
  Serial.print("Acesso: ");
  content.toUpperCase();
  if (content.substring(1) == "E5 81 92 AC") {
    Serial.println("Autorizado!");
    digitalWrite(ledPin, HIGH);
    Serial.println();
  } else {
    Serial.println("Negado!");
  }
}

void pirRead() {
  if (digitalRead(PIRPin) == HIGH) {
    Serial.println("Movimento detectado!");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("Nenhum movimento detectado!");
  }
}