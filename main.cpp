/*
#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include <Arduino.h>
void ShowSerialData();
// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define TRIG 9
#define ECHO 10

void init_sms(String msg)
{
      Serial.println("AT+CMGF=1"); // set the SMS mode to text 
      delay(2500); 
      Serial.write("AT+CMGS="); 
      Serial.write(34); //ASCII of “ 
      Serial.write("+256779319949"); 
      Serial.write(34); 
      Serial.write(13); 
      Serial.write(10); 
      delay(2500);
      Serial.println(msg);
      
       delay(2500); 
       Serial.write(26); 
       Serial.write(13); 
       Serial.write(10);//Ascii code of ctrl+z to send the message   
  
       delay(3000);
}
void setup() {
  lcd.begin(16,2);
  lcd.backlight();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  lcd.setCursor(0, 0);
  lcd.print("SMART H2O LEVEL");
  lcd.setCursor(0, 1);
  lcd.print("    MONITOR");
  delay(2000);
  Serial.begin(9600);               // the GPRS baud rate   
                  
    lcd.clear();
    lcd.print("Checking network");
    lcd.setCursor(0,1);
    lcd.print("Please wait....");
    delay(15000);
    
    Serial.begin(4800);
    delay(1000);
    Serial.println("AT");
    delay(1000);
    Serial.println("AT+CMGF=1");  // set SMS mode to text
    delay(100);
    Serial.println("AT+CNMI=2,2,0,0,0");     
    delay(500);
    Serial.println("AT+CREG?");
    delay(500);

     while (!(Serial.available())) 
  {
     lcd.clear();
      lcd.print("Check GSM Modem");
      lcd.setCursor(0,1);
      lcd.print("and retry....");
      delay(200);  
    
  }
    String response=Serial.readString();
    while(!(response.indexOf("+CREG: 0,1")>-1)) 
    {
     Serial.println("AT+CREG?");
      response=Serial.readString();
       lcd.clear();
      lcd.print("Network ");
      lcd.setCursor(0,1);
      lcd.print("Connecting.....");
      delay(3000);    
       
      
    }
    
    if(response.indexOf("+CREG: 0,1")>-1) 
    {
     Serial.println("AT+CREG?");
      response=Serial.readString();
        lcd.clear();
      lcd.print("  NETWORK");
      lcd.setCursor(0,1);
      lcd.print("  CONNECTED");
    }
    
   Serial.println("AT");
    delay(1000);
   Serial.println("AT+CMGF=1");  // set SMS mode to text
    delay(100);
   Serial.println("AT+CNMI=2,2,0,0,0"); 
    delay(2000);
    init_sms("Connection OK");
  
     
}
void ShowSerialData() {
    // Your code to display data on the serial monitor goes here
    // Example:
    // Serial.print("Some value: ");
    // Serial.println(myVariable);
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


   if (Serial.available())
   Serial.write(Serial.read());
 
  Serial.println("AT");
  delay(1000);
  Serial.println("AT+CPIN?");
  delay(1000);
 
  Serial.println("AT+CREG?");
  delay(1000);
 
  Serial.println("AT+CGATT?");
  delay(1000);
 
  Serial.println("AT+CIPSHUT");
  delay(1000);
 
  Serial.println("AT+CIPSTATUS");
  delay(2000);
 
  Serial.println("AT+CIPMUX=0");
  delay(2000);
  ShowSerialData();
 
  Serial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  Serial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  Serial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  Serial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
  
  ShowSerialData();
 
  Serial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=L6EE5X3VM6Z3N5ZV&field1=" + String(distance) +"&field2="+String(88);
  Serial.println(str);
  Serial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  Serial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  Serial.println();
  
  ShowSerialData();
 
  Serial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 


  //while(Serial.available()!=0)
  
  
  //Serial.write(Serial.read());
 // delay(5000); 
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define TRIG 9
#define ECHO 10
float distance;

void init_sms(String msg)
{
      Serial.println("AT+CMGF=1"); // set the SMS mode to text 
      delay(2500); 
      Serial.write("AT+CMGS="); 
      Serial.write(34); //ASCII of “ 
      Serial.write("+256779319949"); 
      Serial.write(34); 
      Serial.write(13); 
      Serial.write(10); 
      delay(2500);
      Serial.println(msg);
      
       delay(2500); 
       Serial.write(26); 
       Serial.write(13); 
       Serial.write(10);//Ascii code of ctrl+z to send the message   
  
       delay(3000);
}
void setup() {
  lcd.begin();
  lcd.backlight();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  lcd.setCursor(0, 0);
  lcd.print("SMART H2O LEVEL");
  lcd.setCursor(0, 1);
  lcd.print("    MONITOR");
  delay(2000);
  Serial.begin(9600);               // the GPRS baud rate   
                  
    lcd.clear();
    lcd.print("Checking network");
    lcd.setCursor(0,1);
    lcd.print("Please wait....");
    delay(15000);
    
    Serial.begin(4800);
    delay(1000);
    Serial.println("AT");
    delay(1000);
    Serial.println("AT+CMGF=1");  // set SMS mode to text
    delay(100);
    Serial.println("AT+CNMI=2,2,0,0,0");     
    delay(500);
    Serial.println("AT+CREG?");
    delay(500);

     while (!(Serial.available())) 
  {
     lcd.clear();
      lcd.print("Check GSM Modem");
      lcd.setCursor(0,1);
      lcd.print("and retry....");
      delay(200);  
    
  }
    String response=Serial.readString();
    while(!(response.indexOf("+CREG: 0,1")>-1)) 
    {
     Serial.println("AT+CREG?");
      response=Serial.readString();
       lcd.clear();
      lcd.print("Network ");
      lcd.setCursor(0,1);
      lcd.print("Connecting.....");
      delay(3000);    
       
      
    }
    
    if(response.indexOf("+CREG: 0,1")>-1) 
    {
     Serial.println("AT+CREG?");
      response=Serial.readString();
        lcd.clear();
      lcd.print("  NETWORK");
      lcd.setCursor(0,1);
      lcd.print("  CONNECTED");
    }
    
   Serial.println("AT");
    delay(1000);
   Serial.println("AT+CMGF=1");  // set SMS mode to text
    delay(100);
   Serial.println("AT+CNMI=2,2,0,0,0"); 
    delay(2000);
    init_sms("Connection OK");
  
     
}

void loop() {

  long timt=millis();
  do
  {
      digitalWrite(TRIG, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG, LOW);

      float duration = pulseIn(ECHO, HIGH);
      distance = duration * 0.034 / 2; // Speed of sound = 343 m/s or 0.034 cm/µs
      lcd.clear();
      lcd.print("Depth: ");
      lcd.print(distance);
      lcd.println(" cm");
    
      if(distance >23)
      {
        digitalWrite(7, HIGH);
        digitalWrite(6, LOW);
        digitalWrite(5, LOW);
        init_sms("Tank full!");
        
      }
      else if(distance >9)
      {
        digitalWrite(6, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(7, LOW);
      }
       else 
      {
        digitalWrite(5, HIGH);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
        init_sms("H20 level extremely Low!");
      }
      
      
      delay(1000);

  }
  while((millis()-timt)<30000);

  

   if (Serial.available())
   Serial.write(Serial.read());
 
  Serial.println("AT");
  delay(1000);
  Serial.println("AT+CPIN?");
  delay(1000);
 
  Serial.println("AT+CREG?");
  delay(1000);
 
  Serial.println("AT+CGATT?");
  delay(1000);
 
  Serial.println("AT+CIPSHUT");
  delay(1000);
 
  Serial.println("AT+CIPSTATUS");
  delay(2000);
 
  Serial.println("AT+CIPMUX=0");
  delay(2000);
  ShowSerialData();
 
  Serial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  Serial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  Serial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  Serial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
  
  ShowSerialData();
 
  Serial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=L6EE5X3VM6Z3N5ZV&field1=" + String(distance) +"&field2="+String(88);
  Serial.println(str);
  Serial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  Serial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  Serial.println();
  
  ShowSerialData();
 
  Serial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
void ShowSerialData()
{
  //while(Serial.available()!=0)
  
  
  //Serial.write(Serial.read());
 // delay(5000);  
}
#include <WiFi.h>
#include <ThingSpeak.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WiFiClient client;

unsigned long channelID = 3001129;
const char* readAPIKey = "GET https://api.thingspeak.com/update?api_key=L6EE5X3VM6Z3N5ZV&field1=";
const char* writeAPIKey = "YOUR_WRITE_API_KEY";  // Optional, for alerts
unsigned long alertChannelID = YOUR_ALERT_CHANNEL_ID;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected!");

  // Arrays to store data
  float usage[14];
  float usageLastWeek = 0;
  float usageThisWeek = 0;
  String message;

  // Read last 14 entries
  for (int i = 0; i < 14; i++) {
    usage[i] = ThingSpeak.readField(3001129, 1, "GET https://api.thingspeak.com/update?api_key=L6EE5X3VM6Z3N5ZV&field1=", i + 1);
    if (usage[i] == -1) {
      Serial.println("Error reading data at entry " + String(i));
      return;
    }
  }

  // Sum usage: days 1-7 (last week) and 8-14 (this week)
  for (int i = 0; i < 7; i++) {
    usageLastWeek += usage[i];
  }
  for (int i = 7; i < 14; i++) {
    usageThisWeek += usage[i];
  }

  float change = usageThisWeek - usageLastWeek;
  float changePercent = (change / usageLastWeek) * 100;

  // Generate feedback message
  if (change > 0.2 * usageLastWeek) {
    message = "Usage up by " + String(round(changePercent)) + "%. Please check for leaks or overuse.";
  } else if (change < -0.2 * usageLastWeek) {
    message = "Usage down by " + String(round(-changePercent)) + "%. Well done!";
  } else {
    message = "Usage stable. Keep it up!";
  }

  Serial.println("Feedback: " + message);

  // Optional: Send message to another ThingSpeak channel
  ThingSpeak.setField(1, message);
  int result = ThingSpeak.writeFields(3001129,"GET https://api.thingspeak.com/update?api_key=L6EE5X3VM6Z3N5ZV&field1=");
  if (result == 200) {
    Serial.println("Feedback successfully sent to ThingSpeak.");
  } else {
    Serial.println("Error sending feedback: " + String(result));
  }
}

void loop() {
  // Only run once in setup for now
}

