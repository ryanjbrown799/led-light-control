#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <mDNSResolver.h>
#include <ESP8266WebServer.h>

#define WIFI_SSID       ""
#define WIFI_PASS       ""
#define REDPIN 13
#define GREENPIN 12
#define BLUEPIN 2
#define NAME_TO_RESOLVE "DESKTOP-U2DK7RL.local"

ESP8266WebServer server(80);  

WiFiUDP udp;
mDNSResolver::Resolver resolver(udp);
IPAddress ip;

//connect to wifi
void ConnectWifi(){
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
}

//use mDns to resolve ip address of local server
void FindLocalServer(){
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
}

//send http post to local server
void sendPost(String route,String msg){
  HTTPClient http;
  
  String addr = String("http://"+ip.toString()+":3000"+route);
  
  http.begin(addr);      //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Serial.println("msg: "+msg);
  
  int httpCode = http.POST(msg);   //Send the request
  String payload = http.getString();                  //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection    
  
}

void handleLED() {                         
  Serial.println(server.arg("plain"));         
  server.send(200);                         
}

void setup() {
  
  Serial.begin(115200);

  ConnectWifi();
  FindLocalServer();

  //send mac and ip to server
  String info = String("mac="+WiFi.macAddress()+"&"+"ip="+WiFi.localIP().toString());
  sendPost("/test", info);
  
  server.on("/LED", HTTP_POST, handleLED);
  server.begin(); 
  //set pins and set strip to blue
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  analogWrite(BLUEPIN, 255);


}

void loop() {
  server.handleClient();

}
