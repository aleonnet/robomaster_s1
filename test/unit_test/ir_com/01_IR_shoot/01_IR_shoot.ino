/* rawSend.ino Example sketch for IRLib2
 *  Illustrates how to send a code Using raw timings which were captured
 *  from the "rawRecv.ino" sample sketch.  Load that sketch and
 *  capture the values. They will print in the serial monitor. Then you
 *  cut and paste that output into the appropriate section below.
 */
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender
#define RAW_DATA_LEN 18
uint16_t rawData[RAW_DATA_LEN]={
  1746, 1246, 606, 1218, 558, 1242, 582, 1242, 
  554, 1246, 606, 694, 1102, 722, 1106, 690, 
  1086, 1000};


IRsendRaw mySender;

void setup() {
  pinMode(12,INPUT_PULLUP);
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  Serial.println(F("Every time you press a key is a serial monitor we will send."));
  digitalWrite(2,0);
}
/* Cut and paste the output from "rawRecv.ino" below here. It will 
 * consist of a #define RAW_DATA_LEN statement and an array definition
 * beginning with "uint16_t rawData[RAW_DATA_LEN]= {…" and concludes
 * with "…,1000};"
 */

/*
 * Cut-and-paste into the area above.
 */
   
void loop() {
  if(!digitalRead(12)){
    digitalWrite(2,1);
    mySender.send(rawData,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
    Serial.println(F("Sent signal."));
    delay(200);
    digitalWrite(2,0);
  }
}
