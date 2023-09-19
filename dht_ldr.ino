
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT11 

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows


DHT dht(DHTPIN, DHTTYPE);



int ldr_pin = 3;
int pump =1;
int soil_pin = A1;

const int waterlev = A0;
int waterlevValue = 0;

void setup()
{
  Serial.begin(9600); // Start Serial Communication at 9600 Baud Rate

  lcd.init(); // initialize the lcd
  lcd.backlight();

  pinMode(8,OUTPUT);   //buzzer output
  pinMode(9,OUTPUT);   //relay output
  
  digitalWrite(9, HIGH);     //relay set to OFF

  pinMode(ldr_pin, INPUT);
  // Serial.println(F("DHTxx test!"));
  dht.begin();
}




void loop() {
  delay(2000);

  int sensor_analog = analogRead(soil_pin);
  int sensorStatus = digitalRead(ldr_pin);

  if (sensorStatus == 0)
  {
    Serial.print("bright light.! ");
  }
  else
  {
    Serial.print("low light.! "); 
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  int moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  Serial.print(F("Moisture Percentage = "));
  Serial.print(moisture_percentage);


if(sensorStatus == 1)
{
  pump=0;
} 
else
{
  if(t>30){
    pump=1;
  }
  if(moisture_percentage <33)
    pump=1;
  
  if(moisture_percentage >=33)
    pump=0;
}

waterlevValue = analogRead(waterlev);
  // Serial.print("\n");
  // delay(500);
  if((waterlevValue<=200)){
    digitalWrite(8,HIGH);
    delay(500);
    digitalWrite(8,LOW);
    delay(500);

    }

if(pump==1)
  Serial.print(F("     pump: ON"));
else
  Serial.print(F("     pump: OFF"));
  Serial.print("       WATERlev = ");
  Serial.println(waterlevValue);
 
  lcd.clear();                 // clear display
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("T:");        // print message at (0, 0)
  // if(t<30)
  lcd.print(t);
  // else
  // lcd.print("High");
  lcd.print(" L:");
  if(sensorStatus == 1)
  lcd.print("Night");
  else
  lcd.print("Day");

  lcd.setCursor(0, 1); 
  
  lcd.print("Soil:");
  if(moisture_percentage<30)
    lcd.print("DRY");
  else
    lcd.print("WET");
  
  lcd.print(" pmp:");
  if(pump==1){
    lcd.print("ON");
    digitalWrite(9, LOW);
    delay(2000);
    digitalWrite(9, HIGH);
  }
  else{
    digitalWrite(9,HIGH);
    lcd.print("OFF");
 }                 // display the above for two seconds
}
