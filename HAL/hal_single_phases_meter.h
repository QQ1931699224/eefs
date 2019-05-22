/*
* DESC : 与单相电表数据相关的硬件抽象层，里面包含各种从硬件寄存器中读取的电表数据
*/
#ifndef hal_single_phases_meter_h
#define hal_single_phases_meter_h

typedef struct
{
		u32 ENERGY;
		u32 VOLT;
		u32 AMP;
		u32 WATT;
		u32 TEMP;
}SPM_DATA;

/* 返回结构体数据 */
SPM_DATA spm_getAll();
/* 获取电能 */
spm_getEnergy();
/* 获取电压 */
spm_getVolt();
/* 获取电流 */
spm_getAmp();
/* 获取功率 */
spm_getWatt();
/* 获取温度 */
spm_getTemp();

#endif