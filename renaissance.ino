
/*
 Renaissance
 
 Monitor the status of front and back door switches and poll
 temperature from a thermistor. Tones are played when doors are
 opened and closed. The temperature and cumulative time each door
 has been left open is output on the serial.
 
  - Added Ehternet shield to report values to a webpage, buzzer moved to pin 8.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Back Door on pin 2
 * Front Door on pin 3
 * Buzzer on pin 8
 * 4K7 Thermistor & resistor as divider on A0
 
 Created Mar 1, 2016
 Richard Kelland
 
 */

#include <math.h>
#include <SPI.h>
#include <Ethernet.h>

#define THERM_PIN   0  // 10ktherm & 10k resistor as divider.

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xAD, 0x01, 0x04, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 2, 10);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

//globals
  int backDoorState = 0;
  int frontDoorState = 0;
  int lastbackDoorState = 0;
  int lastfrontDoorState = 0;
  unsigned long bkopentime;
  unsigned long ftopentime;
  unsigned long bkdeltatime;
  unsigned long bktotaltime;
  unsigned long ftdeltatime = 0;
  unsigned long fttotaltime = 0;
  unsigned long time;
  long Resistance;
  double Temp;  
  char* doorstatus[]={"<font color='blue'>Closed</font>", "<font color='red'><b>Open</b></font>"};

//runs after hitting reset
void setup(){  
  tone (8,500,200);
  Serial.println("=======VOID SETUP========");
  
  time = millis();
    
  //initialize serial
  Serial.begin(9600);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
 
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  backDoorState = digitalRead(2);
  frontDoorState = digitalRead(3);
  lastbackDoorState = backDoorState;
  lastfrontDoorState = frontDoorState;

  Serial.println("back:");
  Serial.println(backDoorState);
  Serial.println(lastbackDoorState);
  Serial.println("front:");
  Serial.println(frontDoorState);
  Serial.println(lastfrontDoorState);
  
  Serial.println("=======END VOID SETUP========");
  delay(200);
  tone (8,700,200);
  
}

//loop runs FORVEVER
void loop() {
  
    Resistance=((10240000/analogRead(THERM_PIN)) - 10000); 
    Temp = log(Resistance);
    Temp = 1 / (0.0012954816 + (0.00023539242 * Temp) + (0.00000011285038 * Temp * Temp * Temp)); 
    Temp = Temp - 273.15; 
    
    if (millis() - time >5000){   
    time=millis();
    Serial.print(Temp);
    Serial.print(char(186));
    Serial.println("C"); 
    }
    
  
  //readinput on digital pins 2 & 4
 backDoorState = digitalRead(2);
 frontDoorState = digitalRead(3);
// Serial.println("back:");
//  Serial.println(backDoorState);
//  Serial.println(lastbackDoorState);
//  Serial.println("front:");
//  Serial.println(frontDoorState);
//  Serial.println(lastfrontDoorState);
  
 //check backdoor state
 if (backDoorState != lastbackDoorState){
  //door is open
   if (backDoorState == HIGH){
       Serial.println("BackDoor Opened");
        bkopentime = millis();
         for (int i=0; i<4; i++){
          tone (8,911+100*i,100);
          delay(100);
         } 
         
   } else{
     // the back door was closed
     
    bkdeltatime = millis() - bkopentime;
    bktotaltime = bktotaltime + bkdeltatime;
     
    Serial.print("Back Door Closed");
    Serial.print(", door was open just now for ");
    Serial.print(bkdeltatime/1000);
    Serial.print("s and a total of ");
    Serial.print(bktotaltime/1000);
    Serial.println("s");

    for (int i=0; i<4; i++){
    tone (8,1311-100*i,100);
    delay(100);
     }
 }
 delay (50);
 
 }
 
 
  //check frontdoor state
 if (frontDoorState != lastfrontDoorState){
  //front door is open
   if (frontDoorState == HIGH){
       Serial.println("Front Door Opened");
       ftopentime = millis();
        for (int i=0; i<2; i++){
        tone (8,1011+300*i,200);
        delay(100);
         } 
   } else{
     // front door was closed
     
    ftdeltatime = millis()-ftopentime;
    fttotaltime = fttotaltime + ftdeltatime;
     
    Serial.print("Front Door Closed");
    Serial.print(", door was open just now for ");
    Serial.print(ftdeltatime/1000);
    Serial.print("s and a total of ");
    Serial.print(fttotaltime/1000);
    Serial.println("s");
    
     for (int i=0; i<2; i++){
     tone (8,1311-300*i,200);
     delay(100);
     }
 }
 delay (50);
  
}

 lastbackDoorState = backDoorState;
 lastfrontDoorState = frontDoorState;
 
   // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<title>Renaisance</title>");
          //add html detail here
          client.println("<h1>System Status</h1>");
          
          //Door Status
          client.println("<h2>Doors</h2>");
          client.print("<ul>");
          
          client.print("<li>Front Door</li><li><ul><li>Status: ");
          client.print(doorstatus[frontDoorState]);
          client.print("</li><li>Open time: ");
          client.print(fttotaltime/1000);
          client.println("s</li></ul></li>");
          
          client.print("<li>Back Door</li><li><ul><li>Status: ");
          client.print(doorstatus[backDoorState]);
          client.print("</li><li>Open time: ");
          client.print(bktotaltime/1000);
          client.println("s</li></ul></li>");
          
          
          client.print("</ul>");
          
          
          //Environment Sensors
          client.println("<h2>Environment</h2>");
          client.print("<ul><li>Temperature: ");
          client.print(Temp);
          client.print(char(186));
          client.print("C</li></ul>");
          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

}
