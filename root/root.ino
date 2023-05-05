const int id = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
//  Serial.print("arduino setup\n");
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
}
bool state[4] = { false, false, false, false };
bool refreshing = false;

void handleRefresh(){
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print(' ');
  Serial.print(0);
  Serial.print('\n');
  // if (Serial.available()) {
  //   if (Serial.peek() == '#') {
  //     char c = Serial.read();
  //     state[0] = true;
  //   }
  //   else {
  //     char x = Serial.read();
  //     char y = Serial.read(f) - 1;
  //     Serial.print(x);
  //     Serial.print('\t');
  //     Serial.print(y);
  //     Serial.print('\n');
  //   }
  // }
  while (Serial1.available()) {
//    Serial.print("serial1\n");
    if (Serial1.peek() == '#') {
      int c = Serial1.read();
      state[1] = true;
    } else {
      if (state[1]) {
        int receiveId = Serial1.read();
        int x = Serial1.read();
        int y = Serial1.read() - 1;
        Serial.print(receiveId);
        Serial.print(' ');
        Serial.print(x);
        Serial.print(' ');
        Serial.print(y);
        Serial.print('\n');
        delay(50);
      }
    }
  }
  while (Serial2.available()) {
//    Serial.print("serial2\n");
    if (Serial2.peek() == '#') {
      int c = Serial2.read();
      state[2] = true;
    } else {
      if (state[2]) {
        int receiveId = Serial2.read();
        int x = Serial2.read() - 1;
        int y = Serial2.read();
        Serial.print(receiveId);
        Serial.print(' ');
        Serial.print(x);
        Serial.print(' ');
        Serial.print(y);
        Serial.print('\n');
        delay(50);
      }
    }
  }
//  Serial.print("3\n");
  while (Serial3.available()) {
//    Serial.print("serial3\n");
    if (Serial3.peek() == '#') {
      int c = Serial3.read();
      state[3] = true;
    } else {
      if (state[3]) {
        int receiveId = Serial1.read();
        int x = Serial3.read() + 1;
        int y = Serial3.read();
        Serial.print(receiveId);
        Serial.print(' ');
        Serial.print(x);
        Serial.print(' ');
        Serial.print(y);
        Serial.print('\n');
        delay(50);
      }
    }
  }
//  Serial.print("end of refresh\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
    char cmd = Serial.peek();
    if(cmd=='@'&&!refreshing){
      Serial.read();
      refreshing = true;
      Serial1.write('@');
      Serial2.write('@');
      Serial3.write('@');
//      Serial.print("@ sent\n");
    }else if(cmd=='L'){
      // lock
      Serial.read();
      int lockId = Serial.read();
      if(lockId==id){
        digitalWrite(7,LOW);
        delay(1500);
        digitalWrite(7,HIGH);
      }else{
        Serial1.write('L');
        Serial2.write('L');
        Serial3.write('L');
        Serial1.write(lockId);
        Serial2.write(lockId);
        Serial3.write(lockId);
      }
    }else if(cmd=='H'){
      //light
      Serial.read();
      int idNumber = Serial.read();
      for(int i=0;i<idNumber;i++){
        int lightId = Serial.read();
        if(lightId==id){
          Serial.write("light up");
          digitalWrite(7,LOW);
          delay(1500);
          digitalWrite(7,HIGH);
        }else{
          Serial1.write('H');
          Serial2.write('H');
          Serial3.write('H');
          Serial1.write(lightId);
          Serial2.write(lightId);
          Serial3.write(lightId);
        }
      }
//      Serial.print(idNumber);
//      Serial.print(" sent");
    }
  }
  if(refreshing){
    delay(1000);
    handleRefresh();
    Serial.print("end\n");
    refreshing = false;
  }
//   for (int i = 1; i < 4; i++) {
//     Serial.print(state[i]);
//     Serial.print(' ');
//   }
//  Serial.print('\n');
  delay(20);
}
