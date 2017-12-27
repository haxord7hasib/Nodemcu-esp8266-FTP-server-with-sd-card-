#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266FtpServer.h>
#include <SPI.h>
#include <SD.h>

const char* ssid = "xx";
const char* password = "awse2313";
 
ESP8266WebServer server(80);
FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
 
 
 
void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
 
}
 
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
 
  server.on("/", handleRoot);
 
  server.onNotFound(handleNotFound);
 
  server.begin();
  Serial.println("HTTP server started");
 
  /////FTP Setup, ensure SPIFFS is started before ftp;  /////////
  
  //if (SD.begin(4,48000000)) 
  if (SD.begin(4,48000000)) 
  {
      Serial.println("SD opened!");
      //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
      ftpSrv.begin("esp8266","esp8266");    
  }    
  
 /* if (SPIFFS.begin()) {
      Serial.println("SPIFFS opened!");
      //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
      ftpSrv.begin("esp8266","esp8266");    
  } */
}
 
void loop(void){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!  
  server.handleClient();
 
}
