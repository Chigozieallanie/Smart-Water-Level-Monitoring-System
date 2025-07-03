#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define TRIG 9
#define ECHO 10

void setup() {
  lcd.begin();
  lcd.backlight();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  lcd.setCursor(0, 0);
  lcd.print("SMART H2O LEVEL");
  lcd.setCursor(0, 1);
  lcd.print("    MONITOR");
  delay(2000);
}

void loop() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2; // Speed of sound = 343 m/s or 0.034 cm/µs
  lcd.clear();
  lcd.print("Depth: ");
  lcd.print(distance);
  lcd.println(" cm");

  delay(500);

 
}
