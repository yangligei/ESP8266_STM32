#ifndef __ESP8266_H
#define __ESP8266_H
#include "sys.h"



u8 esp8266_SendCmd(u8 *cmd , u8 * reply , u16 wait);
u8 check_cmd(u8 *str) ;
u8 alreday_init(void);
u8 init_wifi(void);
u8 esp8266_TCP_Released(void);
extern u16 USART_RX_STA; 
//ָ��궨��

//#define AT_at = "AAT\r\n";    //���� AT ����
//#define AT_CWMODE = "AAT+CWMODE_DEF=1\r\n";  // ����WiFiģʽ
//#define AT_RST = "AAT+RET\r\n";    //����ģ��
//#define AT_CIPMUX = "AAT+CIPMUX=0\r\n";  //���ö�����ģʽ
//#define AT_connect_wifi = "AAT+CWJAP=\"315\",\"315315315\"\r\n";    //��һλ��WiFi���� �ڶ�λ������
//#define AT_TCP = "AAT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n" ;      //   ����TCP����
//#define AT_CIPMODE =  "AAT+CIPMODE=1\r\n";    //͸��ģʽ
//#define AT_CIPSEND = "AAT+CIPSEND\r\n";     //��������
//#define zhixin_weather_api = "AGET https://api.seniverse.com/v3/weather/now.json?key=SAkH50Olwvsp_E1iI&location=BaoDing&language=zh-Hans&unit=c\r\n";
//#define AT_EXIT = "A+++";    //�˳�at
//#define AT_SCAN_wifi = "AAT+CWLAP\r\n"; //ɨ����Χ��WiFi
//#define AT_disconnect = "AAT+CWQAP\r\n"; //�Ͽ�ap����
//#define AT_CIPCLOSE = "AAT+CIPCLOSE\r\n"; //�ر�cip����
#endif
