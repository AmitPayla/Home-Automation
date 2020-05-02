#include <ESP8266WiFi.h>

const char* ssid = "Mind Mapperz";//type your ssid
const char* password = "@mindmapperz@1";//type your password
boolean   last;
int relayPin = 5; // D1 of ESP8266
int led = 15; // D8 of ESP8266
int button = 4,i=0,j=0,k=0,currentstate=0,previousstate=0,manualbutton=0,l=0,m=0,n=0,flag=0; // D2 of ESP8266
WiFiServer ESPserver(80);//Service Port

void setup() 
{
Serial.begin(115200);
pinMode(relayPin, OUTPUT);
pinMode(led,OUTPUT);
pinMode(button,INPUT);

digitalWrite(led,HIGH);
Serial.println();
Serial.println();
Serial.print("Connecting to: ");
Serial.println(ssid);

WiFi.begin(ssid, password);
delay(3000);
/*
 The following four line of the 
 code will assign a Static IP Address to 
 the ESP Module. If you do not want this, 
 comment out the following four lines.  
 */

IPAddress ip(192,168,0,100); //ip(192,168,1,4); for home  
IPAddress gateway(192,168,1,4);//gateway(192,168,1,1); for home   
IPAddress subnet(255,255,255,0);   
WiFi.config(ip, gateway, subnet);
delay(2000);

while (WiFi.status() != WL_CONNECTED) 
{
delay(100);
Serial.print("*");
}
Serial.println("");
Serial.println("WiFi connected");

// Start the server
ESPserver.begin();
Serial.println("Server started");

// Print the IP address
Serial.print("The URL to control ESP8266: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
digitalWrite(relayPin, LOW);
delay(5);
digitalWrite(led,LOW);

}

void loop() 
{
  //Serial.println("hello");
while (WiFi.status() != WL_CONNECTED) 
{
delay(100);
Serial.print("*");
k=1;
digitalWrite(led,HIGH);
delay(200);
digitalWrite(led,LOW);
}
if(k==1)
{
Serial.println("");
Serial.println("WiFi connected");

// Start the server
ESPserver.begin();
Serial.println("Server started");
digitalWrite(led,LOW);
}
k=0;
buttoncall();

// Check if a client has connected
WiFiClient client = ESPserver.available();
if (!client) 
{
return;
}
// Wait until the client sends some data
Serial.println("New Client");
while(!client.available())
{
 buttoncall(); 
delay(1);
}

// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

// Match the request

int value = LOW;
if (request.indexOf("/RELAYON") != -1)
{
Serial.println("LAMP is ON");
digitalWrite(relayPin, LOW);
delay(2);
value = LOW;
i=1;
j=0;
} 
if (request.indexOf("/RELAYOFF") != -1)
{
Serial.println("LAMP is OFF");
digitalWrite(relayPin, HIGH);
delay(2);
value = HIGH;
j=1;
i=0;
}

// Return the response
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); //  IMPORTANT
//client.println("<!DOCTYPE HTML>");
//client.println("<html>");

if(value==LOW)
{
  client.println("ON");
  Serial.println("mON");
}
if(value==HIGH)
{
  client.println("OFF");
  Serial.println("mOFF");
}
delay(1);
//client.stop();
Serial.println("Client disconnected");
Serial.println("");
}


void buttoncall()
{
  currentstate=digitalRead(button);
if(currentstate==0&&i==0&&j==0&&l==0)
{
digitalWrite(relayPin,HIGH);
previousstate=0;
//Serial.println("OFF");
}
else
if(currentstate==1&&i==0&&j==0&&l==0)
{
  digitalWrite(relayPin,LOW);
  previousstate=1;
  Serial.println("ON");
}
if(previousstate!=currentstate)
{
  l=1;
  if(i==1)
  {
    digitalWrite(relayPin,HIGH);
    flag=1;
    last=HIGH;
    i=0;
  Serial.println("OFF");
  }
  if(j==1)
{
  digitalWrite(relayPin,LOW);
  flag=1;
  last=LOW;
  j=0;
  Serial.println("ON");
}
previousstate=currentstate;

if(flag==0)
{
 // if(currentstate==1){
 last= !last;
  digitalWrite(relayPin,last);
  
  Serial.println(last);
  //else{
  delay(10);
  //digitalWrite(relayPin,HIGH);
  //Serial.println("OFF");}
}
  flag=0;
 
}
}
