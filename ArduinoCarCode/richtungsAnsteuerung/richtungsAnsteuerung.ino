#include "MPUheader.h"
#include <avr/io.h> 
#include <avr/wdt.h>

//***********************************************************************************************************************************************************************************************************************************
//****************************************************************************************************** Variabeln die von Hand gesetzt werden müssen********************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int parcours = 0; //0 = Parcours rechts    1 = Parcours Links
int ampelerkennung = 1;    //Ampelerkennung ausgeschaltet = 0, eingeschaltet = 1
int zahlenerkennung = 1;   //Zahlenerkennung ausgeschaltet = 0, eingeschaltet = 1
int startSignalPin = 45;


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Variabeln Positionsbestimmung************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************



boolean EndeParcours = false;
boolean Kurvenloop = false;
boolean KURVEBEENDEN = false;
boolean SELBSTHALTUNG = false;
boolean NACHHINTENGEKIPPT = false;
boolean NACHVORNEGEKIPPT = false;
boolean treppeUeberwunden = false;
boolean VERSCHRAENKUNG = false;
boolean KURVEEINLEITEN = false;
boolean  HALBEKURVEABGESCHLOSSEN = false;
boolean KURVEABGESCHLOSSEN = false;
boolean  geradeAus = false;
boolean offsetDone = false;
boolean startOffset = false;
boolean hasAlreadyHitThe179er = false;


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Variabeln Driftkorrektur*****************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int antiDrift = 0;
int driftTime = 0;
int antiDcount = 0;


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Variabeln Stepper Motor******************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int motorBlockiert = 1;


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Variabeln Zahlenerkennung****************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int messung = 0;   // gehört zu Zahlenerkennung
int erkanntezahl = 0;


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Variabeln Offset*************************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int softwareReset = 0;
int myYAWOffset = 0;
int myROLLOffset = 0;
int myPITCHOffset = 0;
int startKurveOffset = 0;

int parcoursLinksOffset = 0;


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Variabeln Schlussausrichtung*************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


int absCnt = 0;
int abstandRes = 0;
int abstandGemessen = 0;



//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Diverse Variabeln************************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************

int verzoegerungKurvenfahrt = 0;
int parcoursInfo = 0;





//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************SET UP***********************************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************






void setup() {

  
  mpuSetup();
  zahlenSetup();
  motorSetup();
  ultraschallSetup();
  ampelSetup();
  umschalterSetup();
   tasteSetup();
  parcours = schalterstellung();
  pinMode(startSignalPin, OUTPUT);


}


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Start LOOP************************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************





void loop() {


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Fixieren des Drückers********************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************

if(motorBlockiert == 1)
{

  blockiereMotor();

}

else
{
  loeseMotor();
}



//***********************************************************************************************************************************************************************************************************************************
//***********************************************************************************************Definieren eines Offsets für der Parcours mit Kurve nach Rechts*********************************************************************
//***********************************************************************************************************************************************************************************************************************************

  
  if(parcours == 1)
  {
    parcoursLinksOffset = 4;
  }


 
//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Serielleausgabe zu Parcourswahl**********************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


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
 
  setRomanNumber(erkanntezahl); 
  
 }
 }
}
else
{
   setRomanNumber(3); 
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
         
            float yaw = ((ypr[0] * 180)/M_PI)-myYAWOffset - antiDrift  +parcoursLinksOffset  ;            
            //float yawGefiltert = yaw -yawKalibriert;
            float pitch = (ypr[1] * 180)/M_PI-myPITCHOffset;
            float roll = (ypr[2] * 180)/M_PI-myROLLOffset;
            //Serial.print("yaw\t");

           if(driftTime %100 == 0 && antiDcount <12)
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
              if(yaw>185 || (hasAlreadyHitThe179er&& yaw > 120))
              {
                hasAlreadyHitThe179er = true;

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
                 /* if( abstandHintenRechts()>20 )
                  {

                    setParcours();
                    
                     for(;absCnt < 10;absCnt++)
                     {

                      stopMotor();

                      

                    abstandRes =  abstandVorneLinks();

                    delay(100);
                      
                     }

                    
                      if(abstandGemessen == 0)
                      {
                     setAbstandVorhanden(abstandRes);
                      moveMotor ();
                      delay(1000);
                      abstandGemessen++;
                      }
                     
                         if(abstandVorne()> 4 )
                   {
                    faehrtGeradeAus();
                   }
                   else
                   {
                    while(1)
                    {
                    stopMotor();
                    }
                   }
                   
                 
                  }*/
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
              if(yaw<-163 || (hasAlreadyHitThe179er && yaw < -120))  //******************************************************* parcours rechts
              {
                hasAlreadyHitThe179er = true;

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
                 if(parcours == 0 && abstandHintenLinks()>20  ||  parcours == 1 && abstandHintenRechts()>20 )      //*************************hier änderung für parcours wahl
                  {
                    motorBlockiert = 0;
                   // loeseMotor();
                    if(parcours==0)
                    {
                      
                     for(;absCnt < 10;absCnt++)
                     {

                      stopMotor();

                      

                    abstandRes =  abstandVorneRechts();

                    delay(100);
                      
                     }

                    
                      if(abstandGemessen == 0)
                      {
                     setAbstandVorhanden(abstandRes);
                      moveMotor ();
                      delay(1000);
                      abstandGemessen++;
                      }
                     
                         if(abstandVorne()> 4 )
                   {
                    faehrtGeradeAus();
                   }
                   else
                   {
                    while(1)
                    {
                    stopMotor();
                    }
                   }
                   

                  }

                  else
                  {


                      //setParcours();
                    
                     for(;absCnt < 10;absCnt++)
                     {

                      stopMotor();

                      

                    abstandRes =  abstandVorneLinks();

                    delay(100);
                      
                     }

                    
                      if(abstandGemessen == 0)
                      {
                     setAbstandVorhanden(abstandRes);
                      moveMotor ();
                      delay(1000);
                      abstandGemessen++;
                      }
                     
                         if(abstandVorne()> 4 )
                   {
                    faehrtGeradeAus();
                   }
                   else
                   {
                    while(1)
                    {
                    stopMotor();
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
              setSpeedGeradeAusLT();
              }
              else
              {
                EndeParcours = true;
              }

              
              
              
              NACHVORNEGEKIPPT = false;
              
              
              
            }
            else if(pitch > 6 && startOffset == true)
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
            nachVorneGekippt(); //*****************************************************************************************************************************
            //faehrtGeradeAus();
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
                if(verzoegerungKurvenfahrt == 0)
                {
                if( parcours == 0 && abstandHintenLinks()> 30   ||parcours == 1 &&  abstandHintenRechts()>30 ) //************************** hier änderung parcours wahl
                {
                  Kurvenloop = true;
                  verzoegerungKurvenfahrt++;
                  delay(1000);
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
                if((abstandVorne()> 8||abstandVorne()== -1) && KURVEBEENDEN == false){ //war vorher auf 14
                  //delay(2000);
                  
                  while((abstandVorne()> 8||abstandVorne()== -1)){ //war vorher auf 14
                    setSpeedGeradeAusLL();
                    faehrtGeradeAus();
                    
                    
                    HALBEKURVEABGESCHLOSSEN = true;
                    Serial.println("     Fahre Gerade aus !!!!!    ");
                  }
                  
                 
                  
                }
                else if (KURVEABGESCHLOSSEN == false)
                {
                  
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
                                   
                  }
                 
                
                  
                }
                
               


                
                
                  
                }
               
                
               
                
              } // Treppe Überwunden aktiviert
              else
                  {
                    if(KURVEABGESCHLOSSEN == true)
                    {
                
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


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************Software Reset***************************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************

     if(softwareReset==0)
  {    
     softwareReset = 1;
     myYAWOffset = getYAW();
     myPITCHOffset = getPITCH();
     myROLLOffset = getROLL();
     startOffset = true;
     Serial.println(   softwareReset);
     
  }

  
  Serial.print(  "softwareReset: ");
  Serial.println(   softwareReset);

}


//***********************************************************************************************************************************************************************************************************************************
//******************************************************************************************************getter Methoden**************************************************************************************************************
//***********************************************************************************************************************************************************************************************************************************


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




