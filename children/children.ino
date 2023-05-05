#include <Servo.h>
#include <FastLED.h>
bool children[4] = { false, false, false, false };
int parent = -1;
unsigned long t = 0;
const int own_id = 3;
bool coordinateIsSent = false;

Servo myservo;  // 建立一個 servo 物件，最多可建立 12個 servo

int pos = 0;  // 設定 Servo 位置的變數

///////LED
bool light_on = false;
unsigned long light_duration = 5000;
unsigned long lightOnTime;
unsigned long currentTime;

const int LIGHT_PIN = 4;     //Digital PIN
const int BRIGHTNESS = 128;  //亮度
const int LIGHT_NUM = 100;   //最多燈數(可超過實際燈數)
//edit end
CRGB leds[LIGHT_NUM];  //定義FastLED類別
//colors
int rainbow16[16][3] = { { 255, 0, 0 },        //紅
                         { 255, 85, 0 },       //橙
                         { 255, 170, 0 },      //
                         { 255, 255, 0 },      //黃
                         { 127, 255, 0 },      //
                         { 0, 255, 0 },        //綠
                         { 0, 255, 85 },       //
                         { 0, 255, 170 },      //
                         { 0, 255, 255 },      //青
                         { 0, 127, 255 },      //
                         { 0, 0, 255 },        //藍
                         { 85, 0, 255 },       //紫
                         { 170, 0, 255 },      //
                         { 255, 0, 255 },      //洋紅
                         { 255, 0, 127 },      //
                         { 255, 255, 255 } };  //白

//self defined function
void showAll(int color) {
  for (int i = 0; i < LIGHT_NUM; i++) {
    leds[i] = CRGB(rainbow16[color][0], rainbow16[color][1], rainbow16[color][2]);
  }
  FastLED.show();
}
void clearAll() {
  for (int i = 0; i < LIGHT_NUM; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void Gradient(int color, int _start, int _end, int delay_t) {
  /*
  Serial.print(color);
  Serial.print(", ");
  Serial.print(_start);
  Serial.print(", ");
  Serial.println(_end);
  */
  if (_start >= _end) {
    for (int i = _start; i <= _end; i++) {
      leds[i] = CRGB(rainbow16[color][0], rainbow16[color][1], rainbow16[color][2]);
      FastLED.show();
      delay(delay_t);
    }
  } else {
    for (int i = _start; i >= _end; i--) {
      leds[i] = CRGB(rainbow16[color][0], rainbow16[color][1], rainbow16[color][2]);
      FastLED.show();
      delay(delay_t);
    }
  }
}
//self defined function end
/////LED part


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LIGHT_PIN, GRB>(leds, LIGHT_NUM);  //設定串列全彩LED參數
  FastLED.setBrightness(BRIGHTNESS);
  myservo.attach(9);
  Serial.begin(115200);   //up
  Serial1.begin(115200);  //down
  Serial2.begin(115200);  //left
  Serial3.begin(115200);  //right
}

int serialAvailable(int index) {
  switch (index) {
    case 0:
      return Serial.available();
      break;
    case 1:
      return Serial1.available();
      break;
    case 2:
      return Serial2.available();
      break;
    case 3:
      return Serial3.available();
      break;
    default:
      break;
  }
  return -1;
}
void lockControl(int isLocked) {
  if (isLocked == 1) {
    for (pos = 0; pos <= 90; pos++) {
      myservo.write(pos);  // 告訴 servo 走到 'pos' 的位置
      delay(5);            // 等待 50ms 讓 servo 走到指定位置
    }
  } else if (isLocked == 0) {
    for (pos = 90; pos >= 0; pos--) {
      myservo.write(pos);  // 告訴 servo 走到 'pos' 的位置
      delay(5);            // 等待 05ms 讓 servo 走到指定位置
    }
  }
}
int serialRead(int index) {
  switch (index) {
    case 0:
      return Serial.read();
      break;
    case 1:
      return Serial1.read();
      break;
    case 2:
      return Serial2.read();
      break;
    case 3:
      return Serial3.read();
      break;
    default:
      break;
  }
  return -1;
}
int serialPeek(int index) {
  switch (index) {
    case 0:
      return Serial.peek();
      break;
    case 1:
      return Serial1.peek();
      break;
    case 2:
      return Serial2.peek();
      break;
    case 3:
      return Serial3.peek();
      break;
    default:
      break;
  }
  return -1;
}
void serialWriteChar(int index, char ch) {
  switch (index) {
    case 0:
      Serial.write(ch);
      break;
    case 1:
      Serial1.write(ch);
      break;
    case 2:
      Serial2.write(ch);
      break;
    case 3:
      Serial3.write(ch);
      break;
    default:
      break;
  }
}
void serialWriteInt(int index, int val) {
  switch (index) {
    case 0:
      Serial.write(val);
      break;
    case 1:
      Serial1.write(val);
      break;
    case 2:
      Serial2.write(val);
      break;
    case 3:
      Serial3.write(val);
      break;
    default:
      break;
  }
}

void refresh(int in) {
  // Serial.println("refresh");
  // Serial.println(in);
  for (int i = 0; i < 4; i++) {
    if (in == i) {
      parent = i;
      serialWriteChar(i, '#');
    } else {
      serialWriteChar(i, '@');
    }
  }
  // Serial.print(parent);
  // Serial.print(' ');
  // for (int i = 0; i < 4; i++) {
  //   Serial.print(children[i]);
  // }
}
void setConnected(int child) {
  children[child] = true;
}
void broadcast(char a, int id) {
  for (int i = 0; i < 4; i++) {
    if (i == parent) continue;
    if (a == 'L') {
      serialWriteChar(i, 'L');
    } else if (a == 'H') {
      serialWriteChar(i, 'H');
    }
    serialWriteInt(i, id);
  }
}
/////////////////////////////
/////////////////////////////
void loop() {
  if (light_on) {
    if (millis() - lightOnTime > light_duration) {
      light_on = false;
      clearAll();
    }
  }
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 4; i++) {
    if (serialAvailable(i)) {
      int c = serialPeek(i);
      if (c == 0) {
        serialRead(i);
      }  //d for stm32 clean word
      if (c == '@') {
        Serial.println("check");
        serialRead(i);  //delete '@'or'#'
        refresh(i);
        t = millis();
        coordinateIsSent = true;
      }
      if (c == '#') {
        serialRead(i);
        setConnected(i);
      }
    }
  }

  if (parent != -1) {
    int c = serialPeek(parent);
    //Serial.println(c);
    if (c == 'L' || c == 'U' || c == 'H') {
      int d = serialRead(parent);
      int receive_id = serialRead(parent);
      Serial.println(receive_id);
      if (c == 'L') {
        if (own_id == receive_id) {
          //servo motion
          lockControl(1);
          Serial.println(12345);
        } else {
          broadcast(c, receive_id);
          Serial.println("broadcast");
        }
      } else if (c == 'H') {
        if (own_id == receive_id) {
          Serial.println("HInt");
          showAll(5);
          delay(500);
          light_on = true;
          lightOnTime = millis();
          //light_motion
        } else {
          broadcast(c, receive_id);
        }
      } else if (c == 'U') {
        if (own_id == receive_id) {
          //servo unlock motion
          lockControl(0);
        } else {
          broadcast(c, receive_id);
        }
      }
    }
  }

  if (millis() - t > 500 && coordinateIsSent) {  //children have been ckecked
    if (children[0]) {
      delay(100);
      while (serialAvailable(0)) {
        int a, b, id;
        id = serialRead(0);
        serialWriteInt(parent, id);
        a = serialRead(0);
        serialWriteInt(parent, a);
        b = serialRead(0) + 1;
        serialWriteInt(parent, b);
      }
    }
    if (children[1]) {
      delay(100);
      while (serialAvailable(1)) {
        int a, b, id;
        id = serialRead(1);
        serialWriteInt(parent, id);
        a = serialRead(1);
        serialWriteInt(parent, a);
        b = serialRead(1) - 1;
        serialWriteInt(parent, b);
      }
    }
    if (children[2]) {
      delay(100);
      while (serialAvailable(2)) {
        int a, b, id;
        id = serialRead(2);
        serialWriteInt(parent, id);
        a = serialRead(2) - 1;
        serialWriteInt(parent, a);
        b = serialRead(2);
        serialWriteInt(parent, b);
      }
    }
    if (children[3]) {
      delay(100);
      while (serialAvailable(3)) {
        int a, b, id;
        id = serialRead(3);
        serialWriteInt(parent, id);
        a = serialRead(3) + 1;
        serialWriteInt(parent, a);
        b = serialRead(3);
        serialWriteInt(parent, b);
      }
    }
    Serial.print('\n');
    Serial.println(parent);
    Serial.println(own_id);
    Serial.println(0);
    Serial.println(0);
    serialWriteInt(parent, own_id);
    serialWriteInt(parent, 0);
    serialWriteInt(parent, 0);
    coordinateIsSent = false;
    Serial.println("coordinate is sent");
  }
  //  Serial.print(parent);
  //  Serial.print(' ');
  //  for (int i = 0; i < 4; i++) {
  //    Serial.print(children[i]);
  //  }
  //  Serial.print('\n');
  delay(50);
}