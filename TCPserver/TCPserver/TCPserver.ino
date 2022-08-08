/*
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * Create a TCP Server on ESP8266 NodeMCU. 
 * TCP Socket Server Send Receive Demo
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int port = 1709;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "LYLY1";  //Enter your wifi SSID
const char *password = "0902715710";  //Enter your wifi Password

const int PCpwr = 0;
const int PCreset = 2;
char PCmode;
int count=0;
//=======================================================================
//                    Power on setup
//=======================================================================
void setup() 
{
  Serial.begin(115200);
  pinMode(SendKey,INPUT_PULLUP);  //Btn to send data
  Serial.println();
  

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
  pinMode(PCpwr, OUTPUT);
  pinMode(PCreset, OUTPUT);
  // Set outputs to LOW
  digitalWrite(PCpwr, LOW);
  digitalWrite(PCreset, LOW);
}
//=======================================================================
//                    Loop
//=======================================================================

void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      while(client.available()>0){
        // read data from the connected client
        PCmode = client.read();
        if (PCmode == '0')
        {
          digitalWrite(PCpwr, HIGH); 
          client.write("PC is turning OFF\n");
          delay(5000); 
          digitalWrite(PCpwr, LOW);
          client.write("PC offline\n");
        }
        if (PCmode == '1')
        {
          digitalWrite(PCpwr, HIGH);
          client.write("PC is turning ON\n");
          delay(500);
          digitalWrite(PCpwr, LOW);
          client.write("PC online\n"); 
        }
        if (PCmode == '2')
        {
          digitalWrite(PCreset, HIGH);
          client.write("PC reset push\n");
          delay(500);
          digitalWrite(PCreset, LOW);
          client.write("PC reset release\n");  
        }

      }
      //Send Data to connected client
//      while(Serial.available()>0)
//      {
//        client.write(Serial.read());
//      }
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}
//=======================================================================
