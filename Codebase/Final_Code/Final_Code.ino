#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define TRIG 9
#define ECHO 10

// Tank parameters
const int TANK_DEPTH_CM = 24;
const int TANK_LITERS = 5;

// GSM and ThingSpeak
String thingspeakChannelID = "3001129";
String thingspeakWriteAPIKey = "L6EE5X3VM6Z3N5ZV";
String lastCommand = "";
const char *phoneNumber = "+256786178453";

// State
float distance;
int depth, ltr;
bool lowAlertSent = false;
bool fullAlertSent = false;
unsigned long lastUpload = 0;
const unsigned long uploadInterval = 30000; // 30 seconds

void sendSMS(String msg)
{
  Serial.println("AT+CMGF=1");
  delay(1000);
  Serial.print("AT+CMGS=\"");
  Serial.print(phoneNumber);
  Serial.println("\"");
  delay(1000);
  Serial.print(msg);
  delay(500);
  Serial.write(26); // Ctrl+Z
  delay(3000);
}

void setup()
{
  lcd.begin();
  lcd.backlight();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(11, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("SMART H2O LEVEL");
  lcd.setCursor(0, 1);
  lcd.print("    MONITOR");
  delay(2000);
  Serial.begin(9600);

  lcd.clear();
  lcd.print("Checking network");
  lcd.setCursor(0, 1);
  lcd.print("Please wait....");
  delay(15000);

  Serial.begin(4800);
  delay(1000);
  Serial.println("AT");
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(100);
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(500);
  Serial.println("AT+CREG?");
  delay(500);

  while (!(Serial.available()))
  {
    lcd.clear();
    lcd.print("Check GSM Modem");
    lcd.setCursor(0, 1);
    lcd.print("and retry....");
    delay(200);
  }
  String response = Serial.readString();
  while (!(response.indexOf("+CREG: 0,1") > -1))
  {
    Serial.println("AT+CREG?");
    response = Serial.readString();
    lcd.clear();
    lcd.print("Network ");
    lcd.setCursor(0, 1);
    lcd.print("Connecting.....");
    delay(3000);
  }
  if (response.indexOf("+CREG: 0,1") > -1)
  {
    Serial.println("AT+CREG?");
    response = Serial.readString();
    lcd.clear();
    lcd.print("  NETWORK");
    lcd.setCursor(0, 1);
    lcd.print("  CONNECTED");
  }
  Serial.println("AT");
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(100);
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(2000);
  sendSMS("Connection OK");
}

// --- ThingSpeak helpers ---
String getThingSpeakField8()
{
  String cmd = "";
  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(6000);
  ShowSerialData();
  Serial.println("AT+CIPSEND");
  delay(4000);
  ShowSerialData();
  String getCmd = "GET /channels/" + thingspeakChannelID + "/fields/8/last.txt\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n";
  Serial.print(getCmd);
  delay(4000);
  ShowSerialData();
  Serial.println((char)26);
  delay(5000);
  cmd = readSerialResponse();
  Serial.println("AT+CIPSHUT");
  delay(100);
  ShowSerialData();
  int idx = cmd.indexOf("\r\n\r\n");
  if (idx != -1)
  {
    cmd = cmd.substring(idx + 4);
    cmd.trim();
  }
  return cmd;
}
void clearThingSpeakField8()
{
  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(6000);
  ShowSerialData();
  Serial.println("AT+CIPSEND");
  delay(4000);
  ShowSerialData();
  String clrCmd = "GET /update?api_key=" + thingspeakWriteAPIKey + "&field8=\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n";
  Serial.print(clrCmd);
  delay(4000);
  ShowSerialData();
  Serial.println((char)26);
  delay(5000);
  ShowSerialData();
  Serial.println("AT+CIPSHUT");
  delay(100);
  ShowSerialData();
}
String readSerialResponse()
{
  String resp = "";
  unsigned long start = millis();
  while (millis() - start < 8000)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      resp += c;
    }
  }
  return resp;
}
void processCommand(String cmd)
{
  if (cmd.startsWith("REPORT:"))
  {
    String reportType = cmd.substring(7);
    if (reportType == "Daily")
    {
      sendSMS("Daily Report: Water level is " + String(depth) + "%");
    }
    else if (reportType == "Weekly")
    {
      sendSMS("Weekly Report: Water level is " + String(depth) + "%");
    }
    else
    {
      sendSMS("Report: Water level is " + String(depth) + "%");
    }
  }
  else if (cmd.startsWith("SMS:"))
  {
    String msg = cmd.substring(4);
    sendSMS(msg);
  }
  else if (cmd.length() > 0)
  {
    sendSMS("Unknown command: " + cmd);
  }
}
void ShowSerialData()
{
  // Optionally print serial data for debugging
}

void updateTankReadings()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  float duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;
  // Map to tank
  if (distance > TANK_DEPTH_CM)
  {
    depth = 0;
    ltr = 0;
  }
  else
  {
    depth = map(distance, 0, TANK_DEPTH_CM, 100, 0);
    if (depth < 0)
      depth = 0;
    ltr = map(depth, 100, 0, TANK_LITERS, 0);
    if (ltr < 0)
      ltr = 0;
  }
}

void updateLCD()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ltrs: ");
  lcd.print(ltr);
  lcd.print(" Ltrs");
  lcd.setCursor(0, 1);
  lcd.print("Depth: ");
  lcd.print(depth);
  lcd.print(" %");
}

void handleAlerts()
{
  // Full alert (≥95%)
  if (depth >= 95)
  {
    // Tank full: LED/buzzer logic
    digitalWrite(7, HIGH);  // Full LED ON
    digitalWrite(11, HIGH); // Buzzer ON
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    if (!fullAlertSent)
    {
      sendSMS("Tank full! Consider closing the tap.");
      fullAlertSent = true;
      lowAlertSent = false; // Reset low alert
    }
  }
  // Normal range (21–94%)
  else if (depth > 20)
  {
    digitalWrite(6, HIGH); // Normal LED ON
    digitalWrite(5, LOW);
    digitalWrite(7, LOW);
    digitalWrite(11, LOW);
    // Reset alerts
    lowAlertSent = false;
    fullAlertSent = false;
  }
  // Low alert (≤20%)
  else
  {
    digitalWrite(5, HIGH); // Low LED ON
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(11, HIGH); // Buzzer ON
    if (!lowAlertSent)
    {
      sendSMS("H2O level low! Please refill soon. Check for leaks if unexpected.");
      lowAlertSent = true;
      fullAlertSent = false; // Reset full alert
    }
  }
}

void uploadToThingSpeak()
{
  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  delay(6000);
  ShowSerialData();
  Serial.println("AT+CIPSEND");
  delay(4000);
  ShowSerialData();
  // Swap field2 and field3: field2 = liters, field3 = percent
  String str = "GET /update?api_key=" + thingspeakWriteAPIKey + "&field1=" + String(distance) + "&field2=" + String(ltr) + "&field3=" + String(depth) + "\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n";
  Serial.print(str);
  delay(4000);
  ShowSerialData();
  Serial.println((char)26);
  delay(5000);
  ShowSerialData();
  Serial.println("AT+CIPSHUT");
  delay(100);
  ShowSerialData();
}

void loop()
{
  // Update tank readings and LCD
  updateTankReadings();
  updateLCD();
  handleAlerts();

  // Every 30 seconds: upload to ThingSpeak and poll for commands
  if (millis() - lastUpload >= uploadInterval)
  {
    lastUpload = millis();
    uploadToThingSpeak();
    String command = getThingSpeakField8();
    command.trim();
    if (command.length() > 1 && command != lastCommand)
    {
      processCommand(command);
      lastCommand = command;
      clearThingSpeakField8();
    }
  }
  delay(1000); // Small delay to avoid LCD flicker
}
