#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define WIFI_SSID       ""
#define WIFI_PASS       ""
#define REDPIN 13
#define GREENPIN 12
#define BLUEPIN 2
#define SERVER_NAME "DESKTOP-U2DK7RL.local" //name of device node.js server on
#define SERVER_PORT 3000 //port used by local node.js server

/*
 * Global variables
 */
ESP8266WebServer server(80);

/*
 * Function prototypes
 */
void setup(void);
void loop(void);
void ConnectWifi();
void SetupMDNS(String);
void sendPost(String,String);
void HandleLED();
void HandleRoot();

 
void setup() {

  String hostName = String(WiFi.macAddress());  //use MAC address for domain name to ensure uniqueness
  hostName.replace(":","-"); //change : for - 
  Serial.begin(115200);

  ConnectWifi();
  SetupMDNS(hostName); 

  //routes
  server.on("/LED", HTTP_POST, HandleLED);
  server.on("/", HTTP_GET, HandleRoot);
  server.begin(); 

  MDNS.addService("http", "tcp", 80);

  sendPost("/init", "name="+hostName);
  
  //set pins and set strip to blue
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  analogWrite(BLUEPIN, 255);

}

void loop() {
  server.handleClient();
  MDNS.update();
}

/*
 * connect to wifi
*/
void ConnectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

/*
 * send http post to local server
 */
void sendPost(String route,String msg){
  HTTPClient http;
  
  String address = String("http://"+String(SERVER_NAME)+":"+String(SERVER_PORT)+route);
  
  http.begin(address);      //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Serial.println("msg: "+msg);
  
  int httpCode = http.POST(msg);   //Send the request
  String payload = http.getString();                  //Get the response payload
 
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
 
  http.end();  //Close connection     
}


/*
 * setup mdns responder to be domainName.local
 */
void SetupMDNS(String domainName){
  
  if (!MDNS.begin(domainName)) {             // Start the mDNS responder
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

}

/*
 * handle posts to /LED
 */
void HandleLED() {                         
  Serial.println(server.arg("plain"));         
  server.send(200);                         
}

/*
 * hande get to /
 */
void HandleRoot() {                         
  Serial.println("recieved");         
  server.send(200);                         
}
