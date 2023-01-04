#include "ana.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "string.h"

u8 sys_sta; //״ָ̬ʾ
u8	 temperature =100 ; 
u8  weather = 100;
 char get_api[512];
static u8 str2int(u8 *str)
{
	u8 len,res;
	len = strlen((const char *)str);
	switch(len)
	{
		case 1:
			res = str[0]-0x30;
			break;
		case 2:
			res = (str[0]-0x30)*10+(str[1]-0x30);
			break;
		default:
			break;
	}
	return res;
//	if (s==NULL) return 0;
//    int temp=0;
//    int vl=0;
//    u8 flag=0;
//    while(*s!='\0')
//    {
//        if (*s=='-')
//        {
//            flag=1;
//            s++;
//        }
//        temp = *s-'0';
//        vl = vl*10+temp;
//        s++;
//    }
//    return flag==1 ? -vl:vl;
}

//json����
u8 parse_now_weather(void)
{
	//char USART_RX_BUF[]="{\"results\":[{\"now\":{\"text\":\"��\",\"code\":\"31\",\"temperature\":\"11\"}}]}";
	cJSON *json , *arrayitem , *object	, *subobject , *item;
	

	json = cJSON_Parse((const char*)USART_RX_BUF);
	if(json == NULL)   //json �﷨���󣬲�����
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		return 1;
	}
	else //����
	{
		if((arrayitem = cJSON_GetObjectItem(json,"results")) != NULL)  //ƥ�� ��results��
		{
			int size = cJSON_GetArraySize(arrayitem); //��ȡ�����ж������

			if((object = cJSON_GetArrayItem(arrayitem,0)) !=NULL) //��ȡ�����������  
			{
				
				//ƥ��ص�
				if((subobject = cJSON_GetObjectItem(object,"location")) !=NULL)
				{
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL)   //��ȡ�¶�
					{
					    char *str;
						str = item->valuestring;
						LCD_Fill(130,30,220,60,BLACK);
						LCD_ShowString(130,30,(char *)str,GREEN,BLACK,16,0);
					}
				
				}
				
				//ƥ������
				if((subobject = cJSON_GetObjectItem(object,"now")) !=NULL)
				{
					if((item = cJSON_GetObjectItem(subobject,"temperature")) != NULL)   //��ȡ�¶�
					{
						char *str;
						str = item->valuestring;
						//temperature = str2int((u8 *)str);
					//temperature = json_integer_value(json_object_get(item, "temperature"));
						//LCD_ShowIntNum(120,95,temperature,4,GREEN,BLACK,16);
						LCD_Fill(100,95,130,115,BLACK);
						LCD_ShowString(100,95,(char *)str,GREEN,BLACK,16,0);
						LCD_Fill(30,180,240,210,BLACK);
						LCD_ShowString(30,180,"Temperature_Get",GREEN,BLACK,16,0);
						delay_ms(100);
						
					}
					if((item = cJSON_GetObjectItem(subobject,"code")) != NULL) //��ȡ��������
					{   
						char *str;
						str = item->valuestring;
						weather = str2int((u8 *)str);
						LCD_Fill(30,180,240,210,BLACK);
						LCD_ShowString(30,180,"Weather_Get",GREEN,BLACK,16,0);
						LCD_Fill(100,70,240,90,BLACK);
					   // LCD_ShowIntNum(128,70,weather,4,GREEN,BLACK,16); 
						show_weaher(weather);//������ʾ
					}
//					if(!esp8266_exit())
//					{
//						  	LCD_Fill(30,150,240,180,BLACK);
//							LCD_ShowString(30,150,"Pass_Through_Released",GREEN,BLACK,16,0);
//					}
					
				}
				//ƥ��ʱ��
				if((subobject = cJSON_GetObjectItem(object,"last_update")) !=NULL)
				{
				        char *str;
						str = subobject->valuestring;
					     LCD_Fill(10,230,240,250,BLACK);
					     LCD_DrawLine(0,215,240,215,GREEN);
					     LCD_ShowString(10,220,"Update_time:",GREEN,BLACK,16,0);
					     LCD_ShowString(10,240,(char *)str,GREEN,BLACK,16,0);
	                   					
				}
				else
				{
				    LCD_Fill(30,180,240,210,BLACK); 
					LCD_ShowString(30,180,"Time_ana_Error",GREEN,BLACK,16,0);
				}
				
				
					//LCD_ShowPicture(200,162,16,16,gImage_dui);//״ָ̬ʾ dui
				  sys_sta = 2;//״ָ̬ʾ
				
				if(!esp8266_exit())
				{
					LCD_Fill(30,180,240,210,BLACK);
					LCD_ShowString(30,180,"Pass_Through_Released",GREEN,BLACK,16,0);
					if(!esp8266_TCP_Released())
					{
					  LCD_Fill(30,180,240,210,BLACK);
					   LCD_ShowString(30,180,"TCP_Released",GREEN,BLACK,16,0);
					}
				}				
				
			}

		}
	}


   cJSON_Delete(json);
 return 0;

}
 //����������ʾ
static void show_weaher(u8 code)
{
	
	switch (code) //case2 case3 Ϊ��������
	{
	case 0:
	    {
			LCD_ShowChinese16x16(100,70,"���졪��",GREEN,BLACK,16,0);
		}
		break;
	case 1:
	    {
			LCD_ShowChinese16x16(100,70,"ҹ������",GREEN,BLACK,16,0);
		}
		break;
	case 4:
	    {
			LCD_ShowChinese16x16(100,70,"����",GREEN,BLACK,16,0);
		}
		break;
	case 5:
	    {
			LCD_ShowChinese16x16(100,70,"���졪������",GREEN,BLACK,16,0);			
		}
		break;
	case 6:
	    {
			LCD_ShowChinese16x16(100,70,"ҹ��������",GREEN,BLACK,16,0);
		}
		break;
	case 7:
	    {
			LCD_ShowChinese16x16(100,70,"���졪�󲿶���",GREEN,BLACK,16,0);
		}
		break;
	case 8:
	    {
			LCD_ShowChinese16x16(100,70,"ҹ���󲿶���",GREEN,BLACK,16,0);
		}
		break;
	case 9:
	    {
			LCD_ShowChinese16x16(100,70,"��",GREEN,BLACK,16,0);
		}
		break;
	case 10:
	    {
			LCD_ShowChinese16x16(100,70,"����",GREEN,BLACK,16,0);
		}
		break;
	case 11:
	    {
			LCD_ShowChinese16x16(100,70,"������",GREEN,BLACK,16,0);
		}
		break;
	case 12:
	    {
			LCD_ShowChinese16x16(100,70,"��������б���",GREEN,BLACK,16,0);
		}
		break;
	case 13:
	    {
			LCD_ShowChinese16x16(100,70,"С��",GREEN,BLACK,16,0);
		}
		break;
	case 14:
	    {
			LCD_ShowChinese16x16(100,70,"����",GREEN,BLACK,16,0);
		}
		break;
	case 15:
	    {
			LCD_ShowChinese16x16(100,70,"����",GREEN,BLACK,16,0);
		}
		break;
	case 16:
	    {
			LCD_ShowChinese16x16(100,70,"����",GREEN,BLACK,16,0);
		}
		break;
	case 17:
	    {
			LCD_ShowChinese16x16(100,70,"����",GREEN,BLACK,16,0);
		}
		break;
	case 18:
	    {
			LCD_ShowChinese16x16(100,70,"�ش���",GREEN,BLACK,16,0);
		}
		break;
	case 19:
	    {
			LCD_ShowChinese16x16(100,70,"����",GREEN,BLACK,16,0);
		}
		break;
	case 20:
	    {
			LCD_ShowChinese16x16(100,70,"���ѩ",GREEN,BLACK,16,0);
		}
		break;
	case 21:
	    {
			LCD_ShowChinese16x16(100,70,"��ѩ",GREEN,BLACK,16,0);
		}
		break;
	case 22:
	    {
			LCD_ShowChinese16x16(100,70,"Сѩ",GREEN,BLACK,16,0);
		}
		break;
	case 23:
	    {
			LCD_ShowChinese16x16(100,70,"��ѩ",GREEN,BLACK,16,0);
		}
		break;
	case 24:
	    {
			LCD_ShowChinese16x16(100,70,"��ѩ",GREEN,BLACK,16,0);
		}
		break;
	case 25:
	    {
			LCD_ShowChinese16x16(100,70,"��ѩ",GREEN,BLACK,16,0);
		}
		break;
	case 26:
	    {
			LCD_ShowChinese16x16(100,70,"����",GREEN,BLACK,16,0);
		}
		break;
	case 27:
	    {
			LCD_ShowChinese16x16(100,70,"��ɳ",GREEN,BLACK,16,0);
		}
		break;
	case 28:
	    {
			LCD_ShowChinese16x16(100,70,"ɳ����",GREEN,BLACK,16,0);
		}
		break;
	case 29:
	    {
			LCD_ShowChinese16x16(100,70,"ǿɳ����",GREEN,BLACK,16,0);
		}
		break;
	case 30:
	    {
			LCD_ShowChinese16x16(100,70,"��",GREEN,BLACK,16,0);
		}
		break;
	case 31:
	    {
			LCD_ShowChinese16x16(100,70,"��",GREEN,BLACK,16,0);
		}
		break;
	case 32:
	    {
			LCD_ShowChinese16x16(100,70,"��",GREEN,BLACK,16,0);
		}
		break;
	case 33:
	    {
			LCD_ShowChinese16x16(100,70,"���",GREEN,BLACK,16,0);
		}
		break;
	case 34:
	    {
			LCD_ShowChinese16x16(100,70,"쫷�",GREEN,BLACK,16,0);
		}
		break;
	case 35:
	    {
			LCD_ShowChinese16x16(100,70,"�ȴ��籩",GREEN,BLACK,16,0);	
		}
		break;
	case 36:
	    {
			LCD_ShowChinese16x16(100,70,"�����",GREEN,BLACK,16,0);
		}
		break;
	case 37:
	    {
			LCD_ShowChinese16x16(100,70,"��",GREEN,BLACK,16,0);
		}
		break;
	case 38:
	    {
			LCD_ShowChinese16x16(100,70,"��",GREEN,BLACK,16,0);
		}
		break;
	case 99:
	    {
			LCD_ShowChinese16x16(100,70,"δ֪",GREEN,BLACK,16,0);
		}
		break;	
	default:
		break;
	}


}


u8 parse_now_ip(void)//����ip��ַ
{
  // char USART_RX_BUF[]="{\"country\": \"�й�\", \"short_name\": \"CN\", \"province\": \"����ʡ\", \"city\": \"������\", \"area\": \"μ����\", \"isp\":\ \"����\", \"net\": \"������\"}";
	cJSON *json , *arrayitem;
	

	json = cJSON_Parse((const char*)USART_RX_BUF);
	if(json == NULL)   //json �﷨���󣬲�����
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		return 1;
	}
	else //����
	{
		if((arrayitem = cJSON_GetObjectItem(json,"city")) != NULL)  //ƥ�� ��city��
		{
			//int size = cJSON_GetArraySize(arrayitem); //��ȡ�����ж������
			char *str;
			char lo;
			str =arrayitem->valuestring;
			//lo = str*; 
			LCD_Fill(30,180,240,210,BLACK);
			LCD_ShowString(30,180,"Location_Get",GREEN,BLACK,16,0);
			 //LCD_ShowPicture(200,145,16,16,gImage_dui);//״ָ̬ʾ ��
			
			//ƴ������������ȡ��api
			char get_api1[]="GET https://api.seniverse.com/v3/weather/now.json?key=SAkH50Olwvsp_E1iI&location=";
			char get_api2[]="&language=en\r\n";
			strcat(get_api,get_api1);
			strcat(get_api,(char *)str);
			strcat(get_api,get_api2); 
			sys_sta = 1;
			//�˳�͸����IP_API������
			if(!esp8266_exit())
			{
				LCD_Fill(30,180,240,210,BLACK);
				LCD_ShowString(30,180,"Pass_Through_Released",GREEN,BLACK,16,0);
				if(!esp8266_TCP_Released())
				{
					LCD_Fill(30,180,240,210,BLACK);
					LCD_ShowString(30,180,"TCP_Released",GREEN,BLACK,16,0);
					
				}
			}
			
			//
			
		}
	}

   cJSON_Delete(json);
			
 return 0;	
}