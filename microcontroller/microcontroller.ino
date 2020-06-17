#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include "definitions.h"
#include <EEPROM.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <Ticker.h>


  //CONFIG---------------------------------------------------------------------------------------------------------------------------------------------------
    //EEPROM address--------------
      const int eeAddress = 0;                                    //start adress of eeprom last waypoint storage
      const int eeAddressStackCounter = sizeof(int);              //start address of the counter which states the amount of waypoints on stack
      const int eeAddressStack = ((2*sizeof(double))+sizeof(int)); //start address of the stack
      const int eeAddressMax = 512;                               //this is the max address of the EEPROM on the ESP8266

    //HTTP------------------------
      //as these are sensible information, they are excluded from the git upload! --> gitignore

    //Sleep
      const int SleepTimeS = 120;                                 //deep sleep time in seconds
      const bool wifi_enable = false;                              //states if wifi is enabled or disabled after sleep: enable = true, disable = false



    //GPS------------------------------
    //  static const int RXPin = 5, TXPin = 4;                      //software serial pins for gps communication
    //  static const uint32_t GPSBaud = 9600;                       //Baud rate for gps communication
      int gps_failcounter = 0;                                    //Counter for failed GPS tries
    //
      TinyGPSPlus gps;                                            //The TinyGPS++ object
    //  SoftwareSerial ss(RXPin, TXPin);                            //defining the serial connection to the GPS device

  
      
    //Modem-Interface----------------------
      #define DEBUG 1                                             //set to debug mode
      static const int RXPin = 5, TXPin = 4;                      //Software serial pins
      static const uint32_t softBaud = 9600;                      //Baud rate for communication with module

      SoftwareSerial softSerial(RXPin, TXPin);                    //Software Serial for SimCom Modem

      //https://arduino-projekte.info/wemos-d1-mini/
      //Port 2 10k pull-up Port 15 10 k pull-down
      #define PWR_PORT 15

      #define SIM_PWR_ON_OFF
      //#define SIM_DEEP_SLEEP
      #define GPS
      #define HTTP
      //#define HTTP_UPLOAD
      //#define HTTP_UPLOAD_TEST
      //#define HTTP_TEST


      //LED---------------------------------
      #define LED 2
      #define LED_R 3
      #define LED_G 4
      #define LED_B 5
      #define BAT_VOLTAGE_PIN A0
      #define LONG_INTERVAL 40000000
      #define SHORT_INTERVAL 20000000
      #define FULL_INTERVAL 80000000
      bool timerInUse = false;
      int counter = 0;
      int batStatus = 19;
      byte state = 0;

     



  //Function prototypes-------------

  bool wifi_send(float flat, float flon, const char* host, const int httpPort); 
  void Sleep(const int SleepTimeS, const bool wifi_enable); 
 bool filter_gps(float flat, float flon);
 
  void printSerial(const char *format, ...);
  int powerUpSimModule();
  int powerDownSimModule();
  int wakeUpSimModule();
  int deepSleepSimModule();
  int softPowerDownSimModule();
  int initSimModule();
  int getSimInfo();
  int setupSim7080NBIot1nce();
  int testSim(int counts);
  int checkSimConnectionStatus();
  int checkSimCsq();
  int activateAppNetwork(bool activate);
  int getGpsPos(float timeout);
  int httpGetSim(const double lat, const double lon);
  int parseAtResponse(const char **param);
  int expect_at(const char *command, const char *expect, float timeout, int tries);
  int send_at(const char *command, const char *expect, float timeout);
  int read_ser(const char *expect, float timeout);
  double latConvert();
  double lonConvert();

  void beginPowerLED();
  void ICACHE_RAM_ATTR bat20();
  void ICACHE_RAM_ATTR bat40();
  void ICACHE_RAM_ATTR bat60();
  void ICACHE_RAM_ATTR bat80();
  void ICACHE_RAM_ATTR bat100();

  
    
  
void setup()
{
  pinMode(PWR_PORT,OUTPUT);
  digitalWrite(PWR_PORT, LOW);  //Keep initial peak to high as short as possible -> for PWR-LTE
  #ifdef DEBUG 
  delay(10000); //only for Debugging!
  #endif
   

  Serial.begin(115200);      //open debug serial connection
  
  printSerial("\n**** BOOT ************************************************************************************************************");

    //LED----------
    pinMode(LED_R,OUTPUT);
    pinMode(LED_G,OUTPUT);
    pinMode(LED_B,OUTPUT);
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    beginPowerLED();
  

    //LTE--------------------
    int retval = 0;                           //LTE return value
    pinMode(4,OUTPUT);                        //Initialize Pin 4 as OUTPUT
    softSerial.begin(softBaud);               //open SoftwareSerial connection to Simcom Board
    delay(20);  
    printSerial("-- Power Up SimCom Module");
	  #ifdef SIM_PWR_ON_OFF
  powerUpSimModule();
#endif
#ifdef SIM_DEEP_SLEEP
  wakeUpSimModule();
#endif   
    delay(1000);                              //wait 1s that the LTE module is ready
    printSerial("-- Init SimCom Module");
	  retval = initSimModule();
	  if (retval)
	  {
		  printSerial("ERROR initSimModule %d", retval);
		  //exit -1;
      Serial.println("Going to sleep, and try again later to Initialize LTE module");
      Sleep(SleepTimeS, wifi_enable); 
	  }

    send_at("AT+CPSMS=0", "OK", 1.0);
  	printSerial("-- Get SimCom Module Info");  //Nur zum Testen interessant
	  getSimInfo(); //dto
  	printSerial("-- Init Sim7080 NB-Iot");
	  setupSim7080NBIot1nce();
    Serial.println("activate AppNetwork");
  	activateAppNetwork(true);
   Serial.println("check connection status");
   checkSimConnectionStatus();
   //activateAppNetwork(false);
}

    
//--------------------LOOP------------------------------------------


void loop()
{
  Serial.println("---------------loop begins!----------------");
  double lat;
  double lon;
  bool send_succesful = false;
  
    
      Serial.println("SoftwareSerial connection successful, starting GPS read");
      if(getGpsPos(60.0))
      {
      lat = latConvert();
      lon = lonConvert();


          if(filter_gps(lat, lon))
            {
            Serial.println("Waypoints are at least 50m distant");
            EEPROM.begin(512);
            double lat_old = lat;                                     //update the old waypoint
            double lng_old = lon;
            EEPROM.put(eeAddress, lat_old);                                     //write old waypoint to EEPROM
            EEPROM.put((eeAddress + sizeof(double)),lng_old);
            EEPROM.end();

  
            //Sending procedure--------------------

            if(httpGetSim(lat, lon)==0)
              { 
              Serial.println("send succesful");
              send_succesful = true;
            int StackCounter = 0;
                //routine for sending the queue
                
                EEPROM.begin(512); //Initialize EEPROM

                EEPROM.get(eeAddressStackCounter,StackCounter);    //read EEPROM eeAddressStackCounter if there are any waypoints on stack
                Serial.println("StackCounter is right now:");
                Serial.println(StackCounter);
                while(StackCounter > 0)                           //jump in while loop if there any waypoints on stack
                  {
                  double lat_tmp;
                  double lng_tmp;
                  EEPROM.get((eeAddress + ((eeAddressStackCounter-1)*sizeof(double))), lat_tmp);                        //read last waypoint from stack
                  Serial.println("read waypoint from stack: lat=");
                  Serial.println(lat_tmp);
                  EEPROM.get((eeAddressStack + sizeof(double) + ((eeAddressStackCounter-1)*sizeof(double)) ),lng_tmp);
                  Serial.println("read waypoint from stack: lng=");
                  Serial.println(lng_tmp);

                    if(httpGetSim(lat_tmp, lng_tmp))
                      { 
                      Serial.print("Send latest waypoint from stack, reducing stack counter to:");
                      StackCounter--;
                      Serial.println(StackCounter);
                      }

                    else
                      {
                      Serial.println("sending waypoints from stack failed, trying it next time");
                      break;                                                                      //jumping out of while loop because there is a failed try
                      }
                  }
                EEPROM.put(eeAddressStackCounter,StackCounter);                                  //write updated StackCounter to EEPROM
                EEPROM.end();
              }         
            else 
              {
              Serial.println("error in sending procedure");
              send_succesful = false;
              
              
              //routine for saving failed sending points
              Serial.print("Due to the error in sending procedure this waypoint is saved to the EEPROM of the ESP8266");
              EEPROM.begin(512);
              int StackCounter = 0;
              EEPROM.get(eeAddressStackCounter,StackCounter);

              if(StackCounter<=(eeAddressMax-(2*sizeof(double))))                                                                          //check if overflow
                {
                  EEPROM.put((eeAddressStack + ((eeAddressStackCounter-1)*sizeof(double)) ), lat_old);                                     //write waypoint to EEPROM to send it later if internet connection is available
                  EEPROM.put((eeAddressStack + sizeof(double) + ((eeAddressStackCounter-1)*sizeof(double)) ),lng_old);
                  StackCounter++;
                }

              else
                {
                Serial.println("Prevented Overflow in EEPROM, so no more storage is available!!!!");  
                }
              EEPROM.put(eeAddressStackCounter, StackCounter);
              EEPROM.end();         

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
        Serial.println("Going to sleep, and try again later to get GPS Connection");
        Sleep(SleepTimeS, wifi_enable); 
        }

      gps_failcounter ++;                                                           //count fail counter up
      Serial.println("waiting for GPS connection");
      Serial.print("GPS Failcounter is right now:");
      Serial.println(gps_failcounter);
      delay(1000);                                                                  //wait 1 second for next try if GPS is available
      }


      
  
    
    //if(send_succesful == true)
    //{
     Serial.print("go to sleep for:");
     Serial.println(SleepTimeS);
     Sleep(SleepTimeS, wifi_enable); 
    //}
    


}
