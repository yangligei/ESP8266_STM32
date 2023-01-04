//////////////
//8266��غ���
//���ߣ�yjy
//ʱ�䣺2021-9-12
/////////////
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "esp8266.h"
#include "ana.h"
#include "lcd.h"

//ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:1,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)

u8 check_cmd(u8 *str)  
{ 	
	char *strx=0;

	if(USART_RX_STA&0X8000)		//���յ�һ��������
	{ 
		
		
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
		USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//��ӽ�����
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
 * [ESP8266_SendCmd �������ESP8266]
 * @param  cmd   [��Ҫ���͵�ATָ��]
 * @param  reply [����ģ����Ե�����]
 * @param  wait  [�ȴ���ʱ��(ms)]
 * @return       [�������ݵ���ʵ�ʻ������ݷ���1������0]
 *************/
u8 esp8266_SendCmd(u8 *cmd ,u8 * ack , u16 waittime)
{
    u8 res=0;
	 USART_RX_STA=0;
    USART1_Send_String(cmd); //��������
	//USART_SendData(USART2,strcmp( reply , ""));
	delay_ms(20);
    if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		//USART_SendData(USART2,11);
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(!check_cmd(ack))
				{
					
					break;//�õ���Ч���� 
				}
			     USART_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}

	return res;
}
 

/************
 * esep8266 ��ʼ��
 *************/

u8 esp8266_Init()
{
  int res = 1;
  res = esp8266_SendCmd("AT+CWMODE_DEF=1\r\n","OK\r\n",50);
  return res;	
}

/************
 * esep8266 ����
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
 * esep8266 ����
 *************/

u8 esp8266_Rst()
{
  int res = 1;
  res = esp8266_SendCmd("AT+RET\r\n","ready",50);
  return res;	
}


/************
 * esep8266 ����WiFi
 *************/

u8 esp8266_ConWifi()
{
  int res = 1;
  res = esp8266_SendCmd("AT+CWJAP=\"0202\"\,\"123456yjy\"\r\n","OK",2000);
  return res;	
}


/************
 * esep8266 ���������
* num=0--->������
* num=1--->������
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
 * esep8266 ��ʼ����TCPͨ�ţ�������api��
 *************/

u8 esp8266_TCP_Start()
{
  int res = 1; 
  USART_RX_STA=0;
  res = esp8266_SendCmd("AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n","OK\r\n",500);

  return res;	
}

/************
 * esep8266 ����TCPģʽΪ͸��
 *************/

u8 esp8266_TCP_Mode()
{
  int res = 1;
  res = esp8266_SendCmd("AT+CIPMODE=1\r\n","OK",500);
  return res;	
}


/************
 * esep8266 ��������
 *************/

u8 esp8266_TCP_Send()
{
  int res = 1;
  res = esp8266_SendCmd("AT+CIPSEND\r\n","OK",500);
  return res;	
}

/************
 * esep8266 ��������//������
 *************/

int esp8266_TCP_API()
{
  int res = 0;
   USART_RX_STA=0;	//���㴮��1����
  //char get_api[]="GET https://api.seniverse.com/v3/weather/now.json?key=SAkH50Olwvsp_E1iI&location=Baoji&language=zh-Hans&unit=c\r\n";//����ҲҪ����
   USART1_Send_String(get_api); //��������
  //USART_RX_STA=0;	//���㴮��1����
    delay_ms(2000);//�ȴ�2s
	delay_ms(5000);//�ȴ�2s
   if((USART_RX_STA)&0X8000)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊ����������
	//if(1)
	{ 
	  USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//��ӽ�����
		parse_now_weather();
	} 
  //����
	 
	
   	
  return res;	
}




/************
 * esep8266 �˳�͸��
 *************/

u8 esp8266_exit()
{
    char exit[]="+++";
	USART1_Send_String(exit); //��������
	delay_ms(500);					//�ȴ�500ms
	return esp8266_Test();//�˳�͸���ж�.
}


u8 esp8266_TCP_Released() //�ر�TCP����
{
  int res = 1;
  res = esp8266_SendCmd("AT+CIPCLOSE\r\n","OK",500);
  return res;	
}

/************
 * esep8266 ��ʼ����TCPͨ�ţ�������api��
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
   USART_RX_STA=0;	//���㴮��1����
  char get_api[]="GET https://ip.useragentinfo.com/json\r\n";//����ҲҪ����
   USART1_Send_String(get_api); //��������
  //USART_RX_STA=0;	//���㴮��1����
    delay_ms(2000);//�ȴ�2s
	delay_ms(5000);//�ȴ�2s
   if((USART_RX_STA)&0X8000)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊ����������
	//if(1)
	{ 
	  USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//��ӽ�����
	  parse_now_ip();//����
		
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
	 if(!esp8266_ConWifi()) //����WiFi
	 {
		 LCD_Fill(30,180,240,210,BLACK);
		 LCD_ShowString(30,180,"WiFI_Connected",GREEN,BLACK,16,0);
		if(!esp8266_CIP(1)) //����Ϊ������
		{
			if(!esp8266_TCP_Start_Ip()) //����tcp����
			{
				LCD_Fill(30,180,240,210,BLACK);
		        LCD_ShowString(30,180,"Tcp_Ip_Connected",GREEN,BLACK,16,0);
				if(!esp8266_TCP_Mode()) //����TCPģʽΪ͸��
				{
				  if(!esp8266_TCP_Send()) //��ʼ͸��
				  {
					delay_ms(2000);//��ʱ2s�ȴ�͸������
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
	
	   
     if(1) //����WiFi
	 {
//		 LCD_Fill(30,180,240,210,BLACK);
//		 LCD_ShowString(30,180,"WiFI_Connected",GREEN,BLACK,16,0);
		if(!esp8266_CIP(0)) //����Ϊ������
		 {

		  if(!esp8266_TCP_Start()) //����TCP����
		  {
			  LCD_Fill(30,180,240,210,BLACK);
			  LCD_ShowString(30,180,"TCP_Connected",GREEN,BLACK,16,0);
		    if(!esp8266_TCP_Mode()) //����TCPģʽΪ͸��
			{
	 
			    if(!esp8266_TCP_Send())
				{

					delay_ms(2000);//��ʱ2s�ȴ�͸������
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
    delay_ms(5000);//�ȴ��Զ�����wifi
	
			if(esp8266_CIP(0)) //����Ϊ������
		 {

		  if(!esp8266_TCP_Start()) //����TCP����
		  {
	 
		    if(!esp8266_TCP_Mode()) //����TCPģʽΪ͸��
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



