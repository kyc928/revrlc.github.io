#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "your-SSID"; //wifi network name
const char* password = "your-PASSWORD";  //wifi network password (leave blank if no password)

Servo myServo;
int servoPin = D1;  // GPIO Pin for Servo
int angleOn = 90;   // Angle to turn switch ON  (readjust as needed)
int angleOff = 0;   // Angle to turn switch OFF (readjust as needed)

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  
  Serial.println("Connected to Wi-Fi");
  
  server.begin();
  
  myServo.attach(servoPin);
  myServo.write(angleOff);  // Initialize with switch OFF
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Check if request is for turning the lights ON or OFF
    if (request.indexOf("/lights/on") != -1) {
      myServo.write(angleOn);  // Turn switch ON
    } 
    if (request.indexOf("/lights/off") != -1) {
      myServo.write(angleOff);  // Turn switch OFF
    }

    // HTTP response with updated HTML and CSS
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<!DOCTYPE html>");
    client.println("<html lang=\"en\">");
    client.println("<head>");
    client.println("  <meta charset=\"UTF-8\">");
    client.println("  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    client.println("  <title>Light Control</title>");
    client.println("  <style>");
    client.println("    * { margin: 0; padding: 0; box-sizing: border-box; }");
    client.println("    body { display: flex; justify-content: center; align-items: center;");
    client.println("           height: 100vh; background-color: #f4f4f4; font-family: Arial, sans-serif; }");
    client.println("    .container { text-align: center; padding: 20px; border-radius: 10px;");
    client.println("                 background-color: #ffffff; box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.1); }");
    client.println("    h1 { margin-bottom: 20px; font-size: 2rem; color: #333; }");
    client.println("    button { padding: 15px 30px; margin: 10px; font-size: 1.2rem;");
    client.println("             border: none; border-radius: 5px; cursor: pointer; color: white;");
    client.println("             background-color: #007BFF; transition: background-color 0.3s; }");
    client.println("    button:hover { background-color: #0056b3; }");
    client.println("    @media (max-width: 600px) { button { width: 100%; margin: 10px 0; } }");
    client.println("  </style>");
    client.println("</head>");
    client.println("<body>");
    client.println("  <div class=\"container\">");
    client.println("    <h1>Control Your Lights</h1>");
    client.println("    <button onclick=\"fetch('/lights/on')\">Turn ON</button>");
    client.println("    <button onclick=\"fetch('/lights/off')\">Turn OFF</button>");
    client.println("  </div>");
    client.println("</body>");
    client.println("</html>");
    client.println();
  }
}