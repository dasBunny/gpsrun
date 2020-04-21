void Sleep(const int SleepTimeS, const bool wifi_enable) {

  if(wifi_enable == true)
  {
   ESP.deepSleep(SleepTimeS*1000000, WAKE_RF_DEFAULT); // Sleep for SleepTimeS seconds with wifi enabled
  }

  if(wifi_enable == false)
  {
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
