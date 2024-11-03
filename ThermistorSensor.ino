void ThermistorModule() {
  Dmeasured   = analogRead(A0);
  voltage     = Dmeasured * (5z.0/1023.0);
  Rthermistor = (voltage * R0) / (5.0 - voltage);
  Temp        = (T0 * B) / (B + (T0 * log(Rthermistor / R0)));
  C           = Temp - 273.15;
}