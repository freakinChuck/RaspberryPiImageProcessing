// GF --- Smart Robot Car ---
// Playlist: https://www.youtube.com/playlist?list=PLRFnGJH1nJiJxoO0woBW6vl_8URTQPhfL
// Smart Robot Car: Part 4 - DC Motors & L298N Dual Motor Controller 
// Wiring & Video Demo: https://www.youtube.com/watch?v=0RLAivgppBM

// connect motor controller pins to Arduino digital pins
// motor one
int enA = 34;
int in1 = 12;
int in2 = 11;
// motor two
int enB = 35;
int in3 = 10;
int in4 = 9;

int speedGeradeAus = 55;
int speedKurve = 100;






void motorSetup()
{
pinMode(enA, OUTPUT);
pinMode(enB, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);


}

void setSpeedGeradeAusL()
{
  speedGeradeAus = 65;
}




void setSpeedGeradeAusLL()
{
  speedGeradeAus = 30;
}


void setSpeedGeradeAusH()
{
  speedGeradeAus = 80;
}

void faehrtNachRechts()
{
  

                   digitalWrite(in1,LOW);
                   analogWrite(in2, 120);
                   analogWrite(in3, 120);
                   digitalWrite(in4, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
}

void faehrtNachLinks()
{
                   analogWrite(in1, 120);
                   digitalWrite(in2,LOW);                 
                   digitalWrite(in3, LOW);
                   analogWrite(in4, 120);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
}

void faehrtUeberVerschraenkung()
{
                
                   analogWrite(in1, 150);
                   digitalWrite(in2,LOW);
                   analogWrite(in3, 70);
                   digitalWrite(in4, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
}

void faehrtGeradeAus()
{
                   analogWrite(in1, speedGeradeAus);
                   digitalWrite(in2,LOW);
                   analogWrite(in3, speedGeradeAus);
                   digitalWrite(in4, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
}

void faehrtEGeradeAus()
{
                   analogWrite(in1, 100);
                   digitalWrite(in2,LOW);
                   analogWrite(in3, 100);
                   digitalWrite(in4, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
}

void nachHintenGekippt()
{
                   analogWrite(in1, 80);
                   digitalWrite(in2,LOW);
                   analogWrite(in3, 80);
                   digitalWrite(in4, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
}

void nachVorneGekippt()
{
                   analogWrite(in2, 5);
                   digitalWrite(in1,LOW);
                   analogWrite(in4, 5);
                   digitalWrite(in3, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
}


 void fahreKurveNachRechts()
{

 
  
                   analogWrite(in1, speedKurve);
                   digitalWrite(in2,LOW);
                   digitalWrite(in3, LOW);
                   analogWrite(in4, speedKurve);
                   // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
                 
                
            

  
              
} 



void fahreKurveNachLinks()
{
  
             


                   digitalWrite(in1, LOW);
                   analogWrite(in2,speedKurve); // waren auf 100
                   analogWrite(in3, speedKurve);
                   digitalWrite(in4, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
                 
                
             
  
}

void nachLinksGekippt()
{
                   analogWrite(in1, 120);
                   digitalWrite(in2,LOW);
                   analogWrite(in3, 80);
                   digitalWrite(in4, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, HIGH);
                  digitalWrite(enA, HIGH);
                  //delay(250);
}

void stopMotor()
{
  
                   digitalWrite(in1, LOW);
                   digitalWrite(in2,LOW);
                   digitalWrite(in4, LOW);
                   digitalWrite(in3, LOW);
                // set speed to 200 out of possible range 0~255
                  digitalWrite(enB, LOW);
                  digitalWrite(enA, LOW);
  
}

