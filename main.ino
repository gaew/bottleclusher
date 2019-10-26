#include "HX711.h"
#include <Servo.h>
//loadcell bottle
const int LOADCELL_DOUT_PIN = A2;
const int LOADCELL_SCK_PIN = A3;

//loadcell cover
const int LOADCELL2_DOUT_PIN = A4;
const int LOADCELL2_SCK_PIN = A5;

const int trigPin = 11;
const int echoPin = 10;

HX711 scale_b(A2, A3);
HX711 scale_c(A4, A5);

float calibration_factor = 2400; 
float units;
float ounces;

Servo myservo; 
Servo myservo2;

void setup() {  
  pinMode(12, OUTPUT);//clash pin relay
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(9600);
 // scale_b.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
 // scale_c.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);
  myservo.attach(9);
  myservo2.attach(8);
  
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), full, HIGH);
  
}

void loop() {
  myservo.write(100); 
  digitalWrite(12, HIGH);
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    switch (inByte) {
      case '$':
                
        int bt_status = chk_bottle(); 
        int cv_status = chk_cover();
               
        // Serial.print("Bottle status :"); 
        //Serial.println(bt_status);          
        // Serial.print("Cover status :"); 
        // Serial.println(cv_status);         
        //Serial.print("Bottle size :"); 

         //Serial.println(bt_status);

        if (bt_status>0){
          size_bottle();  
        }else {
           Serial.println("9");
        }
                  
        take_bottle(bt_status);
        take_cover(cv_status);           

        ///////////////
        break;
      case 'b':
        // take_bottle(1);
        //take_cover(1);
        break;
     
      default:
        Serial.print("");
    }
  }
}


void full() {
  while(digitalRead(2)){
    Serial.println("full");
    delay(3000);
  }
  
}


int chk_bottle(){ 
   delay (100);
   int sent_back;

  scale_b.set_scale(calibration_factor);
 // Serial.print("Reading bottle : ");
  units = scale_b.get_units(), 10;//59
  if (units < 0)
  {
    units = 0.00;
  }
  ounces = units * 0.035274;
  //Serial.println(units);

        if (units<90) {
                //Serial.println("yes");
                sent_back = 1;
         } else {
          sent_back = 0;
          
         }    
  
  return sent_back;
}

int chk_cover(){ 
   delay (100);
   int sent_back;


  scale_c.set_scale(calibration_factor);
  //Serial.print("Reading cover: ");
  units = scale_c.get_units(), 10; //71
  if (units < 0)
  {
    units = 0.00;
  }
  ounces = units * 0.035274;
  //Serial.println(units);

     if (units<75) {
                //Serial.println("yes");
                sent_back = 1;
         } else {
          sent_back = 0;
         }    

  return sent_back;
}

int size_bottle(){ 
  long duration;
  int distance, bsize;

   delay (1000);      
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance= duration*0.034/2;

      //Serial.println(distance);

      if (distance<10){
        bsize = 3; //large
        Serial.println("3");
      }else if (distance<20){
        bsize = 2; //middle
        Serial.println("2");
      }else if (distance<25){
        bsize = 1; //small
        Serial.println("1");
      }else {
        Serial.println("9");
        take_bottle(0);///////////blank
      }
      
  return bsize;

}

void take_bottle(int x){ 
  
  if (x<1){
     myservo.write(0); 
        delay (500);
  }else if(x>0){
     myservo.write(180);      
     delay (500);
     myservo.write(100);    
     delay (15);
     
  }
   myservo.write(100);    
     delay (15);
    
}

void take_cover(int x){ 
  
  if (x<1){
     myservo2.write(0); 
        delay (500);
  }else if(x>0){
     myservo2.write(180);      
     delay (500);
     myservo2.write(90);    
     delay (15);
    
  }
   myservo2.write(90);    
     delay (15);

     //clash();
    
}

void clash(){   
  digitalWrite(12, LOW);
  delay(5000); 
  digitalWrite(12, HIGH);
}




   
