#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define TRIG 9
#define ECHO 10
float distance;
int depth,ltr;
void init_sms(String msg)
{
      Serial.println("AT+CMGF=1"); // set the SMS mode to text 
      delay(2500); 
      Serial.write("AT+CMGS="); 
      Serial.write(34); //ASCII of “ 
      Serial.write("+256786178453"); 
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
      depth=map(distance,0,24,100,0);
      ltr=map(depth,100,0,5,0);
      if(depth<0)
      {
        depth=0;
      }
      lcd.clear();
      lcd.print("Ltrs: ");
      lcd.print(ltr);
      lcd.println(" Ltrs");
      lcd.setCursor(0,1);
      lcd.print("RDepth: ");
      lcd.print(depth);
      lcd.println(" %");
      
    
      if(depth >85)
      {
        digitalWrite(7, HIGH);
        digitalWrite(6, LOW);
        digitalWrite(5, LOW);
        init_sms("Tank full!");
        
      }
      else if(depth>40)
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
