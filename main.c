#include <LiquidCrystal.h>
#include "dht.h"

#define DHT11_PIN 8
dht DHT;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // (RS, E, DB4, DB5, DB6, DB7)

// Символ градуса
byte circle[8] = {
  B01110,
  B10001,
  B10001,
  B01110,
  B00000,
  B00000,
  B00000,
};

void setup() {
  Serial.begin(115200);
  // Размерность экрана
  lcd.begin(16, 2);
  // Создаём символ градуса
  lcd.createChar(1, circle);
}

void loop() {
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk) {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }

  // Выводим в СОМ-порт данные о влажности и температуре
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t\t");
  Serial.println(DHT.temperature, 1);
  
  // Выводим данные о влажности и температуре на экран
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(int(DHT.temperature));
  lcd.print(" \1C, ");
  lcd.print(int(DHT.humidity));
  lcd.print("%");
  
  delay(2000);
}