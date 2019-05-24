
#include "tools_string.h"

/* 将16进制的asc表达值，转换为10进制数值，例如： 0x20 -> 20 */
u8 tools_HexValue2Dec(u8 hex)
{
	return hex - ((hex & 0xF0) >> 4) * 6;
}