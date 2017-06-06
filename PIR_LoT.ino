
#include <SPI.h>
#include <Ethernet.h>
/* 
 *Javier Palacios Garcia -1525286
 *Jose David Vargas - 1529176
 *Jhon Hadder Diaz -1533586
*/
//Variables ThingSpeak
//Internet
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; 
long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;
EthernetClient client; //inicializamos
//ThingSpeak
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "ZIS9NINXB4CWZNUU"; 
String Time;

//Variables
//el tiempor que se le da al sensor para calibrar.
int calibrationTime = 30;        

//El tiempo que el sesonr tira una señal "low"
long unsigned int lowIn;         

//Tiempo que deber de estar en señal "low" para que podamos decir que ya no hay movimento
long unsigned int Pause = 8000;

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 3;    //Pin del sensor
int ledPin = 12;   //Pin del Led

void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(pirPin, LOW);
  startEthernet();
  

  //Calibracion
  Serial.print("Calibrando Sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" Listo ");
    Serial.println("Sensor Activo");
    delay(50);
   

  }


void loop(){
 
 while (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
  if (client.connected() && lastConnected)
  { 
    Serial.println("...disconnected");
    Serial.println();
      client.stop();
  }
   
 
  if (failedCounter > 3 ) {startEthernet();}
  
  lastConnected = client.connected();
  
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   
       if(lockLow){  
         lockLow = false;            
         Serial.println("---");
         Serial.print("Movimiento Detectado a los ");
         Serial.print(millis()/1000);     
         Serial.println(" sec");
         Time = String(0);
         updateThingSpeak("field1="+Time); // Update ThingSpeak
         delay(5);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){        

       if(takeLowTime){
        lowIn = millis();          
        takeLowTime = false;       
        }
       if(!lockLow && millis() - lowIn > Pause){  
           lockLow = true;                        
           Serial.print("Movimineto detenido a los ");
           Serial.print((millis() - Pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
           digitalWrite(ledPin, LOW); 
       }
  }

void startEthernet()
{
  
  client.stop();

  Serial.println("Connecting Arduino to network...");
  Serial.println();  

  delay(1000);
  
  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
  }
  else
  {
    Serial.println("Arduino connected to network using DHCP");
    Serial.println();
  }
  
  delay(1000);
}

void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {         
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    
    
    if (client.connected())
    {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();
      
      failedCounter = 0;
    }
    else
    {
      failedCounter++;
  
      Serial.println("Connection to ThingSpeak failed ("+String(failedCounter, DEC)+")");   
      Serial.println();
    }
    
  }
  else
  {
    failedCounter++;
    
    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");   
    Serial.println();
    
    
  }
}

  
