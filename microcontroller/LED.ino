int checkBat(){
  float voltage = analogRead(BAT_VOLTAGE_PIN)*7.9; //Reading analog input voltage and converting it to the actual battery voltage
  Serial.print(voltage);
  if(voltage < 4.8){ //If voltage is below 4.8V the device should power down without sending anything
    //power off
    Serial.print("Power to low, shutting down");
    return 0;
  }
  else{
    voltage=-4.8;
    int ret = int(voltage*10)%4;
    Serial.println("Voltage State: "+ret);
    return ret;
  }
}

void beginPowerLED(){
  state = 1; //Resetting state, incase it was used before
  timerInUse = true;
  switch(checkBat()){
    case 0:
      digitalWrite(LED_R, HIGH);
      delay(500);
      digitalWrite(LED_R, LOW);
      //SHUTDOWN CODE HERE
      Serial.println("Battery low, going to sleep");
      Sleep(SleepTimeS, wifi_enable);
      break;
    case 1:
      timer1_attachInterrupt(bat20);
      Serial.println("Battery below 20%");
      digitalWrite(LED_R, HIGH);
      timer1_write(FULL_INTERVAL);
      break;
    case 2:
      digitalWrite(LED_R, HIGH);
      timer1_attachInterrupt(bat40);
      timer1_write(LONG_INTERVAL);
      break;
    case 3:
      timer1_attachInterrupt(bat60);
      break;
    case 4:
      timer1_attachInterrupt(bat80);
      break;
    case 5:
      timer1_attachInterrupt(bat100);
      break;
    default:
      Serial.println("Unknown voltage level, no LED sequence started");
      break;
      timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  }
}

void ICACHE_RAM_ATTR bat20(){
  digitalWrite(LED_R,LOW);
  timerInUse = false;
}

void ICACHE_RAM_ATTR bat40(){
  switch(state){
    case 1:
      Serial.println("Case 1");
      state = 2;
      digitalWrite(LED_R, LOW);
      timer1_write(LONG_INTERVAL);
      break;
    case 2:
      Serial.println("Case 2");
      state = 3;
      digitalWrite(LED_R, HIGH);
      timer1_write(LONG_INTERVAL);
      break;
    case 3:
      Serial.println("Case 3");
      state = 4;
      digitalWrite(LED_R, LOW);
      timer1_write(LONG_INTERVAL);
      break;
    case 4:
      Serial.println("Case 4");
      state = 5;
      digitalWrite(LED_R, HIGH);
      timer1_write(LONG_INTERVAL);
      break;
    case 5:
      Serial.println("State 5 - LED blinking finished");
      digitalWrite(LED_R, LOW);
      timerInUse = false;
      break;
    default:
      timerInUse = false;
      Serial.println("State unknown");
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, LOW);
      break;
  }
}

void ICACHE_RAM_ATTR bat60(){
  switch(state){
    case 1:
      Serial.println("Case 1");
      state = 2;
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      timer1_write(LONG_INTERVAL);
      break;
    case 2:
      Serial.println("Case 2");
      state = 3;
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, HIGH);
      timer1_write(LONG_INTERVAL);
      break;
    case 3:
      Serial.println("Case 3");
      state = 4;
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_R, LOW);
      timer1_write(LONG_INTERVAL);
      break;
    case 4:
      Serial.println("Case 4");
      state = 5;
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, HIGH); 
      timer1_write(LONG_INTERVAL);
      break;
    case 5:
      Serial.println("State 5 - LED blinking finished");
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      timerInUse = false;
      break;
    default:
      timerInUse = false;
      Serial.println("State unknown");
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, LOW);
      break;
  }
}

void ICACHE_RAM_ATTR bat80(){
    switch(state){
    case 1:
      Serial.println("Case 1");
      state = 2;
      digitalWrite(LED_G, LOW);
      timer1_write(LONG_INTERVAL);
      break;
    case 2:
      Serial.println("Case 2");
      state = 3;
      digitalWrite(LED_G, HIGH);
      timer1_write(LONG_INTERVAL);
      break;
    case 3:
      Serial.println("Case 3");
      state = 4;
      digitalWrite(LED_G, LOW);
      timer1_write(LONG_INTERVAL);
      break;
    case 4:
      Serial.println("Case 4");
      state = 5;
      digitalWrite(LED_G, HIGH);
      timer1_write(LONG_INTERVAL);
      break;
    case 5:
      Serial.println("State 5 - LED blinking finished");
      digitalWrite(LED_G, LOW);
      timerInUse = false;
      break;
    default:
      timerInUse = false;
      Serial.println("State unknown");
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, LOW);
      break;
  }
}

void ICACHE_RAM_ATTR bat100(){
  digitalWrite(LED_G,LOW);
  timerInUse = false;
}
