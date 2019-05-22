/*
* DESC : 与三相电表数据相关的硬件抽象层，里面包含各种从硬件寄存器中读取的电表数据
*/
#ifndef hal_three_phases_meter_h
#define hal_three_phases_meter_h

typedef struct
{
		u32 TOTAL_ENERGY;
		u32 TOTAL_VOLT;
		u32 TOTAL_AMP;
		u32 TOTAL_WATT;
		u32 A_ENERGY;
		u32 A_VOLT;
		u32 A_AMP;
		u32 A_WATT;
		u32 B_ENERGY;
		u32 B_VOLT;
		u32 B_AMP;
		u32 B_WATT;
		u32 C_ENERGY;
		u32 C_VOLT;
		u32 C_AMP;
		u32 C_WATT;
		u32 TEMP;
}TPM_DATA;

/* 返回结构体数据 */
TPM_DATA tpm_getAll();
/* 获取总电能 */
tpm_getTotalEnergy();
/* 获取总电压 */
tpm_getTotalVolt();
/* 获取总电流 */
tpm_getTotalAmp();
/* 获取总功率 */
tpm_getTotalWatt();
/* 获取温度 */
tpm_getTemp();

/* 获取A相电能 */
tpm_getAEnergy();
/* 获取A相电压 */
tpm_getAVolt();
/* 获取A相电流 */
tpm_getAAmp();
/* 获取A相功率 */
tpm_getAWatt();

/* 获取B相电能 */
tpm_getBEnergy();
/* 获取B相电压 */
tpm_getBVolt();
/* 获取B相电流 */
tpm_getBAmp();
/* 获取B相功率 */
tpm_getBWatt();

/* 获取C相电能 */
tpm_getCEnergy();
/* 获取C相电压 */
tpm_getCVolt();
/* 获取C相电流 */
tpm_getCAmp();
/* 获取C相功率 */
tpm_getCWatt();

#endif