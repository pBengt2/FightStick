byte arr[14];
unsigned long timePressed = -1;
unsigned long timeLastPressed = -1;

void setup() {
  setupPins();
  Serial.begin(4800);
}

void loop() {
  byte curPressed[14];
  for (int i=0; i< 14; i++)
    curPressed[i] = 0;
  if(Serial.available())
  {
    Serial.readBytes(arr, 14);
  }

  for(int i = 0; i < 14; i++)
  {
    byte value = arr[i];
    if(value == 1){
      curPressed[value-1] = 1;
      digitalWrite(A1, HIGH);
    }
    else if(value == 2){
      curPressed[value-1] = 1;
      digitalWrite(A2, HIGH);
    }
    else if(value == 3){
      curPressed[value-1] = 1;
      digitalWrite(A3, HIGH);
    }
    else if(value == 4){
      curPressed[value-1] = 1;
      digitalWrite(A4, HIGH);
    }
    else if (value >= 5 && value <= 14){
      curPressed[value-1] = 1;
      digitalWrite(value - 1, HIGH);
    }
  }
  
  if (!curPressed[0])
    digitalWrite(A1, LOW);
  if (!curPressed[1])
    digitalWrite(A2, LOW);
  if (!curPressed[2])
    digitalWrite(A3, LOW);
  if (!curPressed[3])
    digitalWrite(A4, LOW);
    
  for(int i = 4; i < 14; i++)
  {
    if (!curPressed[i])
      digitalWrite(i, LOW);
  }
  
  if(curPressed[12] == 1 && curPressed[13] == 1 && timePressed == -1){
    timePressed = millis();
    timeLastPressed = millis();
  }
  else if (curPressed[12] == 1 && curPressed[13] == 1){
    timeLastPressed = millis();
  }
  else if (millis() - timeLastPressed >= 100)
  {
    timePressed = -1;
    timeLastPressed = -1;
  }
  
  if(timePressed != -1 && (millis() - timePressed >= 3000))
  {
    Serial.write((byte)1);
    timePressed = -1;
    timeLastPressed = -1;
  }
}

void setupPins(void){
  for (int i = 4; i < 14; i++){
    pinMode(i, OUTPUT);
  }

  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
}

