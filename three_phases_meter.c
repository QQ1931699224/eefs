#include <stdio.h>
#include <string.h>
#include "eefs_lib.h"
#include "meter_base.h"
#include "three_phases_meter.h"

/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 掉电标志,5字节，不带crc，存1份
 * @data 数据接收
 * @return : 0,1
 */
u8 service_tpm_getNoPowerFlag(u8* data) {
	meter_circle_read(NOPOWERFLAG_INDEX, data);
}	
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 掉电标志,5字节，不带crc，存1份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setNoPowerFalg(u8* data, u16 len) {
	meter_circle_write(NOPOWERFLAG_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 发送参数，24字节，不带crc，存1份
 * @data 数据接收
 * @return : 0,1
 */
u8 service_tpm_getSendParameter(u8* data) {
	meter_circle_read(SENDPARAMETER_INDEX, data);
} 
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 发送参数，24字节，不带crc，存1份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setSendParameter(u8* data, u16 len) {
	return meter_circle_write(SENDPARAMETER_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 升级参数，415字节，不带crc，存1份
 * @data 数据
 * @return : 0,1
 */
u8 service_tpm_getUpgrade(u8* data) {
	return meter_circle_read(UPGRADE_INDEX, data);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 升级参数，415字节，不带crc，存1份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setUpgrade(u8* data, u16 len) {
	return meter_circle_write(UPGRADE_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 首次判断，4字节，不带crc，存1份
 * @data 数据
 * @return : 0,1
 */
u8 service_tpm_getFirstJudge(u8* data) {
	return meter_circle_read(FIRSTJUDGE_INDEX, data);
} 
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 首次判断，4字节，不带crc，存1份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setFirstJudge(u8* data, u16 len) {
	return meter_circle_write(FIRSTJUDGE_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 电能，160字节带crc，存4份
 * @data 数据
 * @return : 0,1
 */
u8 service_tpm_getEnergy(u8* data) {
	return meter_circle_read(ENERGY_INDEX, data);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 电能，160字节带crc，存4份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setEnergy(u8* data, u16 len) {
	return meter_circle_write(ENERGY_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 校表参数 78字节 1份 不带crc
 * @data 数据
 * @return : 0,1
 */
u8 service_tpm_getCheckMeter(u8* data) {
	return meter_circle_read(CHECKMETER_INDEX, data);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 校表参数 78字节 1份 不带crc
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setCheckMeter(u8* data, u16 len) {
	return meter_circle_write(CHECKMETER_INDEX, data, len);
}