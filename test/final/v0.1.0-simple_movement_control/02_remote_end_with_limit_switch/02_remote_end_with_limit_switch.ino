#include <SPI.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>

//RF24L01配置
RF24 radio(A0, A1); // CE, CSN
const byte address[6] = "01001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
//定义输入按键引脚
int btnA = A6;
int btnB = A7;
int btnC = 4;
int joystickA = A3;
int joystickB = A2;
int joystickC = A4;
int joystickD = A5;

//发送的数据结构
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

/*-----初始化函数-----*/
void setup() {
  Serial.begin(9600);
  btnSetup();
  radioSetup();
}

/*-----主循环函数-----*/
void loop() {
  sendKey();
  delay(50);

  
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

/*-----自定义函数-----*/

void sendKey() { //读取并发送按键电平信号
  if(analogRead(btnA)>1000){myData.btnA = 0;}else{myData.btnA = 1;}
  if(analogRead(btnB)>1000){myData.btnB = 0;}else{myData.btnB = 1;}
//  myData.btnA = map(analogRead(btnA),0,1023,1,0);
//  myData.btnB = map(analogRead(btnB),0,1023,1,0);
  myData.btnC = !digitalRead(btnC);
  myData.joyStick_A_X = analogRead(joystickA);
  myData.joyStick_A_Y = analogRead(joystickB);
  myData.joyStick_B_X = analogRead(joystickC);
  myData.joyStick_B_Y = analogRead(joystickD);
  myData._micros = micros();
  radio.write(&myData, sizeof(myData));
}

void btnSetup() { //设置按键为输入并内部上拉
  pinMode(btnA, INPUT);
  pinMode(btnB, INPUT);
  pinMode(btnC, INPUT_PULLUP);
  pinMode(joystickA, INPUT);
  pinMode(joystickB, INPUT);
  pinMode(joystickC, INPUT);
  pinMode(joystickD, INPUT);

}

void radioSetup() { //RF24L01发射端初始化
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}
