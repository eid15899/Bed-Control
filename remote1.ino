#include <Keypad.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);

char keys[3][5] = {
  {'d', 'c', 'b', 'a', '9'},
  {'8', '7', '6', '5', '4'},
  {'3', 'f', 'e', '2', '1'}
};
byte Mode = 0;
byte rowPins[3] = {A3, A1, 9};
byte colPins[5] = {A5, A0, 12, 11, 10};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 3, 5);

byte MotorState[6] = {0, 0, 0, 0, 0, 0};
char KeyLeft[6] = {'1', '4', '5', '6', '7', '8'};
char KeyRight[6] = {'2', '9', 'a', 'b', 'c', 'd'};
const int ledPins[3] = {5, 6, A4};
void ControlMotor3( char key, KeyState state) {           
    if(key=='3'){
      if(state== PRESSED){
        Serial.println((char)('w'));
        mySerial.print((char)('w'));
      }       
      
    }
}
void ControlMotor2( char key, KeyState state) {
  for(byte i=0;i<6;i++){          
        if(key==KeyLeft[i]){
            if(state== RELEASED){
              Serial.println((char)('a' + 3 * i));
              mySerial.print((char)('a' + 3 * i));
            }
            else {
              if(state== PRESSED){
                Serial.println((char)('b' + 3 * i));
                mySerial.print((char)('b' + 3 * i));
              }
            }

        }
        if(key==KeyRight[i]){
          if(state== RELEASED){
            Serial.println((char)('a' + 3 * i));
            mySerial.print((char)('a' + 3 * i));
          }
          else {
            if(state== PRESSED){
              Serial.println((char)('c' + 3 * i));
              mySerial.print((char)('c' + 3 * i));
            }
          }
        }
    
  }

}
void ControlMotor(byte i, char key, KeyState state) {
  switch (MotorState[i]) {
    case 0:
      if (state == PRESSED) {
        if (key == KeyLeft[i]) {
          MotorState[i] = 1;
          Serial.println((char)('b' + 3 * i));
          mySerial.print((char)('b' + 3 * i));
        }
        if (key == KeyRight[i]) {
          MotorState[i] = 2;
          Serial.println((char)('c' + 3 * i));
          mySerial.print((char)('c' + 3 * i));
        }
      }
      break;
    case 1:
      if (state == RELEASED && key == KeyLeft[i]) {
        MotorState[i] = 0;
        Serial.println((char)('a' + 3 * i));
        mySerial.print((char)('a' + 3 * i));
      }
      break;
    case 2:
      if (state == RELEASED && key == KeyRight[i]) {
        MotorState[i] = 0;
        Serial.println((char)('a' + 3 * i));
        mySerial.print((char)('a' + 3 * i));
      }
      break;
  }
}
void ControlLED(char key, KeyState state) {
  if (state == PRESSED) {
    switch (key) {
      case '1':
        digitalWrite(ledPins[0], HIGH);
        break;
      case '2':
        digitalWrite(ledPins[1], HIGH);
        break;
      
    }
  } else if (state == RELEASED) {
    switch (key) {
      case '1':
        digitalWrite(ledPins[0], LOW);
        break;
      case '2':
        digitalWrite(ledPins[1], LOW);
        break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Eid");
  mySerial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  if (mySerial.available() > 0) {
    char receivedChar = mySerial.read();
    if (receivedChar == 'Z') {
      Mode = 1;
      digitalWrite(ledPins[2], HIGH); 
    } else if (receivedChar == 'z') {
      Mode = 0;
      digitalWrite(ledPins[2], LOW); 
      for (int i = 0; i < 6; i++) {
        MotorState[i] = 0;
      }
      for (int i = 0; i < 3; i++) {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }

  if (Mode == 1) {
    if (keypad.getKeys()) {
      if (keypad.key[0].stateChanged) {
        //Serial.print(keypad.key[0].kchar); Serial.println(keypad.key[0].kstate);
        ControlMotor2(keypad.key[0].kchar, keypad.key[0].kstate);
        ControlMotor3(keypad.key[0].kchar, keypad.key[0].kstate);
        ControlLED(keypad.key[0].kchar, keypad.key[0].kstate);
      }
    }
  }
  else {
     if (keypad.getKeys()) {
      if (keypad.key[0].stateChanged) {
        //Serial.print(keypad.key[0].kchar); Serial.println(keypad.key[0].kstate);
       ControlMotor3(keypad.key[0].kchar, keypad.key[0].kstate);
       ControlLED(keypad.key[0].kchar, keypad.key[0].kstate);
      }
    }
  }
}
