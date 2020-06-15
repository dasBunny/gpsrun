void Sleep(const int SleepTimeS, const bool wifi_enable) {

  if(wifi_enable == true)
  {
    Serial.println("sleep function, wifi enabled");
   //--Power Down the sim module--
      #ifdef SIM_PWR_ON_OFF
        Serial.println("SimPWRONOFF defined");
        //softPowerDownSimModule(); //## TEST Power Down OK; COPS macht Probleme nach Neustart??
        powerDownSimModule(); //## TEST Power Down OK; COPS läuft besser braucht etwa 6 sek
      #endif
      #ifdef SIM_DEEP_SLEEP
        deepSleepSimModule(); //## TEST Deep Sleep OK, Wake Up 
      #endif                                                       
              
    
   ESP.deepSleep(SleepTimeS*1000000, WAKE_RF_DEFAULT); // Sleep for SleepTimeS seconds with wifi enabled
  }

  if(wifi_enable == false)
  {
    Serial.println("sleep function, wifi disabled");

 //--Power Down the sim module--
      #ifdef SIM_PWR_ON_OFF
      Serial.println("SimPWRONOFF defined");
        //softPowerDownSimModule(); //## TEST Power Down OK; COPS macht Probleme nach Neustart??
        powerDownSimModule(); //## TEST Power Down OK; COPS läuft besser braucht etwa 6 sek
      #endif
      #ifdef SIM_DEEP_SLEEP
        deepSleepSimModule(); //## TEST Deep Sleep OK, Wake Up 
      #endif
    
   ESP.deepSleep(SleepTimeS*1000000, RF_DISABLED); // Sleep for SleepTimeS seconds with wifi disabled
  }

  
}


/*
 * Parameter: -SleepTimeS:      deep sleep time in seconds
 *            -wifi_enable:     defines whether wifi should be enabled or disabled after wakeup
 *            
 *            
 *            
 *  Return values:void        
 *            
 */
