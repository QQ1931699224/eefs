/*
 * 三相表参数存取
 */
#ifndef three_phases_meter_h
#define three_phases_meter_h
#include <stdio.h>
#include "meter_base.h"
#include "eefs_lib.h" 

#define NOPOWERFLAG_INDEX 0                                                             //掉电标志索引
#define SENDPARAMETER_INDEX  1                                                          //发送参数索引
#define UPGRADE_INDEX 2                                                                 //升级参数索引
#define FIRSTJUDGE_INDEX 3                                                              //通首次判断索引
#define ENERGY_INDEX 4                                                                  //电能索引
#define CHECKMETER_INDEX 5														        //校表参数索引
////////////////////////////////////////////////////////////////////////

#define BIT_TIME_UNMASK 0xF0                                                  // 时间状态位掩码
#define BIT_TIME_MASK 0xF                                                  // 时间状态位掩码
#define SMALLINDEX_CAPACITY 24              // 小索引24个字节
#define SMALLINDEX_STATUSCAPACITY 1         // 小索引状态位1字节
#define SMALLINDEX_CRCCAPACITY 2            // 小索引CRM验证2字节
#define SMALLINDEX_DATACOUNT 100            // 小索引数据个数
#define SMALLINDEX_CURRENTSTATUS 1       // 小索引当前status
#define SMALLINDEX_DEFAULTSTATUS 0       // 小索引不是当前status

#define SMALLINDEX_NAME 1000                 // 小索引在索引区的名字
#define SMALLINDEX_SENDTATUS 1               // 小索引已发送status
#define SMALLINDEX_NOSENDSTATUS 2            // 小索引未发送status

#define MONTHDATA_NAME 1001                  // 月份数据空间的名字
#define MONTHDATA_COUNT 12                   // 月份数据空间的个数
#define MONTHDATA_CAPACITY 4                 // 月份数据空间的个数

#define LOSTVOLT_NAME 1002                   // 失压数据的名字
#define LOSTVOLT_COUNT 20                    // 失压数据空间的个数
#define LOSTVOLT_CAPACITY 9                  // 失压数据单个空间容量
#define LOSTVOLT_DESCRIBE 1                  // 失压数据空间描述位容量
#define LOSTVOLT_CURRENTSTATUS 1             // 失压数据小索引当前status
#define LOSTVOLT_DEFAULTSTATUS 0             // 失压数据小索引不是当前status

///////////////////////////////////////////////////////////////////////////////////

typedef struct SmallIndexNode {
	u32 electric1;
	u32 electric2;
	u32 electric3;
	u32 electric4;
	u32 electric5;
	u32 electric6;
}SMALLINDEXNODE;

u8 service_tpm_getNoPowerFlag(u8* data);	                                                    //掉电标志,5字节，不带crc，存1份 get set
u8 service_tpm_setNoPowerFalg(u8* data,u16 len);
u8 service_tpm_getSendParameter(u8* data);                                                    //发送参数，24字节，不带crc，存1份 get set
u8 service_tpm_setSendParameter(u8* data, u16 len);
u8 service_tpm_getUpgrade(u8* data);                                                          //升级参数，415字节，不带crc，存1份 get set
u8 service_tpm_setUpgrade(u8* data, u16 len);
u8 service_tpm_getFirstJudge(u8* data);                                                       //首次判断，4字节，不带crc，存1份 get set
u8 service_tpm_setFirstJudge(u8* data, u16 len);
u8 service_tpm_getEnergy(u8* data);                                                           //电能，160字节带crc，存4份 get set
u8 service_tpm_setEnergy(u8* data, u16 len);
u8 service_tpm_getCheckMeter(u8* data);                                                       //校表参数 78字节 1份 不带crc get set
u8 service_tpm_setCheckMeter(u8* data, u16 len);

///////////////////////////////////////////////////////////////////////////////////////


u8 service_tpm_create_breakeNetCapacity(void);      // 创建断网数据空间
u8 service_tpm_saveOnceBreakeNetData(u16 index, SMALLINDEXNODE smallNode); // 存一次断网数据
u8 service_tpm_saveBreakeNetData(SMALLINDEXNODE node);  // 保存断网数据
u16 service_tpm_getBreakeNetDataAddress(u16 index);         //获得断网数据在数据区的首地址
u8 service_tpm_disconnect_getData(u8* retData);  //返回最新的断网数据
u8 service_tpm_disconnect_getDataWithIndex(u16 index, u8* retData); // 根据下标返回断网数据
u8 service_tpm_getSmallIndexCurrentStatus(u16 index); // 获取小索引7,8位的当前状态
u8 service_tpm_getSmallIndexSendStatus(u16 index);   // 获取小索引5,6位的发送状态
u8 service_tpm_getSmallIndexTimeStatus(u16 index);   // 获取小索引1234位的时间状态
u8 service_tpm_setSmallIndexSendStatus(u16 index, u8 val);         // 根据角标设置5,6位的发送状态
u8 service_tpm_setSmallIndexCurrentStatus(u16 index, u8 val);       // 根据角标设置7,8位的当前状态
u8 service_tpm_setSmallIndexCTimeStatus(u16 index, u8 val);       // 根据角标设置1,2,3,4位的时间状态
u8 service_tpm_disconnect_getDataAndChangeStatus(u8* retData);  //获取角标对应的断网数据, 并改变状态

u8 service_tpm_create_monthCapacity(void);      // 创建月数据空间
u8 service_tpm_saveMonthData(u8 month, u8* data);  // 保存月数据
u8 service_tpm_getMonthData(u8 month, u8* ret_data);  // 获取月数据

u8 service_tpm_create_lostVoltCapacity(void);      // 创建失压数据空间
u8 service_tpm_saveLostVoltData(u8* data);             // 保存失压数据
u8 service_tpm_saveOnceLostVoltData(u16 index, u8* data);             // 保存失压数据
u8 service_tpm_getLostVoltData(u16 index, u8* retData);         // 根据角标取失压数据
u16 service_tpm_getLostVoltDataAddress(u16 index);    // 获得失压小索引在数据区对应的地址
u8 service_tpm_getLostVoltCurrentStatus(u16 index);   // 获取失压1,2位的当前状态
u8 service_tpm_setLostVoltCurrentStatus(u16 index, u8 val); // 设置失压1,2位的当前状态

#endif
