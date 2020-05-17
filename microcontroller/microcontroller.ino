#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include "definitions.h"
#include <EEPROM.h>
#include <string.h>


/* 
  TODO: 
  
  anzeige
  filter  
  lte 
*/
  //CONFIG---------------------------------------------------------------------------------------------------------------------------------------------------
    //EEPROM address--------------
      const int eeAddress = 0;                                    //start adress of eeprom last waypoint storage
      const int eeAddressStackCounter = sizeof(int);              //start address of the counter which states the amount of waypoints on stack
      const int eeAddressStack = ((2*sizeof(double))+sieof(int)); //start address of the stack
      const int eeAddressMax = 512;                               //this is the max address of the EEPROM on the ESP8266

    //HTTP------------------------
      //as these are sensible information, they are excluded from the git upload! --> gitignore

    //Sleep
      const int SleepTimeS = 120;                                 //deep sleep time in seconds
      const bool wifi_enable = true;                              //states if wifi is enabled or disabled after sleep: enable = true, disable = false



    //GPS------------------------------
      static const int RXPin = 5, TXPin = 4;                      //software serial pins for gps communication
      static const uint32_t GPSBaud = 9600;                       //Baud rate for gps communication
      int gps_failcounter = 0;                                    //Counter for failed GPS tries

      TinyGPSPlus gps;                                            //The TinyGPS++ object
      SoftwareSerial ss(RXPin, TXPin);                            //defining the serial connection to the GPS device

    //Cellular------------------------
      #define DEBUG 1                                             //set to debug mode
      #define SIM800_TX_PIN D1                                    // TX pin of SIM800L               
      #define SIM800_RX_PIN D2                                    // RX pin of SIM800L

      SoftwareSerial serialSIM800(SIM800_TX_PIN,SIM800_RX_PIN);   //Software Serial for sim800



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
    
  
        //GPS----------------------------------
        Serial.println("Valid GPS position");
        flat = gps.location.lat();
        flon = gps.location.lng();


          if(filter_gps(flat, flon))
            {
            EEPROM.begin(512);
            double lat_old = (double) flat;                                     //update the old waypoint
            double lng_old = (double) flon;
            EEPROM.put(eeAddress, lat_old);                                     //write old waypoint to EEPROM
            EEPROM.put((eeAddress + sizeof(double)),lng_old);

  
            //Sending procedure--------------------
  
            if(wifi_send(flat, flon, host, httpPort))
              { 
              Serial.println("send succesful");
              send_succesful = true;

                //routine for sending the queue
                
                EEPROM.begin(512); //Initialize EEPROM
                int StackCounter = 0;
                EEPROM.get(eeAddressStackCounter,StackCounter);    //read EEPROM eeAddressStackCounter if there are any waypoints on stack
                while(StackCounter > 0)                           //jump in while loop if there any waypoints on stack
                  {
                  EEPROM.begin(512);                              //Initialize EEPROM
                  double lat_tmp;
                  double lng_tmp;
                  EEPROM.get((eeAddress + ((eeAddressStackCounter-1)*sizeof(double))), lat_tmp);                        //read last waypoint from stack
                  EEPROM.get((eeAddressStack + sizeof(double) + ((eeAddressStackCounter-1)*sizeof(double)) ),lng_tmp);

                    if(wifi_send(lat_tmp, lng_tmp, host, httpPort))
                      { 
                      Serial.print("Send last waypoint from stack, reducing stack counter to:");
                      StackCounter--;
                      Serial.println(StackCounter);
                      }

                    else
                      {
                      println("sending waypoints from stack failed, trying it next time");
                      break;                                                                      //jumping out of while loop because there is a failed try
                      }
                  }
                EEPROM.put(eeAddressStackCounter,StackCounter);                                  //write updated StackCounter to EEPROM
              }

      
            else 
              {
              Serial.println("error in sending procedure");
              send_succesful = true;                                                          //This should be false, but there is no answer routine from server yet!!!!!!!!!!!!!
              
              
              //routine for saving failed sending points
              Serial.print("Due to the error in sending procedure this waypoint is saved to the EEPROM of the ESP8266");
              EEPROM.begin(512);
              int StackCounter = 0;
              EEPROM.get(eeAddressStackCounter,StackCounter);

              if(StackCounter<=(eeAddressMax-(2*sizeof(double))))                                                                          //check if overflow
                {
                  EEPROM.put((eeAddressStack + ((eeAddressStackCounter-1)*sizeof(double)) ), lat_old);                                     //write waypoint to EEPROM to send it later if internet connection is available
                  EEPROM.put((eeAddressStack + sizeof(double) + ((eeAddressStackCounter-1)*sizeof(double)) ),lng_old);
                  Stackcounter++;
                }

              else
                {
                Serial.println("Prevented Overflow in EEPROM, so no more storage is available!!!!");  
                }
              EEPROM.put(eeAddressStackCounter, StackCounter);         

              }
     
            }

          else
            {
            Serial.println("distance between waypoints less than 50m -> no sending procedure");
            }

            
      }

      else 
      {

      if(gps_failcounter>5)                                                         //If the GPS failcounter is over 5 the device goes to sleep and tries again after sleep
        {
        Serial.println("Going to sleep, and try again later to get GPS Connection")
        Sleep(SleepTimeS, wifi_enable); 
        }

      gps_failcounter ++;                                                           //count fail counter up
      Serial.println("waiting for GPS connection");
      Serial.print("GPS Failcounter is right now:");
      Serial.println(gps_failcounter);
      delay(1000);                                                                  //wait 1 second for next try if GPS is available
      }


      }
  
    }
    if(send_succesful == true)
    {
     Serial.print("go to sleep for:");
     Serial.println(SleepTimeS);
     Sleep(SleepTimeS, wifi_enable); 
    }
    


}
