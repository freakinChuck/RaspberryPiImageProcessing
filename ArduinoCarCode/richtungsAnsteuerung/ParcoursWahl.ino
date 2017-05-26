// PIN 45

#define schalterPin 40

void umschalterSetup()
{
  pinMode(schalterPin, INPUT);
}

int schalterstellung()
{
  //delay(100); // entprellen
  int schalter = digitalRead(schalterPin);
  Serial.println(schalter);
  return schalter;
}

