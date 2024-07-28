int buttonPin = 26;

void test(){
  while(true){
    Serial.print("Find correct place for IR Led.");
    powerOff(); 
    if(digitalRead(buttonPin)){break;}
  }
  delay(250);
  Serial.println("Led Placement Complete.");
}
