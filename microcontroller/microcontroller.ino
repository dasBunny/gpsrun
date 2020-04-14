#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include "definitions.h"


/* 
  TODO: 
  
  anzeige
  filter  
  lte 
*/
  //HTTP------------------------


  //Sleep
  const int SleepTimeS = 120;
  const bool wifi_enable = true; //enable = true, disable = false



  //GPS------------------------------
  static const int RXPin = 5, TXPin = 4;
  static const uint32_t GPSBaud = 9600;
  float lat_old, lng_old;

  TinyGPSPlus gps; // The TinyGPS++ object
  SoftwareSerial ss(RXPin, TXPin); // serial connection to the GPS device





  //Function prototypes-------------//function prototype because the Arduino IDE does not make it automatic

  bool wifi_send(float flat, float flon, const char* host, const int httpPort); 
  void Sleep(const int SleepTimeS, const bool wifi_enable); 
  bool filter_gps(float flat, float flon, float lat_old, float lng_old);
  


  
  
  
void setup()
{
  delay(10000); //only for Debugging!
  Serial.begin(115200);
  
  Serial.print("Testing serial connection to computer");
  
  ss.begin(GPSBaud); //Software Serial to GPS-Device

    //HTTP--------------------
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
      }
      
    Serial.println("");
    Serial.println(F("WiFi connected"));
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
}

    
//--------------------LOOP------------------------------------------


  


void loop()
{
  
  float flat, flon;
  bool send_succesful = false;

  while (ss.available() > 0)
    {
      //Serial.println("SoftwareSerial connection successful, starting GPS read");
      if(gps.encode(ss.read()))
      {
       
      if (gps.location.isValid())
        {
    
  
        //GPS--------------
        Serial.println("Valid GPS position");
        flat = gps.location.lat();
        flon = gps.location.lng();


        if(filter_gps(flat, flon, lat_old, lng_old))
          {
          
          lat_old = flat; //update the old waypoint
          lng_old = flon;

        
  
  
            //HTTP----------------
  
            if(wifi_send(flat, flon, host, httpPort))
            { 
             Serial.println("send succesful");
             send_succesful = true;
            }
      
            else 
              {
              Serial.println("error in sending procedure");
              send_succesful = true; //This should be false, but there is no answer routine from server yet
              }
     

            }

            else
         {
          Serial.println("distance between waypoints less than 50m -> no sending procedure");
         }

         
            
         }

         else 
         {
         Serial.println("waiting for GPS connection");
         delay(1000); //wait 1 second for next try if GPS is available
         }


      }
  
    }
    if(send_succesful == true)
    {
     Serial.print("goto sleep for:");
     Serial.println(SleepTimeS);
     Sleep(SleepTimeS, wifi_enable); 
    }
    


}
