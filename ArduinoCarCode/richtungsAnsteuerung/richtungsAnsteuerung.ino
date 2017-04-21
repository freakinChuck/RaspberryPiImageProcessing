#include "MPUheader.h"
#include <avr/io.h> 
#include <avr/wdt.h>

int parcours = 0; //0 = Parcours rechts    1 = Parcours Links
boolean STARTerfolgreich = true;
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
int messung = 0;
int erkanntezahl = 0;
int softwareReset = 0;
int myOffset = 0;





void setup() {

  
  mpuSetup();
  zahlenSetup();
  motorSetup();
  ultraschallSetup();
  ampelSetup();
     


}

void loop() {

 

// hier Ampelerkennungs Methode einsetzen

/* ampelErkennung();
 if(messung<=0)
 {
 if(0 != zahlenErkennung())
 {

  delay(1000);
  erkanntezahl = zahlenErkennung();
  messung ++;
 
  
  
 }
 }*/
 
 


  

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



        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
         
               float yaw = ((ypr[0] * 180)/M_PI)-myOffset;
            
            //float yawGefiltert = yaw -yawKalibriert;
            float pitch = (ypr[1] * 180)/M_PI;
            float roll = (ypr[2] * 180)/M_PI;
            //Serial.print("yaw\t");
            Serial.print("Das Fahrzeug  ");

            if(yaw>0)
            {
            
              //Ab hier muss mit dem grössten Zustand begonnen werden
              if(yaw>170)
              {
                Serial.print("faehrt in die entgegengesetzte Richtung ");
                if(KURVEBEENDEN == true)
                {
                KURVEABGESCHLOSSEN = true;
                SELBSTHALTUNG = false;
                }
                
                
                 faehrtGeradeAus();
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
              if(yaw<-170)
              {
                Serial.print("faehrt in die entgegengesetzte Richtung ");
                if(KURVEBEENDEN == true)
                {
                KURVEABGESCHLOSSEN = true;
                SELBSTHALTUNG = false;
                }
               
                 faehrtGeradeAus();
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

            
           
            if(pitch > 15)
            {
              
              Serial.print(" ,ist nach hinten gekippt  ");
              
              nachHintenGekippt();
              NACHHINTENGEKIPPT = true;
              setSpeedGeradeAusL();
              
              
              NACHVORNEGEKIPPT = false;
              
              
              
            }
            else if(pitch > 10)
            {
              Serial.print(" ,ist leicht nach hinten gekippt  ");
              if( treppeUeberwunden == true)
              {
                faehrtUeberVerschraenkung();
              }
            }
            else if(pitch < -1)
            {
              Serial.print(" ,ist nach vorne gekippt ");
            nachVorneGekippt();
            treppeUeberwunden = true;
            NACHVORNEGEKIPPT = true;
            NACHHINTENGEKIPPT = false;
            setSpeedGeradeAusH();
            }
            else{
              Serial.print("  ");
              NACHVORNEGEKIPPT = false;
              NACHHINTENGEKIPPT = false;

              //getDistanz2 ist Sensor auf der Seite

              if(treppeUeberwunden == true && KURVEABGESCHLOSSEN == false)
              {

                
                if((abstandLinks()> 30 && KURVEABGESCHLOSSEN == false ) || KURVEEINLEITEN == true || SELBSTHALTUNG == true )
                {
                  SELBSTHALTUNG = true;

                if(yaw > -75 && HALBEKURVEABGESCHLOSSEN == false) // war auf -85
                {  
                fahreKurveNachLinks();
               
                //fahreKurveNachRechts();
                
                KURVEEINLEITEN = true;
                 Serial.print("     Fahre Kurve !!!!!    ");
                }
                 else 
                {
                   
                KURVEEINLEITEN = false;
                if((abstandVorne()> 18||abstandVorne()== -1) && KURVEBEENDEN == false){ //war vorher auf 20
                  //delay(2000);
                  
                  while((abstandVorne()> 18||abstandVorne()== -1)){ //war vorher auf 20
                    setSpeedGeradeAusLL();
                    faehrtGeradeAus();
                    
                    
                    HALBEKURVEABGESCHLOSSEN = true;
                    Serial.println("     Fahre Gerade aus !!!!!    ");
                  }
                  
                 
                  
                }else if (KURVEABGESCHLOSSEN == false){
                  //while(getYAW()>-160/*KURVEABGESCHLOSSEN == false*/){
                  KURVEBEENDEN = true;
                  fahreKurveNachLinks();
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
                      if((abstandVorne() == -1 || abstandVorne()>20 )&& yaw<-170 && TorKorrekrurNachLinks == false  )
                      {
                    Serial.println("Kurve Fertig !!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                    setSpeedGeradeAusH();
                      }
                      else{
                        TorKorrekrurNachLinks = true;
                        while((abstandVorne()==-1 || abstandVorne()<50)&& KeineKorrektur == false && abstandLinks()>30)
                        {
                        Serial.println("Fehler Tor Links");
                      fahreKurveNachRechts();
                      }
                      KeineKorrektur = true;
                    }
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
            
        #endif



        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }

     if(softwareReset==0)
  {
  if(geradeAus == false)
  {
     //wdt_enable(WDTO_1S); 
     softwareReset = 1;
     //mpu.resetFIFO();
     STARTerfolgreich = false;
     myOffset = getYAW();
     Serial.println(   softwareReset);
     
  }
  }

  
  Serial.print(  "softwareReset: ");
  Serial.println(   softwareReset);

}

int getYAW()
{
  return (int)(ypr[0] * 180)/M_PI;
}

