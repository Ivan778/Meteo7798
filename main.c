#include "dht.h"
#include "SFE_BMP180.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

dht DHT;

SFE_BMP180 pressure;

#define DHT11_PIN 8
#define pwmPin 5

int prevVal = LOW;
long th, tl, h, l, ppm;

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

double getPressure(){
    char status;
    double T,P,p0,a;

    status = pressure.startTemperature();
    if (status != 0){
        // ожидание замера температуры
        delay(status);
        status = pressure.getTemperature(T);
        if (status != 0){
            status = pressure.startPressure(3);
            if (status != 0){
                // ожидание замера давления
                delay(status);
                status = pressure.getPressure(P,T);
                if (status != 0){
                    return(P);
                }
            }
        }
    }
}

double getTemperature(){
    char status;
    double T,P,p0,a;

    status = pressure.startTemperature();
    if (status != 0){
        // ожидание замера температуры
        delay(status);
        status = pressure.getTemperature(T);
        if (status != 0){
            return T;
        }
    }
}

void setup()
{
  Serial.begin(9600);
  pressure.begin();
  pinMode(13, OUTPUT);
  pinMode(pwmPin, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(2000);
  display.clearDisplay();

  ppm = 0;

//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(0,0);
//  display.println("Hello, world!");
//  display.display();
}

void loop()
{
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t\n"); 
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

  display.clearDisplay();
  
  // DISPLAY DATA
  double P = getPressure();
  double T = getTemperature();
  
  Serial.print("Humidity - ");
  Serial.println(DHT.humidity);
  displayData(0, 0, "Humidity: ", DHT.humidity, " %");
  
  Serial.print("Pressure - ");
  Serial.println(P);
  displayData(0, 8, "Pressure: ", P, " Pa");
  
  Serial.print("Temperature - ");
  Serial.println(T);
  displayData(0, 16, "Temperature: ", T, " C");

  displayData(0, 24, "PPM: ", double(ppm), "");

  long tt = millis();
  int myVal = digitalRead(pwmPin);

  //Если обнаружили изменение
  if (myVal == HIGH) {
    if (myVal != prevVal) {
      h = tt;
      tl = h - l;
      prevVal = myVal;
    }
  }  else {
    if (myVal != prevVal) {
      l = tt;
      th = l - h;
      prevVal = myVal;
      ppm = 5000 * (th - 2) / (th + tl - 4);
      Serial.println("PPM = " + String(ppm));
    }
  }

  Serial.println();
  
  delay(2000);
}

void displayData(int x, int y, char *text, double value, char *text1) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.print(text);
  display.print(value);
  display.print(text1);
  display.display();
}
