#define BLINKER_WIFI

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Gree.h>
#include <Blinker.h>

char auth[] = ""; //填写你的点灯科技项目的密钥
char ssid[] = ""; //填写你的WiFi账号
char pswd[] = ""; //填写你的WiFi密码

//风速显示数值栏
BlinkerNumber NUM1("fan");
//设置的湿度显示栏
BlinkerNumber NUM2("settemp");

//App界面里的功能按键：电源开关、灯光开关、风速开关、节能开关
BlinkerButton Button_power("btn-pwr");
BlinkerButton Button_Light("btn-deng");
BlinkerButton Button_sxfan("btn-sx");
BlinkerButton Button_save("btn-save");

//App界面里的空调模式按键，制冷开关、制热开关、送风开关、除湿开关
BlinkerButton Button_Cold("btn-cold");
BlinkerButton Button_Hot("btn-hot");
BlinkerButton Button_Wind("btn-wind");
BlinkerButton Button_Hum("btn-hum");

//App界面里的湿度及风速滑块
BlinkerSlider Slider1("ran-wen");
BlinkerSlider Slider2("ran-fan");

//定义红外数据发送的引脚端口
const uint16_t kIrLed = 14;

//默认上电状态
uint8_t KeyStatus = 0x02;

//实例化一个格力空调ac对象
IRGreeAC ac(kIrLed);

//心跳包
void heartbeat(void);

int nowtemp = 26;
int nowfan = 2;

/**
  * @file    main.cpp
  * @brief   空调电源开关按键
  * @param   Unknow
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void Button_power_callback(const String &state)
{
  if (state == BLINKER_CMD_ON)
  {
    ac.on();
    ac.send();
    KeyStatus |= 1<<0;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    ac.off();
    ac.send();
    KeyStatus &= ~(1<<0);
  }

  Blinker.vibrate(100);
  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   空调灯光开关按键
  * @param   Unknown
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void Button_Light_callback(const String &state)
{
  if (state == BLINKER_CMD_ON)
  {
    ac.setLight(true);
    ac.send();
    KeyStatus |= 1<<1;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    if(ac.getPower()) ac.on();
    else ac.off();
    ac.setLight(false);
    ac.send();
    KeyStatus &= ~(1<<1);
  }

  Blinker.vibrate(100);
  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   上下扇风开关按键
  * @param   Unknown
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void Button_sxfan_callback(const String &state)
{
  if (state == BLINKER_CMD_ON)
  {
    ac.setSwingVertical(true,kGreeSwingLastPos);
    ac.send();
    KeyStatus |= 1<<2;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    ac.setSwingVertical(false,kGreeSwingHOff);
    ac.send();
    KeyStatus &= ~(1<<2);
  }

  Blinker.vibrate(100);
  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   节能开关按键
  * @param   Unknown
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void Button_Save_callback(const String &state)
{
  if (state == BLINKER_CMD_ON)
  {
    ac.setEcono(true);
    ac.send();
    KeyStatus |= 1<<3;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    ac.setEcono(false);
    ac.send();
    KeyStatus &= ~(1<<3);
  }

  Blinker.vibrate(100);
  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   制冷模式
  * @param   Unknown
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void Button_Cold_callback(const String &state)
{
  Blinker.vibrate(100);
  ac.setFan(1);
  ac.setMode(GREE_COOL);
  ac.send();

  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   制热模式
  * @param   Unknown
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void Button_Hot_callback(const String &state)
{
  Blinker.vibrate(100);

  ac.setFan(1);
  ac.setMode(GREE_HEAT);
  ac.send();

  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   送风模式
  * @param   Unknown
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void Button_Wind_callback(const String &state)
{
  Blinker.vibrate(100);

  ac.setFan(1);
  ac.setMode(GREE_FAN);
  ac.send();

  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   除湿模式
  * @param   Unknown
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void Button_Hum_callback(const String &state)
{
  Blinker.vibrate(100);

  ac.setMode(GREE_DRY);
  ac.send();

  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   用于检测连接和状态的心跳包
  * @param   None
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void heartbeat()
{
  switch(ac.getMode())
  {
    case 0:
      Button_Cold.color("#808080");
      Button_Hot.color("#808080");
      Button_Wind.color("#808080");
      Button_Hum.color("#808080");
      NUM2.icon("fab fa-adn");
      NUM2.text("自动模式");
    break;

    case 1:
      Button_Cold.color("#00FF00");
      Button_Hot.color("#808080");
      Button_Wind.color("#808080");
      Button_Hum.color("#808080");
    break;

    case 2:
      Button_Cold.color("#808080");
      Button_Hot.color("#808080");
      Button_Wind.color("#808080");
      Button_Hum.color("#0000FF");
    break;

    case 3:
      Button_Cold.color("#808080");
      Button_Hot.color("#808080");
      Button_Wind.color("#EE00EE");
      Button_Hum.color("#808080");
    break;

    case 4:
      Button_Cold.color("#808080");
      Button_Hot.color("#FF0000");
      Button_Wind.color("#808080");
      Button_Hum.color("#808080");
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
  if(ac.getFan() == 0) NUM1.text("自动风速");
  NUM1.print(ac.getFan());
  NUM2.print(ac.getTemp());
  Slider1.print(ac.getTemp());
  Slider2.print(ac.getFan());
}

/**
  * @file    main.cpp
  * @brief   空调风速滑动块
  * @param   value：需要设置的风速档位0-3档
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void slider2_callback(int32_t value)
{
  nowfan=value;
  Blinker.vibrate(100);

  ac.setFan(nowfan);
  ac.send();

  heartbeat();
}

/**
  * @file    main.cpp
  * @brief   空调湿度滑动块
  * @param   value：需要设置的温度数值：16-30度
  * @return  None
  * @version v1.0.0
  * @date    2022-09-05
  */
void slider1_callback(int32_t value)
{
  nowtemp=value;
  Blinker.vibrate(100);

  ac.setTemp(nowtemp);
  ac.send();

  heartbeat();
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
  WiFi.setAutoReconnect(true);
}
