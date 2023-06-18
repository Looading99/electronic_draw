# electronic_draw
基于Arduino、点阵屏、OLED屏幕的电子抽签器

## 功能介绍
点击抽签后，抽签器会先随机抽取编号并显示，重复10次，每次间隔150毫秒，最后再随机抽取一个编号并闪烁，代表最终抽到的人。

细节：抽签器每次启动都会根据悬空的模拟引脚A0和A2的乘积作为随机数种子，由于悬空的模拟引脚读取到的值是随机的，所以每次启动随机数都不会重复。
抽签器会记住之前抽到的31(由宏`LEN_CALLED`设定)个编号，被记住的编号不会被抽到。由于其储存在EEPROM中，即使重启也不会导致记住的编号丢失。

## 用到的库
+LedControl (http://wayoda.github.io/LedControl/)
+ssd1306 (https://github.com/lexus2k/ssd1306)
+Enerlib (https://playground.arduino.cc/Code/Enerlib/)
