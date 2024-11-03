void SDCardFunction(){
  while (!Serial) {
  }
    Serial.println("LOG - Initializing SD Card.");
    if (!SD.begin(10)) {
    Serial.println("LOG - Initialization Failed.");
    digitalWrite(9, LOW);
    //while (1);
    } else {
    Serial.println("LOG - Initialization Complete.");
    digitalWrite(9, HIGH); 
  }
}