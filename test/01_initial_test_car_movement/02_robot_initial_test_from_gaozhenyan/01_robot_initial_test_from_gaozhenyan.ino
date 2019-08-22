#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

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


void setup() {
  for (int i = 2; i < 10; i++) {
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
  radioSetup();
}

void loop() {
  getData();
  
}

void AMove(int mode)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
  }
  else if (mode == 1) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
  }
  else if (mode == 2) {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
  }
}
void BMove(int mode)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  else if (mode == 1) {
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
  }
  else if (mode == 2) {
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
  }
}
void CMove(int mode)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }
  else if (mode == 1) {
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
  }
  else if (mode == 2) {
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }
}
void DMove(int mode)// 0 is no action, 1 is foward, 2, is backward (movement)
{
  if (mode == 0) {
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
  }
  else if (mode == 1) {
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
  }
  else if (mode == 2) {
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
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
    case 3:
      AMove(1);
      BMove(2);
      CMove(2);
      DMove(1);
      break;
    case 4:
      AMove(2);
      BMove(1);
      CMove(1);
      DMove(2);
      break;
    case 5:
      AMove(1);
      BMove(1);
      CMove(2);
      DMove(2);
      break;
    case 6:
      AMove(2);
      BMove(2);
      CMove(1);
      DMove(1);
      break;
    case 7:///// left foward
      AMove(0);
      BMove(1);
      CMove(1);
      DMove(0);
      break;
    case 8: ////right foward
      AMove(1);
      BMove(0);
      CMove(0);
      DMove(1);
      break;
    case 9://left backward
      AMove(2);
      BMove(0);
      CMove(0);
      DMove(2);
      break;
    case 10://right backward
      AMove(0);
      BMove(2);
      CMove(2);
      DMove(0);
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
      if (myData.joyStick_A_X > 750 && myData.joyStick_A_Y < 350) {//right backward
        robotMove(9);
      } else if (myData.joyStick_A_X > 750 && myData.joyStick_A_Y > 750) {//right forward
        robotMove(10);
      } else if (myData.joyStick_A_X < 350 && myData.joyStick_A_Y < 350) {
        robotMove(7);
      } else if (myData.joyStick_A_X < 350 && myData.joyStick_A_Y > 750) {
        robotMove(8);
      } else if (myData.joyStick_A_X < 350) { ////// primary functions
        robotMove(1);
      } else if (myData.joyStick_A_X > 750) {
        robotMove(2);
      } else if (myData.joyStick_A_Y > 750) {
        robotMove(3);
      } else if (myData.joyStick_A_Y < 350) {
        robotMove(4);
      } else if (myData.joyStick_B_Y > 750) {
        robotMove(5);
      } else if (myData.joyStick_B_Y < 350) {
        robotMove(6);
      }
      else {
        robotMove(0);
      }
    }else{
      robotMove(0);
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
