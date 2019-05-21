/*
 * 单相表参数存取
 */
#ifndef single_phases_meter_h
#define single_phases_meter_h
#include <stdio.h>
#include "meter_base.h"
#include "eefs_lib.h"

#define NOPOWERFLAG_INDEX 0                                                             //掉电标志索引
#define SENDPARAMETER_INDEX  1                                                          //发送参数索引
#define UPGRADE_INDEX 2                                                                 //升级参数索引
#define FIRSTJUDGE_INDEX 3                                                              //通首次判断索引
#define ENERGY_INDEX 4                                                                  //电能索引
#define CHECKMETER_INDEX 5														        //校表参数索引

u8 service_spm_getNoPowerFlag(u8* data);	                                                    //掉电标志,5字节，不带crc，存1份 get set
u8 service_spm_setNoPowerFalg(u8* data, u16 len);
u8 service_spm_getSendParameter(u8* data);                                                    //发送参数，24字节，不带crc，存1份 get set
u8 service_spm_setSendParameter(u8* data, u16 len);
u8 service_spm_getUpgrade(u8* data);                                                          //升级参数，415字节，不带crc，存1份 get set
u8 service_spm_setUpgrade(u8* data, u16 len);
u8 service_spm_getFirstJudge(u8* data);                                                       //首次判断，4字节，不带crc，存1份 get set
u8 service_spm_setFirstJudge(u8* data, u16 len);
u8 service_spm_getEnergy(u8* data);                                                           //电能，160字节带crc，存4份 get set
u8 service_spm_setEnergy(u8* data, u16 len);
u8 service_spm_getCheckMeter(u8* data);                                                       //校表参数 78字节 1份 不带crc get set
u8 service_spm_setCheckMeter(u8* data, u16 len);

#endif