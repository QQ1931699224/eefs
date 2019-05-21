#include <stdio.h>
#include <string.h>
#include "eefs_lib.h"
#include "meter_base.h"
#include "three_phases_meter.h"

/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:��ȡ �����־,5�ֽڣ�����crc����1��
 * @data ���ݽ���
 * @return : 0,1
 */
u8 service_tpm_getNoPowerFlag(u8* data) {
	
 return meter_circle_read(NOPOWERFLAG_INDEX, data);
}	
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:д�� �����־,5�ֽڣ�����crc����1��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 service_tpm_setNoPowerFalg(u8* data, u16 len) {
	return meter_circle_write(NOPOWERFLAG_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:��ȡ ���Ͳ�����24�ֽڣ�����crc����1��
 * @data ���ݽ���
 * @return : 0,1
 */
u8 service_tpm_getSendParameter(u8* data) {
	return meter_circle_read(SENDPARAMETER_INDEX, data);
} 
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:д�� ���Ͳ�����24�ֽڣ�����crc����1��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 service_tpm_setSendParameter(u8* data, u16 len) {
	return meter_circle_write(SENDPARAMETER_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:��ȡ ����������415�ֽڣ�����crc����1��
 * @data ����
 * @return : 0,1
 */
u8 service_tpm_getUpgrade(u8* data) {
	return meter_circle_read(UPGRADE_INDEX, data);
}
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:д�� ����������415�ֽڣ�����crc����1��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 service_tpm_setUpgrade(u8* data, u16 len) {
	return meter_circle_write(UPGRADE_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:��ȡ �״��жϣ�4�ֽڣ�����crc����1��
 * @data ����
 * @return : 0,1
 */
u8 service_tpm_getFirstJudge(u8* data) {
	return meter_circle_read(FIRSTJUDGE_INDEX, data);
} 
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:д�� �״��жϣ�4�ֽڣ�����crc����1��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 service_tpm_setFirstJudge(u8* data, u16 len) {
	return meter_circle_write(FIRSTJUDGE_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:��ȡ ���ܣ�160�ֽڴ�crc����4��
 * @data ����
 * @return : 0,1
 */
u8 service_tpm_getEnergy(u8* data) {
	return meter_circle_read(ENERGY_INDEX, data);
} 
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:д�� ���ܣ�160�ֽڴ�crc����4��
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 service_tpm_setEnergy(u8* data, u16 len) {
	return meter_circle_write(ENERGY_INDEX, data, len);
}
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:��ȡ У����� 78�ֽ� 1�� ����crc
 * @data ����
 * @return : 0,1
 */
u8 service_tpm_getCheckMeter(u8* data) {
	return meter_circle_read(CHECKMETER_INDEX, data);
}
/*
 * Auth: �����
 * Date: 2019-5-21
 * Desc:д�� У����� 78�ֽ� 1�� ����crc
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 service_tpm_setCheckMeter(u8* data, u16 len) {
	return meter_circle_write(CHECKMETER_INDEX, data, len);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:�����������ݿռ�
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_create_breakeNetCapacity(void)
{
	// ---------- �ֲ�����������---------- //
	int i;
	USERNODE usernode;
	u16 index;              // ��¼������index

	// ---------- ��������������---------- //

	// ---------- ҵ����---------- //
	//(1)ѭ������������, �ҵ�һ��ûʹ�õ�����
	usernode.name = SMALLINDEX_NAME;
	index = 0;
	usernode.size = (SMALLINDEX_CAPACITY + SMALLINDEX_STATUSCAPACITY + SMALLINDEX_CRCCAPACITY) * SMALLINDEX_DATACOUNT;
	for (i = 0; i < MAX_INDEX; i++) {
		if (eefs_mbr_getIndexStatus(i) == ZERO_INDEXSTATUS) {
			eefs_mbr_setIndexStatus(i, DEFAULT_INDEXSTATUS);
		}
		if (eefs_mbr_getIndexStatus(i) == DEFAULT_INDEXSTATUS) {
			eefs_create(i, usernode); // ����������
			index = i;
			i = MAX_INDEX;
		}
	}

	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:��һ�ζ�������
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_saveOnceBreakeNetData(u16 index, SMALLINDEXNODE smallNode)
{
	// ---------- �ֲ�����������---------- //
//    u8 status;
//    u8 lastStatus;
	u16 address;     // �׵�ַ
	u16 lastAddress; // ��һ����ַ
	u16 newAddress; // д��ĵ�ַ
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	address = 0;
	lastAddress = 0;
	newAddress = 0;

	//(1)ѭ���ҳ����������׵�ַ
	address = meter_getBreakeNetDataAddress(0);
	//(2)����С������������ĵ�ַ
	newAddress = meter_getBreakeNetDataAddress(index);
	if (index == 0) {
		lastAddress = meter_getBreakeNetDataAddress(SMALLINDEX_DATACOUNT - 1);
	}
	else
	{
		lastAddress = meter_getBreakeNetDataAddress(index) - SMALLINDEX_CAPACITY - SMALLINDEX_STATUSCAPACITY - SMALLINDEX_CRCCAPACITY;
	}

	//(3)��newAddress��������
	eefs_base_writeBytes(newAddress, (u8*)& smallNode, SMALLINDEX_CAPACITY);

	//(4)д��״̬λ(7,8λ=1Ϊ��ǰ����, 0Ϊ��ǰ����, 1234λ����������ʱ��, 5,6λ���Ʒ���״̬,2ΪĬ��״̬, 1Ϊ����״̬)
	// д�뵱ǰ״̬
	meter_setSmallIndexCurrentStatus(index, SMALLINDEX_CURRENTSTATUS);
	// д��Ĭ�Ϸ���״̬
	meter_setSmallIndexSendStatus(index, SMALLINDEX_NOSENDSTATUS);
	// ����һ��index����״̬
	if (index == 0) {
		meter_setSmallIndexCurrentStatus(SMALLINDEX_DATACOUNT - 1, SMALLINDEX_DEFAULTSTATUS);
	}
	else
	{
		meter_setSmallIndexCurrentStatus(index - 1, SMALLINDEX_DEFAULTSTATUS);
	}
	// ʱ��״̬

	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:�����������
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_saveBreakeNetData(SMALLINDEXNODE node)
{
	// ---------- �ֲ�����������---------- //
	int j;
	// ---------- ��������������---------- //

	// ---------- ҵ����---------- //
	//(1)����С������, �ҵ�status��7,8λΪ1��С����
	for (j = 0; j < SMALLINDEX_DATACOUNT; j++) {
		if (meter_getSmallIndexCurrentStatus(j) == SMALLINDEX_CURRENTSTATUS) {  // �ǵ�ǰ����״̬
			if (j == SMALLINDEX_DATACOUNT - 1) {// ��������һ��, ���浽0
				meter_saveOnceBreakeNetData(0, node);
				return RET_SUCCESS;
			}
			else // ���򱣴浽��һ��
			{
				meter_saveOnceBreakeNetData(j + 1, node);
				return RET_SUCCESS;
			}
		}
	}
	meter_saveOnceBreakeNetData(0, node);
	return RET_SUCCESS;

}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:��ȡС������ǰ״̬
 * @index:����
 * @return : u8 status
 */
u8 meter_getSmallIndexCurrentStatus(u16 index)
{
	// ---------- �ֲ�����������---------- //
	u16 statusAddress;      // status����������λ��
	u8 status;              // statusֵ
	u8 newStatus;           // ����6λ���ֵ
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)���������ҵ�������address
	//(2)�ҵ�status��7,8λ
	statusAddress = meter_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
	status = eefs_base_readByte(statusAddress);
	//(3)������7,8λ������λ����
	newStatus = status & BIT_DATA_MASK;
	newStatus = newStatus >> 6;
	return newStatus;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:��ȡС��������״̬
 * @index:����
 * @return : u8 status
 */
u8 meter_getSmallIndexSendStatus(u16 index)
{
	// ---------- �ֲ�����������---------- //
	u16 statusAddress;      // status����������λ��
	u8 status;              // statusֵ
	u8 newStatus;           // ����4λ���ֵ
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)���������ҵ�������address
	//(2)�ҵ�status��5,6λ
	statusAddress = meter_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
	status = eefs_base_readByte(statusAddress);
	//(3)������5,6λ������λ����
	newStatus = status & BIT_INDEX_MASK;
	newStatus = newStatus >> 4;
	return newStatus;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:��ȡС����1234λ��ʱ��״̬
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_getSmallIndexTimeStatus(u16 index)
{
	// ---------- �ֲ�����������---------- //
	u16 statusAddress;      // status����������λ��
	u8 status;              // statusֵ
	u8 newStatus;           // ���ƺ��ֵ
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)���������ҵ�������address
	//(2)�ҵ�status��5,6λ
	statusAddress = meter_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
	status = eefs_base_readByte(statusAddress);
	//(3)������1234λ������λ����
	newStatus = status & BIT_TIME_MASK;
	return newStatus;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:���С��������������Ӧ�ĵ�ַ
 * @index:����
 * @return : u16 ��ַ
 */
u16 meter_getBreakeNetDataAddress(u16 index)
{
	// ---------- �ֲ�����������---------- //
	int i;
	u16 address;
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	address = 0;
	for (i = 0; i < MAX_INDEX; i++) {
		if (eefs_mbr_getName(i) == SMALLINDEX_NAME) {
			address = eefs_mbr_getAddress(i);
		}
	}
	address = address + index * SMALLINDEX_CAPACITY + index * SMALLINDEX_STATUSCAPACITY + index * SMALLINDEX_CRCCAPACITY;
	return address;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:�������µĶ�������
 * @paramName:xxxxx
 * @return : u16 ��������
 */
u8 meter_disconnect_getData(u8 * retData)
{
	// ---------- �ֲ�����������---------- //
	int i;
	u8 status;
	// ---------- ��������������---------- //

	// ---------- ҵ����---------- //
	//(1)ѭ����ȡС��������״̬
	for (i = 0; i < SMALLINDEX_DATACOUNT; i++) {
		status = meter_getSmallIndexCurrentStatus(i);
		if (status == SMALLINDEX_CURRENTSTATUS) { // ��������
			meter_disconnect_getDataWithIndex(i, retData);
		}
	}
	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:�����������ض�������
 * @index:����
 * @return : u16 ��������
 */
u8 meter_disconnect_getDataWithIndex(u16 index, u8 * retData)
{
	// ---------- �ֲ�����������---------- //
	u16 address;
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)��ȡ����ֵ��Ӧ��������λ��
	address = meter_getBreakeNetDataAddress(index);
	eefs_base_readBytes(address, retData, SMALLINDEX_CAPACITY);
	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:���ݽǱ�����5,6λ����״̬
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_setSmallIndexSendStatus(u16 index, u8 val)
{
	// ---------- �ֲ�����������---------- //
	u16 statusAddress;      // status����������λ��
	u8 status;              // statusֵ
	u8 newStatus;           // ������ֵ
	u8 newVal;
	u8 data;
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	if (val > 3) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)��ȡ�����׵�ַ
	statusAddress = meter_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
	status = eefs_base_readByte(statusAddress);
	//(2)&����, ������λ����: 11110000 & 11001111 = 11000000
	newStatus = status & BIT_INDEX_UNMASK;
	//(3)�������������4λ, ��׼λ��:00000011 << 4 = 00110000
	newVal = val << 4;
	//(4)��������״̬: 11000000 | 00110000 = 11110000
	data = newStatus | newVal;
	//(5)д������״̬
	eefs_base_writeByte(statusAddress, &data);

	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:���ݽǱ�����7,8λ����״̬
 * @index:�Ǳ�
 * @val:״̬
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_setSmallIndexCurrentStatus(u16 index, u8 val)
{
	// ---------- �ֲ�����������---------- //
	u16 statusAddress;      // status����������λ��
	u8 status;              // statusֵ
	u8 newStatus;           // ������ֵ
	u8 newVal;
	u8 data;
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	if (val > 3) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)��ȡ�����׵�ַ
	statusAddress = meter_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
	status = eefs_base_readByte(statusAddress);
	//(2)&����, ���߰�λ����: 11110000 & 00111111 = 00110000
	newStatus = status & BIT_DATA_UNMASK;
	//(3)�������������6λ, ��׼λ��:00000011 << 6 = 11000000
	newVal = val << 6;
	//(4)��������״̬: 00110000 | 11000000 = 11110000
	data = newStatus | newVal;
	//(5)д������״̬
	eefs_base_writeByte(statusAddress, &data);

	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:���ݽǱ�����1,2,3,4λ��ʱ��״̬
 * @index:����
 * @val:�����״ֵ̬
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_setSmallIndexCTimeStatus(u16 index, u8 val)
{
	// ---------- �ֲ�����������---------- //
	u16 statusAddress;      // status����������λ��
	u8 status;              // statusֵ
	u8 newStatus;           // ������ֵ
	u8 data;
	// ---------- ��������������---------- //
	if (index > SMALLINDEX_DATACOUNT - 1) {
		return RET_FAILD;
	}
	if (val > 3) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)��ȡ�����׵�ַ
	statusAddress = meter_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
	status = eefs_base_readByte(statusAddress);
	//(2)&����, ��1234λ����: 00001111 & 11110000 = 00000000
	newStatus = status & BIT_TIME_UNMASK;
	//(3)��������״̬: 00110000 | 11000000 = 11110000
	data = newStatus | val;
	//(4)д������״̬
	eefs_base_writeByte(statusAddress, &data);
	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:��ȡ�Ǳ��Ӧ�Ķ�������, ���ı䷢��״̬, ����Ϊ1���ѷ���, 2Ϊδ����
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_disconnect_getDataAndChangeStatus(u8 * retData)
{
	// ---------- �ֲ�����������---------- //
	int i;
	// ---------- ��������������---------- //
	// ---------- ҵ����---------- //
	//(1)ѭ����ȡ��������
	for (i = 0; i < SMALLINDEX_DATACOUNT; i++) {
		if (meter_getSmallIndexSendStatus(i) == SMALLINDEX_NOSENDSTATUS) {// ΪĬ��״̬2, ֤��������
			meter_disconnect_getDataWithIndex(i, retData);
			//(2)���ýǱ�״̬
			meter_setSmallIndexSendStatus(i, SMALLINDEX_SENDTATUS);
			meter_setSmallIndexCurrentStatus(i, SMALLINDEX_DEFAULTSTATUS);
			return RET_SUCCESS;
		}
	}
	return RET_FAILD;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:���������ݿռ�
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_create_monthCapacity(void)
{
	// ---------- �ֲ�����������---------- //
	int i;
	USERNODE usernode;
	u16 index;              // ��¼������index

	// ---------- ��������������---------- //

	// ---------- ҵ����---------- //
	//(1)ѭ������������, �ҵ�һ��ûʹ�õ�����
	usernode.name = MONTHDATA_NAME;
	index = 0;
	usernode.size = MONTHDATA_COUNT * MONTHDATA_CAPACITY;
	for (i = 0; i < MAX_INDEX; i++) {
		if (eefs_mbr_getIndexStatus(i) == ZERO_INDEXSTATUS) {
			eefs_mbr_setIndexStatus(i, DEFAULT_INDEXSTATUS);
		}
		if (eefs_mbr_getIndexStatus(i) == DEFAULT_INDEXSTATUS) {
			eefs_create(i, usernode); // ����������
			index = i;
			i = MAX_INDEX;
		}
	}

	return RET_SUCCESS;

}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:����������
 * @month:�·�
 * @data:Ҫ���������
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_saveMonthData(u8 month, u8 * data)
{
	// ---------- �ֲ�����������---------- //
	int i;
	u16 address;        // �������׵�ַ
	u16 saveAddress;    // �����λ��
	// ---------- ��������������---------- //
	if (month <= 0 || month > 12) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	address = 0;
	//(1)��ȡ�·ݿռ��Ӧ���������׵�ַ
	for (i = 0; i < MAX_INDEX; i++) {
		if (eefs_mbr_getName(i) == MONTHDATA_NAME) {
			address = eefs_mbr_getAddress(i);
		}
	}
	//(2)��ȡ�����λ��
	saveAddress = address + (month - 1) * MONTHDATA_CAPACITY;
	//(3)д������
	eefs_base_writeBytes(saveAddress, data, MONTHDATA_CAPACITY);
	return RET_SUCCESS;

}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:��ȡ������
 * @month:�·�
 * @data:Ҫ���������
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_getMonthData(u8 month, u8 * ret_data)
{
	// ---------- �ֲ�����������---------- //
	int i;
	u16 address;        // �������׵�ַ
	u16 saveAddress;    // �����λ��
	// ---------- ��������������---------- //

	// ---------- ҵ����---------- //
	address = 0;
	//(1)��ȡ�·ݿռ��Ӧ���������׵�ַ
	for (i = 0; i < MAX_INDEX; i++) {
		if (eefs_mbr_getName(i) == MONTHDATA_NAME) {
			address = eefs_mbr_getAddress(i);
		}
	}
	//(2)��ȡ�����λ��
	saveAddress = address + (month - 1) * MONTHDATA_CAPACITY;
	//(2)��ȡ����
	eefs_base_readBytes(saveAddress, ret_data, MONTHDATA_CAPACITY);
	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:����ʧѹ���ݿռ�
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_create_lostVoltCapacity(void)
{
	// ---------- �ֲ�����������---------- //
	int i;
	USERNODE usernode;
	u16 index;              // ��¼������index

	// ---------- ��������������---------- //

	// ---------- ҵ����---------- //
	//(1)ѭ������������, �ҵ�һ��ûʹ�õ�����
	usernode.name = LOSTVOLT_NAME;
	index = 0;
	usernode.size = LOSTVOLT_COUNT * (LOSTVOLT_CAPACITY + LOSTVOLT_DESCRIBE);
	for (i = 0; i < MAX_INDEX; i++) {
		if (eefs_mbr_getIndexStatus(i) == ZERO_INDEXSTATUS) {
			eefs_mbr_setIndexStatus(i, DEFAULT_INDEXSTATUS);
		}
		if (eefs_mbr_getIndexStatus(i) == DEFAULT_INDEXSTATUS) {
			eefs_create(i, usernode); // ����������
			index = i;
			i = MAX_INDEX;
		}
	}

	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:����ʧѹ����
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_saveLostVoltData(u8 * data)
{
	// ---------- �ֲ�����������---------- //
	u16 address;
	int i;
	int j;
	// ---------- ��������������---------- //

	// ---------- ҵ����---------- //
	address = 0;
	//(1)ȡ���������׵�ַ
	for (i = 0; i < MAX_INDEX; i++) {
		if (eefs_mbr_getName(i) == LOSTVOLT_NAME) {
			address = eefs_mbr_getAddress(i);
		}
	}
	//(2)ѭ����������λ, 1Ϊ��ǰ״̬, 0Ϊ��ǰ״̬
	for (j = 0; j < LOSTVOLT_COUNT; j++) {
		if (meter_getLostVoltCurrentStatus(j) == 1) {
			if (j == LOSTVOLT_COUNT - 1) {// ��������һ��, ���浽0
				meter_saveOnceLostVoltData(0, data);
				return RET_SUCCESS;
			}
			else // ���򱣴浽��һ��
			{
				meter_saveOnceLostVoltData(j + 1, data);
				return RET_SUCCESS;
			}
		}
	}

	meter_saveOnceLostVoltData(0, data);
	return RET_SUCCESS;

}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:����һ��ʧѹ������index��
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_saveOnceLostVoltData(u16 index, u8 * data)
{
	// ---------- �ֲ�����������---------- //
	//    u8 status;
	//    u8 lastStatus;
	u16 address;     // С������ַ
	u16 lastAddress; // ��һ����ַ
	// ---------- ��������������---------- //
	// ---------- ҵ����---------- //
	address = 0;
	lastAddress = 0;

	//(1)����С������������ĵ�ַ
	address = meter_getLostVoltDataAddress(index);
	if (index == 0) {
		lastAddress = meter_getLostVoltDataAddress(LOSTVOLT_COUNT - 1);
	}
	else
	{
		lastAddress = meter_getLostVoltDataAddress(index) - LOSTVOLT_CAPACITY - LOSTVOLT_DESCRIBE;
	}

	//(3)��newAddress��������
	eefs_base_writeBytes(address, data, LOSTVOLT_CAPACITY);

	//(4)д��״̬λ, 1,2ΪΪ��ǰ״̬, 1Ϊ��ǰ, 0Ϊ��ǰ
	meter_setLostVoltCurrentStatus(index, LOSTVOLT_CURRENTSTATUS);
	// ����һ��index����״̬
	if (index == 0) {
		meter_setLostVoltCurrentStatus(LOSTVOLT_COUNT - 1, LOSTVOLT_DEFAULTSTATUS);
	}
	else
	{
		meter_setLostVoltCurrentStatus(index - 1, LOSTVOLT_DEFAULTSTATUS);
	}
	// ʱ��״̬

	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:��ȡʧѹС�����������׵�ַ
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u16 meter_getLostVoltDataAddress(u16 index)
{
	// ---------- �ֲ�����������---------- //
	int i;
	u16 address;
	// ---------- ��������������---------- //
	if (index > LOSTVOLT_COUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	address = 0;
	for (i = 0; i < LOSTVOLT_COUNT; i++) {
		if (eefs_mbr_getName(i) == LOSTVOLT_NAME) {
			address = eefs_mbr_getAddress(i);
		}
	}
	address = address + index * LOSTVOLT_CAPACITY + index * LOSTVOLT_DESCRIBE;
	return address;
}
/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:��ȡʧѹ1,2λ�ĵ�ǰ״̬
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_getLostVoltCurrentStatus(u16 index)
{
	// ---------- �ֲ�����������---------- //
	u16 statusAddress;      // status����������λ��
	u8 status;              // statusֵ
	u8 newStatus;           // ���ƺ��ֵ
	// ---------- ��������������---------- //
	if (index > LOSTVOLT_COUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)���������ҵ�������address
	//(2)�ҵ�status��1,2λ
	statusAddress = meter_getLostVoltDataAddress(index) + LOSTVOLT_CAPACITY;
	status = eefs_base_readByte(statusAddress);
	//(3)������12λ������λ����
	newStatus = status & BIT_GENFLAG_MASK;
	return newStatus;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc: ����ʧѹ1,2λ�ĵ�ǰ״̬
 * @index:����
 * @val:�����״̬
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_setLostVoltCurrentStatus(u16 index, u8 val)
{
	// ---------- �ֲ�����������---------- //
	u16 statusAddress;      // status����������λ��
	u8 status;              // statusֵ
	u8 newStatus;           // ������ֵ
	u8 data;
	// ---------- ��������������---------- //
	if (index > LOSTVOLT_COUNT - 1) {
		return RET_FAILD;
	}
	if (val > 3) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)��ȡ�����׵�ַ
	statusAddress = meter_getLostVoltDataAddress(index) + SMALLINDEX_CAPACITY;
	status = eefs_base_readByte(statusAddress);
	//(2)&����, ��1,2λ����: 00001111 & 11111100 = 00001100
	newStatus = status & BIT_GENFLAG_UNMASK;
	//(3)��������״̬: 00110000 | 11000000 = 11110000
	data = newStatus | val;
	//(4)д������״̬
	eefs_base_writeByte(statusAddress, &data);
	return RET_SUCCESS;
}

/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:���ݽǱ�ȡʧѹ����
 * @paramName:xxxxx
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_getLostVoltData(u16 index, u8 * retData)
{
	// ---------- �ֲ�����������---------- //
	u16 address;
	// ---------- ��������������---------- //
	if (index > LOSTVOLT_COUNT - 1) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//(1)��ȡ����ֵ��Ӧ��������λ��
	address = meter_getLostVoltDataAddress(index);
	eefs_base_readBytes(address, retData, LOSTVOLT_CAPACITY);
	return RET_SUCCESS;
}