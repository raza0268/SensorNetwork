#include <stdlib.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <ArduinoJson.h>
SoftwareSerial nodemcu(2,3);

String jsonString;
#define senseInput 1 //Set to A0 as Analog Read
int senseRawValue; //Some variable
float senseTurbidity; //Some floating variable
 
#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
 
// for float value to string converstion
int f;
  float val; // also works with double. 
  char buff2[10];
  char buff3[10];
  String valueString = "";
  String Value = ""; 
  String valueString1 = "";
  String Value1 = ""; 
  
void setup()
{
  pinMode(13,OUTPUT);  
  Serial.begin(9600);  
  nodemcu.begin(9600); 
//turbidity code
  Serial.println(" 14CORE | TURBIDITY SENSING ANALOG TEST CODE ");
  Serial.println("Initializing.................................");
  delay(2000);
}
void loop()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  
     Value =  dtostrf(phValue, 4, 2, buff2);  //4 is mininum width, 6 is precision
   valueString = valueString + Value +"-"; 
   Serial.println(valueString);
 //  nodemcu.println(valueString);

 delay(1000);

//turbidity code
 senseRawValue = analogRead(senseInput); //Read input raw value fromt the sensor
senseTurbidity = senseRawValue * (5.0 / 1024.0); //Convert analog data from 0 -1024 to voltage 0 - 5v;
Serial.println("TURBIDITY VALUE > "); //Print the output data to the serial
Serial.println(senseTurbidity);

  Value1 =  dtostrf(senseTurbidity, 4, 2, buff3);  //4 is mininum width, 6 is precision
   valueString1 = valueString1 + Value1; 
   Serial.println(valueString1);
   jsonString = valueString;
   //jsonString += ",";
  jsonString += valueString1;
  Serial.println(jsonString);
   nodemcu.println(jsonString);
     valueString = "";
  valueString1 = "";
  
delay(3000);


 
}
