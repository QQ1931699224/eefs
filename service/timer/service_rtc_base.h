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

typedef struct
{
    u8 timer_hour;
    u8 timer_minite;
    u8 timer_second;
    u8 status;
    u8 sendStatus;
    void (* service_rtc_callBack)(void);
}TIMING_FREQ;

typedef struct
{
    u8 timer_hour;
    u8 timer_minite;
    u8 timer_second;
}TIMING_TIMING;

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

#define SERVICE_RTC_TIMING_MAXCOUNT 1      // 定时器数量
#define SERVICE_RTC_TIMING_SEND 1      // 定时器发送
#define SERVICE_RTC_TIMING_NOTSEND 0      // 定时器未发送
#define SERVICE_RTC_ONEDAYSECONDS 86400      // 一天的秒数
#define SERVICE_RTC_TOLERANTVALUE 10      // 容错值
extern FREQ G_TIMER_LISI[TIMER_MAX_INDEX];
extern TIMING_FREQ timerList[SERVICE_RTC_TIMING_MAXCOUNT]; // 定时器数组

void service_rtc_callBack(void);                  // 定时器回调函数
u8 service_rtc_timing_create(TIMING_TIMING timing, void (* service_rtc_callBack)(void));               // 定时器初始化
u8 service_rtc_timing_loop(void);               // 定时器工作
u8 service_rtc_timing_start(TIMING_TIMING timing);              // 开始
u8 service_rtc_timing_stop(TIMING_TIMING timing);               // 停止
u8 service_rtc_timing_delete(TIMING_TIMING timing);             // 删除

u8 service_rtc_freq_init(void);                                                                                        //初始化计时器空间
u8 service_rtc_freq_create(u32 freq, void (*todo)(void));                                                              //创建定时器
u8 service_rtc_freq_loop(void);                                                                                        //执行计时
u8 service_rtc_freq_start(u32 freq);                                                                                   //启动计时器
u8 service_rtc_freq_stop(u32 freq);                                                                                    //暂停
u8 service_rtc_freq_delete(u32 freq);                                                                                  //删除/停用
s8 service_rtc_freq_getIndex(void);                                                                                    //获取可用索引
u8 SYS_TIMER0_TODO(void);
u8 isLastDayNow(void);

u8 service_rtc_base_getLastDayByMonth(u16 year, u8 mounth);                                                            //获取当月最大日
u8 service_rtc_base_isLeapYear(u16 year);                                                                              //判断是否是闰年
u8 service_rtc_base_isLastDayOfMonth(u8 yearHead, u8 yearEnd, u8 mounth, u8 day);                                      //判断是否是当月最后一天

#endif