/*
* DESC : 与RTC相关的硬件抽象层数据，包括各类从寄存器中读取的与日期时间有关的数据
*/
#ifndef hal_rtc_h
#define hal_rtc_h

#define DEFAULT_YEAR_HEAD 0x20

typedef struct
{
	u8 YR_HEAD;
	u8 YR;
	u8 MO;
	u8 DT;
	u8 HR;
	u8 MN;
	u8 SC;
	u8 DW;
}DATATIME;

/* 获取当前系统时间，返回DATATIME结构体值 */
DATATIME rtc_getDataTime();
/* meter_rtc_getDataTime方法的另一种表达，获取当前系统时间，返回DATATIME结构体值 */
DATATIME rtc_now();
/* 获取当前年份后2位 */
u8 rtc_getYear();
/* 获取当前月份 */
u8 rtc_getMonth();
/* 获取当前几号 */
u8 rtc_getDay();
/* 获取当前小时 */
u8 rtc_getHour();
/* 获取当前分钟 */
u8 rtc_getMinite();
/* 获取当前秒数 */
u8 rtc_getSecond();
/* 获取星期 */
u8 rtc_getDW();




#define

#endif