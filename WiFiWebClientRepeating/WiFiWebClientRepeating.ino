/*
  Repeating WiFi Web Client

 This sketch connects to a a web server and makes a request
 using an Arduino WiFi shield.

 Circuit:
 * WiFi shield attached to pins SPI pins and pin 7

 created 23 April 2012
 modified 31 May 2012
 by Tom Igoe
 modified 13 Jan 2014
 by Federico Vanzati

 http://arduino.cc/en/Tutorial/WiFiWebClientRepeating
 This code is in the public domain.
 */

#include <SPI.h>
#include <WiFi101.h>
#include <Dps310.h>

#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int32_t pressure_old = 0;

// Dps310 Opject
Dps310 Dps310PressureSensor = Dps310();

int status = WL_IDLE_STATUS;

// Initialize the WiFi client library
WiFiClient client;

// server address:
char server[] = "http://tf.xampit.de";
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 5L * 1000L; // delay between updates, in milliseconds

void setup() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/
  Dps310PressureSensor.begin(Wire);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWiFiStatus();
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  //while (client.available()) {
  //  char c = client.read();
  //  Serial.write(c);
  //}

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    int32_t temperature;
    int32_t pressure;
    int16_t oversampling = 7;
    int16_t ret;
    WiFi.begin(ssid, pass);
    Serial.println();
  
    ret = Dps310PressureSensor.measureTempOnce(temperature, oversampling);
    if (ret != 0)
    {
      //Something went wrong.
      //Look at the library code for more information about return codes
      Serial.print("FAIL! ret = ");
      Serial.println(ret);
    }
    else
    {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" degrees of Celsius");
    }
    //Pressure measurement behaves like temperature measurement
    //ret = Dps310PressureSensor.measurePressureOnce(pressure);
    ret = Dps310PressureSensor.measurePressureOnce(pressure, oversampling);
    if (ret != 0)
    {
      //Something went wrong.
      //Look at the library code for more information about return codes
      Serial.print("FAIL! ret = ");
      Serial.println(ret);
    }
    else
    {
      Serial.print("Pressure: ");
      Serial.print(pressure);
      Serial.println(" Pascal");
    }
    if(abs(pressure - pressure_old) > 10)
    {
      String diff = String(pressure-pressure_old);
      Serial.print("Diff:" + diff);
      Serial.println("");
      pressure_old = pressure;
      httpRequest(temperature, pressure);
    }
  }
}

// this method makes a HTTP connection to the server:
void httpRequest(int32_t temperature, int32_t pressure) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    String temp = String(temperature);
    String pres = String(pressure);
    
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /elevator.php?pressure=" +pres+ "&temp=" +temp+ " HTTP/1.1");
    client.println("Host: tf.xampit.de");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println("");

    // note the time that the connection was made:
    lastConnectionTime = millis();

    client.stop();
    WiFi.end();
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    pressure_old = 0;
  }
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
