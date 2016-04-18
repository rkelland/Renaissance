/*
 Renaissance
 
 Monitor the status of front and back door switches and poll
 temperature from a thermistor. Tones are played when doors are
 opened and closed. The temperature and cumulative time each door
 has been left open is output on the serial.
 
  - Added Ethernet shield to report values to a webpage, buzzer moved to pin 8.
  - Removing webserver and changing to web client to send values to external server
  - Added capability for tracking door status
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Back Door on pin 2
 * Front Door on pin 3
 * DHT22 Temp & Humidity on pin 4
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
byte mac[] = { 0x00, 0x01, 0x04, 0xEF, 0xFE, 0xED };
//server is Trevor
IPAddress server(192, 168, 1, 115);

//Backup static IP
IPAddress ip(192, 168, 1, 130);

// Initialize the Ethernet server library
EthernetClient client;

//globals
  int backDoorState = 0;
  int frontDoorState = 0;
  int lastbackDoorState = 0;
  int lastfrontDoorState = 0;
  unsigned long bkopentime;
  unsigned long ftopentime;
  unsigned long bkdeltatime;
  unsigned long ftdeltatime = 0;
  unsigned long time = 0;
  unsigned long smoothTime = 0;
  long Resistance;
  double Temp;
  const int numSamples = 6;
  int samples[numSamples];
  int sampleIndex = 0;
  int total = 0;
  int avgSample = 0;
  //Time in millis for each reading corrected for drift.
  unsigned long sampleInterval = 1796700;

//runs after hitting reset
void setup(){  
  tone (8,500,200);
  
    //initialize serial
  Serial.begin(9600);
  Serial.println("=======VOID SETUP========");


  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  //Show ethenet status
    Serial.println("connecting...");
    Serial.print("Local IP: ");
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
 
    if((millis() - smoothTime) >= (sampleInterval/numSamples)-100){
      //reset timer
      smoothTime = millis();
      //Serial.print("SampleIndex: ");
      //Serial.println(sampleIndex);
      //Serial.print(" old total: ");
      //Serial.println(total);
      total = total - samples[sampleIndex];
      samples[sampleIndex] =  analogRead(THERM_PIN);
      //Serial.print("reading: ");
      //Serial.println(samples[sampleIndex]);
      total = total + samples[sampleIndex];
      //Serial.print(" new total: ");
      //Serial.println(total);
      sampleIndex = sampleIndex+1;
    }//end if smoothTime
  
  //send temperature info to database base on sampleInterval time
    if (millis() - time >= sampleInterval){   
  
      //reset timers and index
      time=millis();
      smoothTime = millis();
      sampleIndex = 0;
      
      avgSample = total/numSamples;
      
      Serial.print("Average Sample:");
      Serial.println(avgSample);
      
      Resistance=((10240000/avgSample) - 10000); 
      Temp = log(Resistance);
      Temp = 1 / (0.0012954816 + (0.00023539242 * Temp) + (0.00000011285038 * Temp * Temp * Temp)); 
      Temp = Temp - 273.15; 
 
      Serial.print(Temp);
      Serial.print(char(186));
      Serial.println("C");
 
      report_enviro_to_db ("baement", Temp, 0);   
 
    }  //end if - temp timer
    
  //Check door status - read input on digital pins 2 & 3
 backDoorState = digitalRead(2);
 frontDoorState = digitalRead(3);
  
 //check backdoor state
 if (backDoorState != lastbackDoorState) {

   if (backDoorState == HIGH){
       //door is open
       Serial.println("BackDoor Opened");
        
        //record door open time
        bkopentime = millis();
        
        //play door tone
        for (int i=0; i<4; i++){
          tone (8,911+100*i,100);
          delay(100);
        }
   
   report_zone_to_db("back", 1, 0);
   
            }//end if door open  
   
   else{
     // the back door was closed
    bkdeltatime = millis() - bkopentime;
     
    Serial.print("Back Door Closed");
    Serial.print(", door was open just now for ");
    Serial.print(bkdeltatime/1000);
    Serial.println("s");    
    
    //play tone
    for (int i=0; i<4; i++){
    tone (8,1311-100*i,100);
    delay(100);
     }
     
  report_zone_to_db("back", 0, bkdeltatime);
       
 }
 }//end check backdoor state
 
  //check frontdoor state
 if (frontDoorState != lastfrontDoorState){
  //front door is open
   if (frontDoorState == HIGH){
       Serial.println("Front Door Opened");
       ftopentime = millis();
        
        //play tone
        for (int i=0; i<2; i++){
        tone (8,1011+300*i,200);
        delay(100);
         } 
         
  report_zone_to_db("front", 1, 0);
         
   } else{
     // front door was closed
     
    ftdeltatime = millis()-ftopentime;
     
    Serial.print("Front Door Closed");
    Serial.print(", door was open just now for ");
    Serial.print(ftdeltatime/1000);
    Serial.println("s");
    
    //play tone
     for (int i=0; i<2; i++){
     tone (8,1311-300*i,200);
     delay(100);
     }
     
   report_zone_to_db("front", 0, ftdeltatime);
 }
  
}//end check front door state

 lastbackDoorState = backDoorState;
 lastfrontDoorState = frontDoorState;

}

void report_zone_to_db (String doorName, int zoneState, unsigned long deltaTime){
  
      if (client.connect(server, 80)) {
        Serial.print("Connected...sending zone report for ");
        Serial.println(doorName);
        client.print( "GET /ren/add.php?db=");
        client.print("zones");
        client.print("&&doorname=");
        client.print( doorName);
        client.print("&&event=");
        client.print( zoneState );
        client.print("&&deltatime=");
        client.print(deltaTime);
        client.println( " HTTP/1.1");
        client.println( "Host: 192.168.1.115" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );
        client.println( "Connection: close" );
        client.println();
        client.println();
        client.stop();
      } //end if
      else
      {
      Serial.print("Error connecting to client for ");
      Serial.println(doorName);
      }//end else
}

void report_enviro_to_db (String name, double temp, double humi){
  
      if (client.connect(server, 80)) {
        Serial.print("Connected...sending enviro reading for");
        Serial.println(name);
        client.print( "GET /ren/add.php?db=");
        client.print("enviro");
        client.print("&&name=");
        client.print(name);
        client.print("&&temp=");
        client.print( temp );
        client.print("&&humi=");
        client.print( humi );
        client.println( " HTTP/1.1");
        client.println( "Host: 192.168.1.115" );
        client.println( "Content-Type: application/x-www-form-urlencoded" );
        client.println( "Connection: close" );
        client.println();
        client.println();
        client.stop();
      } //end if
      else
      {
      Serial.print("Error connecting to client for ");
      Serial.println(name);
      }//end else
}
