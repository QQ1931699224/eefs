//
//  hal_rtc.c
//  eefs
//
//  Created by 吴晗帅 on 2019/5/23.
//  Copyright © 2019 吴晗帅. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include "hal_rtc.h"

/* 获取当前小时 */
u8 rtc_getHour(void)
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p=gmtime(&timep);
//    printf("%d小时\n",8+p->tm_hour);/*获取当前时,这里获取西方的时间,刚好相差八个小时*/
    return 8+p->tm_hour;
}

/* 获取当前分钟 */
u8 rtc_getMinite(void)
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p=gmtime(&timep);
//    printf("%d分钟\n",p->tm_min);/*获取当前分钟*/
    return p->tm_min;
}
/* 获取当前秒数 */
u8 rtc_getSecond(void)
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p=gmtime(&timep);
//    printf("%d秒\n",p->tm_sec);/*获取当前秒*/
    return p->tm_sec;
}

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
