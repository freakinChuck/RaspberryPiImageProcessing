#include "MPUheader.h"


boolean NACHHINTENGEKIPPT = false;
boolean NACHVORNEGEKIPPT = false;
boolean treppeUeberwunden = false;
boolean VERSCHRAENKUNG = false;
boolean KURVEEINLEITEN = false;
boolean  HALBEKURVEABGESCHLOSSEN = false;
boolean KURVEABGESCHLOSSEN = false;



void setup() {
  mpuSetup();

 
  motorSetup();
  ultraschallSetup();
     


}

void loop() {


  

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
            
            float yaw = (ypr[0] * 180)/M_PI;
            //float yawGefiltert = yaw -yawKalibriert;
            float pitch = (ypr[1] * 180)/M_PI;
            float roll = (ypr[2] * 180)/M_PI;
            //Serial.print("yaw\t");
            Serial.print("Das Fahrzeug  ");

            if(yaw>0)
            {
            
              //Ab hier muss mit dem grössten Zustand begonnen werden
              if(yaw>160)
              {
                Serial.print("faehrt in die entgegengesetzte Richtung ");
                KURVEABGESCHLOSSEN = true;
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
                                 
                    if(NACHVORNEGEKIPPT == false && NACHHINTENGEKIPPT == false && KURVEEINLEITEN == false)
                  {
                   faehrtNachRechts();
                  }
                              
              }
              else if(yaw >= 0)
              {
                Serial.print("faehrt gerade aus ");

                if(KURVEEINLEITEN == false)
                {
               faehrtGeradeAus();
                }
              
                    
                   
               
              }
              Serial.print( yaw );
              
            }
            else if(yaw<0)
            {
              if(yaw<-160)
              {
                Serial.print("faehrt in die entgegengesetzte Richtung ");
                KURVEABGESCHLOSSEN = true;
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
                 
                

                  
                  if(NACHVORNEGEKIPPT == false && NACHHINTENGEKIPPT == false && KURVEEINLEITEN == false)
                  {
                                   
                  faehrtNachLinks();
                  }      

                         
                  
                
              }
              else if(yaw <= 0)
              {
                Serial.print("faehrt gerade aus ");
                
                if(KURVEEINLEITEN == false)
                {
                  faehrtGeradeAus();
                 
                }
                                                 // Achtung hier kann es vlt nicht funktionieren
              }
              Serial.print( yaw );
              
            }

            
           
            if(pitch > 20)
            {
              
              Serial.print(" ,ist nach hinten gekippt  ");
              nachHintenGekippt();
              NACHHINTENGEKIPPT = true;
              NACHVORNEGEKIPPT = false;
              
              
              
            }
            else if(pitch < -5)
            {
              Serial.print(" ,ist nach vorne gekippt ");
            nachVorneGekippt();
            treppeUeberwunden = true;
            NACHVORNEGEKIPPT = true;
            NACHHINTENGEKIPPT = false;
            }
            else{
              Serial.print("  ");
              NACHVORNEGEKIPPT = false;
              NACHHINTENGEKIPPT = false;

              if(treppeUeberwunden == true )//&& VERSCHRAENKUNG == true)
              {

                
                if(getDistanz()> 30 || KURVEEINLEITEN == true )
                {

                if(yaw > -80)
                {  
                fahreKurveNachLinks();
                
                KURVEEINLEITEN = true;
                 Serial.print("     Fahre Kurve !!!!!    ");
                }
                 else 
                {
                   
                KURVEEINLEITEN = false;
                if(getDistanz2() <30){
                  while(getDistanz2() < 30){
                    faehrtGeradeAus();
                    HALBEKURVEABGESCHLOSSEN = true;
                    Serial.print("     Fahre Gerade aus !!!!!    ");
                  }
                }else if (KURVEABGESCHLOSSEN == false){
                  fahreKurveNachLinks();
                   Serial.print("     Kurve fertiiiiiiiiiiigggggg !!!!!    ");
                  
                  
                  }
                
                  
                }
                
                if( HALBEKURVEABGESCHLOSSEN == true)
                {
                  
                }
                


                
                
                  
                }
               
                
               
                
              } // Treppe Überwunden aktiviert

              
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
            //Serial.println(ypr[2] * 180/M_PI);
            
        #endif



        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }

}

int getYAW()
{
  return (int)(ypr[0] * 180)/M_PI;
}

