# Smart Water Level Monitoring System - Usage Guide

## Overview
This smart water monitoring system integrates your website with ThingSpeak cloud platform and ESP32 hardware to provide real-time water level monitoring and automatic SMS notifications.

## System Architecture
```
ESP32 Hardware → ThingSpeak Cloud → Website Dashboard → User SMS Alerts
```

## Features Implemented

### 1. **ThingSpeak Integration**
- Real-time data fetching from ThingSpeak API
- Automatic data updates every 30 seconds
- Support for multiple users with different API keys

### 2. **Automatic SMS Scheduling**
- **Daily Reports**: Configurable time for daily water level summaries
- **Weekly Reports**: Choose day and time for weekly analysis
- **Custom Reports**: Set custom intervals in hours

### 3. **Multi-User Support**
- Each user can configure their own ThingSpeak API keys
- Configuration stored in browser's local storage
- No user accounts required

### 4. **Real-time Dashboard**
- Live water level indicators
- Current status displays
- System health monitoring

### 5. **Manual Controls**
- Send immediate SMS alerts
- Test SMS functionality
- Refresh data on demand

## How to Use

### Step 1: ThingSpeak Setup
1. Create account at https://thingspeak.com
2. Create a channel with these fields:
   - Field 1: Distance (cm)
   - Field 2: Water Level (liters)
   - Field 3: Depth Percentage (%)
   - Field 4: Immediate Commands
   - Field 5: Analysis Commands

### Step 2: Website Configuration
1. Open dashboard.html in your browser
2. Scroll to "ThingSpeak Configuration" section
3. Enter your API keys and Channel ID
4. Save configuration

### Step 3: Schedule Configuration
1. Enable desired report types (Daily/Weekly/Custom)
2. Set preferred times and intervals
3. Save schedule configuration

### Step 4: Arduino Code Update
Replace the API key in your ESP32 code:
```cpp
String str="GET https://api.thingspeak.com/update?api_key=YOUR_WRITE_API_KEY&field1=" + String(distance) +"&field2="+String(ltr)+"&field3="+String(depth);
```

### Step 5: Test System
- Use "Send Test SMS" to verify SMS functionality
- Use "Send Manual Alert" for emergency notifications
- Use "Refresh Data" to update dashboard

## SMS Report Examples

### Daily Report
```
SMART H2O DAILY REPORT
Date: Jul 10, 2025
Current: 45L (75%)
Usage Today: 12L
Change: +3L (25% more)
Trend: Normal consumption
Refill Needed: In 4 days
Status: All systems OK
```

### Weekly Report
```
SMART H2O WEEKLY ANALYSIS
Week: Jul 4-10, 2025
Total Usage: 85L (Avg: 12L/day)
Peak Day: Tuesday (18L)
Low Day: Sunday (8L)
Vs Last Week: -5L (6% less)
Efficiency: Good
Trend: Stable usage pattern
```

## Technical Details

### Data Flow
1. **ESP32 → ThingSpeak**: Hardware sends sensor data every 30 seconds
2. **Website → ThingSpeak**: Dashboard fetches data via REST API
3. **Website → ThingSpeak**: Schedule triggers send commands
4. **ThingSpeak → ESP32**: ESP32 reads commands and sends SMS

### Configuration Storage
- All settings stored in browser's localStorage
- No server-side storage required
- Configuration persists between sessions

### GitHub Pages Deployment
This system works perfectly with GitHub Pages:
1. Push code to GitHub repository
2. Enable GitHub Pages in repository settings
3. Access your dashboard at `https://username.github.io/repository-name/dashboard.html`

## Multi-User Scenarios

### Scenario 1: Multiple Tanks
- Each user configures different ThingSpeak channels
- Each gets their own SMS alerts
- All use the same website

### Scenario 2: Multiple Locations
- Different users monitor different locations
- Each has their own API keys and phone numbers
- Shared dashboard interface

### Scenario 3: Family/Organization
- Multiple people monitor the same tank
- Each person can configure their own notification preferences
- Shared data source

## Troubleshooting

### Common Issues
1. **No Data Showing**: Check API keys and Channel ID
2. **SMS Not Working**: Verify ESP32 GSM module and network
3. **Schedule Not Working**: Keep browser tab open
4. **Configuration Lost**: Browser cleared localStorage

### Best Practices
1. **Keep Configuration Backup**: Save your API keys separately
2. **Test Regularly**: Use manual controls to verify system
3. **Monitor Costs**: Be aware of SMS costs from your GSM plan
4. **Update Regularly**: Check for system updates periodically

## Future Enhancements
- Email notifications (if requested)
- Mobile app integration
- Advanced analytics
- Multiple sensor support
- Weather integration

## Support
For questions or issues:
1. Check the setup guide (setup-guide.html)
2. Review browser console for error messages
3. Verify ThingSpeak channel configuration
4. Test ESP32 connectivity

---

**Note**: This system is designed to work entirely client-side for maximum compatibility and ease of deployment. No server infrastructure is required.
