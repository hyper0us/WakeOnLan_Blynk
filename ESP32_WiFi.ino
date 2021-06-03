#define BLYNK_PRINT Serial

// Including required libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiUDP.h>
#include <WakeOnLan.h>

// Inlcuding file with our data
#include "secret.h"

//Setting up a broadcast address for our magic packet 
//    Lib requires it
IPAddress computer_ip(255,255,255,255); 


WiFiUDP UDP;
bool state = false, last_state = false;

// Virtual defining a function
void sendWOL();

BLYNK_WRITE(1) 
{ 
    // Doing action on button state change 
    //    im not sure if it's necessary, just made it like digital buttons
    state = (param.asInt())? true : false;
    if(state&&!last_state){
      // Blinking diode 
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);
      digitalWrite(LED_BUILTIN, HIGH);

      // Sending magic packet
      WakeOnLan::sendWOL(computer_ip, UDP, mac, sizeof mac);
    }
    last_state = state;
}

void setup()
{
  // Serial to analog debugging
  Serial.begin(9600);

  // UDP client for sending magic packet
  UDP.begin(9);

  // Connecting with Blynk app
  Blynk.begin(auth, ssid, pass);

  // Way to digital debugging
  //    it blinks when you try to wake up pc
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  Blynk.run();
}
