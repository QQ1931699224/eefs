//
//  hal_rtc.c
//  eefs
//
//  Created by 吴晗帅 on 2019/5/23.
//  Copyright © 2019 吴晗帅. All rights reserved.
//
#include <stdio.h>
#include "eeprom/eefs_lib.h"

/*************************************************
计算时间差
sTime：开始时间
eTime：结束时间
return 时间差
*****************************************************/
u32 timerElapse_ms(u32 sTime, u32 eTime)
{
	s32 s = eTime - sTime;
	if (s >= 0)
		return (s);
	else
		return (s + 0xFFFFFFFF + 1);
}

/*************************************************
获取当前时间值
return 当前时间值
*****************************************************/
u32 Get_Current_Timer0_Counter(void)
{
	time_t t = time(NULL);
	return (u32)t;
}