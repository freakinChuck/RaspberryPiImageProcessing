// Bluetooth sketch HC-06_02
// Turn a LED on and off from an Android app
// App can be downloaded from www.martyncurrey.com
// 
// Pins
// 2 Software serial - RX
// 3 Software serial - TX
// 12 LED
 
boolean debug = true;
 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(14,15); // RX | TX
// Connect the HC-06 TX to the Arduino RX. 
// Connect the HC-06 RX to the Arduino TX through a voltage divider.
 
// max length of command is 20 chrs
const byte numChars = 20;
char receivedChars[numChars];
boolean newData = false;
 
 
 
void bluetoothSetup() 
{
  
 pinMode(LED_BUILTIN, OUTPUT);
     //pinMode(LEDpin, OUTPUT); 
     //Serial.begin(115200);     // *************************************3 wird das gebraucht ?
     Serial.println("<Arduino is ready>");
 
     // The default baud rate for the HC-06s I have is 9600. Other modules may have a different speed. 38400 is common.
     BTserial.begin(9600); 
}
 
void bluetotthPrint() 
{
     if (BTserial.available() > 0)     {  recvWithStartEndMarkers(); }
     if (newData) { parseData();
     
      }
}     
 
 
void parseData()
{  
        newData = false;    
        if (debug) {  Serial.println( receivedChars ); }
        if (receivedChars[0] == 'O'  && receivedChars[1] == 'N' )  { digitalWrite(LED_BUILTIN,HIGH); }
        if (receivedChars[0] == 'O'  && receivedChars[1] == 'F' )  { digitalWrite(LED_BUILTIN,LOW);    }       
}
 
 
void recvWithStartEndMarkers() 
{
 
     // function recvWithStartEndMarkers by Robin2 of the Arduino forums
     // See  http://forum.arduino.cc/index.php?topic=288234.0
 
     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '<';
     char endMarker = '>';
     char rc;
 
     if (BTserial.available() > 0) 
     {
          rc = BTserial.read();
          if (recvInProgress == true) 
          {
               if (rc != endMarker) 
               {
                    receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx >= numChars) { ndx = numChars - 1;}
               }
               else 
               {
                     receivedChars[ndx] = '\0'; // terminate the string
                     recvInProgress = false;
                     ndx = 0;
                     newData = true;
               }
          }
 
          else if (rc == startMarker) { recvInProgress = true; }
     }
}
