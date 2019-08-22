/* rawSend.ino Example sketch for IRLib2
 *  Illustrates how to send a code Using raw timings which were captured
 *  from the "rawRecv.ino" sample sketch.  Load that sketch and
 *  capture the values. They will print in the serial monitor. Then you
 *  cut and paste that output into the appropriate section below.
 */

#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender
#include <IRLibRecvPCI.h> 
#include <FastLED.h>

#define RAW_DATA_LEN 18
uint16_t rawData[RAW_DATA_LEN]={
  1746, 1246, 606, 1218, 558, 1242, 582, 1242, 
  554, 1246, 606, 694, 1102, 722, 1106, 690, 
  1086, 1000};

IRrecvPCI myReceiver(2);
IRsendRaw mySender;

#define NUM_LEDS 5
#define DATA_PIN 4
#define CLOCK_PIN 13
CRGB leds[NUM_LEDS];

int blood = 5;

void setup() {
  pinMode(12,INPUT_PULLUP);
  Serial.begin(9600);
  myReceiver.enableIRIn();
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  showLed();
}

void loop() {
  
  if(!digitalRead(12)){
    digitalWrite(2,1);
    mySender.send(rawData,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
    Serial.println(F("Sent signal."));
    delay(200);
    digitalWrite(2,0);
  }
  
  if (myReceiver.getResults()) { 
    if(blood>1){
      Serial.println("hit!");
      blood--;
      leds[4-blood] = CRGB::Black;
      FastLED.show();
      delay(500);
    }else{
      dead();
    }
    myReceiver.enableIRIn();
  }
}

void showLed(){
  clearLed();
  for(int i=0;i<blood;i++)  {
      leds[i] = CRGB::Blue;
  }
  FastLED.show();
}

void clearLed(){
  for(int i=0;i<NUM_LEDS;i++)  {
      leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void dead(){
  for(int k=0;k<3;k++){
    for(int i=0;i<NUM_LEDS;i++){
      leds[i] = CRGB::Red;
    } 
    FastLED.show();
    delay(300);
    clearLed();
    delay(300);
  }
  delay(1000);
  blood=5;
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB::Blue;
  } 
  FastLED.show();
  delay(1000);
}
