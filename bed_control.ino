#include <SoftwareSerial.h>
const int relays[] = {7, 13, 8, A0, 9, A1, 10, A2, 11, A3, 12, A4};
const int numRelays = sizeof(relays) / sizeof(relays[0]);
#define pump A5
SoftwareSerial mySerial(3, 2);
byte mode = 0;
long timer1 = 0;
byte isBusy = 0; 

void onpump (char receivedChar ){
  
    switch(receivedChar ){
    case 'e':
    digitalWrite(pump , !isBusy );
    break;
     case 'b':
    digitalWrite(pump , !isBusy );
    break;
     case 'c':
    digitalWrite(pump , !isBusy );
    break;
    case 'f':
    digitalWrite(pump , !isBusy );
    break;
    case 'd':
    digitalWrite(pump , LOW );
    break;
     case 'a':
    digitalWrite(pump , LOW );
    break;
   }
 
}
void MakeNotBusy(){
  for(byte i=0;i<numRelays;i++){
    if(digitalRead(relays[i])){
       isBusy = 1;
        return ;
    }    
  }
  isBusy = 0;
  return ;
}
          
void setup() {
  for (int i = 0; i < numRelays; i++) {
    pinMode(relays[i], OUTPUT);
    digitalWrite(relays[i], LOW);
  }
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("control");
  Serial.println(__DATE__);
}

void loop() {
 if (mySerial.available() > 0) {
    char receivedChar = mySerial.read();

    if (receivedChar == 'w') {
      mode = !mode;
      Serial.print("Change Mode to ");
      Serial.println(mode);
      if (mode == 0) {
        for (int i = 0; i < numRelays; i++) {
          digitalWrite(relays[i], LOW);
        }
      }
    }

    if (mode) {
      onpump(receivedChar);
      int relayGroup = (receivedChar - 'a') / 3;
      int relayAction = (receivedChar - 'a') % 3;
      Serial.print(isBusy);Serial.print(" ");
      if (relayGroup >= 0 && relayGroup < numRelays / 2) {
        int relay1 = relays[relayGroup * 2];
        int relay2 = relays[relayGroup * 2 + 1];
        
        if (relayAction == 0) {
          digitalWrite(relay1, LOW);
          digitalWrite(relay2, LOW);
          MakeNotBusy();
          
        } else if ((relayAction == 1) && !isBusy) {
          isBusy = 1; 
          digitalWrite(relay1, HIGH);
          digitalWrite(relay2, LOW);
        } else if ((relayAction == 2) && !isBusy) {
          isBusy = 1; 
          digitalWrite(relay1, LOW);
          digitalWrite(relay2, HIGH);
        }
      }
      Serial.println(isBusy);
    }
  }
  long timer2 = millis();
  if (timer2 - timer1 >= 1000) {
    timer1 = timer2;
    if (mode) {
      Serial.println('Z');
      mySerial.write('Z');
    } else {
      Serial.println('z');
      mySerial.write('z');
    }
  } 
}
