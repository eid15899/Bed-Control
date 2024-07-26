#include <Keypad.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);

char keys[4][3] = {
  {'1', '2', '3'},
  {'7', '8', '9'},
  {'c', 'b', 'a'},
  {'6', '5', '4'}
};

byte Mode = 0; 
byte rowPins[4] = {A3, A2, 12, 11};
byte colPins[3] = {A1, A0, 13};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 3);

byte MotorState[6] = {0, 0, 0, 0, 0, 0};
char KeyLeft[6] = {'1', '2', '3', '9', '8', '7'};
char KeyRight[6] = {'6', '5', '4', 'a', 'b', 'c'};

const int buttonPin = 6; 
byte lastButtonState = 0; 

const int ledPins[] = {A5, A4, 7}; 

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
void ControlLED(char key, KeyState state) {
  if (state == PRESSED) {
    switch (key) {
      case '1':
        digitalWrite(ledPins[1], HIGH);
        break;
      case '6':
        digitalWrite(ledPins[2], HIGH);
        break;
      
    }
  } else if (state == RELEASED) {
    switch (key) {
      case '1':
        digitalWrite(ledPins[1], LOW);
        break;
      case '6':
        digitalWrite(ledPins[2], LOW);
        break;
    }
  }
}


void setup() {
  Serial.begin(9600);
  Serial.println("Eid");
  mySerial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP); 
  
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
      digitalWrite(ledPins[0], HIGH); 
    } else if (receivedChar == 'z') {
      Mode = 0;
      digitalWrite(ledPins[0], LOW); 
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
        ControlMotor2(keypad.key[0].kchar, keypad.key[0].kstate);
        ControlLED(keypad.key[0].kchar, keypad.key[0].kstate);
      }
    }
     byte buttonState = digitalRead(buttonPin);
     if (buttonState == 1 && lastButtonState == 0) {
   
       mySerial.println('w');
       Serial.println('w');
    }
      lastButtonState = buttonState; 

  }
  else {
      
     byte buttonState = digitalRead(buttonPin);
     if (buttonState == 1 && lastButtonState == 0) {
       mySerial.println('w');
       Serial.println('w');
    }
      lastButtonState = buttonState; 

  }
}
