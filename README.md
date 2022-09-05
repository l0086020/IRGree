# 格力空调远程遥控程序
[BiliBili视频演示](https://www.bilibili.com/video/BV1zt4y1V7iK?share_source=copy_web&vd_source=a87486ca7ecd0a754606aaf5b7b2b5ff)
- 基于ESP8266的远程控制格力空调
  
# 使用方法
- 代码使用Visual Code - Platform IO插件编写，代码完全兼容Arduino平台
- 如果使用Arduino IDE开发环境，可复制 ` src ` 目录下的 ` main.cpp ` 文件里的代码到Arduino IDE里面粘贴使用（注意添加相应的头文件，如` [ESP8266红外编码库](https://github.com/crankyoldgit/IRremoteESP8266.git) `、` Blinker.h ` ，同时需删除 ` Arduino.h ` ）

# App界面
- 使用 ` 点灯科技 ` APP新建一个WiFi设备，点击右上角 ` 三个点 ` —— ` 界面配置 ` ，把目录里面的json数据复制粘贴到对话框里面，点击 ` 更新配置 ` 后退出软件，再次打开即可更新界面

# v1.2.0
- 更改空调模式交互方法
- 修复BUG
