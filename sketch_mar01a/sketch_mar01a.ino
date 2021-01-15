/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe


This is an example sketch offered by the Arduino IDE. You will
have to add your network information to gather internet access.

Luca Hernández Acosta ©
University of Göttingen
 */


#include <SPI.h>
#include <WiFi101.h>
//#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "MG2019LAB";        // your network SSID (name)
char pass[] = "passwordd";     // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "192.168.4.5";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;


   //@@@@@  @@@@@@@  @@@@@@ Add temp LED
   //@@@@@  @@@@@@@  @@@@@@ add temp abbility

int yellow_pin = 0;
int red_pin = 1;
int blue_pin = 2;

// Buffer to store incoming commands from serial port
String inData;

//Light
String sss;
bool dlight = true;
// End Light
// Temperature
String ssss;
bool dtemp = true;
//End Temperature
//Humidity
String sssss;
bool dhumid = true;
//End Humidity


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only

   //@@@@@  @@@@@@@  @@@@@@ add temp abbility
    pinMode(yellow_pin, OUTPUT); //configures pin as OUTPUT
    pinMode(red_pin, OUTPUT); //configures pin as OUTPUT
    pinMode(blue_pin, OUTPUT); //configures pin as OUTPUT
//-=-=
    
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
}

void loop() {
      dlight = dtemp = dhumid = true;
while (client.available() > 0)
    {
      char recieved = client.read();
        inData += recieved; 
        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            Serial.print(inData);
            if(inData.indexOf("HUMID:ON")>0)   
            {         
             digitalWrite(blue_pin, HIGH);
             dhumid = false;
            }
             else if (inData.indexOf("HUMID:OFF")>0 && dhumid == true)
             digitalWrite(blue_pin, LOW);
             if(inData.indexOf("FAN:ON")>0) 
             {           
             digitalWrite(red_pin, HIGH);
              dtemp = false;
            }
             else if (inData.indexOf("FAN:OFF")>0 && dtemp == true)
             digitalWrite(red_pin, LOW);
             if(inData.indexOf("LED:ON")>0)
             {            
             digitalWrite(yellow_pin, HIGH); 
              dlight = false;
            }
             else if (inData.indexOf("LED:OFF")>0 && dlight == true)
             digitalWrite(yellow_pin, LOW);          
            inData = ""; // Clear recieved buffer
            
        }
        // if the server's disconnected, stop the client:
        if (!client.connected()) 
        {
          Serial.println();
          Serial.println("disconnecting from server.");
          client.stop();
          delay(20000);
          setup();          
          // do nothing forevermore:
          //while (true);
        }
      }// ^^^^^^^^^^^^^ End rec 
         

  }










void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
