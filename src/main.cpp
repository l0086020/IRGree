#define BLINKER_WIFI

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Kelvinator.h>
#include <Blinker.h>

char auth[] = "839d1d3c4112";
char ssid[] = "Smart Home";
char pswd[] = "321162955";

int nowtemp = 24; 
int num_mode = 1; 
int nowfan = 5;   

BlinkerNumber NUM1("fan");
BlinkerNumber NUM2("settemp");
BlinkerButton Kelvinator_power("btn-pwr");
BlinkerButton Kelvinator_setmode("btn-mode");
BlinkerButton Kelvinator_Light("btn-deng");
BlinkerButton Kelvinator_sxfan("btn-sx");
BlinkerSlider Slider1("ran-wen");
BlinkerSlider Slider2("ran-fan");

const uint16_t kIrLed = 14;
IRKelvinatorAC ac(kIrLed);

void printState() {
  Serial.println("Kelvinator A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());

  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kKelvinatorStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

//空调电源开关
void Kelvinator_power_callback(const String &state)
{
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_ON)
  {
    ac.on();
    ac.setFan(5);
    ac.setMode(kKelvinatorCool);
    ac.setTemp(26);
    ac.setLight(true);
    ac.send();
    Kelvinator_power.icon("fal fa-power-off");
    Kelvinator_power.color("#00FF00");
    Kelvinator_power.text("开");
    Kelvinator_power.print("on");
  }
  else if (state == BLINKER_CMD_OFF)
  {
    ac.off();
    ac.setLight(true);
    ac.send();
    Kelvinator_power.icon("fal fa-power-off");
    Kelvinator_power.color("#FF0000");
    Kelvinator_power.text("关");
    Kelvinator_power.print("off");
  }
}

//空调灯光
void Kelvinator_Light_callback(const String &state)
{
  BLINKER_LOG("get button state: ", state);
  if (state == BLINKER_CMD_ON)
  {
    ac.setLight(true);
    ac.send();
    Kelvinator_Light.color("#00FF00");
    Kelvinator_Light.text("灯光开");
    Kelvinator_Light.print("on");
  }
  else if (state == BLINKER_CMD_OFF)
  {
    ac.setLight(false);
    ac.send();
    Kelvinator_Light.color("#999999");
    Kelvinator_Light.text("灯光关");
    Kelvinator_Light.print("off");
  }
}

//上下扇风
void Kelvinator_sxfan_callback(const String &state)
{
  BLINKER_LOG("get button state: ", state);
  if (state == BLINKER_CMD_ON)
  {
    // ac.setSwingVertical(true);
    ac.send();
    Kelvinator_sxfan.color("#00FF00");
    Kelvinator_sxfan.text("上下风");
    Kelvinator_sxfan.print("on");
  }
  else if (state == BLINKER_CMD_OFF)
  {
    // ac.setSwingVertical(false);
    ac.send();
    Kelvinator_sxfan.color("#999999");
    Kelvinator_sxfan.text("上下风");
    Kelvinator_sxfan.print("off");
  }
}

//心跳包
void heartbeat()
{
  switch (num_mode)
  {
    case 1:

      Kelvinator_setmode.icon("far fa-recycle");
      Kelvinator_setmode.color("#7FFFD4");
      Kelvinator_setmode.text("自动");

      break;
    case 2:

      Kelvinator_setmode.icon("fas fa-snowflake");
      Kelvinator_setmode.color("#1E90FF");
      Kelvinator_setmode.text("制冷");
      break;
    case 3:

      Kelvinator_setmode.icon("fas fa-tint");
      Kelvinator_setmode.color("#7CFC00");
      Kelvinator_setmode.text("除湿");
      break;
    case 4:

      Kelvinator_setmode.icon("fab fa-first-order-alt");
      Kelvinator_setmode.color("#E0FFFF");
      Kelvinator_setmode.text("送风");
      break;
    case 5:

      Kelvinator_setmode.icon("fas fa-sun");
      Kelvinator_setmode.color("#FF0000");
      Kelvinator_setmode.text("制热");
      break;
  }
  Kelvinator_setmode.print();
  NUM2.print(nowtemp);
}
//空调模式按钮
void Kelvinator_setmode_callback(const String &state)
{
  if (state == BLINKER_CMD_BUTTON_TAP)
  {
    num_mode++;
    if (num_mode >= 6)
    {
      num_mode = 1;
    }

    switch (num_mode)
    {
      case 1:
        ac.setMode(kKelvinatorAuto);
        Kelvinator_setmode.icon("fal fa-recycle");
        Kelvinator_setmode.color("#008000");
        Kelvinator_setmode.text("自动");
        break;

      case 2:
        ac.setMode(kKelvinatorCool);
        Kelvinator_setmode.icon("fal fa-snowflake");
        Kelvinator_setmode.color("#1E90FF");
        Kelvinator_setmode.text("制冷");
        break;

      case 3:
        ac.setMode(kKelvinatorDry);
        Kelvinator_setmode.icon("fal fa-tint-slash");
        Kelvinator_setmode.color("#87CEFA");
        Kelvinator_setmode.text("除湿");
        break;

      case 4:
        ac.setMode(kKelvinatorFan);
        Kelvinator_setmode.icon("fab fa-first-order-alt");
        Kelvinator_setmode.color("#2E8B57");
        Kelvinator_setmode.text("送风");
        break;

      case 5:
        ac.setMode(kKelvinatorHeat);
        Kelvinator_setmode.icon("fas fa-sun");
        Kelvinator_setmode.color("#FF0000");
        Kelvinator_setmode.text("制热");
        break;
    }
    Kelvinator_setmode.print();
    ac.send();
  }
}


//空调风速
void slider2_callback(int32_t value)
{
  nowfan=value;
  NUM1.print(nowfan);
  ac.setFan(nowfan);
  ac.setXFan(true);
  ac.send();
}

//空调温度
void slider1_callback(int32_t value)
{
  nowtemp=value;
  NUM2.print(nowtemp);
  ac.setTemp(nowtemp);
  ac.send();
}

void setup()
{
  ac.begin();
  Kelvinator_power.attach(Kelvinator_power_callback);
  Kelvinator_setmode.attach(Kelvinator_setmode_callback);
  Kelvinator_Light.attach(Kelvinator_Light_callback);
  Kelvinator_sxfan.attach(Kelvinator_sxfan_callback);
  Slider1.attach(slider1_callback);
  Slider2.attach(slider2_callback);
  printState();
  Blinker.attachHeartbeat(heartbeat);
  Blinker.begin(auth,ssid,pswd); 
}

void loop()
{
  Blinker.run(); 
}
