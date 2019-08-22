#include <FastLED.h>

#define NUM_LEDS 24
#define DATA_PIN 4

#define BRIGHTNESS  64
#define plate 3


CRGB leds[NUM_LEDS];
int score = 24;
unsigned long plate_debounce = millis();
char team = 'B';//A->蓝队 B->红队

void setup() {
  Serial.begin(9600);
  pinMode(plate, INPUT_PULLUP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  ledAnimation();
}

void loop() {
  Serial.print(digitalRead(plate));
  Serial.print("  ");
  Serial.println(score);
  if(!digitalRead(plate) && millis()-plate_debounce >500){
    plate_debounce = millis();
    if(score>0)  {
      score-=2;  
    }
    if(score==0){
      lightUp();
      FastLED.show();
      delay(500);
      score=NUM_LEDS;
      gameOver();
    }
  }
  
  lightUp();
  FastLED.show();
  
}

int lightUp(){
  clearLed();
  for(int i=0;i<score ;i++){
    if(team == 'A'){
      leds[i] = CRGB::Blue; 
    }else if(team == 'B'){
      leds[i] = CRGB::Red;
    }
  }
}


void clearLed(){
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = CRGB::Black; 
  }  
  
}

void ledAnimation(){
  clearLed();
  FastLED.show();
  delay(500);
  for(int i=0;i<NUM_LEDS ;i++){
    if(team == 'A'){
      leds[i] = CRGB::Blue; 
    }else if(team == 'B'){
      leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(50);
  }

  for(int i=0;i<2;i++){
    clearLed();
    FastLED.show();
    delay(100);
    for(int i=0;i<NUM_LEDS ;i++){
      if(team == 'A'){
        leds[i] = CRGB::Blue; 
      }else if(team == 'B'){
        leds[i] = CRGB::Red;
      }
      FastLED.show();
    }
    delay(200);
  }
}

void gameOver(){
  for(int i=0;i<3;i++){
    clearLed();
    FastLED.show();
    delay(100);
    for(int i=0;i<NUM_LEDS ;i++){
      leds[i] = CRGB::Purple; 
      FastLED.show();
    }
    delay(200);
  }
  while(1){}
}
