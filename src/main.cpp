#define BLINKER_WIFI

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Kelvinator.h>
#include <Blinker.h>

char auth[] = "839d1d3c4112";
char ssid[] = "TP-LINK_7F78";
char pswd[] = "321162955";

int nowtemp = 24; 
int num_mode = 1; 
int nowfan = 2;   

BlinkerNumber NUM1("fan");
BlinkerNumber NUM2("settemp");

BlinkerButton Button_power("btn-pwr");
BlinkerButton Button_Light("btn-deng");
BlinkerButton Button_sxfan("btn-sx");
BlinkerButton Button_save("btn-save");

BlinkerButton Button_Cold("btn-cold");
BlinkerButton Button_Hot("btn-hot");
BlinkerButton Button_Wind("btn-wind");
BlinkerButton Button_Hum("btn-hum");

BlinkerSlider Slider1("ran-wen");
BlinkerSlider Slider2("ran-fan");

const uint16_t kIrLed = 14;
uint8_t KeyStatus = 0x02,ModeStatus = 1;
IRKelvinatorAC ac(kIrLed);

//空调电源开关
void Button_power_callback(const String &state)
{
  if (state == BLINKER_CMD_ON)
  {
    Button_power.print("on");
    Blinker.vibrate(1000);
    ac.on();
    ac.setFan(5);
    ac.setMode(kKelvinatorCool);
    ac.setTemp(26);
    ac.setLight(true);
    ac.send();
    KeyStatus |= 1<<0;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    Button_power.print("off");
    Blinker.vibrate(1000);
    ac.off();
    ac.setLight(true);
    ac.send();
    KeyStatus &= ~(1<<0);
  }
}

//空调灯光
void Button_Light_callback(const String &state)
{
  if (state == BLINKER_CMD_ON)
  {
    Button_Light.print("on");
    Blinker.vibrate(1000);
    ac.setLight(true);
    ac.send();
    KeyStatus |= 1<<1;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    Button_Light.print("off");
    Blinker.vibrate(1000);
    ac.setLight(false);
    ac.send();
    KeyStatus &= ~(1<<1);
  }
}

//上下扇风
void Button_sxfan_callback(const String &state)
{
  if (state == BLINKER_CMD_ON)
  {
    Button_sxfan.print("on");
    Blinker.vibrate(1000);
    // ac.setSwingVertical(true);
    ac.send();
    KeyStatus |= 1<<2;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    Button_sxfan.print("off");
    Blinker.vibrate(1000);
    // ac.setSwingVertical(false);
    ac.send();
    KeyStatus &= ~(1<<2);
  }
}

//节能开关
void Button_Save_callback(const String &state)
{
  if (state == BLINKER_CMD_ON)
  {
    Button_save.print("on");
    Blinker.vibrate(1000);
    // ac.setSwingVertical(true);
    ac.send();
    KeyStatus |= 1<<3;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    Button_save.print("off");
    Blinker.vibrate(1000);
    // ac.setSwingVertical(false);
    ac.send();
    KeyStatus &= ~(1<<3);
  }
}

//制冷开关
void Button_Cold_callback(const String &state)
{
  Button_Cold.color("#00FF00");
  Button_Hot.color("#808080");
  Button_Wind.color("#808080");
  Button_Hum.color("#808080");
  Button_Cold.print();
  Button_Hot.print();
  Button_Wind.print();
  Button_Hum.print();

  Blinker.vibrate(1000);

  ModeStatus = 0x01;
  ac.setMode(kKelvinatorCool);
  ac.send();
}

//制热开关
void Button_Hot_callback(const String &state)
{
  Button_Cold.color("#808080");
  Button_Hot.color("#FF0000");
  Button_Wind.color("#808080");
  Button_Hum.color("#808080");
  Button_Cold.print();
  Button_Hot.print();
  Button_Wind.print();
  Button_Hum.print();

  Blinker.vibrate(1000);

  ModeStatus = 0x02;
  ac.setMode(kKelvinatorHeat);
  ac.send();
}

//送风开关
void Button_Wind_callback(const String &state)
{
  Button_Cold.color("#808080");
  Button_Hot.color("#808080");
  Button_Wind.color("#EE00EE");
  Button_Hum.color("#808080");
  Button_Cold.print();
  Button_Hot.print();
  Button_Wind.print();
  Button_Hum.print();

  Blinker.vibrate(1000);

  ModeStatus = 0x04;
  ac.setMode(kKelvinatorFan);
  ac.send();
}

//除湿开关
void Button_Hum_callback(const String &state)
{
  Button_Cold.color("#808080");
  Button_Hot.color("#808080");
  Button_Wind.color("#808080");
  Button_Hum.color("#0000FF");
  Button_Cold.print();
  Button_Hot.print();
  Button_Wind.print();
  Button_Hum.print();

  Blinker.vibrate(1000);

  ModeStatus = 0x08;
  ac.setMode(kKelvinatorDry);
  ac.send();
}

//心跳包
void heartbeat()
{
  switch(ModeStatus)
  {
    case 0x01:
      Button_Cold.color("#00FF00");
      Button_Hot.color("#808080");
      Button_Wind.color("#808080");
      Button_Hum.color("#808080");
    break;

    case 0x02:
      Button_Cold.color("#808080");
      Button_Hot.color("#FF0000");
      Button_Wind.color("#808080");
      Button_Hum.color("#808080");
    break;

    case 0x04:
      Button_Cold.color("#808080");
      Button_Hot.color("#808080");
      Button_Wind.color("#EE00EE");
      Button_Hum.color("#808080");
    break;

    case 0x08:
      Button_Cold.color("#808080");
      Button_Hot.color("#808080");
      Button_Wind.color("#808080");
      Button_Hum.color("#0000FF");
    break;
  }

  Button_Cold.print();
  Button_Hot.print();
  Button_Wind.print();
  Button_Hum.print();

  switch (KeyStatus)
  {
    case 0x00:
      Button_power.print("off");
      Button_Light.print("off");
      Button_sxfan.print("off");
      Button_save.print("off");
    break;

    case 0x01:
      Button_power.print("on");
      Button_Light.print("off");
      Button_sxfan.print("off");
      Button_save.print("off");
    break;

    case 0x02:
      Button_power.print("off");
      Button_Light.print("on");
      Button_sxfan.print("off");
      Button_save.print("off");
    break;

    case 0x03:
      Button_power.print("on");
      Button_Light.print("on");
      Button_sxfan.print("off");
      Button_save.print("off");
    break;

    case 0x04:
      Button_power.print("off");
      Button_Light.print("off");
      Button_sxfan.print("on");
      Button_save.print("off");
    break;

    case 0x05:
      Button_power.print("on");
      Button_Light.print("off");
      Button_sxfan.print("on");
      Button_save.print("off");
    break;

    case 0x06:
      Button_power.print("off");
      Button_Light.print("on");
      Button_sxfan.print("on");
      Button_save.print("off");
    break;

    case 0x07:
      Button_power.print("on");
      Button_Light.print("on");
      Button_sxfan.print("on");
      Button_save.print("off");
    break;

    case 0x08:
      Button_power.print("off");
      Button_Light.print("off");
      Button_sxfan.print("off");
      Button_save.print("on");
    break;

    case 0x09:
      Button_power.print("on");
      Button_Light.print("off");
      Button_sxfan.print("off");
      Button_save.print("on");
    break;

    case 0x0A:
      Button_power.print("off");
      Button_Light.print("on");
      Button_sxfan.print("off");
      Button_save.print("on");
    break;

    case 0x0B:
      Button_power.print("on");
      Button_Light.print("on");
      Button_sxfan.print("off");
      Button_save.print("on");
    break;

    case 0x0C:
      Button_power.print("off");
      Button_Light.print("off");
      Button_sxfan.print("on");
      Button_save.print("on");
    break;

    case 0x0D:
      Button_power.print("on");
      Button_Light.print("off");
      Button_sxfan.print("on");
      Button_save.print("on");
    break;

    case 0x0E:
      Button_power.print("off");
      Button_Light.print("on");
      Button_sxfan.print("on");
      Button_save.print("on");
    break;

    case 0x0F:
      Button_power.print("on");
      Button_Light.print("on");
      Button_sxfan.print("on");
      Button_save.print("on");
    break;
  }
  NUM1.print(nowfan);
  NUM2.print(nowtemp);
  Slider1.print(nowtemp);
  Slider2.print(nowfan);
}

//空调风速
void slider2_callback(int32_t value)
{
  nowfan=value;
  Blinker.vibrate(50);
  NUM1.print(nowfan);
  ac.setFan(nowfan);
  ac.setXFan(true);
  ac.send();
}

//空调温度
void slider1_callback(int32_t value)
{
  nowtemp=value;
  Blinker.vibrate(50);
  NUM2.print(nowtemp);
  ac.setTemp(nowtemp);
  ac.send();
}

void setup()
{
  ac.begin();

  Button_power.attach(Button_power_callback);
  Button_Light.attach(Button_Light_callback);
  Button_sxfan.attach(Button_sxfan_callback);
  Button_save.attach(Button_Save_callback);
  Button_Cold.attach(Button_Cold_callback);
  Button_Hot.attach(Button_Hot_callback);
  Button_Wind.attach(Button_Wind_callback);
  Button_Hum.attach(Button_Hum_callback);

  Slider1.attach(slider1_callback);
  Slider2.attach(slider2_callback);
  Blinker.attachHeartbeat(heartbeat);
  Blinker.begin(auth,ssid,pswd); 
}

void loop()
{
  Blinker.run(); 
}
