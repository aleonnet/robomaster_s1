/* rawSend.ino Example sketch for IRLib2
 *  Illustrates how to send a code Using raw timings which were captured
 *  from the "rawRecv.ino" sample sketch.  Load that sketch and
 *  capture the values. They will print in the serial monitor. Then you
 *  cut and paste that output into the appropriate section below.
 */
 #include <IRLibRecvPCI.h> 
#include <IRLibSendBase.h>    //We need the base code
#include <IRLib_HashRaw.h>    //Only use raw sender
#define RAW_DATA_LEN 18
uint16_t rawData[RAW_DATA_LEN]={
  1746, 1246, 606, 1218, 558, 1242, 582, 1242, 
  554, 1246, 606, 694, 1102, 722, 1106, 690, 
  1086, 1000};


IRsendRaw mySender;
IRrecvPCI myReceiver(2);//pin number for the receiver

void setup() {
  pinMode(12,INPUT_PULLUP);
  Serial.begin(9600);
  myReceiver.enableIRIn();
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
  if (myReceiver.getResults()) { 
    Serial.println(F("Do a cut-and-paste of the following lines into the "));
    Serial.println(F("designated location in rawSend.ino"));
    Serial.print(F("\n#define RAW_DATA_LEN "));
    Serial.println(recvGlobal.recvLength,DEC);
    Serial.print(F("uint16_t rawData[RAW_DATA_LEN]={\n\t"));
    for(bufIndex_t i=1;i<recvGlobal.recvLength;i++) {
      Serial.print(recvGlobal.recvBuffer[i],DEC);
      Serial.print(F(", "));
      if( (i % 8)==0) Serial.print(F("\n\t"));
    }
    Serial.println(F("1000};"));//Add arbitrary trailing space
    myReceiver.enableIRIn();      //Restart receiver
  }
  if(!digitalRead(12)){
    digitalWrite(2,1);
    mySender.send(rawData,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
    Serial.println(F("Sent signal."));
    delay(200);
    digitalWrite(2,0);
  }
}
