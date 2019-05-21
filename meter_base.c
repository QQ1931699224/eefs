#include <stdio.h>
#include <string.h>
#include "eefs_lib.h"
#include "meter_base.h"


/*
 * Auth: ����˧
 * Date: 2019-5-10
 * Desc:����ָ�����͵����ݿռ�
 * @meaterVer:��������
 * @index ����
 * @return : 1:�ɹ� 0��ʧ��
 */
u8 meter_register(u16 index,MEATERVAR meaterVer) {
	// ---------- �ֲ�����������---------- //
	USERNODE userNode; 
	u8 dataStatus;
	u8 netStatus;
	u8 genFlag;
	u8 netStaus;
	u8 offset;
	u8 crcSize;
	// ---------- ��������������---------- //
	// (1)�ж�index�Ƿ�Ϸ�
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	// ��ֵuserNode
	if (meaterVer.crc == 1){
		crcSize = DATA_CRC_SIZE;
	}else{
		crcSize = 0;
	}
	userNode.name = meaterVer.name;
	userNode.size = (meaterVer.size + crcSize + DATA_STATUS_SIZE) * meaterVer.type;
	if (eefs_create(index, userNode) != RET_SUCCESS)
	{
		return RET_FAILD;
	}
	dataStatus = meter_get_data_status(meaterVer.type);
	netStatus = meaterVer.net;
	genFlag = meaterVer.crc;
	netStatus = meaterVer.net;
	eefs_mbr_setDataStatus(index, dataStatus);
	eefs_mbr_setNetStatus(index, netStatus);
	eefs_mbr_setGenFlag(index, genFlag);
	return RET_SUCCESS;
}  
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:�ж�д�����ͻ�ȡ����״̬
 * @WRITE_TYPE д������
 * @return : dateStatus
 */
TYPE_WRITE meter_get_data_status(u8 WRITE_TYPE) {
	if (WRITE_TYPE == TYPE_WRITE_1)
	{
		return tpye_write_1;
	}
	else if(WRITE_TYPE == TYPE_WRITE_4)
	{
		return tpye_write_4;
	}
	else if (WRITE_TYPE == TYPE_WRITE_8)
	{
		return tpye_write_8;
	}
	else if (WRITE_TYPE == TYPE_WRITE_16)
	{
		return tpye_write_16;
	}
}

/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:����������ȡд������
 * @index ����
 * @return : WRITE_TYPE д������
 */
s8 meter_get_write_type(u16 index) {
	//�ֲ�����
	s8 dataStatus;
	//�ж�д������
	dataStatus = eefs_mbr_getDataStatus(index);

	if (dataStatus == tpye_write_1)
	{
		return TYPE_WRITE_1;
	}
	else if (dataStatus == tpye_write_4)
	{
		return TYPE_WRITE_4;
	}
	else if (dataStatus == tpye_write_8)
	{
		return TYPE_WRITE_8;
	}
	else if (dataStatus == tpye_write_16)
	{
		return TYPE_WRITE_16;
	}
	return RET_ERROR;
}
/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:����ѭ��д��
 * @index ����
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 meter_circle_write(u16 index, u8* data, u16 len) {
	// ---------- �ֲ�����������---------- //
	s8 writeType;                          //д������
	u16 writeAddr;                         //д���ַ
	u16 dataSize;	                       //���ݴ�С
	u8 *datas;                             //��ʱ����
	int i;
	int zero;	                           //д����0
	u8 indexStatusFlag;                    //ͨ�ñ��λ/CRC
	u8 crcOffset;                          //crcƫ����
	u16 oldStatusAddr;                     //��һ������״̬λ��ַ
	u8 ns;                                 //��״̬
	u8 os;                                 //��״̬
	// ---------- ��������������---------- //
	// (1)�ж�index�Ƿ�Ϸ�
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	//��ȡ��ǰ����ͨ�ñ��(�ж�CRC)
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	if (indexStatusFlag == RET_ERROR)
	{
		return RET_FAILD;
	}
	//�����Ƿ���ҪCRCƫ����
	if (indexStatusFlag == CRC_Y) {
		crcOffset = DATA_CRC_SIZE;
	}
	else {
		crcOffset = 0;
	}
	//��ȡд������(����д�����ݳ���)
	writeType = meter_get_write_type(index);
	if (writeType == RET_ERROR )
	{
		return RET_FAILD;
	}
	//����д�����ݳ���
	dataSize = eefs_mbr_getDataSize(index)/writeType - crcOffset - DATA_STATUS_SIZE;
	if (dataSize == 0 )
	{
		return RET_FAILD;
	}
	datas = malloc(dataSize);
	zero = 0;
	//�������Ȳ�0 ��������������
	if (len< dataSize)
	{
		memcpy(datas,data,len);
		for (i = 0; i < (dataSize-len); i++)
		{
			memcpy(datas + len + i, &zero, 1);
		}
	}
	else //���ڻ�������������� д�����������ȵ�����
	{
		memcpy(datas, data, dataSize);
	}
	//�ҵ���ǰ��д��λ��
	writeAddr = meter_get_write_address(index);
	//д������
	eefs_base_writeBytes(writeAddr,datas,dataSize);
	//�޸�datastatus״̬
	ns = DATA_NEW_POS_STATUS;
	os = DATA_OLD_POS_STATUS;

	if (eefs_base_writeByte(writeAddr + dataSize, &ns)!=RET_SUCCESS){ return RET_FAILD; }

	//��һ������д�� �޸���һ������״̬ �̶����һ�������ַ������λ
	if (writeAddr == eefs_data_getHeadAddr(index)){
		oldStatusAddr = writeAddr + (dataSize + crcOffset + DATA_STATUS_SIZE) * writeType - crcOffset - DATA_STATUS_SIZE;
	}
	//д�� ��һ���������޸�����λ״̬ 
	else{
		oldStatusAddr = writeAddr - crcOffset -DATA_STATUS_SIZE;
	}
	if (eefs_base_writeByte(oldStatusAddr, &os) != RET_SUCCESS) { return RET_FAILD; }
	
	free(datas);
	return RET_SUCCESS;
}


/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:����ѭ����ȡ
 * @index ����
 * @data ����
 * @len ����
 * @return : 0,1
 */
u8 meter_circle_read(u16 index, u8* retData) {
	// ---------- �ֲ�����������---------- //
	u16 writeAddr;
	u16 dataSize;
	u8 indexStatusFlag;
	u8 crcOffset;
	s8 writeType;
	// ---------- ��������������---------- //
	// (1)�ж�index�Ƿ�Ϸ�
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
		//��ȡ��ǰ����ͨ�ñ��
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	if (indexStatusFlag == RET_ERROR)
	{
		return RET_FAILD;
	}
	//�����Ƿ���ҪCRCƫ����
	if (indexStatusFlag == CRC_Y) {
		crcOffset = DATA_CRC_SIZE;
	}
	else {
		crcOffset = 0;
	}
	//��ȡд������
	writeType = meter_get_write_type(index);
	if (writeType == RET_ERROR)
	{
		return RET_FAILD;
	}
	dataSize = eefs_mbr_getDataSize(index)/writeType;
	if (dataSize == 0)
	{
		return RET_FAILD;
	}
	//�ҵ���ǰ�Ķ�ȡλ��
	writeAddr = meter_get_data_status_address(index)-(dataSize- crcOffset - DATA_STATUS_SIZE);
	if (writeAddr == RET_FAILD)
	{
		return RET_FAILD;
	}
	//��ȡ���� TODO:CRC���� 
	if (eefs_base_readBytes(writeAddr, retData, dataSize - DATA_STATUS_SIZE - crcOffset) != RET_SUCCESS) { return RET_FAILD; }
	return RET_SUCCESS;
}


/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:��ȡ��ǰ����д��λ��
 * @index ����
 * @return : ��д�������׵�ַ
 */
u16 meter_get_write_address(u16 index) {
	// ---------- �ֲ�����������---------- //
	int i;
	u8 indexStatusFlag;
	s8 writeType;
	u16 dataHeadAddr;
	u16 writeDataStatus;
	u16 writeDataStatusAddr;
	u8 offset;
	u16 dataSize;
	u16 writeAddr;
	// ---------- ��������������---------- //
	// (1)�ж�index�Ƿ�Ϸ�
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	writeType = meter_get_write_type(index);
	if (writeType == RET_ERROR)
	{
		return RET_FAILD;
	}
	dataSize = eefs_mbr_getDataSize(index)/writeType;
	dataHeadAddr = eefs_data_getHeadAddr(index);
	//��ȡ��ǰ����ͨ�ñ��
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	if (indexStatusFlag == RET_ERROR)
	{
		return RET_FAILD;
	}
	//�����Ƿ���ҪCRCƫ����
	if (indexStatusFlag == CRC_Y) {
		offset = DATA_CRC_SIZE;
	}
	else {
		offset = 0;
	}
	for (i = 0; i < writeType; i++)
	{
		
		writeDataStatusAddr = dataHeadAddr + dataSize * (i + 1) - offset - DATA_STATUS_SIZE;
		writeDataStatus = eefs_base_readByte(writeDataStatusAddr);  //�¾�д��״̬
		if (writeDataStatus == DATA_NEW_POS_STATUS) {
			writeAddr = writeDataStatusAddr + DATA_STATUS_SIZE + offset;
			if (writeAddr+dataSize >= eefs_data_getTailAddr(index)- DATA_DESCRIBE)
			{
				return dataHeadAddr;
			}
			else
			{
				return writeAddr;
			}		
		}
		
	}
	return dataHeadAddr;
}

/*
 * Auth: �����
 * Date: 2019-5-10
 * Desc:��ȡ��ǰ�ɶ����ݵ�״̬λ
 * @index ����
 * @return : ��ȡ���ݵ�״̬λ
 */
u16 meter_get_data_status_address(u16 index) {
	// ---------- �ֲ�����������---------- //
	int i;
	u8 indexStatusFlag;
	s8 writeType;
	u16 dataHeadAddr;
	u16 dataSize;
	u16 writeDataStatus;
	u8 writeDataStatusAddr;
	u8 offset;
	// ---------- ��������������---------- //
	// (1)�ж�index�Ƿ�Ϸ�
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- ҵ����---------- //
	// �ҵ���ǰ��д��λ��
	//��ȡ��ǰ����ͨ�ñ��
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	if (indexStatusFlag == RET_ERROR)
	{
		return RET_FAILD;
	}
	//�����Ƿ���ҪCRCƫ����
	if (indexStatusFlag == CRC_Y) {
		offset = DATA_CRC_SIZE;
	}
	else {
		offset = 0;
	}
	//��ȡд������
	writeType = meter_get_write_type(index);
	if (writeType == RET_ERROR)
	{
		return RET_FAILD;
	}
	//�ҵ���ǰ��д��λ��
	dataHeadAddr = eefs_data_getHeadAddr(index);
	if (dataHeadAddr == RET_FAILD)
	{
		return RET_FAILD;
	}
	dataSize = eefs_mbr_getDataSize(index)/writeType;
	for (i = 0; i < writeType; i++)
	{
		writeDataStatusAddr = dataHeadAddr + dataSize * i - offset;//�¾�״̬λ��ַ
		writeDataStatus = eefs_base_readByte(writeDataStatusAddr);  //�¾�д��״̬
		if (writeDataStatus == DATA_NEW_POS_STATUS)
		{
			return writeDataStatusAddr;
		}
	}
	return eefs_data_getHeadAddr(index) + dataSize - offset - DATA_STATUS_SIZE;
}

