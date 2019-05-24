//
//  service_rtc_freq.c
//  eefs
//
//  Created by 吴晗帅 on 2019/5/23.
//  Copyright © 2019 吴晗帅. All rights reserved.
//
#include "service_rtc_base.h"

FREQ G_TIMER_LISI[TIMER_MAX_INDEX];
u8 TIMER_COUNT = 0;


/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:获取可以创建定时器的索引
 * @freq 频率
 */
u8 service_rtc_freq_init() {
	int i;
	for (i = 0; i < TIMER_MAX_INDEX; i++)
	{
		G_TIMER_LISI[i].status = DISCONTINUE_USE;
	}
	return RET_ERROR;
}



/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:创建定时器
 * @timer 索引
 * @freq 频率
 * @statu 状态
 * @todo 回调函数
 * @return : dateStatus
 */
u8 service_rtc_freq_create(u32 freq, void (*todo)(void)) {
	// ---------- 局部变量定义区---------- //
	FREQ fq;					//计时器
	int i;
	// ---------- 输入参数条件检测---------- //
	//校验是否有这个频率的定时器
	for (i = 0; i < TIMER_MAX_INDEX; i++)
	{
		if (freq == G_TIMER_LISI[i].frequency) {
			return RET_FAILD;
		}
	}
	// ---------- 业务处理---------- //
	fq.frequency = freq;
	fq.status = SUSPEND;
	fq.count = -1;
	fq.callback = todo;
	//放入计时器数组
	if (service_rtc_freq_getIndex() == RET_ERROR) { return RET_FAILD; }
	G_TIMER_LISI[service_rtc_freq_getIndex()] = fq;
	return RET_SUCCESS;
}


/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:创建定时器
 * @timer 索引
 * @freq 频率
 * @statu 状态
 * @todo 回调函数
 * @return : dateStatus
 */
u8 service_rtc_freq_loop(void) {
	// ---------- 局部变量定义区---------- //
	int i;
	u32 nowTime; //当前时间
	u32 startTime; //起始时间
	FREQ freq;
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	for (i = 0; i < TIMER_MAX_INDEX; i++) {
		freq = G_TIMER_LISI[i];
		if (freq.status == IN_OPERATION)
		{
			if (freq.count < 0)
			{
				G_TIMER_LISI[i].count = Get_Current_Timer0_Counter();//初始化赋值
			}
			startTime = freq.count; //开始时间
			nowTime = Get_Current_Timer0_Counter();//当前时间
			if (freq.frequency <= timerElapse_ms(startTime, nowTime))//比较时间差
			{
				G_TIMER_LISI[i].count = nowTime; //重置开始时间
				freq.callback();
			}
		}
	}
    return RET_SUCCESS;
}
/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:开始定时器
 * @freq 频率
 */
u8 service_rtc_freq_start(u32 freq) {
	int i;
	for (i = 0; i < TIMER_MAX_INDEX; i++)
	{
		if (G_TIMER_LISI[i].frequency == freq)
		{
			G_TIMER_LISI[i].status = IN_OPERATION;
			return RET_SUCCESS;
		}
	}
	return RET_FAILD;
}
/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:暂停定时器
 * @freq 频率
 */
u8 service_rtc_freq_stop(u32 freq) {
	int i;
	for (i = 0; i < TIMER_MAX_INDEX; i++)
	{
		if (G_TIMER_LISI[i].frequency == freq)
		{
			G_TIMER_LISI[i].status = SUSPEND;
			return RET_SUCCESS;
		}
	}
	return RET_FAILD;
}
/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:删除定时器
 * @freq 频率
 */
u8 service_rtc_freq_delete(u32 freq) {
	int i;
	for (i = 0; i < TIMER_MAX_INDEX; i++)
	{
		if (G_TIMER_LISI[i].frequency == freq)
		{
			G_TIMER_LISI[i].status = DISCONTINUE_USE;
			G_TIMER_LISI[i].frequency = 0;
			return RET_SUCCESS;
		}
	}
	return RET_FAILD;
}


/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:获取可以创建定时器的索引
 * @freq 频率
 */
s8 service_rtc_freq_getIndex() {
	int i;
	for (i = 0; i < TIMER_MAX_INDEX; i++)
	{
		if (G_TIMER_LISI[i].status == DISCONTINUE_USE)
		{
			return i;
		}
	}
	return RET_ERROR;
}
