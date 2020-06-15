void ICACHE_RAM_ATTR ledTimerISR(){
  switch(state){
    case 1:
      Serial.println("Case 1");
      state = 2;
      digitalWrite(LED, HIGH);
      timer1_write(20000000);
      break;
    case 2:
      Serial.println("Case 2");
      state = 3;
      digitalWrite(LED, LOW);
      timer1_write(40000000);
      break;
    case 3:
      Serial.println("Case 3");
      state = 0;
      digitalWrite(LED, HIGH);
      break;
    default:
      Serial.println("Default");
      digitalWrite(LED, HIGH);
      break;
  }
}
