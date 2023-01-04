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


// ������������������������ע�⣡��������������������������
//��ֲʱ��ע��cjson��������malloc�����޸ģ�malloc���������ڴ��С���ܹ���ϵͳ�����ļ� �� ջ �Ĵ�СҪ�����޸ģ�������ڴ�й©��
// ������������������������ע�⣡��������������������������



//v0.32��v0.31��ʧ�ܵĻ��������¸Ľ�
//2022.12.31_22��00������tcp����GET������ʱʱ����̣�Ӧ���ڷ���GET�������ʱ����1s�ȴ���Ӧ��
//v0.4�����json���ݽ�������
//     json�̴����Խ��������ڳ�������ʱ������⣬��v0.41���
//v0.41:����ڴ�й¶��esp8266�Լ�TCPͨ�Ž����
//v0.5������1.96IPS ��ʾ ʹ��SPIͨ��
//v0.6������ip��ַ��ȡ����λ�á�
//׼����ֲIVGL�⣬�˰汾ֹͣ���¡�
//�˰汾��ɣ�����esp8266��MCU�����ӣ���ȡ������JSONЭ�顣

int main(void)
 {	
	 //
	 u8 i,j;
	float t=0;
	// u8 sys_sta = 0;  //״ָ̬ʾ
	 delay_init();
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 uart_init2(115200);
     uart_init(115200);
	 mem_init(); //�ڴ�����ʼ��
	 LCD_Init();//LCD��ʼ��
	 LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	 
	 //��ʾ������Ϣ
	 LCD_ShowString(0,30,"The_Weather_of:",GREEN,BLACK,16,0);
	 LCD_ShowString(130,30,"---",GREEN,BLACK,16,0);
	 LCD_ShowString(0,70,"Weather:",GREEN,BLACK,16,0);
	 LCD_ShowString(0,95,"Temperature:",GREEN,BLACK,16,0);
	 LCD_ShowIntNum(100,95,10,4,GREEN,BLACK,16);
	 LCD_ShowString(100,95,"--- ",GREEN,BLACK,16,0);
	 LCD_ShowChinese16x16(135,95,"��",GREEN,BLACK,16,0);
	 LCD_ShowString(0,150,"Syetem_Status:",GREEN,BLACK,16,0);
	 LCD_ShowString(130,150,"Location",GREEN,BLACK,12,0);
	 LCD_ShowPicture(200,145,16,16,gImage_cuo);//״ָ̬ʾ ��
	 LCD_ShowString(130,165,"Weather_Inf",GREEN,BLACK,12,0);
     LCD_ShowPicture(200,162,16,16,gImage_cuo);//״ָ̬ʾ ��
	 LCD_ShowString(30,180,"LCD_Init_Finished",GREEN,BLACK,16,0);
	 LCD_DrawLine(0,130,240,130,GREEN);
	 LCD_DrawLine(0,215,240,215,GREEN);
	 LCD_ShowString(10,220,"Update_time:",GREEN,BLACK,16,0);
	 LCD_ShowString(150,270,"Ver:0.6",GREEN,BLACK,12,0);
	 LCD_ShowString(100,70,"---",GREEN,BLACK,16,0);
	 delay_ms(100);
	 
     //
	  extern  u8 sys_sta;//״ָ̬ʾ 
      get_ip_location(); // wifi��ʼ������ȡ��ǰIP��Ӧ�ĵ���λ��
	  if(sys_sta ==1)
	  {
		LCD_ShowPicture(200,145,16,16,gImage_dui);//״ָ̬ʾ 
	  
		init_wifi(); //��ȡ������Ϣ
		if(sys_sta == 2)
		{
			LCD_ShowPicture(200,162,16,16,gImage_dui);	//״ָ̬ʾ
		}		
      }
	  while(1)
	 {
		
	 }
 }
 

