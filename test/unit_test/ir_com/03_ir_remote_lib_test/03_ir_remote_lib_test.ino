#include <IRremote.h>

String code = "23BA2F91"; //7df

IRsend irsend;
IRrecv irrecv(2);
decode_results results;
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
	for (int i = 0; i < 3; i++) {
		irsend.send(code, 12);
		delay(40);
	}
//  if (irrecv.decode(&results)) {
//    Serial.println(results.value, HEX);
//    irrecv.resume(); // Receive the next value
//  }
	//delay(1000); //5 second delay between each signal burst
}
