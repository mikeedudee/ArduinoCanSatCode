void MS5611Function(){
  while(!ms5611.begin()) {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    digitalWrite(6, LOW);
  }
  referencePressure = ms5611.readPressure(); // Get reference pressure for relative altitude 
  checkSettings();
}

void checkSettings() {
  //Serial.print("Oversampling: ");
  //Serial.println(ms5611.getOversampling());
}

void MS5611Readings() {
  digitalWrite(6, HIGH);
  realTemperature  = ms5611.readTemperature();
  realPressure     = ms5611.readPressure();
  relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);
}