# SmartEVSE 项目学习

## CP部分

### 输出

使用`LEDC`模块输出PWM信号, 然后有一个模拟开关芯片控制`CP`信号与汽车的连接。

```c
#define PIN_CP_OUT 19
#define PIN_CPOFF 15

pinMode(PIN_CP_OUT, OUTPUT);
pinMode(PIN_CPOFF, OUTPUT);
digitalWrite(PIN_CPOFF, HIGH);  // 用来控制模拟开关

ledcSetup(0, 1000, 10);
ledcAttachPin(PIN_CP_OUT, 0);
ledcWrite(0, 500);  // 设置完占空比后, 就会开启PWM输出
```

#### CP输出控制优化方案

SmartEVSE的作者是使用模拟开关来控制CP信号是否输出, 但是这个开关比较贵, 所以看看是否可以使用一个替代的方案, 比如关闭PWM输出(直接输出高电平或者低电平)。

```c
ledc_stop(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 0); // 强制输出低电平
ledcWrite(0, 500);  // 重新开启PWM输出
```

### 采样

ADC采样在定时器中断中触发:
1. 当CP信号没有输出时, 定时器每1ms触发一次中断, 并在CP_OUT中断中重置定时器。
2. 当CP开启PWM输出时, 将定时器触发位置设置在CP信号5%占空比的时候(大概是上升沿过后50us)。
