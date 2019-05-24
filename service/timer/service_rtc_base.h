#ifndef service_rtc_base_h
#define service_rtc_base_h

#include "eeprom/eefs_lib.h"
#include "HAL/hal_rtc.h"


typedef struct day {
	u32 frequency;                                                                           //频率
	u8 status;                                                                               //定时器状态
	s32 count;                                                                               //计数器
	void (*callback)(void);                                                                 //回调函数
}DAY;

typedef struct freq {
	u32 frequency;                                                                           //频率
	u8 status;                                                                               //定时器状态
	s32 count;                                                                               //计数器
	void (*callback)(void);                                                                 //回调函数
}FREQ;

#define SYS_TIMER_0_INTERVAL 7200                                                            //0号定时器扫描间隔

#define TIMER_MAX_INDEX 8  //定时器数组最大值

#define IN_OPERATION 1                                                                      // 运行中
#define SUSPEND 2                                                                           // 暂停
#define DISCONTINUE_USE 0                                                                   // 停用/已删除

extern FREQ G_TIMER_LISI[TIMER_MAX_INDEX];

u8 FREQ_Init();//初始化计时器空间
u8 FREQ_Create(u32 freq, void (*todo)()); //创建定时器
u8 FREQ_Loop();//执行计时
u8 FREQ_Start(u32 freq);//启动计时器
u8 FREQ_Stop(u32 freq);//暂停
u8 FREQ_Delete(u32 freq);//删除/停用
s8 FREQ_getIndex();//获取可用索引
u8 SYS_TIMER0_TODO();
u8 isLastDayNow();

u8 getLastDayByMonth(u16 year, u8 mounth);//获取当月最大日
u8 RTC_LeapYear(u16 year);//判断是否是闰年
u8 isLastDayOfMonth(u8 yearHead, u8 yearEnd, u8 mounth, u8 day);//判断是否是当月最后一天

#endif