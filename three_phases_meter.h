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
////////////////////////////////////////////////////////////////////////

#define BIT_TIME_UNMASK 0xF0                                                  // ʱ��״̬λ����
#define BIT_TIME_MASK 0xF                                                  // ʱ��״̬λ����
#define SMALLINDEX_CAPACITY 24              // С����24���ֽ�
#define SMALLINDEX_STATUSCAPACITY 1         // С����״̬λ1�ֽ�
#define SMALLINDEX_CRCCAPACITY 2            // С����CRM��֤2�ֽ�
#define SMALLINDEX_DATACOUNT 100            // С�������ݸ���
#define SMALLINDEX_CURRENTSTATUS 1       // С������ǰstatus
#define SMALLINDEX_DEFAULTSTATUS 0       // С�������ǵ�ǰstatus

#define SMALLINDEX_NAME 1000                 // С������������������
#define SMALLINDEX_SENDTATUS 1               // С�����ѷ���status
#define SMALLINDEX_NOSENDSTATUS 2            // С����δ����status

#define MONTHDATA_NAME 1001                  // �·����ݿռ������
#define MONTHDATA_COUNT 12                   // �·����ݿռ�ĸ���
#define MONTHDATA_CAPACITY 4                 // �·����ݿռ�ĸ���

#define LOSTVOLT_NAME 1002                   // ʧѹ���ݵ�����
#define LOSTVOLT_COUNT 20                    // ʧѹ���ݿռ�ĸ���
#define LOSTVOLT_CAPACITY 9                  // ʧѹ���ݵ����ռ�����
#define LOSTVOLT_DESCRIBE 1                  // ʧѹ���ݿռ�����λ����
#define LOSTVOLT_CURRENTSTATUS 1             // ʧѹ����С������ǰstatus
#define LOSTVOLT_DEFAULTSTATUS 0             // ʧѹ����С�������ǵ�ǰstatus

///////////////////////////////////////////////////////////////////////////////////

typedef struct SmallIndexNode {
	u32 electric1;
	u32 electric2;
	u32 electric3;
	u32 electric4;
	u32 electric5;
	u32 electric6;
}SMALLINDEXNODE;

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

///////////////////////////////////////////////////////////////////////////////////////


u8 meter_create_breakeNetCapacity(void);      // �����������ݿռ�
u8 meter_saveOnceBreakeNetData(u16 index, SMALLINDEXNODE smallNode); // ��һ�ζ�������
u8 meter_saveBreakeNetData(SMALLINDEXNODE node);  // �����������
u16 meter_getBreakeNetDataAddress(u16 index);         //��ö������������������׵�ַ
u8 meter_disconnect_getData(u8* retData);  //�������µĶ�������
u8 meter_disconnect_getDataWithIndex(u16 index, u8* retData); // �����±귵�ض�������
u8 meter_getSmallIndexCurrentStatus(u16 index); // ��ȡС����7,8λ�ĵ�ǰ״̬
u8 meter_getSmallIndexSendStatus(u16 index);   // ��ȡС����5,6λ�ķ���״̬
u8 meter_getSmallIndexTimeStatus(u16 index);   // ��ȡС����1234λ��ʱ��״̬
u8 meter_setSmallIndexSendStatus(u16 index, u8 val);         // ���ݽǱ�����5,6λ�ķ���״̬
u8 meter_setSmallIndexCurrentStatus(u16 index, u8 val);       // ���ݽǱ�����7,8λ�ĵ�ǰ״̬
u8 meter_setSmallIndexCTimeStatus(u16 index, u8 val);       // ���ݽǱ�����1,2,3,4λ��ʱ��״̬
u8 meter_disconnect_getDataAndChangeStatus(u8* retData);  //��ȡ�Ǳ��Ӧ�Ķ�������, ���ı�״̬

u8 meter_create_monthCapacity(void);      // ���������ݿռ�
u8 meter_saveMonthData(u8 month, u8* data);  // ����������
u8 meter_getMonthData(u8 month, u8* ret_data);  // ��ȡ������

u8 meter_create_lostVoltCapacity(void);      // ����ʧѹ���ݿռ�
u8 meter_saveLostVoltData(u8* data);             // ����ʧѹ����
u8 meter_saveOnceLostVoltData(u16 index, u8* data);             // ����ʧѹ����
u8 meter_getLostVoltData(u16 index, u8* retData);         // ���ݽǱ�ȡʧѹ����
u16 meter_getLostVoltDataAddress(u16 index);    // ���ʧѹС��������������Ӧ�ĵ�ַ
u8 meter_getLostVoltCurrentStatus(u16 index);   // ��ȡʧѹ1,2λ�ĵ�ǰ״̬
u8 meter_setLostVoltCurrentStatus(u16 index, u8 val); // ����ʧѹ1,2λ�ĵ�ǰ״̬

#endif