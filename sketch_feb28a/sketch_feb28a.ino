/**

This sketch will set up your MKR1000 Board as a webserver.


Luca Hernández Acosta ©
University of Göttingen

**/
#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>

 /*
 * This example is modified from the original file 
 * https://github.com/arduino-libraries/WiFi101/blob/master/examples/SimpleWebServerWiFi/SimpleWebServerWiFi.ino
 */
#include <SPI.h>
#include <WiFi101.h>
 
char ssid[] = "MG2019LAB";      //  your network SSID (name)
char pass[] = "passwordd";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int ledpin = 0;
bool val = true;

 
int status = WL_IDLE_STATUS;
WiFiServer server(80);

//Arduino Light Measurement 
float val_light;
bool LED_light= false;
String sss="";
// End of Arduino Light Measurement 
//Arduino temerature Measurement 
float val_temp;
bool fan_temp= false;
String ssss="";
// End of Arduino temperature Measurement 
//Arduino humidity Measurement 
#include "DHT.h"
#define DHTPIN A2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float val_humid;
bool motor_humid= false;
String sssss="";
float ttt;
// End of Arduino humidity Measurement 

void setup() {
  Serial.begin(9600);      // initialize serial communication
  Serial.print("Start Serial ");
  pinMode(ledpin, OUTPUT);      // set the LED pin mode
  // Check for the presence of the shield
  Serial.print("WiFi101 shield: ");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("NOT PRESENT");
    return; // don't continue
  }
  Serial.println("DETECTED");
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    digitalWrite(ledpin, LOW);
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);
    digitalWrite(ledpin, HIGH);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
  digitalWrite(ledpin, HIGH);
}
void loop() { 
  
  WiFiClient client = server.available();   // listen for incoming clients
 
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
 
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();


            //light
             val_light = analogRead(A0); // read analog input pin 0
             if(val_light<600.00) //Maximum light is 1023.00
              {
                LED_light=true; 
                Serial.print("true<br>");    
                sss = "LED:ON";                          
              }
              else
              {
                LED_light=false;     
                Serial.print("false<br>"); 
                sss = "LED:OFF";
                
              }
            //temperature
              val_temp = (float)analogRead(A1) * 5.0 / 1024.0; //analogRead will read the voltage a the given pin 
              val_temp = val_temp - 0.5;
              val_temp = val_temp / 0.01;
              val_temp = val_temp - 41.02476117;
             if(val_temp>28.00) //normal temperature is 27.00
              {
                fan_temp =true; 
                Serial.print("true<br>"); 
                ssss= "FAN:ON";                             
              }
              else
              {
                fan_temp=false;     
                Serial.print("false<br>"); 
                ssss="FAN:OFF";
              }
                 
              
              //humidity
                // Wait a few seconds between measurements.
                delay(2000);              
                // Reading temperature or humidity takes about 250 milliseconds!
                // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
                val_humid = dht.readHumidity();
                // Read temperature as Celsius (the default)
                ttt = dht.readTemperature();
                // Read temperature as Fahrenheit (isFahrenheit = true)
                float f = dht.readTemperature(true);              
                // Check if any reads failed and exit early (to try again).
                if (isnan(val_humid) || isnan(ttt) || isnan(f)) {
                  Serial.println("Failed to read from DHT sensor!");
                  return;
                }
                else
                {
                  if(val_humid<21)
                  {
                    sssss="HUMID:ON";
                  }
                  else
                  {
                    sssss="HUMID:OFF";
                  }
                }
              
                /* Compute heat index in Fahrenheit (the default)
                float hif = dht.computeHeatIndex(f, h);
                */
                // Compute heat index in Celsius (isFahreheit = false)
                float hic = dht.computeHeatIndex(ttt, val_humid, false);
              //----- End of humidity
   
            // the content of the HTTP response follows the header:
            client.print("<html>");
            //client.print("Click <a href=\"/H\">here</a> turn the LED on pin 0 on<br>");
            //client.print("Click <a href=\"/L\">here</a> turn the LED on pin 0 off<br>");
           // client.print("Light value is: ");
            //client.print(val_light);

                       

           // --===== HTML =====--
            client.print( R"(<head>
<style>
table, th, td {border: 1px solid black;padding: 5px;}
table {border-spacing: 15px;}
table#t01 tr:nth-child(even) {background-color: #eee;}
table#t01 tr:nth-child(odd) {background-color: #fff;}
table#t01 th {background-color: black; color: white;}
</style></head><body><h2>Sensors Management Panel</h2>)");
client.print( R"(<p>Date/Time: <span id="datetime"></span></p><script>
var dt = new Date();
document.getElementById("datetime").innerHTML = dt.toLocaleTimeString();
</script>)");
client.print( R"(<p>Border spacing specifies the space between the cells.</p>
<table style="width:100%" id="t01">
<tr><th>Node</th><th>Temperature</th><th>Humidity</th><th>Light</th><th>LED Status</th>
<th>Fan Status</th><th>Humidity Status</th></tr><tr>
<td>Number 1</td><td>)"); client.print(val_temp); client.print( R"(</td>
<td>)"); client.print(val_humid); client.print( R"(</td>
<td>)"); client.println(val_light); client.print( R"(</td>
<td>)"); client.print(sss); client.print( R"(</td>
<td>)"); client.print(ssss); client.print( R"(</td>
<td>)"); client.print(sssss); client.print( R"(</td>    
</tr></table><p>Try to change the border-spacing to 5px.</p></body>)");
client.print("<meta http-equiv=\"refresh\" content=\"3\" >");
client.print("</html>\n");
// --===== /HTML ====--
             
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
 
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(ledpin, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(ledpin, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}
 
void printWifiStatus() {
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
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
