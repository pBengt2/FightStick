/* 
 * Code by: Peter Bengtson, David Fulmer
 * cs362, final project
 * 
 * arcadeStick:
 *  - Receives button input, sends to unojoy
 *  - Remaps buttons when signal received
 *  - Sends button input to LED
 * 
 * led:
 *  - Lights up leds based on button input
 *  - Sends remap signal if start/select held down for 3 seconds
 * 
 */

/*********** LED ARRAY VALUES FOR COMMUNICATING*****************
  //0 up, 1 left, 2 down, 3 right, 4 cross, 5 circle, 6 triangle
  //7 square, 8 l1, 9 l2, 10 r1, 11 r2, 12 start, 13 select
***************************************************************/

#include "UnoJoy.h"
#include <SoftwareSerial.h>

//changes:
//analog -> pullup resistor
//check for signals first

//Define pins:
//0,1 open for UnoJoy
//10 rx, 11 tx
SoftwareSerial ledSerial(10, 11); //rx, tx

//directions
//A1 up, A2 left, A3 down, A4 right
//face buttons, can be remapped
//2 Circle, 3 X, 4 Triangle, 5 Square, 6 R1, 7 R2, 8 L1, 9 L2
//12 Start, 13 Select <- cannot be remapped

int CirclePin = 2;
int CrossPin = 3;
int TrianglePin = 4;
int SquarePin = 5;

int R1Pin = 6;
int R2Pin = 7;
int L1Pin = 8;
int L2Pin = 9;

const int StartPin =  12;
const int SelectPin = 13;

void setup(){
  setupPins();
  setupUnoJoy();
  ledSerial.begin(4800);
}

//check for signal, get controller data, send led array to led arduiino
void loop(){  
  dataForController_t controllerData = getControllerData();
  sendLights(controllerData);
  setControllerData(controllerData);  

  //check for remap signal
  if (ledSerial.available())
    if (ledSerial.read())
      remap();
  
}

//Converts controller data to led array, and sends array to led arduino
int sendLights(dataForController_t controllerData){
  byte ledArr[14]; 
  ledArr[0] = controllerData.dpadUpOn;
  ledArr[1] = controllerData.dpadLeftOn * 2;
  ledArr[2] = controllerData.dpadDownOn * 3;
  ledArr[3] = controllerData.dpadRightOn * 4;

  ledArr[4] = controllerData.crossOn * 5;
  ledArr[5] = controllerData.circleOn * 6;
  ledArr[6] = controllerData.triangleOn * 7;
  ledArr[7] = controllerData.squareOn * 8;

  ledArr[8] = controllerData.l1On * 9;
  ledArr[9] = controllerData.l2On * 10;
  ledArr[10] = controllerData.r1On * 11;
  ledArr[11] = controllerData.r2On * 12;

  ledArr[12] = controllerData.startOn * 13;
  ledArr[13] = controllerData.selectOn * 14;

  //send ledArray to led arduino
  ledSerial.write(ledArr, 14);
}

//Send artificial single button signal to led arduino
//int button is index in led array, not current pin!
void sendLight(int button){
  byte ledArr[14];
  for (int i=0; i<14; i++)
    ledArr[i] = 0;
  ledArr[button] = 1 * (button+1);

  //send led array to led arduino
  ledSerial.write(ledArr, 14);
}

void setupPins(void){
  //facePins, remappable
  for (int i = 2; i < 10; i++){
    pinMode(i, INPUT_PULLUP);
    digitalWrite(i, HIGH);
  }
  //start select not remappable
  pinMode(StartPin, INPUT_PULLUP);
  digitalWrite(StartPin, HIGH);
  pinMode(SelectPin, INPUT_PULLUP);
  digitalWrite(SelectPin, INPUT_PULLUP);

  //direction pins
  pinMode(A1, INPUT_PULLUP);
  digitalWrite(A1, HIGH);
  pinMode(A2, INPUT_PULLUP);
  digitalWrite(A2, HIGH);
  pinMode(A3, INPUT_PULLUP);
  digitalWrite(A3, HIGH);
  pinMode(A4, INPUT_PULLUP);
  digitalWrite(A4, HIGH);
}

//waits for a face button input, and returns it's pin number
//curButton is position in led-array, not the current pin!
int waitForInput(int curButton){
  while(1){
    sendLight(curButton);
    for (int i=2; i<10; i++){
      if (!digitalRead(i)){
          delay(200);
          return i;
      }
    }
  }
  return -1;
}

//remaps face buttons (ie, no directions or start/select)
void remap(){
  //send signal to light up led representing button
  //wait for input
  //remap to input pin
  
  //circle
  int newPin = waitForInput(5);
  if (newPin != -1)
    CirclePin = newPin;

  //x
  newPin = waitForInput(4);
  if (newPin != -1)
    CrossPin = newPin;

  //triangle
  newPin = waitForInput(6);
  if (newPin != -1)
    TrianglePin = newPin;

  //square
  newPin = waitForInput(7);
  if (newPin != -1)
    SquarePin = newPin;

  //r1
  newPin = waitForInput(10);
  if (newPin != -1)
    R1Pin = newPin;

  //r2
  newPin = waitForInput(11);
  if (newPin != -1)
    R2Pin = newPin;

  //enable l1 and l2 if using 8 face buttons
  /*
  //l1
  newPin = waitForInput(8);
  if (newPin != -1)
    L1Pin = newPin;

  //l2
  newPin = waitForInput(9);
  if (newPin != -1)
    L2Pin = newPin;
  */
}

//returns the currently pressed buttons
dataForController_t getControllerData(void){
  //start with a 0'd out controller
  dataForController_t controllerData = getBlankDataForController();

  //remappable
  controllerData.triangleOn = !digitalRead(TrianglePin);
  controllerData.circleOn = !digitalRead(CirclePin);
  controllerData.squareOn = !digitalRead(SquarePin);
  controllerData.crossOn = !digitalRead(CrossPin);
  
  controllerData.r1On = !digitalRead(R1Pin);
  controllerData.r2On = !digitalRead(R2Pin);
  controllerData.l1On = !digitalRead(L1Pin);
  controllerData.l2On = !digitalRead(L2Pin);

  //non-remappable
  controllerData.startOn = !digitalRead(StartPin);
  controllerData.selectOn = !digitalRead(SelectPin);

  controllerData.dpadUpOn = !digitalRead(A1);
  controllerData.dpadLeftOn = !digitalRead(A2);
  controllerData.dpadDownOn = !digitalRead(A3);
  controllerData.dpadRightOn = !digitalRead(A4);
  
  return controllerData;
}
