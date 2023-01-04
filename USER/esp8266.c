//////////////
//8266相关函数
//作者：yjy
//时间：2021-9-12
/////////////
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "esp8266.h"
#include "ana.h"
#include "lcd.h"

//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:1,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)

u8 check_cmd(u8 *str)  
{ 	
	char *strx=0;

	if(USART_RX_STA&0X8000)		//接收到一次数据了
	{ 
		
		
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
		USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//添加结束符
	}

	if(strx != NULL)
	{
	  return 0;
	}
	else
	{
	  return 1;
	}
}

/************
 * [ESP8266_SendCmd 发送命令到ESP8266]
 * @param  cmd   [需要发送的AT指令]
 * @param  reply [期望模块回显的内容]
 * @param  wait  [等待的时间(ms)]
 * @return       [期望内容等于实际回显内容返回1，否则0]
 *************/
u8 esp8266_SendCmd(u8 *cmd ,u8 * ack , u16 waittime)
{
    u8 res=0;
	 USART_RX_STA=0;
    USART1_Send_String(cmd); //发送命令
	//USART_SendData(USART2,strcmp( reply , ""));
	delay_ms(20);
    if(ack&&waittime)		//需要等待应答
	{
		//USART_SendData(USART2,11);
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(!check_cmd(ack))
				{
					
					break;//得到有效数据 
				}
			     USART_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}

	return res;
}
 

/************
 * esep8266 初始化
 *************/

u8 esp8266_Init()
{
  int res = 1;
  res = esp8266_SendCmd("AT+CWMODE_DEF=1\r\n","OK\r\n",50);
  return res;	
}

/************
 * esep8266 测试
 *************/

u8 esp8266_Test()
{
  int res = 1;
  //USART_SendData(USART2,res);
  res = esp8266_SendCmd("AT\r\n","OK",50);
  //delay_ms(10);
  //USART_SendData(USART2,res);
  return res;	
}


/************
 * esep8266 重启
 *************/

u8 esp8266_Rst()
{
  int res = 1;
  res = esp8266_SendCmd("AT+RET\r\n","ready",50);
  return res;	
}


/************
 * esep8266 连接WiFi
 *************/

u8 esp8266_ConWifi()
{
  int res = 1;
  res = esp8266_SendCmd("AT+CWJAP=\"0202\"\,\"123456yjy\"\r\n","OK",2000);
  return res;	
}


/************
 * esep8266 单或多链接
* num=0--->单链接
* num=1--->多链接
 *************/

u8 esp8266_CIP(u8 num)
{
  int res = 1;
//  if(num==0)	
//  {res = esp8266_SendCmd("AT+CIPMUX=0\r\n","OK",50);}
//  else if(num==1)
//  {res = esp8266_SendCmd("AT+CIPMUX=1\r\n","OK",50);}
	res = esp8266_SendCmd("AT+CIPMUX=0\r\n","OK\r\n",50);  
  return res;	
}


/************
 * esep8266 开始建立TCP通信（和天气api）
 *************/

u8 esp8266_TCP_Start()
{
  int res = 1; 
  USART_RX_STA=0;
  res = esp8266_SendCmd("AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n","OK\r\n",500);

  return res;	
}

/************
 * esep8266 设置TCP模式为透传
 *************/

u8 esp8266_TCP_Mode()
{
  int res = 1;
  res = esp8266_SendCmd("AT+CIPMODE=1\r\n","OK",500);
  return res;	
}


/************
 * esep8266 发送数据
 *************/

u8 esp8266_TCP_Send()
{
  int res = 1;
  res = esp8266_SendCmd("AT+CIPSEND\r\n","OK",500);
  return res;	
}

/************
 * esep8266 发送数据//并解析
 *************/

int esp8266_TCP_API()
{
  int res = 0;
   USART_RX_STA=0;	//清零串口1数据
  //char get_api[]="GET https://api.seniverse.com/v3/weather/now.json?key=SAkH50Olwvsp_E1iI&location=Baoji&language=zh-Hans&unit=c\r\n";//这里也要换行
   USART1_Send_String(get_api); //发送数据
  //USART_RX_STA=0;	//清零串口1数据
    delay_ms(2000);//等待2s
	delay_ms(5000);//等待2s
   if((USART_RX_STA)&0X8000)		//此时再次接到一次数据，为天气的数据
	//if(1)
	{ 
	  USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//添加结束符
		parse_now_weather();
	} 
  //解析
	 
	
   	
  return res;	
}




/************
 * esep8266 退出透传
 *************/

u8 esp8266_exit()
{
    char exit[]="+++";
	USART1_Send_String(exit); //发送数据
	delay_ms(500);					//等待500ms
	return esp8266_Test();//退出透传判断.
}


u8 esp8266_TCP_Released() //关闭TCP连接
{
  int res = 1;
  res = esp8266_SendCmd("AT+CIPCLOSE\r\n","OK",500);
  return res;	
}

/************
 * esep8266 开始建立TCP通信（和天气api）
 *************/

u8 esp8266_TCP_Start_Ip()
{
  int res = 1; 
  USART_RX_STA=0;
  res = esp8266_SendCmd("AT+CIPSTART=\"TCP\",\"ip.useragentinfo.com\",80\r\n","OK\r\n",500);
  return res;	
}

u8 esp8266_TCP_Ip_API()
{  
	int res = 0;
   USART_RX_STA=0;	//清零串口1数据
  char get_api[]="GET https://ip.useragentinfo.com/json\r\n";//这里也要换行
   USART1_Send_String(get_api); //发送数据
  //USART_RX_STA=0;	//清零串口1数据
    delay_ms(2000);//等待2s
	delay_ms(5000);//等待2s
   if((USART_RX_STA)&0X8000)		//此时再次接到一次数据，为天气的数据
	//if(1)
	{ 
	  USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//添加结束符
	  parse_now_ip();//解析
		
	} 	
  return res;	
}
	



u8 get_ip_location(void)
{
  if(!esp8266_Test())
 {
	
   if(!esp8266_Init())
   {
     LCD_Fill(30,180,240,210,BLACK);
	 LCD_ShowString(30,180,"ESP8266_Ready",GREEN,BLACK,16,0);
	 if(!esp8266_ConWifi()) //链接WiFi
	 {
		 LCD_Fill(30,180,240,210,BLACK);
		 LCD_ShowString(30,180,"WiFI_Connected",GREEN,BLACK,16,0);
		if(!esp8266_CIP(1)) //设置为多链接
		{
			if(!esp8266_TCP_Start_Ip()) //建立tcp链接
			{
				LCD_Fill(30,180,240,210,BLACK);
		        LCD_ShowString(30,180,"Tcp_Ip_Connected",GREEN,BLACK,16,0);
				if(!esp8266_TCP_Mode()) //设置TCP模式为透传
				{
				  if(!esp8266_TCP_Send()) //开始透传
				  {
					delay_ms(2000);//延时2s等待透传建立
					esp8266_TCP_Ip_API();
					return 1;
				  }
				}
			}
		}
	 }
    }
   }
   
}



u8 init_wifi(void)
{

 if(!esp8266_Test())
 {
	LCD_Fill(30,180,240,210,BLACK);
    LCD_ShowString(30,180,"ESP8266_Ready",GREEN,BLACK,16,0);
   if(1)
   {
	
	   
     if(1) //链接WiFi
	 {
//		 LCD_Fill(30,180,240,210,BLACK);
//		 LCD_ShowString(30,180,"WiFI_Connected",GREEN,BLACK,16,0);
		if(!esp8266_CIP(0)) //设置为单链接
		 {

		  if(!esp8266_TCP_Start()) //建立TCP链接
		  {
			  LCD_Fill(30,180,240,210,BLACK);
			  LCD_ShowString(30,180,"TCP_Connected",GREEN,BLACK,16,0);
		    if(!esp8266_TCP_Mode()) //设置TCP模式为透传
			{
	 
			    if(!esp8266_TCP_Send())
				{

					delay_ms(2000);//延时2s等待透传建立
					esp8266_TCP_API();
					return 1;
					
				}
			}
		  }
		 }
	 }
   }
 }
 return 0;
}
u8 alreday_init(void)
{
    delay_ms(5000);
	delay_ms(5000);
	delay_ms(5000);
	delay_ms(5000);
    delay_ms(5000);
	delay_ms(5000);
    delay_ms(5000);//等待自动连接wifi
	
			if(esp8266_CIP(0)) //设置为单链接
		 {

		  if(!esp8266_TCP_Start()) //建立TCP链接
		  {
	 
		    if(!esp8266_TCP_Mode()) //设置TCP模式为透传
			{
	 
			    if(!esp8266_TCP_Send())
				{

//					if(!esp8266_exit())
//					{
//					  USART_SendData(USART2,9);
//					}
					esp8266_TCP_API();
					return 1;
					
				}
			}
		  }
		 }
	
	return 0;
	
}



