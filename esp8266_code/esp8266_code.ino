#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
 

String data;
String I;
 
String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors
const char tok[] = ",";
 
 
 
void setup()
{
  // Debug console
  Serial.begin(9600);
 
   // timer.setInterval(1000L,sensorvalue1); 
 
}
 
void loop()
{
 
   
  if (Serial.available() > 0 ) 
  {
    rdata = Serial.read(); 
    myString = myString+ rdata; 
  // Serial.print(rdata);
    if( rdata == '\n')
    {
   I = getValue(myString, '-', 0);
    myString = "";

     Serial.print("data=");
     Serial.println(I);
    }
    
  }
}

 
void sensorvalue1()
{
data = data + I; 
//lcd.print(0,0,"pH Value:");
// lcd.print(0,1,data);
 data = "";
 
}
  
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
