
int servo1 = A2;
int servo2 = A3;

void setup() {
  pinMode(servo1, OUTPUT);
  pinMode(servo2, OUTPUT);
}
 
void loop() {
  for(int i=0;i<180;i+=10){
    servo(i);
    delay(200);
  
  }
}

void servo(int angle) {
  //发送50个脉冲
  for(int i=0;i<50;i++){
    int pulsewidth = (angle * 11) + 500; 
    digitalWrite(servo1, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servo1, LOW);
    delayMicroseconds(20000 - pulsewidth);

    pulsewidth = ((180 - angle) * 11) + 500; 
    digitalWrite(servo2, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servo2, LOW);
    delayMicroseconds(20000 - pulsewidth);
  
  }
  
}
