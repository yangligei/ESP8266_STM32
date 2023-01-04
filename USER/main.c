#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "malloc.h"
#include "esp8266.h"
#include <jansson.h> 
#include "pic.h"
#include "lcd_init.h"
#include "lcd.h"


// ！！！！！！！！！！！！注意！！！！！！！！！！！！！！
//移植时，注意cjson包中两个malloc函数修改；malloc函数管理内存大小不能过大；系统启动文件 堆 栈 的大小要进行修改，否则会内存泄漏。
// ！！！！！！！！！！！！注意！！！！！！！！！！！！！！



//v0.32在v0.31的失败的基础上重新改进
//2022.12.31_22：00：建立tcp链接GET请求延时时间过短，应当在发送GET请求后，延时至少1s等待响应。
//v0.4：解决json数据解析问题
//     json短串可以解析，存在长串解析时溢出问题，在v0.41解决
//v0.41:解决内存泄露，esp8266以及TCP通信解决。
//v0.5：增加1.96IPS 显示 使用SPI通信
//v0.6：加入ip地址获取地理位置。
//准备移植IVGL库，此版本停止更新。
//此版本完成：建立esp8266与MCU的链接，获取并解析JSON协议。

int main(void)
 {	
	 //
	 u8 i,j;
	float t=0;
	// u8 sys_sta = 0;  //状态指示
	 delay_init();
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 uart_init2(115200);
     uart_init(115200);
	 mem_init(); //内存管理初始化
	 LCD_Init();//LCD初始化
	 LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	 
	 //显示基础信息
	 LCD_ShowString(0,30,"The_Weather_of:",GREEN,BLACK,16,0);
	 LCD_ShowString(130,30,"---",GREEN,BLACK,16,0);
	 LCD_ShowString(0,70,"Weather:",GREEN,BLACK,16,0);
	 LCD_ShowString(0,95,"Temperature:",GREEN,BLACK,16,0);
	 LCD_ShowIntNum(100,95,10,4,GREEN,BLACK,16);
	 LCD_ShowString(100,95,"--- ",GREEN,BLACK,16,0);
	 LCD_ShowChinese16x16(135,95,"℃",GREEN,BLACK,16,0);
	 LCD_ShowString(0,150,"Syetem_Status:",GREEN,BLACK,16,0);
	 LCD_ShowString(130,150,"Location",GREEN,BLACK,12,0);
	 LCD_ShowPicture(200,145,16,16,gImage_cuo);//状态指示 错
	 LCD_ShowString(130,165,"Weather_Inf",GREEN,BLACK,12,0);
     LCD_ShowPicture(200,162,16,16,gImage_cuo);//状态指示 错
	 LCD_ShowString(30,180,"LCD_Init_Finished",GREEN,BLACK,16,0);
	 LCD_DrawLine(0,130,240,130,GREEN);
	 LCD_DrawLine(0,215,240,215,GREEN);
	 LCD_ShowString(10,220,"Update_time:",GREEN,BLACK,16,0);
	 LCD_ShowString(150,270,"Ver:0.6",GREEN,BLACK,12,0);
	 LCD_ShowString(100,70,"---",GREEN,BLACK,16,0);
	 delay_ms(100);
	 
     //
	  extern  u8 sys_sta;//状态指示 
      get_ip_location(); // wifi初始化并获取当前IP对应的地理位置
	  if(sys_sta ==1)
	  {
		LCD_ShowPicture(200,145,16,16,gImage_dui);//状态指示 
	  
		init_wifi(); //获取天气信息
		if(sys_sta == 2)
		{
			LCD_ShowPicture(200,162,16,16,gImage_dui);	//状态指示
		}		
      }
	  while(1)
	 {
		
	 }
 }
 

