#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <mDNSResolver.h>

#define WIFI_SSID       ""
#define WIFI_PASS       ""
#define REDPIN 13
#define GREENPIN 12
#define BLUEPIN 2
#define NAME_TO_RESOLVE "DESKTOP-U2DK7RL.local"


WiFiUDP udp;
mDNSResolver::Resolver resolver(udp);
IPAddress ip;
HTTPClient http;

void setup() {
  
  Serial.begin(115200);

  //connect to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  
  //find ip address of local server
  Serial.print("Resolving ");
  Serial.println(NAME_TO_RESOLVE);
  int res=0;
  while(res==0){
    //use mDns to find our local server
    resolver.setLocalIP(WiFi.localIP());
  
    ip = resolver.search(NAME_TO_RESOLVE);
    if(ip != INADDR_NONE) {
      Serial.print("Resolved: ");
      Serial.println(ip);
      res=1;
    } else {
      Serial.println("Not resolved");
    }
  }
  
  //test connection to local server
  String addr = String("http://"+ip.toString()+":3000/test");
  http.begin(addr);      //Specify request destination
  http.addHeader("Content-Type", "text/plain");
 
  int httpCode = http.POST("Message from ESP8266");   //Send the request
  String payload = http.getString();                  //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection

  //set pins and set strip to blue
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  analogWrite(BLUEPIN, 255);


}

void loop() {
  // put your main code here, to run repeatedly:

}
