#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <ArduinoJson.h>

String lat_str , lng_str;

//variables to get data from arduino
String I;
String v;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int Led_OnBoard = 2;                  // Initialize the Led_OnBoard 

//const char* ssid = "waqar";                  // Your wifi Name       
//const char* password = "22419968";          // Your wifi Password

const char* ssid = "infinix IOT";                  // Your wifi Name       
const char* password = "eab171d1a0b7";          // Your wifi Password

//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = "AIzaSyAFBbJRwrhJ5tQQjRpo_GsyqFznv3Nw6Ag";

int status = WL_IDLE_STATUS;
String jsonString = "{\n";

double latitude    = 0.0;
double longitude   = 0.0;
double accuracy    = 0.0;
int more_text = 1; // set to 1 for more debug output



//wifi connection function start
void connection_wifi(){
  
   WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
   delay(1000);
// WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

   // Set WiFi to station mode and disconnect from an AP if it was previously connected
   WiFi.mode(WIFI_STA);
   WiFi.disconnect();
   delay(100);
  
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
}
//wifi coonection function ends

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  //pinMode(Led_OnBoard, OUTPUT);       // Initialize the Led_OnBoard pin as an output
  Serial.begin(9600);
  
 // digitalWrite(Led_OnBoard, HIGH);
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.println("Connected to Network/SSID");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

   //temperature code
      Serial.println("Getting Sensors Data");

   // Start up the library
      sensors.begin();
  }

void loop() {

  connection_wifi();
  // put your main code here, to run repeatedly:
  HTTPClient http;    //Declare object of class HTTPClient
 
  String LdrValueSend, postData,nodeId,projectId,latlng,s1,s2,s3,s4;

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

 //////////////////////////////////////////////
 
 /////////////////////////////location Data///////////////////////////

  char bssid[6];
  DynamicJsonBuffer jsonBuffer;
  Serial.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found...");

    if (more_text) {
      // Print out the formatted json...
//      Serial.println("{");
//      Serial.println("\"homeMobileCountryCode\": 234,");  // this is a real UK MCC
//      Serial.println("\"homeMobileNetworkCode\": 27,");   // and a real UK MNC
//      Serial.println("\"radioType\": \"gsm\",");          // for gsm
//      Serial.println("\"carrier\": \"Vodafone\",");       // associated with Vodafone
//      //Serial.println("\"cellTowers\": [");                // I'm not reporting any cell towers
//      //Serial.println("],");
//      Serial.println("\"wifiAccessPoints\": [");
      for (int i = 0; i < n; ++i)
      {
//        Serial.println("{");
//        Serial.print("\"macAddress\" : \"");
//        Serial.print(WiFi.BSSIDstr(i));
//        Serial.println("\",");
//        Serial.print("\"signalStrength\": ");
//        Serial.println(WiFi.RSSI(i));
        if (i < n - 1)
        {
          Serial.println("},");
        }
        else
        {
          Serial.println("}");
        }
      }
      Serial.println("]");
      Serial.println("}");
    }
    Serial.println(" ");
  }
  // now build the jsonString...
  jsonString = "{\n";
  jsonString += "\"homeMobileCountryCode\": 234,\n"; // this is a real UK MCC
  jsonString += "\"homeMobileNetworkCode\": 27,\n";  // and a real UK MNC
  jsonString += "\"radioType\": \"gsm\",\n";         // for gsm
  jsonString += "\"carrier\": \"Vodafone\",\n";      // associated with Vodafone
  jsonString += "\"wifiAccessPoints\": [\n";
  for (int j = 0; j < n; ++j)
  {
    jsonString += "{\n";
    jsonString += "\"macAddress\" : \"";
    jsonString += (WiFi.BSSIDstr(j));
    jsonString += "\",\n";
    jsonString += "\"signalStrength\": ";
    jsonString += WiFi.RSSI(j);
    jsonString += "\n";
    if (j < n - 1)
    {
      jsonString += "},\n";
    }
    else
    {
      jsonString += "}\n";
    }
  }
  jsonString += ("]\n");
  jsonString += ("}\n");
  //--------------------------------------------------------------------

  Serial.println("");

  WiFiClientSecure client;
  client.setInsecure();

  //Connect to the client and make the api call
  Serial.print("Requesting URL: ");
  Serial.println("https://" + (String)Host + thisPage + key);
  Serial.println(" ");
  if (client.connect(Host, 443)) {
    Serial.println("Connected");
    client.println("POST " + thisPage + key + " HTTP/1.1");
    client.println("Host: " + (String)Host);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(jsonString.length());
    client.println();
    client.print(jsonString);
    delay(500);
  }else{
    Serial.println("Not Connected");
  }

  //Read and parse all the lines of the reply from server
  while (client.available()) {
    String line = client.readStringUntil('\r');
    if (more_text) {
      Serial.print(line);
    }
    JsonObject& root = jsonBuffer.parseObject(line);
    if (root.success()) {
      latitude    = root["location"]["lat"];
      longitude   = root["location"]["lng"];
      accuracy   = root["accuracy"];
    }
  }

  Serial.println("closing connection");
  Serial.println();
  client.stop();

if(client.available()){
  Serial.print("Latitude = ");
  Serial.println(latitude, 6);
  Serial.print("Longitude = ");
  Serial.println(longitude, 6);
}
//  Serial.print("Accuracy = ");
//  Serial.println(accuracy);

/////////////////////////////////location data end///////////////////////////////


  //Post Data
  //postData = String(tempC);
  nodeId = String(1);
 String node = "nodeId=" + nodeId;
 Serial.print(node);
 
  projectId = String(1);
   String project = "projectId=" + projectId;
 Serial.print(project);

//  latlng = String("31.447943-74.269402");
//value = propertyValue.ToString("G17");


//lng_str=longitude.ToString("G10");
lat_str=String(latitude,7);
lng_str=String(longitude,7);

   latlng = lat_str+"-"+lng_str;
   String longlat = "latlng=" + latlng;
   Serial.print(longlat);

  s1=String(tempC);
  String ss1 = "s1=" + s1;
   Serial.print(ss1);

  s2=String("-1--1");
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

  delay(1000);  //Here there is 4 seconds delay plus 1 second delay below, so Post Data at every 5 seconds
  digitalWrite(Led_OnBoard, LOW);
  delay(1000);
  digitalWrite(Led_OnBoard, HIGH);
}
