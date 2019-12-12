# **driver.launch parameter default**
```js
portname (str, default: /dev/ttyUSB0)
接口名稱

odomframe (str, default: odom)
odom

baseframe (str, default: baselink)
baselink

baudrate (int, default: 115200)
波特率

controlrate (int, default: 10)
控制频率

sensorrate (int, default: 5)
請求電池電量频率

reductionratio (double, default: 1)
減速⽐

encoderresolution (double, default: 1600)
編碼器的編碼數

wheeldiameter (double, default: 0.25)
車輪直徑

modelparamcw (double, default:0.8)
顺時針旋轉運動模型參數

modelparamacw (double, default:0.8)
逆時針旋轉運動模型參数

pidrate (double, default:50.0)
PID控制周期
```
