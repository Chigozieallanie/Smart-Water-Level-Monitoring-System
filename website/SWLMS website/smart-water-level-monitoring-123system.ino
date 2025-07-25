#include <LiquidCrystal_I2C.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define TRIG 9
#define ECHO 10
float distance;
int depth, ltr;

void init_sms(String msg)
{
    Serial.println("AT+CMGF=1"); // Set SMS mode to text
    delay(1000);
    Serial.print("AT+CMGS=");
    Serial.print((char)34); // ASCII of "
    Serial.print("+256786178453");
    Serial.print((char)34);
    Serial.print((char)13);
    Serial.print((char)10);
    delay(1000);
    Serial.println(msg);
    delay(1000);
    Serial.print((char)26); // ASCII of Ctrl+Z
    Serial.print((char)13);
    Serial.print((char)10);
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

    lcd.setCursor(0, 0);
    lcd.print("SMART H2O LEVEL");
    lcd.setCursor(0, 1);
    lcd.print("    MONITOR");
    delay(2000);

    Serial.begin(9600); // Consistent baud rate
    lcd.clear();
    lcd.print("Checking network");
    lcd.setCursor(0, 1);
    lcd.print("Please wait....");
    delay(10000);

    Serial.println("AT");
    delay(1000);
    ShowSerialData();

    Serial.println("AT+CMGF=1"); // Set SMS mode to text
    delay(500);
    Serial.println("AT+CNMI=2,2,0,0,0");
    delay(500);
    Serial.println("AT+CREG?");
    delay(500);
    ShowSerialData();

    while (!Serial.available())
    {
        lcd.clear();
        lcd.print("Check GSM Modem");
        lcd.setCursor(0, 1);
        lcd.print("and retry....");
        delay(200);
    }

    String response = Serial.readString();
    while (response.indexOf("+CREG: 0,1") == -1 && response.indexOf("+CREG: 0,5") == -1)
    {
        Serial.println("AT+CREG?");
        delay(500);
        response = Serial.readString();
        lcd.clear();
        lcd.print("Network ");
        lcd.setCursor(0, 1);
        lcd.print("Connecting.....");
        delay(3000);
        ShowSerialData();
    }

    lcd.clear();
    lcd.print("  NETWORK");
    lcd.setCursor(0, 1);
    lcd.print("  CONNECTED");
    delay(2000);

    init_sms("Connection OK");
}

void loop()
{
    checkForSmsCommands(); // Check for SMS commands from ThingSpeak

    long timt = millis();
    do
    {
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);

        float duration = pulseIn(ECHO, HIGH);
        distance = duration * 0.034 / 2; // Speed of sound = 343 m/s or 0.034 cm/µs
        depth = map(distance, 0, 24, 100, 0);
        ltr = map(depth, 100, 0, 5, 0);
        if (depth < 0)
        {
            depth = 0;
        }

        lcd.clear();
        lcd.print("Ltrs: ");
        lcd.print(ltr);
        lcd.print(" Ltrs");
        lcd.setCursor(0, 1);
        lcd.print("RDepth: ");
        lcd.print(depth);
        lcd.print(" %");

        if (depth > 85)
        {
            digitalWrite(7, HIGH);
            digitalWrite(6, LOW);
            digitalWrite(5, LOW);
            init_sms("Tank full!");
        }
        else if (depth > 40)
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
            init_sms("H2O level extremely Low!");
        }

        delay(1000);
    } while ((millis() - timt) < 30000);

    // Upload data to ThingSpeak
    if (!sendToThingSpeak())
    {
        lcd.clear();
        lcd.print("ThingSpeak Fail");
        delay(2000);
    }
}

bool sendToThingSpeak()
{
    Serial.println("AT+CIPSHUT");
    delay(1000);
    ShowSerialData();

    Serial.println("AT+CIPMUX=0");
    delay(1000);
    ShowSerialData();

    Serial.println("AT+CSTT=\"internet\""); // Replace with your APN
    delay(1000);
    ShowSerialData();

    Serial.println("AT+CIICR");
    delay(3000);
    ShowSerialData();

    Serial.println("AT+CIFSR");
    delay(2000);
    ShowSerialData();

    Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
    delay(6000);
    ShowSerialData();

    Serial.println("AT+CIPSEND");
    delay(2000);
    ShowSerialData();

    String str = "GET /update?api_key=L6EE5X3VM6Z3N5ZV&field1=" + String(distance) + "&field2=" + String(ltr) + "&field3=" + String(depth) + " HTTP/1.1\r\nHost: api.thingspeak.com\r\n\r\n";
    Serial.println(str);
    delay(4000);
    ShowSerialData();

    Serial.print((char)26); // Send CTRL+Z
    delay(5000);
    ShowSerialData();

    Serial.println("AT+CIPSHUT");
    delay(1000);
    ShowSerialData();

    return true; // Add error checking if needed
}

void checkForSmsCommands()
{
    Serial.println("AT+CIPSHUT");
    delay(1000);
    ShowSerialData();

    Serial.println("AT+CIPMUX=0");
    delay(1000);
    ShowSerialData();

    Serial.println("AT+CSTT=\"internet\""); // Replace with your APN
    delay(1000);
    ShowSerialData();

    Serial.println("AT+CIICR");
    delay(3000);
    ShowSerialData();

    Serial.println("AT+CIFSR");
    delay(2000);
    ShowSerialData();

    Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
    delay(6000);
    ShowSerialData();

    Serial.println("AT+CIPSEND");
    delay(2000);
    ShowSerialData();

    // *** CRITICAL FIX: Corrected the HTTP GET request string ***
    // Replace with your ThingSpeak Channel ID and Read API Key
    String getCmd = "GET /channels/YOUR_CHANNEL_ID/fields/8/last.json?api_key=YOUR_READ_API_KEY HTTP/1.1\r\nHost: api.thingspeak.com\r\n\r\n";
    Serial.println(getCmd);
    delay(4000);
    ShowSerialData();

    String response = "";
    unsigned long timeout = millis() + 5000;
    while (millis() < timeout && response.length() < 200)
    {
        while (Serial.available())
        {
            response += (char)Serial.read();
        }
    }

    if (response.indexOf("\"field8\":\"") != -1)
    {
        int startIndex = response.indexOf("\"field8\":\"") + 10;
        int endIndex = response.indexOf("\"", startIndex);
        if (endIndex > startIndex)
        {
            String msg = response.substring(startIndex, endIndex);
            msg.replace("\\n", "\n");
            msg.replace("\\r", "\r");

            if (msg.length() > 0 && msg != "null")
            {
                init_sms(msg);
                // Clear Field 8 after sending SMS
                Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
                delay(6000);
                ShowSerialData();

                Serial.println("AT+CIPSEND");
                delay(2000);
                ShowSerialData();

                String updateCmd = "GET /update?api_key=L6EE5X3VM6Z3N5ZV&field8= HTTP/1.1\r\nHost: api.thingspeak.com\r\n\r\n";
                Serial.println(updateCmd);
                delay(4000);
                ShowSerialData();

                Serial.print((char)26);
                delay(5000);
                ShowSerialData();
            }
        }
    }

    Serial.println("AT+CIPSHUT");
    delay(1000);
    ShowSerialData();
}

void ShowSerialData()
{
    while (Serial.available())
    {
        Serial.write(Serial.read());
    }
}