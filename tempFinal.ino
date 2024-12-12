#include <dht.h>
#include <LiquidCrystal.h>
#define dht_apin A1
dht DHT;
int ThermistorPin = 0;
int Vo;
float R1 = 1000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int waterSensorPin = A2;
float SensePerMilimeter = 4.33;
const int buttonPin = 36;
int buttonState = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  delay(500);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void tempGuy() {
    Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 
  lcd.clear();
  delay(100);
  lcd.print("Temp = ");
  lcd.print(T);   
  lcd.print(" F");
  delay(3000);            
  lcd.clear();
}

void DhtGuy() {
  DHT.read11(dht_apin);
    float F = DHT.temperature;
    float CF = 9/5;
    F = (F * CF) + 32;

    lcd.clear();
    delay(100);
    lcd.print("Humidity:" );
    lcd.print(DHT.humidity);
    delay(3000);//Wait 5 seconds before accessing sensor again.
    lcd.clear();
}

void WaterLevelSensorGuy() {
  delay(1000);
  float rainFallinMillimeters;
  float rainFallPerMinuteInMillimeters;
  float rainFallPerMinuteInInches;
  int sensorValue = analogRead(waterSensorPin);
  if (sensorValue < 250) {
    Serial.println("Bad Sensor Value");
    return;
  } else {
    Serial.println("We got a valid read");
    int newSensorValue1 = (sensorValue - 250);
    delay(10000);
    int sensorValue2 = analogRead(waterSensorPin);
    int count = 0;
      while (sensorValue2 < sensorValue) {
        if (count >= 3) {
          Serial.println("Bad reads resetting");
          return;
        }
        Serial.println("BadRead");
        delay(100);
        sensorValue2 = analogRead(waterSensorPin);
        count++;
    }
    int newSensorValue2 = (sensorValue2 - 250);
    rainFallinMillimeters = ((newSensorValue2 - newSensorValue1) / SensePerMilimeter);
    Serial.print("RainFallinMillimeters every 10 seconds: ");
    Serial.println(rainFallinMillimeters);

    rainFallPerMinuteInMillimeters = (rainFallinMillimeters * 6);
    Serial.print("RainFall in millimeters per minute: ");
    Serial.println(rainFallPerMinuteInMillimeters);

    rainFallPerMinuteInInches = (rainFallPerMinuteInMillimeters / 25.4);
    Serial.print("RainFall in inches per minute: ");
    Serial.println(rainFallPerMinuteInInches);

     Serial.print("newSensorValue1: ");
    Serial.println(newSensorValue1);

     Serial.print("newSensorValue2: ");
    Serial.println(newSensorValue2);
  }
    lcd.clear();
    delay(100);
    lcd.print("RainFall MM:" );
    lcd.print(rainFallPerMinuteInMillimeters);
    delay(2000);
    lcd.clear();
    delay(100);
    lcd.print("RainFall In:" );
    lcd.print(rainFallPerMinuteInInches);
    delay(3000);//Wait 5 seconds before accessing sensor again.
    lcd.clear();
}

void loop() { 
// buttonState = digitalRead(buttonPin);
// Serial.println(buttonState);
  while (true) {
    buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
        delay(1000);
        break;
      }
      Serial.println("1");
    tempGuy();
  }
  while (true) {
    buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
        delay(1000);
        break;
      }
      Serial.println(buttonState);
      Serial.println("2");
    DhtGuy();
  }
  while (true) {
    buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
        delay(1000);
        break;
      }
      Serial.println(buttonState);
      Serial.println("3");
    WaterLevelSensorGuy();
  }
  

}
