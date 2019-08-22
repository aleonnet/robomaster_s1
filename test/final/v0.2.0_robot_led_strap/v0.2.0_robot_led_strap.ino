#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
#include <FastLED.h>

#define NUM_LEDS 12
#define DATA_PIN 10
#define BRIGHTNESS  64


#define motorA_A 2
#define motorA_B 3
#define motorB_A 4
#define motorB_B 5
#define motorC_A 6
#define motorC_B 7
#define motorD_A 8
#define motorD_B 9

#define blasterTiltApin A2 //servo
#define blasterTiltBpin A3 //servo
#define blaster A4 //relay
#define plate A5
#define laser A7

RF24 radio(A0, A1); // CE, CSN
const byte address[6] = "01001";
char team = 'B';//A->蓝队； B->红队


struct dataStruct {
  unsigned long _micros;
  bool btnA;
  bool btnB;
  bool btnC;
  int joyStick_A_X;
  int joyStick_A_Y;
  int joyStick_B_X;
  int joyStick_B_Y;
} myData;

CRGB leds[NUM_LEDS];
int score = 6;
unsigned long plate_debounce = millis();
unsigned long blaster_debounce=millis();
unsigned long shootTimer=millis();
bool shootFlag = 0;
bool laserToggle = 0;
int tiltAngle = 90;
Servo blasterTiltA, blasterTiltB;



void setup() {
  Serial.begin(9600);
  for (int i = 2; i < 10; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(plate, INPUT_PULLUP);
  pinMode(laser, OUTPUT);
  digitalWrite(laser, HIGH);
  blasterServoSetup();
  radioSetup();
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  ledAnimation();
}

void loop() {
  getData();
  
  if(!digitalRead(plate) && millis()-plate_debounce >500){
    plate_debounce = millis();
    if(score>0)  {
      score--;  
    }
    if(score==0){
      lightUp(score);
      FastLED.show();
      delay(500);
      score=6;
      gameOver();
    }
  }
  lightUp(score);
  FastLED.show();
  
}



void getData() { //获取数据内容并打印在屏幕上
  if (radio.available()) {
    radio.read( &myData, sizeof(myData) );
    if(myData.joyStick_A_X+myData.joyStick_A_Y+myData.joyStick_B_X+myData.joyStick_B_Y+myData.btnA+myData.btnB != 0){
      if(myData.joyStick_B_Y<400){
        robotMove(9);
      }else if(myData.joyStick_B_Y>700){
        robotMove(10);
      }else if(myData.joyStick_A_X<400 && myData.joyStick_A_Y>700){//left forward
        robotMove(7);
      }else if(myData.joyStick_A_X<400 && myData.joyStick_A_Y<400){//left backward
        robotMove(6);
      }else if(myData.joyStick_A_X>700 && myData.joyStick_A_Y>700){//right forward
        robotMove(5);
      }else if(myData.joyStick_A_X>700 && myData.joyStick_A_Y<400){//right backward
        robotMove(8);
      }else if (myData.joyStick_A_Y>700) {
        robotMove(1);
      }else if(myData.joyStick_A_Y<400){
        robotMove(2);
      }else if(myData.joyStick_A_X<400){
        robotMove(3);
      }else if(myData.joyStick_A_X>700){
        robotMove(4);
      }else {
        robotMove(0);
      }
      
      if(myData.joyStick_B_X < 350){
        if(tiltAngle<90){
          tiltAngle +=1;
        }
      }else if(myData.joyStick_B_X > 750){
        if(tiltAngle>55){
          tiltAngle -=1;
        }
      }
      blasterTiltA.write(tiltAngle);
      blasterTiltB.write(180 - tiltAngle);
      
      if(myData.btnC){
        if(shootFlag == 0){
          shootFlag = 1;
          shootTimer = millis();  
        }
        if(millis()-shootTimer<1000){
          digitalWrite(blaster, HIGH);
        }else{
          digitalWrite(blaster, LOW);
        }
        
      }else{
        shootTimer=millis();
        shootFlag = 0;
        digitalWrite(blaster, LOW);
      }
      if(myData.btnA){
        if(laserToggle){
          digitalWrite(laser, LOW);  
        }else{
          digitalWrite(laser, HIGH);
        }
        
      }
    }else{
      robotMove(0);
    }
    
    

     //打印内容
        Serial.print(myData.btnA);
        Serial.print(" ");
        Serial.print(myData.btnB);
        Serial.print(" ");
        Serial.print(myData.btnC);
        Serial.print(" ");
        Serial.print(myData.joyStick_A_X);
        Serial.print(" ");
        Serial.print(myData.joyStick_A_Y);
        Serial.print(" ");
        Serial.print(myData.joyStick_B_X);
        Serial.print(" ");
        Serial.print(myData.joyStick_B_Y);
        Serial.println(" ");

  }
}

void blasterServoSetup(){
  pinMode(blaster, OUTPUT);
  digitalWrite(blaster, LOW);
  pinMode(blasterTiltApin, OUTPUT);
  pinMode(blasterTiltBpin, OUTPUT);
  blasterTiltA.attach(blasterTiltApin);
  blasterTiltB.attach(blasterTiltBpin);
  blasterTiltA.write(90);
  blasterTiltB.write(90); ;
}


//in mode{0 = no action, 1 = foward, 2 = backward}
void AMove(int mode)
{
  if (mode == 0) {
    digitalWrite(motorA_A, LOW);
    digitalWrite(motorA_B, LOW);
  }
  else if (mode == 1) {
    digitalWrite(motorA_A, HIGH);
    digitalWrite(motorA_B, LOW);
  }
  else if (mode == 2) {
    digitalWrite(motorA_A, LOW);
    digitalWrite(motorA_B, HIGH);
  }
}

void BMove(int mode)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(motorB_A, LOW);
    digitalWrite(motorB_B, LOW);
  }
  else if (mode == 1) {
    digitalWrite(motorB_A, HIGH);
    digitalWrite(motorB_B, LOW);
  }
  else if (mode == 2) {
    digitalWrite(motorB_A, LOW);
    digitalWrite(motorB_B, HIGH);
  }
}
void CMove(int mode)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(motorC_A, LOW);
    digitalWrite(motorC_B, LOW);
  }
  else if (mode == 1) {
    digitalWrite(motorC_A, HIGH);
    digitalWrite(motorC_B, LOW);
  }
  else if (mode == 2) {
    digitalWrite(motorC_A, LOW);
    digitalWrite(motorC_B, HIGH);
  }
}
void DMove(int mode)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(motorD_A, LOW);
    digitalWrite(motorD_B, LOW);
  }
  else if (mode == 1) {
    digitalWrite(motorD_A, HIGH);
    digitalWrite(motorD_B, LOW);
  }
  else if (mode == 2) {
    digitalWrite(motorD_A, LOW);
    digitalWrite(motorD_B, HIGH);
  }
}
void robotMove(int Direction)
{
  switch (Direction) {  // 0 = stop, 1 = forward....5 =
    case 0://stop
      AMove(0);
      BMove(0);
      CMove(0);
      DMove(0);
      break;
    case 1://forward
      AMove(1);
      BMove(1);
      CMove(1);
      DMove(1);
      break;
    case 2://backward
      AMove(2);
      BMove(2);
      CMove(2);
      DMove(2);
      break;
    case 3://left shift
      AMove(2);
      BMove(1);
      CMove(2);
      DMove(1);
      break;
    case 4://right shift
      AMove(1);
      BMove(2);
      CMove(1);
      DMove(2);
      break;
    case 5://right forward
      AMove(1);
      BMove(0);
      CMove(1);
      DMove(0);
      break;
    case 6://left backward
      AMove(2);
      BMove(0);
      CMove(2);
      DMove(0);
      break;
    case 7://left forward
      AMove(0);
      BMove(1);
      CMove(0);
      DMove(1);
      break;
    case 8: //right backward
      AMove(0);
      BMove(2);
      CMove(0);
      DMove(2);
      break;
    case 9://spinning right
      AMove(2);
      BMove(2);
      CMove(1);
      DMove(1);
      break;
    case 10://spinning left
      AMove(1);
      BMove(1);
      CMove(2);
      DMove(2);
      break;
  }
}
void radioSetup() { //RF24L01接收端初始化
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
}


int lightUp(int num){
  if(team == 'A'){
    switch(num){
      case 0:
         leds[0] = CRGB::Black; 
         leds[1] = CRGB::Black; 
         leds[2] = CRGB::Black; 
         leds[3] = CRGB::Black; 
         leds[4] = CRGB::Black; 
         leds[5] = CRGB::Black;
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Black; 
         leds[9] = CRGB::Black; 
         leds[10] = CRGB::Black; 
         leds[11] = CRGB::Black;  
         break;
      case 1:
         leds[0] = CRGB::Blue; 
         leds[1] = CRGB::Black; 
         leds[2] = CRGB::Black; 
         leds[3] = CRGB::Black; 
         leds[4] = CRGB::Black;
         leds[5] = CRGB::Black;  
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Black; 
         leds[9] = CRGB::Black; 
         leds[10] = CRGB::Black; 
         leds[11] = CRGB::Blue; 
         break;
       case 2:
         leds[0] = CRGB::Blue; 
         leds[1] = CRGB::Blue; 
         leds[2] = CRGB::Black; 
         leds[3] = CRGB::Black; 
         leds[4] = CRGB::Black;
         leds[5] = CRGB::Black;
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Black; 
         leds[9] = CRGB::Black; 
         leds[10] = CRGB::Blue; 
         leds[11] = CRGB::Blue;   
         break;
       case 3:
         leds[0] = CRGB::Blue; 
         leds[1] = CRGB::Blue; 
         leds[2] = CRGB::Blue; 
         leds[3] = CRGB::Black; 
         leds[4] = CRGB::Black; 
         leds[5] = CRGB::Black; 
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Black; 
         leds[9] = CRGB::Blue; 
         leds[10] = CRGB::Blue; 
         leds[11] = CRGB::Blue;
         break;
       case 4:
         leds[0] = CRGB::Blue; 
         leds[1] = CRGB::Blue; 
         leds[2] = CRGB::Blue; 
         leds[3] = CRGB::Blue; 
         leds[4] = CRGB::Black;
         leds[5] = CRGB::Black;
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Blue; 
         leds[9] = CRGB::Blue; 
         leds[10] = CRGB::Blue; 
         leds[11] = CRGB::Blue;  
         
         break;
       case 5:
         leds[0] = CRGB::Blue; 
         leds[1] = CRGB::Blue; 
         leds[2] = CRGB::Blue; 
         leds[3] = CRGB::Blue; 
         leds[4] = CRGB::Blue; 
         leds[5] = CRGB::Black; 
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Blue; 
         leds[8] = CRGB::Blue; 
         leds[9] = CRGB::Blue; 
         leds[10] = CRGB::Blue; 
         leds[11] = CRGB::Blue;
         break;
     case 6:
         leds[0] = CRGB::Blue; 
         leds[1] = CRGB::Blue; 
         leds[2] = CRGB::Blue; 
         leds[3] = CRGB::Blue; 
         leds[4] = CRGB::Blue;  
         leds[5] = CRGB::Blue; 
         leds[6] = CRGB::Blue; 
         leds[7] = CRGB::Blue; 
         leds[8] = CRGB::Blue; 
         leds[9] = CRGB::Blue; 
         leds[10] = CRGB::Blue; 
         leds[11] = CRGB::Blue; 
         break;
     case 7:
         leds[0] = CRGB::Red; 
         leds[1] = CRGB::Red; 
         leds[2] = CRGB::Red; 
         leds[3] = CRGB::Red; 
         leds[4] = CRGB::Red; 
         leds[5] = CRGB::Red;
         leds[6] = CRGB::Red; 
         leds[7] = CRGB::Red; 
         leds[8] = CRGB::Red; 
         leds[9] = CRGB::Red; 
         leds[10] = CRGB::Red; 
         leds[11] = CRGB::Red; 
         break;
    }  
  }else if(team == 'B'){
    switch(num){
      case 0:
         leds[0] = CRGB::Black; 
         leds[1] = CRGB::Black; 
         leds[2] = CRGB::Black; 
         leds[3] = CRGB::Black; 
         leds[4] = CRGB::Black; 
         leds[5] = CRGB::Black;
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Black; 
         leds[9] = CRGB::Black; 
         leds[10] = CRGB::Black; 
         leds[11] = CRGB::Black;  
         break;
      case 1:
         leds[0] = CRGB::Red; 
         leds[1] = CRGB::Black; 
         leds[2] = CRGB::Black; 
         leds[3] = CRGB::Black; 
         leds[4] = CRGB::Black;
         leds[5] = CRGB::Black;  
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Black; 
         leds[9] = CRGB::Black; 
         leds[10] = CRGB::Black; 
         leds[11] = CRGB::Red; 
         break;
       case 2:
         leds[0] = CRGB::Red; 
         leds[1] = CRGB::Red; 
         leds[2] = CRGB::Black; 
         leds[3] = CRGB::Black; 
         leds[4] = CRGB::Black;
         leds[5] = CRGB::Black;
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Black; 
         leds[9] = CRGB::Black; 
         leds[10] = CRGB::Red; 
         leds[11] = CRGB::Red;   
         break;
       case 3:
         leds[0] = CRGB::Red; 
         leds[1] = CRGB::Red; 
         leds[2] = CRGB::Red; 
         leds[3] = CRGB::Black; 
         leds[4] = CRGB::Black; 
         leds[5] = CRGB::Black; 
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Black; 
         leds[9] = CRGB::Red; 
         leds[10] = CRGB::Red; 
         leds[11] = CRGB::Red;
         break;
       case 4:
         leds[0] = CRGB::Red; 
         leds[1] = CRGB::Red; 
         leds[2] = CRGB::Red; 
         leds[3] = CRGB::Red; 
         leds[4] = CRGB::Black;
         leds[5] = CRGB::Black;
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Black; 
         leds[8] = CRGB::Red; 
         leds[9] = CRGB::Red; 
         leds[10] = CRGB::Red; 
         leds[11] = CRGB::Red;  
         
         break;
       case 5:
         leds[0] = CRGB::Red; 
         leds[1] = CRGB::Red; 
         leds[2] = CRGB::Red; 
         leds[3] = CRGB::Red; 
         leds[4] = CRGB::Red; 
         leds[5] = CRGB::Black; 
         leds[6] = CRGB::Black; 
         leds[7] = CRGB::Red; 
         leds[8] = CRGB::Red; 
         leds[9] = CRGB::Red; 
         leds[10] = CRGB::Red; 
         leds[11] = CRGB::Red;
         break;
     case 6:
         leds[0] = CRGB::Red; 
         leds[1] = CRGB::Red; 
         leds[2] = CRGB::Red; 
         leds[3] = CRGB::Red; 
         leds[4] = CRGB::Red;  
         leds[5] = CRGB::Red; 
         leds[6] = CRGB::Red; 
         leds[7] = CRGB::Red; 
         leds[8] = CRGB::Red; 
         leds[9] = CRGB::Red; 
         leds[10] = CRGB::Red; 
         leds[11] = CRGB::Red; 
         break;
     case 7:
         leds[0] = CRGB::Red; 
         leds[1] = CRGB::Red; 
         leds[2] = CRGB::Red; 
         leds[3] = CRGB::Red; 
         leds[4] = CRGB::Red; 
         leds[5] = CRGB::Red;
         leds[6] = CRGB::Red; 
         leds[7] = CRGB::Red; 
         leds[8] = CRGB::Red; 
         leds[9] = CRGB::Red; 
         leds[10] = CRGB::Red; 
         leds[11] = CRGB::Red; 
         break;
    }  
  }
  
}

void gameOver(){
  for(int i=0;i<3;i++){
      lightUp(7);
      FastLED.show();
      delay(200);
      lightUp(0);
      FastLED.show();
      delay(200);
  }
  lightUp(0);
  FastLED.show();
  while(1){}
}

void ledAnimation(){
  lightUp(0);
  FastLED.show();
  delay(1000);
  for(int i=0;i<7;i++){
     lightUp(i); 
     FastLED.show();
     delay(50);
  }
}
