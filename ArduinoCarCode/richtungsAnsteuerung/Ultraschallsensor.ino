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
//                                                     Pins müssen noch angepasst werden
//==========================================================================================================================================================================================================================================================================



#define echoPin 6 // Echo Pin
#define trigPin 7 // Trigger Pin
#define LEDPin 13 // Onboard LED
#define echoPin2 5
#define trigPin2 4

int maximumRange = 105; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, duration2,  distance,distance2; // Duration used to calculate distance

void ultraschallSetup() {
  //Serial.begin (115200);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
  pinMode(trigPin2, OUTPUT);
 pinMode(echoPin2, INPUT);

}

int getDistanz() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if (distance >= maximumRange || distance <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.println("-1");
 digitalWrite(LEDPin, HIGH); 
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.println(distance);
 digitalWrite(LEDPin, LOW); 
 }
}

/*
 

int getDistanz2() {

 digitalWrite(trigPin2, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin2, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin2, LOW);
 duration2 = pulseIn(echoPin2, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance2 = duration2/58.2;
 
 if (distance2 >= maximumRange || distance2 <= minimumRange){
 
 Serial.println("-1");
 digitalWrite(LEDPin, HIGH); 
 }
 else {
 
 Serial.println(distance2);
 digitalWrite(LEDPin, LOW); 
 }
 
 
 //Delay 50ms before next reading.
 //delay(50);

int distanz2 = (int)(distance2);
 return distanz2;
}*/







