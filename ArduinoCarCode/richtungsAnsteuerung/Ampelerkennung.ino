// Ampel Pin: 49;

int ampelPin = 49;
int ampelErkannt = 0;



void ampelSetup()
{
   pinMode(ampelPin, INPUT);
}


void ampelErkennung(void)
{
for(ampelErkannt;ampelErkannt<=0;ampelErkannt++)
{
// wait for ready    (kann für digitalen Schalter verwendet werden
    Serial.println("Warte auf Ampelerkennung ...........");
    while (LOW==digitalRead(ampelPin)); 
   Serial.println("Ampel wurde erkannt .....");
}
//return ampelErkannt;
}
