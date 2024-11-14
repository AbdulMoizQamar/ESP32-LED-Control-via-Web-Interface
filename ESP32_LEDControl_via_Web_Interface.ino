#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "";   // Replace with your Wi-Fi SSID
const char* password = ""; // Replace with your Wi-Fi password

// Create an instance of the WebServer
WebServer server(80);

// Define the onboard LED pin
const int ledPin = 2; // Onboard LED is typically connected to GPIO2 on most ESP32 boards

// HTML page content to control the LED with added CSS for modern design
const char* htmlContent = R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 LED Control</title>
  <style>
    body {
      font-family: 'Arial', sans-serif;
      background-color: #f4f4f4;
      color: #333;
      text-align: center;
      padding: 50px;
    }
    h1 {
      color: #0078d4;
      font-size: 2.5em;
      margin-bottom: 30px;
    }
    .btn {
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 10px;
      cursor: pointer;
      border-radius: 5px;
      transition: background-color 0.3s ease;
    }
    .btn:hover {
      background-color: #45a049;
    }
    .btn-off {
      background-color: #f44336;
    }
    .btn-off:hover {
      background-color: #da190b;
    }
    .container {
      display: flex;
      justify-content: center;
      align-items: center;
      flex-direction: column;
      gap: 20px;
    }
  </style>
</head>
<body>

  <h1>Control ESP32 LED</h1>
  <div class="container">
    <a href="/on"><button class="btn">Turn LED ON</button></a>
    <a href="/off"><button class="btn btn-off">Turn LED OFF</button></a>
  </div>

</body>
</html>
)";

// Handle the root URL and display the HTML page
void handleRoot() {
  server.send(200, "text/html", htmlContent);
}

// Handle the "on" URL and turn the LED on
void handleOn() {
  digitalWrite(ledPin, HIGH); // Turn LED on
  server.sendHeader("Location", "/"); // Redirect back to the main page
  server.send(303);
}

// Handle the "off" URL and turn the LED off
void handleOff() {
  digitalWrite(ledPin, LOW); // Turn LED off
  server.sendHeader("Location", "/"); // Redirect back to the main page
  server.send(303);
}

void setup() {
  // Initialize the LED pin
  pinMode(ledPin, OUTPUT);
  
  // Start the serial communication
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Print the ESP32 IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define the routes for the web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, handleOn);
  server.on("/off", HTTP_GET, handleOff);

  // Start the server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();
}
