#ifndef __ANA_H
#define __ANA_H

#include "sys.h"
u8 parse_now_weather(void);

u8 str2int(u8 *str);

extern u8 sys_sta; //״ָ̬ʾ
extern  u8  temperature ;
static void show_weaher(u8 code);
extern u8 parse_now_ip(void);
extern char get_api[512];

#endif
