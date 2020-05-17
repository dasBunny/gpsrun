
/*
 * This is the part, which handles the lte send procedure
 * 
 * 
 * 
 */


/**
 * Don't forget to:
 * 1. Set correct GPRS APN
 * 2. Set correct GPRS username
 * 3. Set correct GPRS password
 * 4. Set SIM800L TX/RX pins
 * 5. Set your server URL
 */
 
//defines-----------------------------------------------------------------------------------------
  char responseBuffer[256];
 
  char *initSim[]       = {
                          (char *)"AT\r\n", // Ping module
                        };
char *gprsStart[]     = {
                          (char *)"AT+SAPBR=3,1,\"Contype\", \"GPRS\"\r\n", // Configure bearer profile
                          (char *)"AT+SAPBR=3,1,\"APN\",\"internet.eplus.de\"\r\n",
                          (char *)"AT+SAPBR=3,1,\"USER\",\"eplus\"\r\n",
                          (char *)"AT+SAPBR=3,1,\"PWD\",\"eplus\"\r\n",
                          (char *)"AT+SAPBR=1,1\r\n", // Open a GPRS context
                          (char *)"AT+SAPBR=2,1\r\n" // Query the GPRS context
                        };
char *gprsEnd[]       = { (char *)"AT+SAPBR=0,1\r\n" }; // Close a GPRS context

char *httpStart[]     = {
                          (char *)"AT+HTTPINIT\r\n", // Init HTTP service
                          (char *)"AT+HTTPPARA=\"CID\",1\r\n", // Set parameters for HTTP session
                          (char *)"AT+HTTPPARA=\"URL\",\"dasbunny.at\"\r\n",
                          (char *)"AT+HTTPPARA=\"CONTENT\",\"image\"\r\n",
                          (char *)"AT+HTTPDATA=5,10000\r\n", // POST the data, params (size as bytes, input latency time ms)
                          (char *)"DEBUG", // POST Data
                          (char *)"AT+HTTPACTION=1\r\n", // POST Session start
                          (char *)"AT+HTTPREAD\r\n" // Read the data
                        };
char *httpEnd[]       = { (char *)"AT+HTTPTERM\r\n" }; // Terminate HTTP Service

void setup() {
  Serial.begin(115200);
  while(!Serial);
  //serialSIM800.begin(115200);
  serialSIM800.begin(9600);
  
  if(DEBUG) Serial.println("SETUP123");
  delay(3000);
  //initSimModule();
  //setGprs();
  //postData();
  //terminateHttp();
  //endGprs();
}

void loop() {
  //initSimModule();
  send_at("AT\r\n", 1000);
  delay(1000);
}

void initSimModule() {
  writeToSim(initSim, sizeof(initSim)/sizeof(*initSim), 1000);
}

void setGprs() {
  if(DEBUG) Serial.println("setGprs");
  writeToSim(gprsStart, sizeof(gprsStart)/sizeof(*gprsStart), 1000);
}

void endGprs() {
  if(DEBUG) Serial.println("endGprs");
  writeToSim(gprsEnd, sizeof(gprsEnd)/sizeof(*gprsEnd), 1000);
}

void postData() { // TODO Insert payload
  if(DEBUG) Serial.println("postData");
  writeToSim(httpStart, sizeof(httpStart)/sizeof(*httpStart), 1000);
}

void terminateHttp() {
  if(DEBUG) Serial.println("terminateHttp");
  writeToSim(httpEnd, sizeof(httpEnd)/sizeof(*httpEnd), 1000);
}

//void writeToSim(char *commands[], int size) {
//  for(int i = 0; i < size; i++) {
//    serialSIM800.println(commands[i]);
//    if(DEBUG) { Serial.print("Writing: "); Serial.println(commands[i]); }
//    delay(1000);
//    while(serialSIM800.available()) Serial.print((char)serialSIM800.read()); // Ignore written commands
//  }
//}








//Serial write to cellular modem
//Serial write to cellular modem
bool writeToSim(char *commands[], int size, unsigned long timeoutms) {
  
  for(int i = 0; i < size; i++) {
    serialSIM800.println(commands[i]);

    
    if(DEBUG) { Serial.print("Writing: "); Serial.println(commands[i]); } //debug output
    delay(1000);

    
    while(serialSIM800.available()){
      char reading = (char)serialSIM800.read();
      Serial.print(reading); // Ignore written commands
      
    }
  } 
}

bool send_at(char *command, unsigned long timeoutms) {

  char buffer[256] = "";
  int  charCount = 0;
  unsigned long delaycounter = 0;
  
  serialSIM800.println(command);                //\zeug einfügen

    
  if(DEBUG) { Serial.print("Writing: "); Serial.println(command); } //debug output
    
  while(1){
    if(serialSIM800.available()) buffer[charCount++] = (char)serialSIM800.read(); //fill char array with serial readings
    //char reading;
      //Serial.println(readSim(reading, 300, 100));
      //Serial.print(*reading); // Ignore written commands?!


      if((strstr(buffer,"OK")) != NULL)
      {
        Serial.println("found OK in answer -> return success (true)");
        Serial.println(buffer);
        Serial.println(delaycounter);
        return true;
      }
      if((strstr(buffer,"ERROR")) != NULL)
      {
        Serial.println("found ERROR in answer -> return failure (false)");
        Serial.println(buffer);
        return false;
      }  

    if(delaycounter > timeoutms)
    {
      Serial.print("no response until timeout -> return failure (false)");
      Serial.println(buffer);
      return false;
    }
    
    delay(1);
    delaycounter ++;
  }
}


// Read Sim response until timeout ---> was anderes wie timeout wäre hier warscheinlich besser
char* readSim(char* buffer, int timeout, int charCountMax) {
  unsigned long timeStart = millis();
  int charCount = 0;

  while(1) {
    if(serialSIM800.available()) buffer[charCount++] = serialSIM800.read(); //fill char array with serial readings

    if(millis() - timeStart > timeout) break;
    if (charCount > charCountMax) break;                                    //check if max length is reached
  }
  buffer[charCount++] = 0;
  return buffer;
}
