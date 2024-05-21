#include <Servo.h>
#include <FastLED.h>

/*---------------------------- global variable/constant declaration ----------------------------*/
const int own_id = 0;
bool state[4] = {false, false, false, false};
bool refreshing = false;
Servo myservo;  // 建立一個 servo 物件，最多可建立 12個 servo
int pos = 0;    // 設定 Servo 位置的變數
int ctsPin = 5; // touch sensor define
bool lockState = false;
/*-------------------------- global variable/constant declaration end --------------------------*/

/*-------------------------------------------- LED --------------------------------------------*/
bool light_on = false;
unsigned long light_duration = 5000;
unsigned long lightOnTime;
unsigned long currentTime;

const int LIGHT_PIN = 4;    // Digital PIN
const int BRIGHTNESS = 128; // 亮度
const int LIGHT_NUM = 100;  // 最多燈數(可超過實際燈數)
// edit end
CRGB leds[LIGHT_NUM]; // 定義FastLED類別
// colors
int rainbow16[16][3] = {{255, 0, 0},      // 紅
                        {255, 85, 0},     // 橙
                        {255, 170, 0},    //
                        {255, 255, 0},    // 黃
                        {127, 255, 0},    //
                        {0, 255, 0},      // 綠
                        {0, 255, 85},     //
                        {0, 255, 170},    //
                        {0, 255, 255},    // 青
                        {0, 127, 255},    //
                        {0, 0, 255},      // 藍
                        {85, 0, 255},     // 紫
                        {170, 0, 255},    //
                        {255, 0, 255},    // 洋紅
                        {255, 0, 127},    //
                        {255, 255, 255}}; // 白

void showAll(int color)
{
  for (int i = 0; i < LIGHT_NUM; i++)
  {
    leds[i] = CRGB(rainbow16[color][0], rainbow16[color][1], rainbow16[color][2]);
  }
  FastLED.show();
}

void clearAll()
{
  for (int i = 0; i < LIGHT_NUM; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void Gradient(int color, int _start, int _end, int delay_t)
{
  // Serial.print(color);
  // Serial.print(", ");
  // Serial.print(_start);
  // Serial.print(", ");
  // Serial.println(_end);
  if (_start >= _end)
  {
    for (int i = _start; i <= _end; i++)
    {
      leds[i] = CRGB(rainbow16[color][0], rainbow16[color][1], rainbow16[color][2]);
      FastLED.show();
      delay(delay_t);
    }
  }
  else
  {
    for (int i = _start; i >= _end; i--)
    {
      leds[i] = CRGB(rainbow16[color][0], rainbow16[color][1], rainbow16[color][2]);
      FastLED.show();
      delay(delay_t);
    }
  }
}
/*------------------------------------------ LED end ------------------------------------------*/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  FastLED.addLeds<WS2812B, LIGHT_PIN, GRB>(leds, LIGHT_NUM); // 設定串列全彩LED參數
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(ctsPin, INPUT);
  myservo.attach(9);
}

void lockControl(int isLocked)
{
  if (isLocked == 1)
  {
    for (pos = 0; pos <= 90; pos++)
    {
      myservo.write(pos); // 告訴 servo 走到 'pos' 的位置
      delay(5);           // 等待 50ms 讓 servo 走到指定位置
    }
  }
  else if (isLocked == 0)
  {
    for (pos = 90; pos >= 0; pos--)
    {
      myservo.write(pos); // 告訴 servo 走到 'pos' 的位置
      delay(5);           // 等待 05ms 讓 servo 走到指定位置
    }
  }
}

void handleRefresh()
{
  Serial.print(own_id);
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
  while (Serial1.available())
  {
    //    Serial.print("serial1\n");
    if (Serial1.peek() == '#')
    {
      int c = Serial1.read();
      state[1] = true;
    }
    else
    {
      if (state[1])
      {
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
  while (Serial2.available())
  {
    //    Serial.print("serial2\n");
    if (Serial2.peek() == '#')
    {
      int c = Serial2.read();
      state[2] = true;
    }
    else
    {
      if (state[2])
      {
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
  while (Serial3.available())
  {
    //    Serial.print("serial3\n");
    if (Serial3.peek() == '#')
    {
      int c = Serial3.read();
      state[3] = true;
    }
    else
    {
      if (state[3])
      {
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

void loop()
{
  // put your main code here, to run repeatedly:
  /*------------------------------ LED control ------------------------------*/
  int ctsValue = digitalRead(ctsPin);
  if (ctsValue == HIGH && light_on == false)
  {
    showAll(2);
    light_on = true;
    lightOnTime = millis();
  }
  if (light_on)
  {
    if (millis() - lightOnTime > light_duration)
    {
      light_on = false;
      clearAll();
    }
  }
  /*---------------------------- LED control end ----------------------------*/
  /*-------------------------- command from server --------------------------*/
  while (Serial.available())
  {
    char cmd = Serial.peek();
    if (cmd == '@' && !refreshing)
    {
      Serial.read();
      refreshing = true;
      Serial1.write('@');
      Serial2.write('@');
      Serial3.write('@');
      //      Serial.print("@ sent\n");
    }
    else if (cmd == 'L')
    {
      // lock
      Serial.read();
      int lockId = Serial.read();
      if (lockId == own_id)
      {
        lockState = !lockState;
        lockControl(lockState);
      }
      else
      {
        Serial1.write('L');
        Serial2.write('L');
        Serial3.write('L');
        Serial1.write(lockId);
        Serial2.write(lockId);
        Serial3.write(lockId);
      }
    }
    else if (cmd == 'H')
    {
      // light
      Serial.read();
      int idNumber = Serial.read();
      for (int i = 0; i < idNumber; i++)
      {
        int lightId = Serial.read();
        if (lightId == own_id)
        {
          showAll(5);
          light_on = true;
          lightOnTime = millis();
        }
        else
        {
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
  /*------------------------ command from server end ------------------------*/

  if (refreshing)
  {
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
