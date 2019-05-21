#include <stdio.h>
#include <string.h>
#include "eefs_lib.h"
#include "meter_base.h"


/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:创建指定类型的数据空间
 * @meaterVer:创建参数
 * @index 索引
 * @return : 1:成功 0：失败
 */
u8 meter_register(u16 index,MEATERVAR meaterVer) {
	// ---------- 局部变量定义区---------- //
	USERNODE userNode;
	u8 dataStatus;
	u8 netStatus;
	u8 genFlag;
	u8 netStaus;
	u8 back;
	u8 offset;
	u8 crcSize;
	// ---------- 输入参数条件检测---------- //
	// (1)判断index是否合法
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
	// 赋值userNode
	if (meaterVer.crc == 1){
		crcSize = DATA_CRC_SIZE;
	}else{
		crcSize = 0;
	}
	userNode.name = meaterVer.name;
	userNode.size = (meaterVer.size + crcSize + DATA_STATUS_SIZE) * meaterVer.type;
	back = eefs_create(index, userNode);
	if (back != RET_SUCCESS)
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
	eefs_mbr_setNetStatus(index, netStatus);
	return RET_SUCCESS;
}  
/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:判断写入类型获取数据状态
 * @WRITE_TYPE 写入类型
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
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:根据索引获取写入类型
 * @index 索引
 * @return : WRITE_TYPE 写入类型
 */
u8 meter_get_write_type(u16 index) {
	//局部变量
	u8 dataStatus;
	//判断写入类型
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
}
/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:数据循环写入
 * @index 索引
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 meter_circle_write(u16 index, u8* data, u16 len) {
	// ---------- 局部变量定义区---------- //
	u8 writeType;
	u8 back;
	u16 writeAddr;
	u16 dataSize;
	u8 *datas;
	int i;
	int zero;
	u8 indexStatusFlag;
	u8 offset;
	u16 oldStatusAddr;
	u8 ns;
	u8 os;
	// ---------- 输入参数条件检测---------- //
	// (1)判断index是否合法
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
			//获取当前数据通用标记
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	//设置是否需要CRC偏移量
	if (indexStatusFlag == CRC_Y) {
		offset = DATA_CRC_SIZE;
	}
	else {
		offset = 0;
	}
	//获取写入类型
	writeType = meter_get_write_type(index);
	//处理写入数据长度
	dataSize = eefs_mbr_getDataSize(index)/writeType - offset - DATA_STATUS_SIZE;
	datas = malloc(dataSize);
	zero = 0;
	if (len< dataSize)
	{
		memcpy(datas,data,len);
		for (i = 0; i < (dataSize-len); i++)
		{
			memcpy(datas + len + i, &zero, 1);
		}
	}
	else
	{
		memcpy(datas, data, dataSize);
	}
	//输入类型1单独区分
	if (writeType == 1){
		writeAddr = eefs_data_getHeadAddr(index);
	}else{
		//找到当前的写入位置
		writeAddr = meter_get_write_address(index);
	}
	//写入数据
	eefs_base_writeBytes(writeAddr,datas,dataSize);
	//修改datastatus状态
	ns = DATA_NEW_POS_STATUS;
	os = DATA_OLD_POS_STATUS;
	eefs_base_writeByte(writeAddr + dataSize, &ns);
	if (writeType == 1)
	{
		free(datas);
		return RET_SUCCESS;
	}
	if (writeAddr == eefs_data_getHeadAddr(index)){
		oldStatusAddr = writeAddr + (dataSize + offset + DATA_STATUS_SIZE) * writeType - offset - DATA_STATUS_SIZE;
	}
	else{
		oldStatusAddr = writeAddr - offset-DATA_STATUS_SIZE;
	}
	eefs_base_writeByte(oldStatusAddr, &os);
	free(datas);
	return RET_SUCCESS;
}


/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:数据循环读取
 * @index 索引
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 meter_circle_read(u16 index, u8* retData) {
	// ---------- 局部变量定义区---------- //
	u16 writeAddr;
	u16 dataSize;
	u8 indexStatusFlag;
	u8 crcOffset;
	u8 writeType;
	// ---------- 输入参数条件检测---------- //
	// (1)判断index是否合法
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
		//获取当前数据通用标记
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	//设置是否需要CRC偏移量
	if (indexStatusFlag == CRC_Y) {
		crcOffset = DATA_CRC_SIZE;
	}
	else {
		crcOffset = 0;
	}
	//获取写入类型
	writeType = meter_get_write_type(index);

	dataSize = eefs_mbr_getDataSize(index)/writeType;
	//找到当前的读取位置
	writeAddr = meter_get_data_status_address(index)-(dataSize- crcOffset - DATA_STATUS_SIZE);
	//读取数据 TODO:CRC检验 
	eefs_base_readBytes(writeAddr, retData, dataSize - DATA_STATUS_SIZE - crcOffset);
	return RET_SUCCESS;
}


/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:获取当前可以写入位置
 * @index 索引
 * @return : 可写入区域首地址
 */
u16 meter_get_write_address(u16 index) {
	// ---------- 局部变量定义区---------- //
	int i;
	u8 indexStatusFlag;
	u8 writeType;
	u16 dataHeadAddr;
	u16 writeDataStatus;
	u16 writeDataStatusAddr;
	u8 offset;
	u16 dataSize;
	// ---------- 输入参数条件检测---------- //
	// (1)判断index是否合法
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
	writeType = meter_get_write_type(index);
	dataSize = eefs_mbr_getDataSize(index)/writeType;
	dataHeadAddr = eefs_data_getHeadAddr(index);
	//获取当前数据通用标记
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	//设置是否需要CRC偏移量
	if (indexStatusFlag == CRC_Y) {
		offset = DATA_CRC_SIZE;
	}
	else {
		offset = 0;
	}
	for (i = 0; i < writeType; i++)
	{
		
		writeDataStatusAddr = dataHeadAddr + dataSize * (i + 1) - offset - DATA_STATUS_SIZE;
		writeDataStatus = eefs_base_readByte(writeDataStatusAddr);  //新旧写入状态

		if (i == 0 && writeDataStatus != DATA_NEW_POS_STATUS && writeDataStatus!= DATA_OLD_POS_STATUS)
		{
			return dataHeadAddr;
		}
		if (writeDataStatus == DATA_NEW_POS_STATUS) {
			if (i == writeType - 1) {
				return dataHeadAddr;
			}
			else
			{
				return writeDataStatusAddr + DATA_STATUS_SIZE + offset;
			}	
		}
	}

}

/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:获取当前可读数据的状态位
 * @index 索引
 * @return : 读取数据的状态位
 */
u16 meter_get_data_status_address(u16 index) {
	// ---------- 局部变量定义区---------- //
	int i;
	u8 indexStatusFlag;
	u8 writeType;
	u16 dataHeadAddr;
	u16 dataSize;
	u16 writeDataStatus;
	u8 writeDataStatusAddr;
	u8 offset;
	// ---------- 输入参数条件检测---------- //
	// (1)判断index是否合法
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
	// 找到当前的写入位置
	//获取当前数据通用标记
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	//设置是否需要CRC偏移量
	if (indexStatusFlag == CRC_Y) {
		offset = DATA_CRC_SIZE;
	}
	else {
		offset = 0;
	}
	//获取写入类型
	writeType = meter_get_write_type(index);
	//找到当前的写入位置
	dataHeadAddr = eefs_data_getHeadAddr(index);
	dataSize = eefs_mbr_getDataSize(index)/writeType;
	for (i = 0; i < writeType; i++)
	{
		writeDataStatusAddr = dataHeadAddr + dataSize * i - offset;//新旧状态位地址
		writeDataStatus = eefs_base_readByte(writeDataStatusAddr);  //新旧写入状态
		if (writeDataStatus == DATA_NEW_POS_STATUS)
		{
			return writeDataStatusAddr;
		}
	}
	return eefs_data_getHeadAddr(index) + dataSize - offset - DATA_STATUS_SIZE;
}

