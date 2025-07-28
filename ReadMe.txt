Smart Water Level Monitoring System
Revolutionizing Water Management for Sustainable Living
Project Overview
The Smart Water Level Monitoring System (SWLMS) is an innovative Internet of Things (IoT) solution designed to address critical challenges in water resource management, particularly in communities reliant on water storage tanks. By transforming traditional, manual monitoring into an automated, real-time process, our system empowers users with precise insights and timely alerts, promoting efficient water usage and preventing wastage. This project represents a significant step towards fostering sustainable water practices and enhancing daily convenience.

Key Features
Real-time Water Level Monitoring: Continuously measures and displays water levels in tanks.

Intelligent Alerting: Sends immediate SMS notifications for low, average, high, and empty water levels.

Local Visual Feedback: Provides instant status updates via an integrated LCD display and multi-colored LED indicators.

Cloud Data Logging & Visualization: Uploads data to ThingSpeak for remote access, historical analysis, and trend visualization.

Water Flow Detection: Monitors water inflow to detect refilling status and potential supply issues.

Automated Pump Readiness: Designed with future integration for automated pump control based on water levels.

Technology Stack
Hardware Components:

Microcontroller: Arduino Uno R3

Sensing: HC-SR04 Ultrasonic Sensor (Water Level), YF-S201 Water Flow Sensor Detector, LM35 Temperature Sensor

Communication: SIM800L GSM Modem (SMS & GPRS)

User Interface: 16x2 I2C LCD Module, 10mm Diffused LEDs (Red, Yellow, Green), HDC 3-24V High Decibel Active Buzzer

Power & Connectivity: 5V DC Power Supply, 5V Water Pump, Breadboard, Jumper Wires, USB-A to B Cable, DC Female Jack

Software & Platforms:

Firmware Development: Arduino Integrated Development Environment (IDE)

Cloud & Mobile Dashboard: Blynk Application (for real-time data visualization, alerts, and remote monitoring)

How It Works
The system operates by continuously sensing the water level using the ultrasonic sensor and water flow using the flow sensor. This data is processed by the Arduino Uno, which then updates the local LCD and LED indicators. Based on predefined thresholds, the system triggers SMS alerts via the GSM module and uploads all relevant data to the ThingSpeak cloud platform. This allows users to monitor their water tanks remotely and receive critical notifications, ensuring proactive management of their water resources.

Benefits
Enhanced Water Conservation: Prevents overflows and promotes mindful usage.

Increased Convenience: Eliminates the need for manual water level checks.

Proactive Problem Solving: Timely alerts enable quick response to shortages or supply issues.

Data-Driven Decisions: Historical data supports better water planning and resource allocation.

Cost-Effective & Scalable: Built with readily available and affordable components, suitable for wide deployment.

Future Enhancements
We are committed to continuously improving the SWLMS. Future work includes:

Developing a dedicated mobile application for enhanced user experience and advanced control.

Implementing fully automated pump control based on water levels.

Integrating robust security measures for data and communication.

Adding local data storage capabilities for offline operation.

Enabling multi-user notification support.

Getting Started
To replicate or build upon this project:

Gather Components: Acquire all listed hardware components.

Assemble Hardware: Follow the provided schematic diagram for wiring connections.

Upload Firmware: Use the Arduino IDE to upload the project firmware (Keith2.ino) to your Arduino Uno R3.

Configure Blynk: Set up your Blynk project dashboard and link it to your device.

Detailed setup instructions and code will be available in the project repository.

Contact & Support
For questions, collaborations, or support, please refer to our GitHub repository or contact the project team members.

GROUP-3 | Makerere University | CSC 1304 Practical Skills Development