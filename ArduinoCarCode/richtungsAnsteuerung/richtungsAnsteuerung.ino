#include "MPUheader.h"
#include <avr/io.h> 
#include <avr/wdt.h>

int parcours = 0; //0 = Parcours rechts    1 = Parcours Links
int ampelerkennung = 0;    //Ampelerkennung ausgeschaltet = 0, eingeschaltet = 1
int zahlenerkennung = 0;   //Zahlenerkennung ausgeschaltet = 0, eingeschaltet = 1
int startSignalPin = 40;


boolean STARTerfolgreich = true;
boolean EndeParcours = false;
boolean Kurvenloop = false;
boolean KeineKorrektur = false;
boolean KURVEBEENDEN = false;
boolean SELBSTHALTUNG = false;
boolean NACHHINTENGEKIPPT = false;
boolean NACHVORNEGEKIPPT = false;
boolean treppeUeberwunden = false;
boolean VERSCHRAENKUNG = false;
boolean KURVEEINLEITEN = false;
boolean  HALBEKURVEABGESCHLOSSEN = false;
boolean KURVEABGESCHLOSSEN = false;
boolean TorKorrekrurNachLinks = false;
boolean  geradeAus = false;
boolean offsetDone = false;
boolean startOffset = false;

 float yaw12 = 0;
 int antiDrift = 0;
 int driftTime = 0;
 int antiDcount = 0;



int messung = 0;
int erkanntezahl = 0;
int softwareReset = 0;
int myYAWOffset = 0;
int myROLLOffset = 0;
int myPITCHOffset = 0;
int startKurveOffset = 0;
int test = 0;
int parcoursInfo = 0;

int driftOffset = 3.5;

double korrekturWinkel = 0;







void setup() {
/*
TCCR0B = _BV(CS00);              //1
TCCR0B = _BV(CS01);              //8
TCCR0B = _BV(CS00) | _BV(CS01);  //64
TCCR0B = _BV(CS02);              //256
TCCR0B = _BV(CS00) | _BV(CS02);  //1024

  TCCR2B = (TCCR2B & 0b11111000) | 0x04; //PWM Frequenz für pin 9 und 10  f = 490.20Hz               ****************** Achtung kann Werte von delay() verändern !!! Kontrolle bei Ultraschallsensoren ******************
  TCCR1B = (TCCR1B & 0b11111000) | 0x04; //PWM Frequenz für pin 11 und 12 f = 490.20Hz
  */
  
  mpuSetup();
  zahlenSetup();
  motorSetup();
  ultraschallSetup();
  ampelSetup();
  umschalterSetup();
  parcours = schalterstellung();
  pinMode(startSignalPin, OUTPUT);


}

int mpuKorrektur()
{  int ctrl = 0;
 /* int z = 0;
  float myYAW;*/

       
 
  /**int ctrl;
  float yaw1,yaw2 = 0;
  
                 
           
            Serial.println("Korrektur!!!!!");
  yaw1 = getYAW();
  Serial.println(yaw1);     

  delay(2000);
  yaw2 = getYAW();
  Serial.println(yaw2);
  delay(2000);
  if(abs( getYAW())<2 || abs(yaw2-yaw1)<0.1)
  {
    ctrl = 1;
  }
  else
  {
    ctrl = 0;
  }*/

  
  return ctrl;
 
}


void loop() {

for(parcoursInfo;parcoursInfo<5;parcoursInfo++)
{
  char* parcoursWahl;
  if(parcours == 0)
  {
    parcoursWahl = "Parcours rechts";
  }
  else
  {
    parcoursWahl = "Parcours links";
  }
  Serial.print("Parcours:   ");
  Serial.println(parcoursWahl);
}

digitalWrite(startSignalPin, HIGH); 

// hier Ampelerkennungs Methode einsetzen
if(ampelerkennung == 1)
{
 ampelErkennung();
}
if(zahlenerkennung == 1)
{
 if(messung<=0)
 {
 if(0 != zahlenErkennung())
 {

  delay(1000);
  erkanntezahl = zahlenErkennung();
  messung ++;
 
  
  
 }
 }
}


 


  

   // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        //Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;



    
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);    // *****************************wurde weggenommen *********************************
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
         
            float yaw = ((ypr[0] * 180)/M_PI)-myYAWOffset - antiDrift  ;            
            //float yawGefiltert = yaw -yawKalibriert;
            float pitch = (ypr[1] * 180)/M_PI-myPITCHOffset;
            float roll = (ypr[2] * 180)/M_PI-myROLLOffset;
            //Serial.print("yaw\t");

           if(driftTime %100 == 0 && antiDcount <50)
           {
            antiDcount++;
            antiDrift = antiDrift + 1;
            driftTime = driftTime -100;
           }
driftTime++;


            
            Serial.print("Das Fahrzeug  ");

            if(yaw>0)
            {
            
              //Ab hier muss mit dem grössten Zustand begonnen werden
              if(yaw>172)
              {
                Serial.print("faehrt in die entgegengesetzte Richtung ");
                if(KURVEBEENDEN == true)
                {
                KURVEABGESCHLOSSEN = true;
                SELBSTHALTUNG = false;
                }
                if(EndeParcours == false )       
                {
                 faehrtEGeradeAus();
                }
                else
                {

                  delay(100);

                  
                  if(abstandHintenLinks()>20 && parcours == 0 || abstandHintenRechts() && parcours == 1)
                  {
                    while(1)
                    {
                  //stopMotor();
                  if(parcours == 0)                  
                  {
                    korrekturWinkel = ausrichtungParcoursRechts();
                    if(korrekturWinkel<0)
                    {
                      fahreKurveNachLinks();
                    }
                    else{
                      fahreKurveNachRechts();
                    }
                  }
                  else
                  {

                     korrekturWinkel = ausrichtungParcoursLinks();
                    if(korrekturWinkel<0)
                    {
                      fahreKurveNachLinks();
                    }
                    else{
                      fahreKurveNachRechts();
                    }
                
                    
                  }
                    
                    
                    
                    
                    
                    }
                  }
                } // ende else
                 
                        
              }
              else if(yaw>140)
              {
                 Serial.print("faehrt in die entgegengesetzte Richtung nach links");
              }
              else if(yaw > 120)
              {
                 Serial.print("faehrt in die entgegengesetzte Richtung stark nach links ");
                 
              }
              else if(yaw > 60)
              {
                 Serial.print("wechselt die Richtung ");
              }
              else if(yaw>40)
              {
                 Serial.print("faehrt stark nach rechts ");
              }
              else if(yaw >20) // war vorher auf 20
              {
                
                  Serial.print("faehrt  nach rechts ");
                                 
                    if(NACHVORNEGEKIPPT == false && NACHHINTENGEKIPPT == false && KURVEEINLEITEN == false && KURVEABGESCHLOSSEN == false)
                  {
                   //faehrtNachRechts();
                  }
                              
              }
              else if(yaw >= 0)
              {
                Serial.print("faehrt gerade aus ");

               

                if(KURVEEINLEITEN == false)
                {
               faehrtGeradeAus();
                geradeAus = true;
                }
              
                    
                   
               
              }
              Serial.print( yaw );
              
            }
            else if(yaw<0)
            {
              if(yaw<-160) //******************************************************* parcours rechts
              {
                Serial.print("faehrt in die entgegengesetzte Richtung ");
                if(KURVEBEENDEN == true)
                {
                KURVEABGESCHLOSSEN = true;
                SELBSTHALTUNG = false;
                }
                if(EndeParcours == false)
                {
              faehrtEGeradeAus();
                }
                else
                {
                  delay(100);
                 if(parcours == 0 && abstandHintenLinks()>20  ||  parcours == 1 && abstandHintenRechts()>20 )      //*************************hier änderung für parcours wahl
                  {
                    while(1)
                    {
                  //stopMotor();
                        if(parcours == 0)                  
                  {
                    korrekturWinkel = ausrichtungParcoursRechts();
                    if(korrekturWinkel<0)
                    {
                      fahreKurveNachLinks();
                    }
                    else{
                      fahreKurveNachRechts();
                    }
                  }
                  else
                  {

                     korrekturWinkel = ausrichtungParcoursLinks();
                    if(korrekturWinkel<0)
                    {
                      fahreKurveNachLinks();
                    }
                    else{
                      fahreKurveNachRechts();
                    }
                
                    
                  }
                  
                    }
                  }
                } // ende else
                 
              }
              else if(yaw<-140)
              {
                Serial.print("faehrt in die entgegengesetzte Richtung nach rechts ");
              }
              else if(yaw<-120)
              {
                Serial.print("faehrt in die entgegengesetzte Richtung stark nach rechts ");
              }
              else if(yaw<-60)
              {
                Serial.print("wechselt die Richtung ");
              }
              else if(yaw<-40)
              {
                Serial.print("faehrt stark nach links ");
              }
              else if(yaw<-20) // war vorher auf 20
              {
                Serial.print("faehrt  nach links ");
                 
                

                  
                  if(NACHVORNEGEKIPPT == false && NACHHINTENGEKIPPT == false && KURVEEINLEITEN == false && KURVEABGESCHLOSSEN==false)
                  {
                                   
                 // faehrtNachLinks();
                  }      
 
                         
                  
                
              }
              else if(yaw <= 0)
              {
                Serial.print("faehrt gerade aus ");
                
                if(KURVEEINLEITEN == false)
                {
                  faehrtGeradeAus();
                  geradeAus = true;
                 
                }
                                                 // Achtung hier kann es vlt nicht funktionieren
              }
              Serial.print( yaw );
              
            }

            
           
            if(pitch > 9 && startOffset == true)
            {
              
              Serial.print(" ,ist nach hinten gekippt  ");
              
              nachHintenGekippt();
              NACHHINTENGEKIPPT = true;
              if(KURVEABGESCHLOSSEN == false)
              {
              setSpeedGeradeAusL();
              }
              else
              {
                EndeParcours = true;
              }
              
              
              NACHVORNEGEKIPPT = false;
              
              
              
            }
<<<<<<< HEAD
            else if(pitch > 6 && startOffset == true)
=======
            else if(pitch > 5 && startOffset == true)
>>>>>>> 619ba6bc72d5584a86b873e4976aa8e08c6e9887
            {
              Serial.print(" ,ist leicht nach hinten gekippt  ");
              if( treppeUeberwunden == true && KURVEABGESCHLOSSEN==false)
              {
                faehrtUeberVerschraenkung();            
              }
              else
              {
               // EndeParcours = true;
              }
            }
            else if(pitch < -8 && startOffset == true)   //    war auf -20   am 19.05.2017
            {
              
              Serial.print(" ,ist nach vorne gekippt ");
              if(KURVEABGESCHLOSSEN == false)
              {
            //nachVorneGekippt(); //*****************************************************************************************************************************
            faehrtGeradeAus();
            treppeUeberwunden = true;
            NACHVORNEGEKIPPT = true;
            NACHHINTENGEKIPPT = false;
            setSpeedGeradeAusH();
              }
              else
              {
               EndeParcours = true;
              }
             
            }
            else{
              Serial.print("  ");
              NACHVORNEGEKIPPT = false;
              NACHHINTENGEKIPPT = false;

              //getDistanz2 ist Sensor auf der Seite

              if(treppeUeberwunden == true && KURVEABGESCHLOSSEN == false)
              {
                if(test == 0)
                {
                if( parcours == 0 && abstandHintenLinks()> 30   ||parcours == 1 &&  abstandHintenRechts()>30 ) //************************** hier änderung parcours wahl
                {
                  Kurvenloop = true;
                  test++;
                  delay(800);
                }
                
                }

                
                if((Kurvenloop == true && KURVEABGESCHLOSSEN == false ) || KURVEEINLEITEN == true || SELBSTHALTUNG == true )
                {
                  SELBSTHALTUNG = true;
                  if(offsetDone == false)
                  {
                    startKurveOffset = 0;//getYAW();
                    offsetDone = true;
                  }
                  

                if(  ((yaw-startKurveOffset) > -100 && parcours == 0)||((yaw-startKurveOffset)  < 110 && parcours == 1)   && HALBEKURVEABGESCHLOSSEN == false) // war auf -85
                {  
                  KURVEEINLEITEN = true;
                 Serial.print("     Fahre Kurve !!!!!    ");
                //  while(getYAW() > -75 && HALBEKURVEABGESCHLOSSEN == false)
                  //{
                if(parcours == 0)
                {
                fahreKurveNachLinks();
                }
                else
                {
                  fahreKurveNachRechts();
                }
                 // }
               
                //fahreKurveNachRechts();
                
                
                }
                 else 
                {
                   
                KURVEEINLEITEN = false;
                if((abstandVorne()> 10||abstandVorne()== -1) && KURVEBEENDEN == false){ //war vorher auf 14
                  //delay(2000);
                  
                  while((abstandVorne()> 10||abstandVorne()== -1)){ //war vorher auf 14
                    setSpeedGeradeAusLL();
                    faehrtGeradeAus();
                    
                    
                    HALBEKURVEABGESCHLOSSEN = true;
                    Serial.println("     Fahre Gerade aus !!!!!    ");
                  }
                  
                 
                  
                }else if (KURVEABGESCHLOSSEN == false){
                  //while(getYAW()>-160/*KURVEABGESCHLOSSEN == false*/){
                  KURVEBEENDEN = true;

                  if(parcours == 0)
                  {
                  fahreKurveNachLinks();
                  }
                  else
                  {
                    fahreKurveNachRechts();
                  }
                   Serial.println("     Kurve beendennnnnnnn !!!!!    ");
                 // }
                  
                  }
                 
                
                  
                }
                
                if( HALBEKURVEABGESCHLOSSEN == true)
                {
                  
                }
                


                
                
                  
                }
               
                
               
                
              } // Treppe Überwunden aktiviert
              else
                  {
                    if(KURVEABGESCHLOSSEN == true)
                    {
                    /*  if((abstandVorne() == -1 || abstandVorne()<20 ) && yaw<-160 && TorKorrekrurNachLinks == false  ) // && yaw<-165    wurde entfernt
                      {
                    Serial.println("Kurve Fertig !!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                    setSpeedGeradeAusH();
                      }*/
                     /* else{
                        delay(2000);
                        TorKorrekrurNachLinks = true;
                        while((abstandVorne()==-1 || abstandVorne()<60)&& KeineKorrektur == false && abstandLinks()>30)
                        {
                        Serial.println("Fehler Tor Links");
                      fahreKurveNachRechts();
                      }
                      KeineKorrektur = true;
                    }*/
                  }

              
            }
            }
            
            
            if(roll > 20)
            {
              
              Serial.println(" und nach rechtsgekippt  ");
              VERSCHRAENKUNG = true;
            }
            else if(roll < -20)
            {
              Serial.println(" und nach linksgekippt  ");
              VERSCHRAENKUNG = true;
            }
            else{
             Serial.println(" ");
            
               
            }
            Serial.print(" erkannte Zahl : ");
            Serial.println(erkanntezahl);
            
      //  #endif



        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }

     if(softwareReset==0)
  {
    
 // if(geradeAus == false)
 // {
mpuKorrektur();

     //wdt_enable(WDTO_1S); 
     softwareReset = 1;
     //mpu.resetFIFO();
     STARTerfolgreich = false;
     myYAWOffset = getYAW();
     myPITCHOffset = getPITCH();
     myROLLOffset = getROLL();
     startOffset = true;
     Serial.println(   softwareReset);
     
//  }
  }

  
  Serial.print(  "softwareReset: ");
  Serial.println(   softwareReset);

}

int getYAW()
{
  return (int)(((ypr[0] * 180)/M_PI)-myYAWOffset);
}
int getPITCH()
{
  return (int) (((ypr[1] * 180)/M_PI)-myPITCHOffset);
}


int getROLL()
{
  return (int) (((ypr[2]*180)/M_PI)-myROLLOffset);
}





