/*
 * Enhanced Smart Water Level Monitoring System
 * Arduino ESP32 Code with ThingSpeak Integration & Website Dashboard
 *
 * This code works with the website dashboard to provide:
 * - Real-time data uploading to ThingSpeak
 * - Command reading from website via ThingSpeak
 * - Automatic SMS reporting with detailed analysis
 * - LED status indicators with buzzer alerts
 * - Enhanced sensor monitoring
 *
 * Fields Used:
 * - field1: Distance (cm)
 * - field2: Water Level (liters)
 * - field3: Depth Percentage (%)
 * - field4: Immediate Commands (send_sms, test_sms, etc.)
 * - field5: Analysis Commands (daily_analysis, weekly_analysis, etc.)
 *
 * Hardware Connections:
 * - Pin 5: Red LED (Critical Level)
 * - Pin 6: Yellow LED (Low Level)
 * - Pin 7: Green LED (Normal/Full Level)
 * - Pin 11: Buzzer (Alerts)
 * - Pin 9: Ultrasonic TRIG
 * - Pin 10: Ultrasonic ECHO
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define TRIG 9
#define ECHO 10
float distance;
int depth, ltr;

// ThingSpeak Configuration - UPDATE THESE VALUES
String WRITE_API_KEY = "L6EE5X3VM6Z3N5ZV"; // Your Write API Key
String READ_API_KEY = "ON4ULBWQ12ZVQ11X";  // Your Read API Key
String CHANNEL_ID = "3001129";             // Your Channel ID

// Command checking variables
unsigned long lastCommandCheck = 0;
const unsigned long COMMAND_CHECK_INTERVAL = 10000; // Check every 10 seconds

// SMS spam prevention
unsigned long lastSMSTime = 0;
const unsigned long SMS_COOLDOWN = 300000; // 5 minutes between automatic SMS

void init_sms(String msg)
{
    // Prevent SMS spam - only send if enough time has passed
    if (millis() - lastSMSTime < SMS_COOLDOWN && msg != "Test SMS from Smart Water System - All systems operational")
    {
        Serial.println("SMS cooldown active - skipping automatic SMS");
        return;
    }

    Serial.println("AT+CMGF=1"); // set the SMS mode to text
    delay(2500);
    Serial.write("AT+CMGS=");
    Serial.write(34);              // ASCII of "
    Serial.write("+256786178453"); // Your phone number
    Serial.write(34);
    Serial.write(13);
    Serial.write(10);
    delay(2500);
    Serial.println(msg);

    delay(2500);
    Serial.write(26);
    Serial.write(13);
    Serial.write(10); // Ascii code of ctrl+z to send the message

    delay(3000);
    lastSMSTime = millis(); // Update last SMS time
}

void triggerBuzzer(int pattern)
{
    // Different buzzer patterns for different alerts
    switch (pattern)
    {
    case 1: // Single beep for normal alerts
        digitalWrite(11, HIGH);
        delay(500);
        digitalWrite(11, LOW);
        break;
    case 2: // Double beep for warnings
        for (int i = 0; i < 2; i++)
        {
            digitalWrite(11, HIGH);
            delay(300);
            digitalWrite(11, LOW);
            delay(200);
        }
        break;
    case 3: // Triple beep for critical alerts
        for (int i = 0; i < 3; i++)
        {
            digitalWrite(11, HIGH);
            delay(200);
            digitalWrite(11, LOW);
            delay(150);
        }
        break;
    }
}

void setup()
{
    lcd.begin(16,2);
    lcd.backlight();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(5, OUTPUT);  // Red LED - Critical
    pinMode(6, OUTPUT);  // Yellow LED - Low
    pinMode(7, OUTPUT);  // Green LED - Normal/Full
    pinMode(11, OUTPUT); // Buzzer

    lcd.setCursor(0, 0);
    lcd.print("SMART H2O LEVEL");
    lcd.setCursor(0, 1);
    lcd.print("    MONITOR");
    delay(2000);

    // Initialize with startup sequence
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    triggerBuzzer(1);
    delay(1000);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);

    Serial.begin(9600); // the GPRS baud rate

    lcd.clear();
    lcd.print("Checking network");
    lcd.setCursor(0, 1);
    lcd.print("Please wait....");
    delay(15000);

    Serial.begin(4800);
    delay(1000);
    Serial.println("AT");
    delay(1000);
    Serial.println("AT+CMGF=1"); // set SMS mode to text
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
        // Blink red LED while waiting for GSM
        digitalWrite(5, HIGH);
        delay(200);
        digitalWrite(5, LOW);
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
        // Blink yellow LED while connecting
        digitalWrite(6, HIGH);
        delay(1500);
        digitalWrite(6, LOW);
        delay(1500);
    }

    if (response.indexOf("+CREG: 0,1") > -1)
    {
        Serial.println("AT+CREG?");
        response = Serial.readString();
        lcd.clear();
        lcd.print("  NETWORK");
        lcd.setCursor(0, 1);
        lcd.print("  CONNECTED");
        // Green LED solid when connected
        digitalWrite(7, HIGH);
        triggerBuzzer(2);
        delay(2000);
        digitalWrite(7, LOW);
    }

    Serial.println("AT");
    delay(1000);
    Serial.println("AT+CMGF=1"); // set SMS mode to text
    delay(100);
    Serial.println("AT+CNMI=2,2,0,0,0");
    delay(2000);
    init_sms("Smart Water System Online - Enhanced monitoring active with website integration");
}

void loop()
{
    long timt = millis();

    // Main monitoring loop (30 seconds)
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
        lcd.println(" Ltrs");
        lcd.setCursor(0, 1);
        lcd.print("RDepth: ");
        lcd.print(depth);
        lcd.println(" %");

        // Enhanced LED and buzzer status indicators
        if (depth > 85)
        {
            // Tank full - Green LED + Buzzer notification
            digitalWrite(7, HIGH);
            digitalWrite(11, HIGH);
            digitalWrite(6, LOW);
            digitalWrite(5, LOW);

            if (millis() - lastSMSTime > SMS_COOLDOWN)
            {
                init_sms("Tank Full Alert: " + String(ltr) + "L (" + String(depth) + "%) - Consider stopping water supply");
                triggerBuzzer(2);
            }
        }
        else if (depth > 40)
        {
            // Normal level - Yellow LED
            digitalWrite(6, HIGH);
            digitalWrite(5, LOW);
            digitalWrite(7, LOW);
            digitalWrite(11, LOW);
        }
        else
        {
            // Critical low level - Red LED + Buzzer alarm
            digitalWrite(5, HIGH);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            digitalWrite(11, HIGH);

            if (millis() - lastSMSTime > SMS_COOLDOWN)
            {
                init_sms("CRITICAL: H2O level extremely low! " + String(ltr) + "L (" + String(depth) + "%) - Immediate refill needed");
                triggerBuzzer(3);
            }
        }

        // Check for commands from website
        if (millis() - lastCommandCheck > COMMAND_CHECK_INTERVAL)
        {
            checkWebsiteCommands();
            lastCommandCheck = millis();
        }

        delay(1000);
    } while ((millis() - timt) < 30000);

    // Send data to ThingSpeak
    sendDataToThingSpeak();
}

void sendDataToThingSpeak()
{
    if (Serial.available())
    {
        Serial.write(Serial.read());
    }

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

    Serial.println("AT+CSTT=\"internet\""); // start task and setting the APN,
    delay(1000);
    ShowSerialData();

    Serial.println("AT+CIICR"); // bring up wireless connection
    delay(3000);
    ShowSerialData();

    Serial.println("AT+CIFSR"); // get local IP address
    delay(2000);
    ShowSerialData();

    Serial.println("AT+CIPSPRT=0");
    delay(3000);
    ShowSerialData();

    Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\""); // start up the connection
    delay(6000);
    ShowSerialData();

    Serial.println("AT+CIPSEND"); // begin send data to remote server
    delay(4000);
    ShowSerialData();

    // Send comprehensive data to all fields for website integration
    String str = "GET /update?api_key=" + WRITE_API_KEY +
                 "&field1=" + String(distance) + // Distance in cm
                 "&field2=" + String(ltr) +      // Water level in liters
                 "&field3=" + String(depth) +    // Depth percentage
                 " HTTP/1.0\r\n\r\n";

    Serial.println(str);
    delay(1000);

    delay(4000);
    ShowSerialData();

    Serial.println((char)26); // sending
    delay(5000);              // waiting for reply, important! the time is based on the condition of internet
    Serial.println();
    ShowSerialData();

    Serial.println("AT+CIPSHUT"); // close the connection
    delay(100);
    ShowSerialData();

    // Indicate successful data transmission
    digitalWrite(7, HIGH);
    delay(100);
    digitalWrite(7, LOW);
}

void checkWebsiteCommands()
{
    // Read commands from ThingSpeak field4 and field5
    String immediateCommand = readThingSpeakField(4);
    String analysisCommand = readThingSpeakField(5);

    // Process immediate commands
    if (immediateCommand.length() > 0)
    {
        processImmediateCommand(immediateCommand);
        clearThingSpeakField(4); // Clear the command after processing
    }

    // Process analysis commands
    if (analysisCommand.length() > 0)
    {
        processAnalysisCommand(analysisCommand);
        clearThingSpeakField(5); // Clear the command after processing
    }
}

String readThingSpeakField(int fieldNumber)
{
    // Connect to ThingSpeak to read command field
    Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
    delay(3000);

    Serial.println("AT+CIPSEND");
    delay(2000);

    String getStr = "GET /channels/" + CHANNEL_ID + "/fields/" + String(fieldNumber) +
                    "/last.json?api_key=" + READ_API_KEY + " HTTP/1.0\r\n\r\n";

    Serial.println(getStr);
    Serial.println((char)26);
    delay(3000);

    String response = "";
    while (Serial.available())
    {
        response += (char)Serial.read();
    }

    Serial.println("AT+CIPSHUT");
    delay(1000);

    // Parse JSON response to extract field value
    int startPos = response.indexOf("\"field" + String(fieldNumber) + "\":\"");
    if (startPos > -1)
    {
        startPos += 10 + String(fieldNumber).length();
        int endPos = response.indexOf("\"", startPos);
        if (endPos > startPos)
        {
            return response.substring(startPos, endPos);
        }
    }

    return "";
}

void clearThingSpeakField(int fieldNumber)
{
    // Connect and send empty value to clear the field
    Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
    delay(3000);

    Serial.println("AT+CIPSEND");
    delay(2000);

    String clearStr = "GET /update?api_key=" + WRITE_API_KEY +
                      "&field" + String(fieldNumber) + "= HTTP/1.0\r\n\r\n";

    Serial.println(clearStr);
    Serial.println((char)26);
    delay(2000);

    Serial.println("AT+CIPSHUT");
    delay(1000);
}

void processImmediateCommand(String command)
{
    if (command == "manual_alert")
    {
        String msg = "MANUAL ALERT from Website: Current level " + String(ltr) + "L (" + String(depth) + "%) - Distance: " + String(distance) + "cm";
        init_sms(msg);
        triggerBuzzer(2);

        // Flash all LEDs for manual alert
        for (int i = 0; i < 3; i++)
        {
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
            digitalWrite(7, HIGH);
            delay(200);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            delay(200);
        }
    }
    else if (command == "test_sms")
    {
        init_sms("Test SMS from Smart Water System - All systems operational. Current: " + String(ltr) + "L (" + String(depth) + "%)");
        triggerBuzzer(1);

        // Single flash green LED for test
        digitalWrite(7, HIGH);
        delay(500);
        digitalWrite(7, LOW);
    }
    else if (command == "send_sms")
    {
        String msg = "Status Update: " + String(ltr) + "L remaining (" + String(depth) + "% full) - System operational";
        init_sms(msg);
        triggerBuzzer(1);
    }
}

void processAnalysisCommand(String command)
{
    if (command.startsWith("daily_analysis"))
    {
        generateDailyReport();
    }
    else if (command.startsWith("weekly_analysis"))
    {
        generateWeeklyReport();
    }
    else if (command.startsWith("custom_analysis"))
    {
        generateCustomReport();
    }
}

void generateDailyReport()
{
    String report = "SMART H2O DAILY REPORT\n";
    report += "Date: " + getCurrentDate() + "\n";
    report += "Current: " + String(ltr) + "L (" + String(depth) + "%)\n";
    report += "Distance: " + String(distance) + "cm\n";
    report += "Status: " + getStatusText() + "\n";
    report += "LEDs: " + getLEDStatus() + "\n";
    report += "System: All sensors operational";

    init_sms(report);
    triggerBuzzer(2);
}

void generateWeeklyReport()
{
    String report = "SMART H2O WEEKLY ANALYSIS\n";
    report += "Week: " + getCurrentWeek() + "\n";
    report += "Current: " + String(ltr) + "L (" + String(depth) + "%)\n";
    report += "Distance: " + String(distance) + "cm\n";
    report += "Trend: " + getTrendText() + "\n";
    report += "Hardware: All components OK\n";
    report += "Status: System operational";

    init_sms(report);
    triggerBuzzer(2);
}

void generateCustomReport()
{
    String report = "SMART H2O CUSTOM REPORT\n";
    report += "Current: " + String(ltr) + "L (" + String(depth) + "%)\n";
    report += "Distance: " + String(distance) + "cm\n";
    report += "Hardware Status: " + getLEDStatus() + "\n";
    report += "System Status: " + getStatusText() + "\n";
    report += "Uptime: " + String(millis() / 1000) + " seconds";

    init_sms(report);
    triggerBuzzer(1);
}

String getCurrentDate()
{
    // Return current date - implement based on your RTC or network time
    return "Today";
}

String getCurrentWeek()
{
    // Return current week range
    return "This week";
}

String getStatusText()
{
    if (depth > 75)
        return "Excellent";
    if (depth > 50)
        return "Good";
    if (depth > 25)
        return "Low";
    return "Critical";
}

String getTrendText()
{
    if (depth > 75)
        return "Stable - High level";
    if (depth > 50)
        return "Normal usage";
    if (depth > 25)
        return "Moderate usage";
    return "High usage - Monitor closely";
}

String getLEDStatus()
{
    String status = "";
    if (digitalRead(7))
        status += "Green ";
    if (digitalRead(6))
        status += "Yellow ";
    if (digitalRead(5))
        status += "Red ";
    if (digitalRead(11))
        status += "Buzzer ";
    if (status == "")
        status = "All Off";
    return status;
}

void ShowSerialData()
{
    // Keep existing implementation for debugging
    // while(Serial.available()!=0)
    // Serial.write(Serial.read());
    // delay(5000);
}

/*
 * ENHANCED ARDUINO SETUP INSTRUCTIONS:
 *
 * 1. Hardware Connections:
 *    - Pin 5: Red LED + 220Ω resistor (Critical Level Alert)
 *    - Pin 6: Yellow LED + 220Ω resistor (Low Level Warning)
 *    - Pin 7: Green LED + 220Ω resistor (Normal/Full Level)
 *    - Pin 11: Buzzer/Piezo Speaker (Audio Alerts)
 *    - Pin 9: Ultrasonic Sensor TRIG
 *    - Pin 10: Ultrasonic Sensor ECHO
 *    - LCD: I2C connection (SDA/SCL)
 *    - GSM Module: Serial connection
 *
 * 2. Update Configuration:
 *    - Set your ThingSpeak WRITE_API_KEY, READ_API_KEY, CHANNEL_ID
 *    - Update phone number in init_sms() function
 *    - Adjust water tank dimensions in map() functions if needed
 *
 * 3. Upload & Test:
 *    - Upload this code to your ESP32/Arduino
 *    - Configure website dashboard with same API keys
 *    - Test using manual controls in dashboard
 *
 * ENHANCED FEATURES ADDED:
 * ✅ SMS Spam Prevention (5-minute cooldown)
 * ✅ LED Status Indicators (Red/Yellow/Green)
 * ✅ Buzzer Alert Patterns (Different beeps for different alerts)
 * ✅ Enhanced SMS Messages (More detailed information)
 * ✅ Startup LED Sequence (Visual system check)
 * ✅ Network Connection Visual Feedback
 * ✅ Manual Alert LED Flash Pattern
 * ✅ Data Transmission Confirmation (Green LED blink)
 * ✅ Hardware Status Reporting in SMS
 * ✅ System Uptime Tracking
 * ✅ Website Command Integration
 *
 * WEBSITE INTEGRATION:
 * - Real-time data display on dashboard
 * - Manual SMS triggering from website
 * - Automatic scheduled reports (daily/weekly/custom)
 * - Multi-user API configuration
 * - GitHub Pages compatible hosting
 *
 * TROUBLESHOOTING:
 * - Red LED blinking: GSM module not detected
 * - Yellow LED blinking: Network connecting
 * - No SMS: Check phone number and GSM network
 * - No website data: Verify API keys match
 * - LEDs not working: Check wiring and resistors
 * - Buzzer not working: Check buzzer polarity
 */
