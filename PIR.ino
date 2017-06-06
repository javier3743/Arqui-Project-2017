
/* 
 *Javier Palacios Garcia -1525286
 *Jose David Vargas - 1529176
 *Jhon Hadder Diaz -1533586
*/

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

     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   
       if(lockLow){  
         lockLow = false;            
         Serial.println("---");
         Serial.print("Movimiento Detectado a los ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
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
