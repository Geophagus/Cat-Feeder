#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo myservo;
const int servo = 14;

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(xx);    // Create a webserver object that listens for HTTP request on port xx

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

void setup(void){
  myservo.attach(servo);
  myservo.write(90);
  delay(2000);
  
  Serial.begin(115200);                       // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

 // wifiMulti.addAP("ssid_1", "password_1");   // add Wi-Fi networks you want to connect to
 // wifiMulti.addAP("ssid_2", "password_2");
 // wifiMulti.addAP("ssid_3", "password_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) {   // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());                 // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());              // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {                 // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);         // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/Feed", HTTP_POST,handleFeedMog);  // Call the 'handlefeedmog' function when a POST request is made to URI "/feed"
  

  server.begin();                               // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                        // Listen for HTTP requests from clients
}

void handleRoot() {                             // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "</form> <form action=\"/Feed\" method=\"POST\"><input type=\"submit\"value=\"FEED ME\"></form>");
}

void handleFeedMog(){
HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin("http://192.168.x.xx:8085/Feed");       //Specify request destination at IP 192.168.x.x
   http.addHeader("Content-Type", "text/plain");      //Specify content-type header
 
   int httpCode = http.POST("Message from ESP8266");   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
  server.sendHeader("Location","/");
  server.send(303);
}
