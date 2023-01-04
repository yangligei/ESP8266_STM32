#ifndef __ESP8266_H
#define __ESP8266_H
#include "sys.h"



u8 esp8266_SendCmd(u8 *cmd , u8 * reply , u16 wait);
u8 check_cmd(u8 *str) ;
u8 alreday_init(void);
u8 init_wifi(void);
u8 esp8266_TCP_Released(void);
extern u16 USART_RX_STA; 
//指令宏定义

//#define AT_at = "AAT\r\n";    //测试 AT 启动
//#define AT_CWMODE = "AAT+CWMODE_DEF=1\r\n";  // 设置WiFi模式
//#define AT_RST = "AAT+RET\r\n";    //重启模块
//#define AT_CIPMUX = "AAT+CIPMUX=0\r\n";  //设置多连接模式
//#define AT_connect_wifi = "AAT+CWJAP=\"315\",\"315315315\"\r\n";    //第一位是WiFi名称 第二位是密码
//#define AT_TCP = "AAT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n" ;      //   建立TCP连接
//#define AT_CIPMODE =  "AAT+CIPMODE=1\r\n";    //透传模式
//#define AT_CIPSEND = "AAT+CIPSEND\r\n";     //发送数据
//#define zhixin_weather_api = "AGET https://api.seniverse.com/v3/weather/now.json?key=SAkH50Olwvsp_E1iI&location=BaoDing&language=zh-Hans&unit=c\r\n";
//#define AT_EXIT = "A+++";    //退出at
//#define AT_SCAN_wifi = "AAT+CWLAP\r\n"; //扫描周围的WiFi
//#define AT_disconnect = "AAT+CWQAP\r\n"; //断开ap连接
//#define AT_CIPCLOSE = "AAT+CIPCLOSE\r\n"; //关闭cip传输
#endif
