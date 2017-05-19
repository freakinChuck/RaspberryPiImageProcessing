// Stellen Pins:  1ste: 46;  2te: 47;  3te: 48

int ersteStellePin = 46;
int zweiteStellePin = 47;
int dritteStellePin = 48;
int erkannteZahl = 0;
int s1,s2,s3 = 0;
int durchlauf = 0;


void zahlenSetup()
{
  pinMode(ersteStellePin, INPUT);
  pinMode(zweiteStellePin, INPUT);
  pinMode(dritteStellePin, INPUT);
  
}


int zahlenErkennung()
{
  s1  = digitalRead(ersteStellePin);
  s2 = digitalRead(zweiteStellePin);
  s3 = digitalRead(dritteStellePin);
  

  erkannteZahl = ( s1 + (s2 << 1) + (s3 << 2));

/*  Serial.print("s1 = ");
  Serial.println(s1);
  Serial.print("s2 = ");
  Serial.println(s2);
  Serial.print("s3 = ");
  Serial.println(s3);
  
  Serial.print(" erkannte Zahl : ");
  Serial.println(erkannteZahl);*/
  return erkannteZahl;
  
}



