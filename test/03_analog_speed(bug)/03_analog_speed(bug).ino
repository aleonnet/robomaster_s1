#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

#define motorA_A 2
#define motorA_B 3
#define motorB_A 4
#define motorB_B 5
#define motorC_A 7
#define motorC_B 6
#define motorD_A 8
#define motorD_B 9

#define blasterTiltApin A2 //servo
#define blasterTiltBpin A3 //servo
#define blaster A4 //relay

RF24 radio(A0, A1); // CE, CSN
const byte address[6] = "00010";
struct dataStruct {
  unsigned long _micros;
  bool btnA;
  bool btnB;
  int joyStick_A_X;
  int joyStick_A_Y;
  int joyStick_B_X;
  int joyStick_B_Y;
} myData;


int tiltAngle = 90;
Servo blasterTiltA, blasterTiltB;

void setup() {
  Serial.begin(9600);
  for (int i = 2; i < 11; i++) {
    pinMode(i, OUTPUT);
  }
  blasterServoSetup();
  radioSetup();
}

void loop() {
  //getData();
    digitalWrite(motorD_A, HIGH);
    
    for(int i=0; i<255; i+=10){
       analogWrite(motorD_B, i); 
       Serial.println(i);
       delay(300);
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
  blasterTiltB.write(90);  
}


//in mode{0 = no action, 1 = foward, 2 = backward}
void AMove(int mode, int v)
{
  
  if (mode == 0) {
    digitalWrite(motorA_A, LOW);
    digitalWrite(motorA_B, LOW);
  }
  else if (mode == 1) {
    digitalWrite(motorA_A, HIGH);
    analogWrite(motorA_B, v);

  }
  else if (mode == 2) {
    digitalWrite(motorA_A, LOW);
    analogWrite(motorA_B, v);
  }
}

void BMove(int mode, int v)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(motorB_A, LOW);
    digitalWrite(motorB_B, LOW);
  }
  else if (mode == 1) {
    digitalWrite(motorB_A, HIGH);
    analogWrite(motorB_B, v);
  }
  else if (mode == 2) {
    digitalWrite(motorB_A, LOW);
    analogWrite(motorB_B, v);
  }
}
void CMove(int mode, int v)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(motorC_A, LOW);
    digitalWrite(motorC_B, LOW);
  }
  else if (mode == 1) {
    digitalWrite(motorC_A, LOW);
    analogWrite(motorC_B, 255-v);
  }
  else if (mode == 2) {
    digitalWrite(motorC_A, HIGH);
    analogWrite(motorC_B, 255-v);
  }
}
void DMove(int mode, int v)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(motorD_A, LOW);
    digitalWrite(motorD_B, LOW);
  }
  else if (mode == 1) {
    digitalWrite(motorD_A, HIGH);
    analogWrite(motorD_B, v);
  }
  else if (mode == 2) {
    digitalWrite(motorD_A, LOW);
    analogWrite(motorD_B, v);
  }
}
void robotMove(int Direction, int v)
{
  switch (Direction) {  // 0 = stop, 1 = forward....5 =
    case 0://stop
      AMove(0,0);
      BMove(0,0);
      CMove(0,0);
      DMove(0,0);
      break;
    case 1://forward
      AMove(1,v);
      BMove(1,v);
      CMove(1,v);
      DMove(1,v);
      break;
    case 2://backward
      AMove(2,v);
      BMove(2,v);
      CMove(2,v);
      DMove(2,v);
      break;
    case 3://left shift
      AMove(2,v);
      BMove(1,v);
      CMove(2,v);
      DMove(1,v);
      break;
    case 4://right shift
      AMove(1,v);
      BMove(2,v);
      CMove(1,v);
      DMove(2,v);
      break;
    case 5://right forward
      AMove(1,v);
      BMove(0,v);
      CMove(1,v);
      DMove(0,v);
      break;
    case 6://left backward
      AMove(2,v);
      BMove(0,v);
      CMove(2,v);
      DMove(0,v);
      break;
    case 7://left forward
      AMove(0,v);
      BMove(1,v);
      CMove(0,v);
      DMove(1,v);
      break;
    case 8: //right backward
      AMove(0,v);
      BMove(2,v);
      CMove(0,v);
      DMove(2,v);
      break;
    case 9://spinning right
      AMove(2,v);
      BMove(2,v);
      CMove(1,v);
      DMove(1,v);
      break;
    case 10://spinning right
      AMove(1,v);
      BMove(1,v);
      CMove(2,v);
      DMove(2,v);
      break;
  }
}
void radioSetup() { //RF24L01接收端初始化
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
}

void getData() { //获取数据内容并打印在屏幕上
  if (radio.available()) {
    radio.read( &myData, sizeof(myData) );
    if(myData.joyStick_A_X+myData.joyStick_A_Y+myData.joyStick_B_X+myData.joyStick_B_Y+myData.btnA+myData.btnB != 0){
      if (myData.joyStick_A_Y>750) {
        robotMove(1,map(myData.joyStick_A_Y,612,1023,255,0));
      }else if(myData.joyStick_A_Y<350){
        robotMove(2,map(myData.joyStick_A_Y,412,0,0,255));
      }else if(myData.joyStick_A_X<350){
        robotMove(3,100);
      }else if(myData.joyStick_A_X>750){
        robotMove(4,100);
      }else {
        robotMove(0,0);
      }
      
      if(myData.joyStick_B_X < 350){
        if(tiltAngle<100){
          tiltAngle +=3;
        }
      }else if(myData.joyStick_B_X > 750){
        if(tiltAngle>50){
          tiltAngle -=3;
        }
      }
      
      blasterTiltA.write(tiltAngle);
      blasterTiltB.write(180 - tiltAngle);
      if(myData.btnB){
        digitalWrite(blaster, HIGH);
      }else{
        digitalWrite(blaster, LOW);
      }
    }else{
      robotMove(0,0);
    }
  
     //打印内容
        Serial.print(myData.btnA);
        Serial.print(" ");
        Serial.print(myData.btnB);
        Serial.print(" ");
        Serial.print(myData.joyStick_A_X);
        Serial.print(" ");
        Serial.print(myData.joyStick_A_Y);
        Serial.print(" ");
        Serial.print(myData.joyStick_B_X);
        Serial.print(" ");
        Serial.print(myData.joyStick_B_Y);
        Serial.println(" ");

    delay(50);

  }
}
