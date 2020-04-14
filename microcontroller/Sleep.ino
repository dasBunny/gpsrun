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

//deep sleep function, you can choice whether wifi should be enabled (wifi_enable = true) or disabled (wifi_enable = false) after wakeup.
