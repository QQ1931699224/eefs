#include <stdio.h>
#include <string.h>
#include "eefs_lib.h"
#include "meter_base.h"
#include "three_phases_meter.h"

/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:��ȡ �����־,5�ֽڣ�����crc����1��
 * @data ���ݽ���
 * @return : 0,1
 */
u8 meter_getNoPowerFlag(u8* data) {
	meter_circle_read(NOPOWERFLAG_INDEX, data);
}	
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:д�� �����־,5�ֽڣ�����crc����1��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 meter_setNoPowerFalg(u8* data, u16 len) {
	meter_circle_write(NOPOWERFLAG_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:��ȡ ���Ͳ�����24�ֽڣ�����crc����1��
 * @data ���ݽ���
 * @return : 0,1
 */
u8 meter_getSendParameter(u8* data) {
	meter_circle_read(SENDPARAMETER_INDEX, data);
} 
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:д�� ���Ͳ�����24�ֽڣ�����crc����1��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 meter_setSendParameter(u8* data, u16 len) {
	meter_circle_write(SENDPARAMETER_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:��ȡ ����������415�ֽڣ�����crc����1��
 * @data ����
 * @return : 0,1
 */
u8 meter_getUpgrade(u8* data) {
	meter_circle_read(UPGRADE_INDEX, data);
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:д�� ����������415�ֽڣ�����crc����1��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 meter_setUpgrade(u8* data, u16 len) {
	meter_circle_write(UPGRADE_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:��ȡ �״��жϣ�4�ֽڣ�����crc����1��
 * @data ����
 * @return : 0,1
 */
u8 meter_getFirstJudge(u8* data) {
	meter_circle_read(FIRSTJUDGE_INDEX, data);
} 
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:д�� �״��жϣ�4�ֽڣ�����crc����1��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 meter_setFirstJudge(u8* data, u16 len) {
	meter_circle_write(FIRSTJUDGE_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:��ȡ ���ܣ�160�ֽڴ�crc����4��
 * @data ����
 * @return : 0,1
 */
u8 meter_getEnergy(u8* data) {
	meter_circle_read(ENERGY_INDEX, data);
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:д�� ���ܣ�160�ֽڴ�crc����4��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 meter_setEnergy(u8* data, u16 len) {
	meter_circle_write(ENERGY_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:��ȡ У����� 78�ֽ� 1�� ����crc
 * @data ����
 * @return : 0,1
 */
u8 meter_getCheckMeter(u8* data) {
	meter_circle_read(CHECKMETER_INDEX, data);
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:д�� У����� 78�ֽ� 1�� ����crc
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 meter_setCheckMeter(u8* data, u16 len) {
	meter_circle_write(CHECKMETER_INDEX, data, len);
}