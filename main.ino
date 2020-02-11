#include <Arduino.h>
// 引脚定义
// 继电器输出
int output = 4; 
// 手动启动
int start = 3;
// 自动停止信号
int stop = 2;      
// 输出引脚状态
volatile int state = LOW;
// 时间定义
int h = 0;
int m = 0;
int s = 0;
// 启动起始时间与结束时间
long start_time = 0;
long now =0;
// 最长输出时间（单位：秒）
int max_out_time = 1200;

void setup()
 {
  pinMode(output, OUTPUT);         
  pinMode(start, INPUT_PULLUP);   
  pinMode(stop, INPUT_PULLUP);    
// 中断
  // 手动启动
  attachInterrupt(1, switch_true, FALLING);
  // 自动停止
  attachInterrupt(0, switch_false, FALLING);
}
void loop() {
// 时间转换
  if (s== 60)
  {
    m += 1;
    s = 0;
  }
  if (m==60)
  {
    h += 1;
    m = 0;
  }
  if (h==24)
  {
    h = 0;
  }
  
  // 定时启动
  if (h==0 && m==0 && s == 10)
  {
     state = true;
     start_time = h * 3600 + m * 60 + s;
  }
  // 判断是否启动
  if (state)
  {
    now = h * 3600 + m * 60 + s;
    // 判断启动是否超时（自动停止失效保护）
    if (now-start_time > max_out_time)
    {
      // 超时复位
      state = false;  
      digitalWrite(output, false); 
    }
  }
  delay(1000);
  s += 1;
  digitalWrite(output, state); 
}

// 手动启动
void switch_true() {               
  state = true;
  start_time = h * 3600 + m * 60 + s;
  digitalWrite(output, true);              
}

// 自动停止
void switch_false() {               
  state = false;  
  digitalWrite(output, false);           
}


