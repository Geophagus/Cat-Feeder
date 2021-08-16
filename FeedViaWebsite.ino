/*
Cat Feeder
Moves servo arm to release cat ball

*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
Servo myservo;  // create servo object 



void handleRoot();              // function prototypes for HTTP handlers
void handleFeed();
void handleNotFound();
int pos1 = 0;    // servo position variable
int pos2 = 180;   // servo position 2 variable

// the setup function runs once when you press reset or power the board

void setup(void){
  Serial.begin(57600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  myservo.attach(D1);  // servo to pin x
  myservo.write(pos1);

  wifiMulti.addAP("ssid", "password");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/Feed", HTTP_POST, handleFeed);  // Call the 'handleLED' function when a POST request is made to URI "/LED"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {                         //  Form action = value to post. input type value = display on button.
  server.send(200, "text/html", "<form action=\"/Feed\" method=\"POST\"><input type=\"submit\" value=\"Feed!\"></form>");
}

void handleFeed() {                          // If a POST request is made to URI /Up
  myservo.write(pos2);              // servo to  'pos2' to release ball
  delay(5000);                       // waits 5s 
  myservo.write(pos1);              // tell servo to go to position in variable 'pos1'
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
