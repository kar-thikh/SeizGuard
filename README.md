🧠 NeuroSense – AI-Based Seizure Detection & Prediction
NeuroSense is an IoT-powered seizure alert system using ESP32, MAX30102 (Heart Rate & SpO2), and ADXL345 (Motion Sensor) to detect and predict seizures. It sends real-time alerts via WiFi to Supabase, ensuring timely intervention.

🚀 Features
✅ Real-time seizure detection using heart rate and motion data
✅ AI-powered prediction (Planned) – Detects pre-seizure patterns for early alerts
✅ WiFi-based notifications – No SMS required, alerts via mobile app
✅ Fail-safe mechanism – Ensures reliable monitoring
✅ Expandable hardware – Future support for temperature (MCP9808) & GSR sensors

🛠️ Hardware Requirements
ESP32 WROOM-32 – Main controller
MAX30102 – Heart rate & SpO2 sensor
ADXL345 – Motion sensor

📌 Installation & Setup
 Install required Arduino libraries:

Adafruit MAX30102 Library
Adafruit ADXL345 Library
WiFi and Supabase client libraries
 Flash the code to ESP32 using Arduino IDE

 Set up Supabase for real-time alerts

Create a Supabase project
Add a table for storing sensor data
Use API keys to connect ESP32

 Deploy the mobile app (Planned)

Android/iOS app for real-time monitoring
📊 Future Upgrades
🔹 AI-based seizure prediction using ML
🔹 Custom PCB design for a compact wearable
🔹 Integration with emergency response services

💡 Contributing
Want to improve NeuroSense? Feel free to fork, submit issues, or contribute!
