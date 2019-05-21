/*
 * 三相表参数存取
 */
#ifndef three_phases_meter_h
#define three_phases_meter_h
#include <stdio.h>
#include "meter_base.h"
#include "eefs_lib.h"

#define NOPOWERFLAG_INDEX 0                                                      //CRC大小
#define SENDPARAMETER_INDEX  1                                                   //新旧写入状态大小
#define UPGRADE_INDEX 2                                                              //通用标记位有CRC
#define FIRSTJUDGE_INDEX 3                                                              //通用标记位无CRC
#define ENERGY_INDEX 4 

u8 meter_getNoPowerFlag(u8* data);	//掉电标志,5字节，不带crc，存1份
u8 meter_setNoPowerFalg(u8* data,u16 len);
u8 meter_getSendParameter(u8* data); //发送参数，24字节，不带crc，存1份
u8 meter_setSendParameter(u8* data, u16 len);
u8 meter_getUpgrade(u8* data);//升级参数，415字节，不带crc，存1份
u8 meter_setUpgrade(u8* data, u16 len);
u8 meter_getFirstJudge(u8* data); //首次判断，4字节，不带crc，存1份
u8 meter_setFirstJudge(u8* data, u16 len);
u8 meter_getEnergy(u8* data); //电能，160字节带crc，存4份
u8 meter_setEnergy(u8* data, u16 len);

#endif