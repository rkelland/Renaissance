/*
 Renaissance
 
 Monitor the status of front and back door switches and poll
 temperature from a thermistor. Tones are played when doors are
 opened and closed. The temperature and cumulative time each door
 has been left open is output on the serial.
 
 Circuit:
 * Back Door on pin 2
 * Front Door on pin 3
 * Buzzer on pin 10
 * 4K7 Thermistor & resistor as divider on A0
 
 Created Mar 1, 2016
 Richard Kelland
 
 */




#include <math.h>

#define THERM_PIN   0  // 4K7ktherm &  resistor as divider.


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


//runs after hitting reset
void setup(){  
  
  //initialize serial
  time = millis();
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  
  backDoorState = digitalRead(2);
  frontDoorState = digitalRead(3);
  lastbackDoorState = backDoorState;
  lastfrontDoorState = frontDoorState;
  
  tone (10,500,900);
  delay(900);
  tone (10,700,200);
  Serial.println("=======VOID SETUP========");
  Serial.println("back:");
  Serial.println(backDoorState);
  Serial.println(lastbackDoorState);
  Serial.println("front:");
  Serial.println(frontDoorState);
  Serial.println(lastfrontDoorState);
  
}

//loop runs FORVEVER
void loop() {
  
    Resistance=((10240000/analogRead(THERM_PIN)) - 10000); 
    Temp = log(Resistance);
    Temp = 1 / (0.0012954816 + (0.00023539242 * Temp) + (0.00000011285038 * Temp * Temp * Temp)); 
    Temp = Temp - 273.15; 
    
    if (millis() - time >5000){   
    time=millis();
    Serial.println(Temp);
    //Serial.print(int(Temp)); Serial.print("."); Serial.println(Temp-int(Temp));
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

}
