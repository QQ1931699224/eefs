//
//  service_rtc_timing.c
//  eefs
//
//  Created by 吴晗帅 on 2019/5/23.
//  Copyright © 2019 吴晗帅. All rights reserved.
//

#include "service_rtc_base.h"
#include <time.h>
TIMING_FREQ timerList[SERVICE_RTC_TIMING_MAXCOUNT];

/*
 * Auth: 吴晗帅
 * Date: 2019-5-23
 * Desc:初始化定时器
 * @index:索引
 * @hour:小时
 * @minite:分钟
 * @second:秒
 * @u8 (* service_rtc_callBack)(void):回调函数(函数指针)
 * @return : 1:成功 0：失败
 */
u8 service_rtc_timing_freq_create(u8 index, u8 hour, u8 minite, u8 second, u8 (* service_rtc_callBack)(void))
{
    // ---------- 局部变量定义区---------- //
    TIMING_FREQ timer;
    // ---------- 输入参数条件检测---------- //
    if (index > SERVICE_RTC_TIMING_MAXCOUNT - 1) {
        return RET_FAILD;
    }
    if (hour > 23 || minite > 59 || second > 59) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    timer.timer_hour = hour;
    timer.timer_minite = minite;
    timer.timer_second = second;
    timer.sendStatus = SERVICE_RTC_TIMING_NOTSEND;   // 未发送
    timer.service_rtc_callBack = service_rtc_callBack;
    timer.status = SERVICE_RTC_TIMING_STOP;
    timerList[index] = timer;
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-23
 * Desc:定时器工作
 * @paramName:无
 * @return : 1:成功 0：失败
 */
u8 service_rtc_timing_freq_loop(void)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u32 currentAllSeconds;         // 当前总的秒数
    u32 setAllSeconds;         // 设置的总的秒数
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)循环遍历所有定时器
    for (i = 0; i < SERVICE_RTC_TIMING_MAXCOUNT; i++) {
        TIMING_FREQ timer = timerList[i];
        timer.status = SERVICE_RTC_TIMING_RUNING;
        // 当前总的秒数
        currentAllSeconds = rtc_getHour() * 3600 + rtc_getMinite() * 60 + rtc_getSecond();
        // 设置的总的秒数
        setAllSeconds = timer.timer_hour * 3600 + timer.timer_minite * 60 + timer.timer_second;
        if (currentAllSeconds < SERVICE_RTC_TOLERANTVALUE) {
            currentAllSeconds += SERVICE_RTC_ONEDAYSECONDS;
        }
        if (currentAllSeconds < setAllSeconds) {
            timer.sendStatus = SERVICE_RTC_TIMING_NOTSEND;
            timerList[i] = timer;
        }
        if (timer.sendStatus == SERVICE_RTC_TIMING_NOTSEND) { // 如果定时器未发送
            if (currentAllSeconds >= setAllSeconds) {
                timer.sendStatus = SERVICE_RTC_TIMING_SEND; // 把状态设为已发送
                timerList[i] = timer;
                service_rtc_callBack();
            }
        }
    }
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-23
 * Desc:定时器回调函数
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_rtc_callBack(void)
{
    printf("%d时%d分%d秒 回调函数成功\n", rtc_getHour(), rtc_getMinite(), rtc_getSecond());
    
    return RET_SUCCESS;
}
