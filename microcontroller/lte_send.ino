

/*
 * MODULE to communicate with SIM7080G derive GPS data and send it to server
 * - Handle AT command interface to SIM7080G
 * - Derive GPS coordinates
 * - Send Data to Server as HTTP GET header
 * 
 */
#include <SoftwareSerial.h>
//#include "TinyGPS++.h"
#include <ESP8266WiFi.h>
#include "definitions.h"
#include <EEPROM.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#define DEBUG 1
#define GPS
#define HTTP
#define HTTP_UPLOAD
//#define HTTP_UPLOAD_TEST


/**
 * Don't forget to:
 * 1. Set correct GPRS APN
 * 2. Set correct GPRS username
 * 3. Set correct GPRS password
 * 4. Set SIM800L TX/RX pins
 * 5. Set your server URL
 */
 
//defines-----------------------------------------------------------------------------------------
#define MAX_INIT_TRIES 20			//Max tries to wake up modem
#define SIZE_AT 256
#define SIZE_AT_RESPONSE 1024
#define SIZE_OUT SIZE_AT_RESPONSE + 128
#define NVAL 20
#define SVAL 20

char at[SIZE_AT] = "";
char atResponse[SIZE_AT_RESPONSE] = "";
char val[NVAL][SVAL] = {"",};
char slat[16] = "";
char slon[16] = "";
char txt[SIZE_OUT] = "";  

int debugLevel = 1;                             //2=verbose mode
const char *gpsparam[20] = {"Status","Fix","UTC TS","Lat","Lon","Alt","SoG","CoG","Mode","R1","HDOP","PDOP","VDOP","R2","NumS","R3","HPA","VPA","##","",};
const char *gpspar[13] = {"Mode","UTC TS","Lat","Lon","Accuracy","Alt","AltSL","SoG","CoG","TS","Flag","##", "",};
const char *cpsi[16] = {"Sys Mode","Op Mode","MCC-MNC","LAC","SCID","PCID","Band","earf","dlbw","ulbw","RSRQ","RSRP","RSSI","RSSNR","##", "",}; 

void printSerial(const char *format, ...)                 //Serial.println with added formation options
{
	#ifdef DEBUG
	char varOut[SIZE_OUT] = "";
	if (debugLevel > 1) { Serial.print("  ## serialPrint: "); Serial.println(format);}

	va_list varptr;
	va_start(varptr,format);
	vsprintf(varOut, format, varptr);
	
	Serial.println(varOut);
	#endif
}

/*
void setup() {
	int retval = 0;
	  Serial.begin(115200);
	  while(!Serial);
	softSerial.begin(softBaud);
  
	printSerial("\n**** BOOT ************************************************************************************************************");
	delay(3000); //## TB ## necessary?
  	printSerial("-- Power Up SimCom Module");
	powerUpSimModule();
  	delay(3000);
  	printSerial("-- Init SimCom Module");
	retval = initSimModule();
	if (retval)
	{
		printSerial("ERROR initSimModule %d", retval);
		//exit -1;  //sleep?--?????
	}
  	printSerial("-- Get SimCom Module Info");  //Nur zum Testen interessant
	getSimInfo(); //dto
  	printSerial("-- Init Sim7080 NB-Iot");
	setupSim7080NBIot1nce();
  	activateAppNetwork(true);
   checkSimConnectionStatus();
   //activateAppNetwork(false);
   delay(100);

	//powerDownSimModule();
	
}
*/
/*
void loop() 
{
	int retaval = 0;
   static int counter = 0;
  	printSerial("** Loop ** %d", counter+1);
   printSerial("-- Check CSQ");
  	checkSimCsq();
   printSerial("-- Check Connection");
  	checkSimConnectionStatus();
	getCellInfo();

#ifdef GPS
	if (getGpsPos(60.0) == 1)  //GPS Position abfragen mit maximaler Wartezeit bis zum Fix. Eventuell auf 3 Min erhöhen
  	{
		//testSim(10);
#ifdef HTTP
		//delay(60000);

    	httpGetSim(slat, slon);
 
    	//httpGetSimExample();
#endif  
  	}
#endif

   if (++counter >= 3)
   {

#ifdef HTTP_UPLOAD_TEST
      printSerial("#### HTTP ####");
      httpGetSim("47.684043", "9.819951");
#endif

      counter = 0;
   }

  	delay(10000);
}
*/

double latConvert()
{
  return(atof(slat));
}

double lonConvert()
{
  return(atof(slon));
}


int powerUpSimModule()  //power down sequence
{
	//power up sequence
  digitalWrite(4, HIGH);
  delay(100);
  digitalWrite(4, LOW);
  delay(1000);
  digitalWrite(4, HIGH);
	return 0;
}

int powerDownSimModule()  //power down sequence
{
  digitalWrite(4, HIGH);
  delay(100);
  digitalWrite(4, LOW);
  delay(1000);	

	return 0;
}


int initSimModule() 
{
	int count = 0;
	//Init Modem UART Communication REMARK: The autobaud detection of the module need single AT to sync.
	while (send_at("AT", "OK", 0.1) != 1)
	{
		delay(10);
		if (++count >= MAX_INIT_TRIES)
		{
			return -1; //Reach max tries to wake up
		}
	}
	//Set Modem Baud Rate to softBaud
	sprintf(at,"AT+IPR=%d",softBaud);
	send_at(at,"OK",0.1);
	//Set Modem Verbose Error Mode
	send_at("AT+CMEE=2","OK",0.1);
	//Check if SIM is ready
	if (send_at("AT+CPIN?","+CPIN: READY",0.5) != 1)
	{
		return -2; //SIM is not ready or SIM PIN is required
	}	
	return 0;	//Modem is ready
}

//Get some helpful modem and sim info
int getSimInfo()
{
	printSerial("Get Version Info");
	send_at("AT+GSV", "OK", 0.5);
	printSerial("Get IMEI ID");
	send_at("AT+GSN", "OK", 0.1);
	printSerial("Get SIM Card ID");
	send_at("AT+CCID", "OK", 0.1);
	return 0;
}	

//Setup Sim7080 NB-Iot for 1nce
int setupSim7080NBIot1nce()
{
	printSerial("Set LTE Mode");
	send_at("AT+CNMP=38", "OK", 0.1);
	printSerial("Set NB-IoT Mode");
	send_at("AT+CMNB=2", "OK", 0.1);
	printSerial("Set NB-IoT Band 8 for 1nce");
	send_at("AT+CBANDCFG=\"NB-IOT\",8", "OK", 0.1);
	printSerial("Set PDP Context for 1nce");
	send_at("AT+CGDCONT=1,\"IP\",\"iot.1nce.net\",\"0.0.0.0\",0,0,0", "OK", 0.5);
	printSerial("Set APN for 1nce with PAP auth");
	send_at("AT+CNCFG=0,1,\"iot.1nce.net\",\"\",\"\",1", "OK", 0.5);
	printSerial("Operator selection 1nce - Telekom");
	send_at("AT+COPS=1,2,\"26201\",9", "OK",120.0);
	printSerial("Network registration");
	send_at("AT+CREG=0", "OK", 150.0);
	return 0;
}

int testSim(int counts)
{
	debugLevel = 2;
	int i;
  	Serial.print("#### START TEST ####");

	for (i = 1; i <= counts; i++)
	{
		Serial.print(">>>> TEST <<<< -> "); Serial.println(i);
		send_at("AT", "OK", 1.0);
		checkSimConnectionStatus();
		checkSimCsq();
    	delay (3000);
	}
 	debugLevel = 1;
   Serial.print("#### END TEST ####");

 return 0;

}

int checkSimConnectionStatus()
{
	printSerial("Get Network Registration");
	send_at("AT+CREG?", "OK", 1.0);
	printSerial("Get Application Network");
	send_at("AT+CNACT?", "OK", 1.0);
	printSerial("Get Network IP");
	send_at("AT+CGNAPN", "OK", 1.0);
	return 0;
}

int checkSimCsq()
{
	printSerial("Get Signal Quality");
	if(send_at("AT+CSQ", "+CSQ: 99,99", 1.0) == 0)	//Check if CSQ Value is available
	{
		return 1;
	}
	return 0;
}

int getCellInfo()
{
	send_at("AT+CPSI?", "OK", 1.0);
   parseAtResponse(cpsi);
   return 0;
}

int activateAppNetwork(bool activate)
{
	if(activate)
	{
		printSerial("Activate Application Network");
    	if (send_at("AT+CNACT?", "+CNACT: 0,0", 1.0) == 1)
    	{
		  	send_at("AT+CNACT=0,1", "OK", 1.0);
			return(expect_at("AT+CNACT?", "+CNACT: 0,1", 1.0, 5));
    	}
	}
	else
	{
		printSerial("Deactivate Application Network");
    	if (send_at("AT+CNACT?", "+CNACT: 0,1", 1.0) == 1)
    	{
		  	send_at("AT+CNACT=0,0", "OK", 2.0);
			return(expect_at("AT+CNACT?", "+CNACT: 0,0", 1.0, 5));
    	}
	}
	return 0;
}


int getGpsPos(float timeout)
{
   printSerial("++ Get GPS position (%.2f)", timeout);
	send_at("AT+SGNSCMD=1,0", "OK", 10.0);
	delay(100);
	read_ser("+SGNSCMD:", timeout);
   parseAtResponse(gpspar);
   strcpy(slat, val[2]);
   strcpy(slon, val[3]);

	delay(100);
	send_at("AT+SGNSCMD=0", "OK", 10.0);
	
}

int httpGetSim(const double lat, const double lon)
{
	char *cptr;
	if (send_at("AT+CNACT?", "+CNACT: 0,1", 1.0) == 0)
		{
			printSerial("HTTP ERROR: App Network not ready");
			return -1;
		}
	printSerial("Check open http socket");
	if (send_at("AT+SHSTATE?", "+SHSTATE: 0", 1.0) == 0)	//if socket is still open?
	{
		send_at("AT+SHDISC", "OK", 1.0); //close it
	}
	//sprintf(at, "AT+SHCONF=\"URL\",\"http://%s:%d\"", host, httpPort);
   sprintf(at, "AT+SHCONF=\"URL\",\"http://%s\"", host);

	printSerial("Set url '%s'", at);
	send_at(at, "OK", 10.0);
	printSerial("Set body length");
	send_at("AT+SHCONF=\"BODYLEN\",350", "OK", 10.0);
	printSerial("Set header length");
	send_at("AT+SHCONF=\"HEADERLEN\",350", "OK", 10.0);
	printSerial("Open Socket");
	while (send_at("AT+SHCONN", "OK", 10.0) < 0)
	{
		printSerial("# ERROR open http socket");
      //if (send_at("AT+SHSTATE?", "+SHSTATE: 0", 1.0) == 0)  //if socket is s open?
      {
         send_at("AT+SHDISC", "OK", 1.0); //close it
      }

		//activateAppNetwork(false);
		//return -1;
	}
	printSerial("Get socket state");
	send_at("AT+SHSTATE?", "OK", 10.0);
	printSerial("Change Header");
	send_at("AT+SHCHEAD", "OK", 10.0);
	send_at("AT+SHAHEAD=\"Accept\",\"text/html, */*\"", "OK", 10.0);
	send_at("AT+SHAHEAD=\"User-Agent\",\"IOE Client\"", "OK", 10.0);
	send_at("AT+SHAHEAD=\"Content-Type\",\"application/x-www-form-urlencoded\"", "OK", 10.0);
	send_at("AT+SHAHEAD=\"Connection\",\"close\"", "OK", 10.0);
	send_at("AT+SHAHEAD=\"Cache-control\",\"no-cache\"", "OK", 10.0);
	sprintf(at, "AT+SHREQ=\"http://%s:%d/lat=%f/lng=%f/token=%s\",1", host, httpPort, lat, lon, token);
	printSerial("GET '%s'", at);
#ifdef HTTP_UPLOAD
	send_at(at, "OK", 10.0);
	delay(100);
	read_ser("+SHREQ: \"GET\"", 20.0);	//Wait for further input
	cptr = strtok(atResponse, ",");  //+SHREQ: "GET"
	cptr = strtok(NULL, ",");        //http return code 301 ??
	cptr = strtok(NULL, "\n");       //http return size
   sprintf(at,"AT+SHREAD=0,%s", cptr);
	send_at(at, "OK", 10.0);
	read_ser("+SHREAD:", 20.0); //Wait for further input
   delay(500);
#else
  printSerial("HTTP UPLOAD SKIPPED");
#endif
	if (send_at("AT+SHSTATE?", "+SHSTATE: 0", 1.0) == 0)  //if socket is s open?
   {
      send_at("AT+SHDISC", "OK", 1.0); //close it
   }
	//activateAppNetwork(false);
	return 0;
}



/*** FUNCTION parseAtResponse *************************************************
******************************************************************************/ 
int parseAtResponse(const char **param)
{
   char *cptr;
   int i;
   int j;

   cptr = strstr(atResponse, " ");
   cptr++;
   i = 0; j = 0;
   while (*cptr != '\0' && param[i][0] != '#')
   {
      //printSerial("# i: %d j: %d s: %s", i, j, cptr);
      if (*cptr == ',' || *cptr == '\n')
      {
         val[i][j] = '\0';
         printSerial("%-10s = %s", param[i], val[i]);
         j = 0;
         if(++i >= NVAL)
         {
            return i;
         }
      }
      else
      {
         val[i][j] = *cptr;
         if (++j >= SVAL)
         {
            j = SVAL - 1;
         }

      }
      cptr++;
   }
   return 0;
}

/*** FUNCTION expect_at *******************************************************
******************************************************************************/ 

int expect_at(const char *command, const char *expect, float timeout, int tries)
{
	int i;
	for (i = 0; i < tries; i++)
	{
		if (send_at(command, expect, timeout) == 1)
			{
				return 1;
			}
	}
	return 0;
	
}

/*** FUNCTION send_at *********************************************************
 send at command to modem and read the response
 input:
 - string command:  at command string
 - string expect:   expected response string 
 - float timeout;   timeout in seconds
 output:
 - int return: 
	  1: response OK and contains expect string
	  0: response OK but no expect string in response
	 -1: response ERROR
	 -2: TIMEOUT
module variable:
 - string atResponse: complete response string for further processing
******************************************************************************/ 
int send_at(const char *command, const char *expect, float timeout) 
{
  	//char  atResponse[1024] = "";  //use Module wide define atResponse instead to allow access to response 
  	int   charCount = 0;
  	float counter = 0.01;
  	atResponse[0] = 0;  //Initialize response buffer
  
  	if (command[0] != 0)
  	{
	 	printSerial("> %s [%s]? (%.2f sec)", command, expect, timeout); //debug output
		sprintf(txt,"%s\r\n", command);     //Add mandatory LF & CR
  		softSerial.println(txt);            //Send at command to modem       is println necessary?     	 
  	}	 
   delay(10);  //wait 10ms
  	while(1)
	{
		while(softSerial.available()) 
		{
			atResponse[charCount++] = (char)softSerial.read(); //fill char array with serial readings
	 		if (charCount >= SIZE_AT_RESPONSE - 2)
	 		{
				if (debugLevel > 1) { Serial.println("  ## SIZE_AT_RESPONSE ");}
				charCount = SIZE_AT_RESPONSE - 2; //Avoid array out of bound runtime error -> hard limit!
        break;
	 		}
         delay(1); //wait 1 ms
			counter += 0.001;
		}
		atResponse[charCount] = 0;  //String termination
		if((strstr(atResponse, "OK")) != NULL)
		{
		  	printSerial("< OK: (%.3f sec) \n %s", counter, atResponse);
		  	if((strstr(atResponse, expect)) != NULL)
		  	{
			 	return 1;
		  	}
		  	else
		  	{
			 	return 0;
		  	} 
		}       
		if((strstr(atResponse, "ERROR")) != NULL)
		{
		  	printSerial("< ERROR: (%.3f sec) \n %s", counter, atResponse);
		  	return -1;
		}
	 	if(counter >= timeout)
	 	{
			printSerial("< TIMEOUT: (%.3f sec) \n %s", counter, atResponse);
			return -2;
	 	}   
	 	delay(1); //wait 1 ms
	 	counter += 0.001;
  }
}
/*****************************************************************************/

/*** FUNCTION read_ser ********************************************************
 read_ser read the response from serial interface
 input:
 - string expect:   expected response string 
 - float timeout;   timeout in seconds
 output:
 - int return: 
	  1: response contains expect string
	 -2: TIMEOUT
module variable:
 - string atResponse: complete response string for further processing
******************************************************************************/ 
int read_ser(const char *expect, float timeout) 
{
 	int   charCount = 0;
  	float counter = 0.01;
  	atResponse[0] = 0;  //Initialize response buffer
   
   delay(10);  //wait 10ms
  	while(1)
	{
		while(softSerial.available()) 
		{
			atResponse[charCount++] = (char)softSerial.read(); //fill char array with serial readings
	 		if (charCount >= SIZE_AT_RESPONSE - 2)
	 		{
				charCount = SIZE_AT_RESPONSE - 2; //Avoid array out of bound runtime error -> hard limit!
            break;
	 		}
        delay(1); //wait 1 ms
		  counter += 0.001;
		}
		atResponse[charCount] = 0;  //String termination
		if((strstr(atResponse, expect)) != NULL)
		{
		  	printSerial("< OK: (%.3f sec) \n %s", counter, atResponse);
			return 1;      
		}  
	 	if(counter >= timeout)
	 	{
			printSerial("< TIMEOUT: (%.3f sec) \n %s", counter, atResponse);
			return -2;
	 	}   
	 	delay(1); //wait 1 ms
	 	counter += 0.001;
  }
}
/*****************************************************************************/
