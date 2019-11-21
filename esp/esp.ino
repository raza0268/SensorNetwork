#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int Led_OnBoard = 2;                  // Initialize the Led_OnBoard 

const char* ssid = "infinix IOT";                  // Your wifi Name       
const char* password = "eab171d1a0b7";          // Your wifi Password

const char *host = "192.168.43.15"; //Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  pinMode(Led_OnBoard, OUTPUT);       // Initialize the Led_OnBoard pin as an output
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(Led_OnBoard, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(Led_OnBoard, HIGH);
    delay(250);
    
  }

  digitalWrite(Led_OnBoard, HIGH);
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.println("Connected to Network/SSID");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

   //temperature code
      Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  HTTPClient http;    //Declare object of class HTTPClient
 
  String LdrValueSend, postData,nodeId,projectId,latlng,s1,s2,s3,s4;
 // int ldrvalue=analogRead(A0);  //Read Analog value of LDR
 // LdrValueSend = String(ldrvalue);   //String to interger conversion

  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }



  //int ldrvalue=analogRead(A0);  //Read Analog value of LDR
 // LdrValueSend = String(ldrvalue);   //String to interger conversion
 
  //Post Data
  //postData = String(tempC);
  nodeId = String(1);
 String node = "nodeId=" + nodeId;
 Serial.print(node);
 
  projectId = String(1);
   String project = "projectId=" + projectId;
 Serial.print(project);

  latlng = String("31.447943-74.269402");
   String longlat = "latlng=" + latlng;
   Serial.print(longlat);

  s1=String(tempC);
  String ss1 = "s1=" + s1;
   Serial.print(ss1);

  s2=String("");
  String ss2 = "s2=" + s2;
   Serial.print(ss2);

  s3=String("");
  String ss3 = "s3=" + s3;
   Serial.print(ss3);

  s4= String("");
  String ss4 = "s4=" + s4;
   Serial.print(ss4);


  postData="data="+ node+","+project+","+longlat+","+ss1+","+ss2+","+ss3+","+ss4;
 
  //Post Data
  //postData = String(tempC);
   Serial.println(postData);    //Print request response payload

  http.begin("http://192.168.43.15/Sensor_network/Api/Api_project/insertData");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  //httpResponseCode = http.POST("date=dayStamp&timeis=timeStamp");
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  //Serial.println("LDR Value=" + ldrvalue);
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  //Serial.println("LDR Value=" + LdrValueSend);
  
  http.end();  //Close connection

  delay(10000);  //Here there is 4 seconds delay plus 1 second delay below, so Post Data at every 5 seconds
  digitalWrite(Led_OnBoard, LOW);
  delay(1000);
  digitalWrite(Led_OnBoard, HIGH);
}
