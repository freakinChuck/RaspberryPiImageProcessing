/*
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 */

 //==========================================================================================================================================================================================================================================================================
//                                                     Pins müssen nicht mehr angepasst werden
//==========================================================================================================================================================================================================================================================================
//Pin Belegung Echopin:      Hinten Links: 22;  Vorne Links: 23;   Vorne: 24;   Vorne Rechts : 25;    Hinten Rechts : 26
//Pin Belegung Triggerpin:   Hinten Links: 3;   Vorne Links: 4;    Vorne: 5;    Vorne Rechts : 6;     Hinten Rechts : 7


#define echoPinHL 22 // Echo Pin
#define trigPinHL 3 // Trigger Pin

#define echoPinVL 23 // Echo Pin
#define trigPinVL 4 // Trigger Pin

#define echoPinV 24 // Echo Pin
#define trigPinV 5 // Trigger Pin

#define echoPinVR 25 // Echo Pin
#define trigPinVR 6 // Trigger Pin

#define echoPinHR 26 // Echo Pin
#define trigPinHR 7 // Trigger Pin



int maximumRange = 105; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long durationHL, durationVL, durationV, durationVR, durationHR,         distanceHL, distanceVL, distanceV, distanceVR, distanceHR; // Duration used to calculate distance

int ausgerichtet = 0;
int messungVR = 0;
int messungHR = 0;
int messungVL = 0;
int messungHL = 0;
double abstandHR = 0;
double abstandVR = 0;
double abstandHL = 0;
double abstandVL = 0;
double winkelKorrekturL = 0;
double winkelKorrekturR = 0;



void ultraschallSetup() {
  //Serial.begin (115200);
 pinMode(trigPinHL, OUTPUT);
 pinMode(echoPinHL, INPUT);
 
 pinMode(trigPinVL, OUTPUT);
 pinMode(echoPinVL, INPUT);

 pinMode(trigPinV, OUTPUT);
 pinMode(echoPinV, INPUT);

 pinMode(trigPinVR, OUTPUT);
 pinMode(echoPinVR, INPUT);

 pinMode(trigPinHR, OUTPUT);
 pinMode(echoPinHR, INPUT);


}


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Abstand Vorne ***************************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************



int abstandVorne() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPinV, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPinV, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPinV, LOW);
 durationV = pulseIn(echoPinV, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distanceV = durationV/58.2;
 
 if (distanceV >= maximumRange || distanceV <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.print(" Abstand Vorne: ");
 Serial.println(distanceV);
 }
 
 //Delay 50ms before next reading.
 //delay(50);

int distanzV = (int)(distanceV);
 return distanzV;
}

//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Abstand Vorne Links**********************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int abstandVorneLinks() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPinVL, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPinVL, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPinVL, LOW);
 durationVL = pulseIn(echoPinVL, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distanceVL = durationVL/58.2;
 
 if (distanceVL >= maximumRange || distanceVL <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.print(" Abstand vorne Links: ");
 Serial.println(distanceVL);
 }
 
 //Delay 50ms before next reading.
 //delay(50);

int distanzVL = (int)(distanceVL);
 return distanzVL;
}



//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Abstand Hinten Links**********************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int abstandHintenLinks() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPinHL, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPinHL, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPinHL, LOW);
 durationHL = pulseIn(echoPinHL, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distanceHL = durationHL/58.2;
 
 if (distanceHL >= maximumRange || distanceHL <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.print(" Abstand hinten Links: ");
 Serial.println(distanceHL);
 }
 
 //Delay 50ms before next reading.
 //delay(50);

int distanzHL = (int)(distanceHL);
 return distanzHL;
}


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Abstand Vorne Rechts*********************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int abstandVorneRechts() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPinVR, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPinVR, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPinVR, LOW);
 durationVR = pulseIn(echoPinVR, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distanceVR = durationVR/58.2;
 
 if (distanceVR >= maximumRange || distanceVR <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.print(" Abstand vorne Rechts: ");
 Serial.println(distanceVR);
 }
 
 //Delay 50ms before next reading.
 //delay(50);

int distanzVR = (int)(distanceVR);
 return distanzVR;
}


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Abstand Hinten Rechts********************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int abstandHintenRechts() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPinHR, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPinHR, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPinHR, LOW);
 durationHR = pulseIn(echoPinHR, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distanceHR = durationHR/58.2;
 
 if (distanceHR >= maximumRange || distanceHR <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.print(" Abstand hinten Rechts: ");
 Serial.println(distanceHR);
 }
 
 //Delay 50ms before next reading.
 //delay(50);

int distanzHR = (int)(distanceHR);
 return distanzHR;
}



//***********************************************************************************************************************************************************************************************************************************
//********************************************************************************************Korrektur für Taste drücken  (Parcours Rechts)*****************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int ausrichtungParcoursRechts()
{
  if(ausgerichtet == 0)
  {
    if(messungHR == 0)
    {
     abstandHR =  abstandHintenRechts();
     messungHR++;
    }
    if(messungVR == 0)
    {
      abstandVR = abstandVorneRechts();
      messungVR++;
    }

    winkelKorrekturR = atan(abstandVR - abstandHR);
    ausgerichtet++;
    
  }

  Serial.print("Resultat: ");
  Serial.println(winkelKorrekturR);
  return winkelKorrekturR;

  
}


//***********************************************************************************************************************************************************************************************************************************
//********************************************************************************************Korrektur für Taste drücken  (Parcours Links)*****************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int ausrichtungParcoursLinks()
{
  if(ausgerichtet == 0)
  {
    if(messungHL == 0)
    {
     abstandHL =  abstandHintenLinks();
     messungHL++;
    }
    if(messungVL == 0)
    {
      abstandVL = abstandVorneLinks();
      messungVL++;
    }

    winkelKorrekturL = atan(abstandVL - abstandHL);
    ausgerichtet++;
    
  }

  Serial.print("Resultat: ");
  Serial.println(winkelKorrekturL);
  return winkelKorrekturL;

  
}





