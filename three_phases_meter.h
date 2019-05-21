/*
 * ����������ȡ
 */
#ifndef three_phases_meter_h
#define three_phases_meter_h
#include <stdio.h>
#include "meter_base.h"
#include "eefs_lib.h"

#define NOPOWERFLAG_INDEX 0                                                      //CRC��С
#define SENDPARAMETER_INDEX  1                                                   //�¾�д��״̬��С
#define UPGRADE_INDEX 2                                                              //ͨ�ñ��λ��CRC
#define FIRSTJUDGE_INDEX 3                                                              //ͨ�ñ��λ��CRC
#define ENERGY_INDEX 4 

u8 meter_getNoPowerFlag(u8* data);	//�����־,5�ֽڣ�����crc����1��
u8 meter_setNoPowerFalg(u8* data,u16 len);
u8 meter_getSendParameter(u8* data); //���Ͳ�����24�ֽڣ�����crc����1��
u8 meter_setSendParameter(u8* data, u16 len);
u8 meter_getUpgrade(u8* data);//����������415�ֽڣ�����crc����1��
u8 meter_setUpgrade(u8* data, u16 len);
u8 meter_getFirstJudge(u8* data); //�״��жϣ�4�ֽڣ�����crc����1��
u8 meter_setFirstJudge(u8* data, u16 len);
u8 meter_getEnergy(u8* data); //���ܣ�160�ֽڴ�crc����4��
u8 meter_setEnergy(u8* data, u16 len);

#endif