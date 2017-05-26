
#include <Stepper.h>
#include <math.h>

/* Mit dem Switch wird die Positon des Taster ausgerechnet. Mit der Funktion claculate Steps wird anschliessend ausgerechnet, wie viele Schritte der Stepper Motor machen muss
*/
const double schritteProUmdrehung = 400.0;  // Anzahl Schritte des Stepper Motors
int abstandVorhanden = 180; //gemessener Abstand durch Ultraschall
int gerechneterAbstand = 0;
int romanNumber = 1; //ausgelesene Römisch Zahl

int abstandFuenf = 135; // Absolut Koordinaten der Tasten
int abstandVier = 195;
int abstandDrei = 255;
int abstandZwei = 315;
int abstandEins = 375;
int i=1;

Stepper myStepper(schritteProUmdrehung, 41, 42, 43, 44); // Initialisierung Stepper


void tasterSetup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  Serial.begin(9600);  
}

 int calculateSteps(double abstand){
  //Serial.print(abstand);
  //Serial.print('\n');
 
    double steps = 0;
    
    steps =(PI/2)- atan(30.0/abs(abstand)); //umrechen von benötigtem abstand in Winkel rad
    
    steps = ((steps * 360)/(2*PI)); //  umerechen von rad in degree
  //Serial.print(steps);
  // Serial.print('\n');
    if (abstand>0) {    // Handling von Abständen kleiner als 0 
        steps =-(steps / (360/schritteProUmdrehung)); // umrechen von degree in steps  
      }
    else {
       steps = (steps /(360/schritteProUmdrehung)); 
       }            
    steps = round(steps); 
    Serial.print(steps);
    return steps;
  }

  void moveMotor (){
      switch (romanNumber){
    case 1: 
        gerechneterAbstand = abstandEins-155-abstandVorhanden;
        myStepper.step(calculateSteps(gerechneterAbstand));  // Je nach Parcourseite muss 155mm oder 65mm angepasst werden
    break;
    case 2:
        gerechneterAbstand = abstandZwei-155-abstandVorhanden;
        myStepper.step(calculateSteps(gerechneterAbstand));  

    break;
    case 3:
       gerechneterAbstand = abstandDrei-155-abstandVorhanden;
       myStepper.step(calculateSteps(gerechneterAbstand));  

    break;
    case 4:
        gerechneterAbstand = abstandVier-65-abstandVorhanden;
        myStepper.step(calculateSteps(gerechneterAbstand)); 
    break;
    case 5:
       gerechneterAbstand = abstandFuenf-65-abstandVorhanden;
       myStepper.step(calculateSteps(gerechneterAbstand));
  
    break;
    delay(4000);
    }}

    
 void setAbstandVorhanden(int abstandU)
 {
  abstandVorhanden = abstandU;
 }

 void setRomanNumber(int number)
 {
  romanNumber = number;
 }

