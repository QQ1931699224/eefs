/*
Lastday.CPP
*/
#include <stdio.h>
#include "HAL/hal_rtc.h"
#include "service/timer/service_rtc_base.h"
#include "tools/tools_string.h"


/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:判断闰年 
 * @year 年
 */
u8 service_rtc_base_isLeapYear(u16 year) {
	if (((year % 4 == 0) && (year % 100 != 0)) || ((year % 4 == 0) && (year % 400 == 0))) {
		return RET_SUCCESS;
	}
	return RET_FAILD;
}


/*
 * Auth: 张添程
 * Date: 2019-5-24
 * Desc:判断是不是目标月最后一天
 * @yearHead 年前两位
 * @yearEnd 年后两位
 * @mounth 月份
 * @days 日
 */
u8 service_rtc_base_isLastDayOfMonth(u8 yearHead, u8 yearEnd,u8 mounth, u8 days) {
	//局部变量
	u16 year;
	u8 mouth;
	u8 day;
	year = tools_HexValue2Dec(yearHead) * 100 + tools_HexValue2Dec(yearEnd);
	mounth = tools_HexValue2Dec(mounth);
	day = tools_HexValue2Dec(days);
	if (day== service_rtc_base_getLastDayByMonth(year,mounth))
	{
		return RET_SUCCESS;
	}
	return RET_FAILD;
}
/*
 * Auth: 张添程
 * Date: 2019-5-23
 * Desc:判断目标月份最大日
 * @year 年
 * @mounth 月份
 */
u8 service_rtc_base_getLastDayByMonth(u16 year,u8 mounth) {
	//校验月份
	if (mounth>12||mounth==0)
	{
		return RET_FAILD;
	}
	switch (mounth)
	{
	case 1:
		return 31;
	case 2:
		if (service_rtc_base_isLeapYear(year)==1)
		{
			return 29;
		}
		return 28;
	case 3:
		return 31;
	case 4:
		return 30;
	case 5:
		return 31;
	case 6:
		return 30;
	case 7:
		return 31;
	case 8:
		return 31;
	case 9:
		return 30;
	case 10:
		return 31;
	case 11:
		return 30;
	case 12:
		return 31;
	default:
            return RET_FAILD;
		break;
	}
}
