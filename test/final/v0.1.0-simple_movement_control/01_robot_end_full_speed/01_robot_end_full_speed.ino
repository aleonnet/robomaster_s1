#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

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

RF24 radio(A0, A1); // CE, CSN
const byte address[6] = "01100";
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


int tiltAngle = 90;
Servo blasterTiltA, blasterTiltB;

void setup() {
  Serial.begin(9600);
  for (int i = 2; i < 10; i++) {
    pinMode(i, OUTPUT);
  }
  blasterServoSetup();
  radioSetup();
}

void loop() {
  getData();
  
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

void getData() { //获取数据内容并打印在屏幕上
  if (radio.available()) {
    radio.read( &myData, sizeof(myData) );
    if(myData.joyStick_A_X+myData.joyStick_A_Y+myData.joyStick_B_X+myData.joyStick_B_Y+myData.btnA+myData.btnB != 0){
      if (myData.joyStick_A_Y>700) {
        robotMove(1);
      }else if(myData.joyStick_A_Y<400){
        robotMove(2);
      }else if(myData.joyStick_A_X<400){
        robotMove(3);
      }else if(myData.joyStick_A_X>700){
        robotMove(4);
      }else if(myData.joyStick_B_Y<400){
        robotMove(9);
      }else if(myData.joyStick_B_Y>700){
        robotMove(10);
      }else {
        robotMove(0);
      }
      
      if(myData.joyStick_B_X < 350){
        if(tiltAngle<100){
          tiltAngle +=7;
        }
      }else if(myData.joyStick_B_X > 750){
        if(tiltAngle>50){
          tiltAngle -=7;
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

    delay(50);

  }
}
