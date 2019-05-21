/*
 * ����������ȡ
 */
#ifndef three_phases_meter_h
#define three_phases_meter_h
#include <stdio.h>
#include "meter_base.h"
#include "eefs_lib.h"

#define NOPOWERFLAG_INDEX 0                                                             //�����־����
#define SENDPARAMETER_INDEX  1                                                          //���Ͳ�������
#define UPGRADE_INDEX 2                                                                 //������������
#define FIRSTJUDGE_INDEX 3                                                              //ͨ�״��ж�����
#define ENERGY_INDEX 4                                                                  //��������
#define CHECKMETER_INDEX 5														        //У���������

u8 service_tpm_getNoPowerFlag(u8* data);	                                                    //�����־,5�ֽڣ�����crc����1�� get set
u8 service_tpm_setNoPowerFalg(u8* data,u16 len);
u8 service_tpm_getSendParameter(u8* data);                                                    //���Ͳ�����24�ֽڣ�����crc����1�� get set
u8 service_tpm_setSendParameter(u8* data, u16 len);
u8 service_tpm_getUpgrade(u8* data);                                                          //����������415�ֽڣ�����crc����1�� get set
u8 service_tpm_setUpgrade(u8* data, u16 len);
u8 service_tpm_getFirstJudge(u8* data);                                                       //�״��жϣ�4�ֽڣ�����crc����1�� get set
u8 service_tpm_setFirstJudge(u8* data, u16 len);
u8 service_tpm_getEnergy(u8* data);                                                           //���ܣ�160�ֽڴ�crc����4�� get set
u8 service_tpm_setEnergy(u8* data, u16 len);
u8 service_tpm_getCheckMeter(u8* data);                                                       //У����� 78�ֽ� 1�� ����crc get set
u8 service_tpm_setCheckMeter(u8* data, u16 len);

#endif