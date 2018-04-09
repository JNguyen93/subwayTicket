const byte arduino0 = 2;
const byte arduino1 = 3;
const byte arduino2 = 4;
const byte arduino3 = 5;
const byte reset = 6;
const byte maglock = 7;
bool flag0, flag1, flag2, flag3;
bool penalty1, penalty2, penalty3;
bool ard0, ard1, ard2, ard3;
bool unlock;
long unlocked;
int timeout = 5000;

void setup() {
  Serial.begin(9600);
  Serial.print("Subway Ticket Master");
  pinMode(arduino0, INPUT);
  pinMode(arduino1, INPUT);
  pinMode(arduino2, INPUT);
  pinMode(arduino3, INPUT);
  pinMode(reset, OUTPUT);
  pinMode(maglock, OUTPUT);
  digitalWrite(reset, LOW);
}

void loop() {
  ard0 = digitalRead(arduino0);
  delay(10);
  ard1 = digitalRead(arduino1);
  delay(10);
  ard2 = digitalRead(arduino2);
  delay(10);
  ard3 = digitalRead(arduino3);
  delay(10);

  debug();
  
  if(ard0 && !penalty1 && !penalty2 && !penalty3){
    flag0 = true;
  }
  else{
    flag0 = false;
  }
  if(ard1 && flag0 && !penalty1 && !penalty2 && !penalty3){
    flag1 = true;
  }
  else if (ard1 && !flag0){
    flag1 = false;
    penalty1 = true;
  }
  else if(!ard1){
    flag1 = false;
    penalty1 = false;
  }
  if(ard3 && flag1 && !penalty1 && !penalty2 && !penalty3){
    flag2 = true;
  }
  else if (ard3 && !flag1){
    flag2 = false;
    penalty2 = true;
  }
  else if(!ard3){
    flag2 = false;
    penalty2 = false;
  }
  if(ard2 && flag2 && !penalty1 && !penalty2 && !penalty3){
    flag3 = true;
    unlock = true;
  }
  else if (ard2 && !flag2){
    flag3 = false;
    penalty3 = true;
  }
  else if(!ard2){
    flag3 = false;
    penalty3 = false;
  }

  if (!flag0 || !flag1 || !flag2 || !flag3){
    unlock = false;
  }

  if (penalty1 || penalty2 || penalty3){
    digitalWrite(reset, HIGH);
    delay(100);
    digitalWrite(reset, LOW);
  }

  if(unlock){
    digitalWrite(maglock, LOW);
    if(unlocked - millis() > timeout){
      Serial.println("Delaying 30 Seconds");
      digitalWrite(reset, HIGH);
      delay(30000);
      digitalWrite(reset, LOW);
      unlocked = millis();
    }
    Serial.println("Door Open!");
  }
  else{
    digitalWrite(maglock, HIGH);
  }
}

void debug(){
  Serial.print("arduino0: ");
  Serial.println(ard0);
  Serial.print("arduino1: ");
  Serial.println(ard1);
  Serial.print("arduino2: ");
  Serial.println(ard2);
  Serial.print("arduino3: ");
  Serial.println(ard3);

  Serial.print("Flag0: ");
  Serial.println(flag0);
  Serial.print("Flag1: ");
  Serial.println(flag1);
  Serial.print("Flag2: ");
  Serial.println(flag2);
  Serial.print("Flag3: ");
  Serial.println(flag3);

  Serial.print("Penalty1: ");
  Serial.println(penalty1);
  Serial.print("Penalty2: ");
  Serial.println(penalty2);
  Serial.print("Penalty3: ");
  Serial.println(penalty3);
}

