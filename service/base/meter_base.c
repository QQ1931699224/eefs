#include <stdio.h>
#include "eeprom/eefs_lib.h"
#include "service/base/meter_base.h"
#include <stdlib.h>
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
    return tpye_write_1;
    
}

/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:根据索引获取写入类型
 * @index 索引
 * @return : WRITE_TYPE 写入类型
 */
s8 meter_get_write_type(u16 index) {
	//局部变量
	s8 dataStatus;
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
	return RET_ERROR;
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
	s8 writeType;                          //写入类型
	u16 writeAddr;                         //写入地址
	u16 dataSize;	                       //数据大小
	u8 *datas;                             //临时数组
	int i;
	int zero;	                           //写入用0
	s8 indexStatusFlag;                    //通用标记位/CRC
	u8 crcOffset;                          //crc偏移量
	u16 oldStatusAddr;                     //上一个数据状态位地址
	u8 ns;                                 //新状态
	u8 os;                                 //旧状态
	// ---------- 输入参数条件检测---------- //
	// (1)判断index是否合法
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
	//获取当前数据通用标记(判断CRC)
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	if (indexStatusFlag == RET_ERROR)
	{
		return RET_FAILD;
	}
	//设置是否需要CRC偏移量
	if (indexStatusFlag == CRC_Y) {
		crcOffset = DATA_CRC_SIZE;
	}
	else {
		crcOffset = 0;
	}
	//获取写入类型(处理写入数据长度)
	writeType = meter_get_write_type(index);
	if (writeType == RET_ERROR )
	{
		return RET_FAILD;
	}
	//处理写入数据长度
	dataSize = eefs_mbr_getDataSize(index)/writeType - crcOffset - DATA_STATUS_SIZE;
	if (dataSize == 0 )
	{
		return RET_FAILD;
	}
	datas = malloc(dataSize);
	zero = 0;
	//不够长度补0 对齐数据区长度
	if (len< dataSize)
	{
		memcpy(datas,data,len);
		for (i = 0; i < (dataSize-len); i++)
		{
			memcpy(datas + len + i, &zero, 1);
		}
	}
	else //大于或等于数据区长度 写入数据区长度的数据
	{
		memcpy(datas, data, dataSize);
	}
	//找到当前的写入位置
	writeAddr = meter_get_write_address(index);
	//写入数据
	eefs_base_writeBytes(writeAddr,datas,dataSize);
	//修改datastatus状态
	ns = DATA_NEW_POS_STATUS;
	os = DATA_OLD_POS_STATUS;

	if (eefs_base_writeByte(writeAddr + dataSize, &ns)!=RET_SUCCESS){ return RET_FAILD; }

	//第一个区域写入 修改上一个数据状态 固定最后一个区域地址的数据位
	if (writeAddr == eefs_data_getHeadAddr(index)){
		oldStatusAddr = writeAddr + (dataSize + crcOffset + DATA_STATUS_SIZE) * writeType - crcOffset - DATA_STATUS_SIZE;
	}
	//写入 上一个数据区修改数据位状态 
	else{
		oldStatusAddr = writeAddr - crcOffset -DATA_STATUS_SIZE;
	}
	if (eefs_base_writeByte(oldStatusAddr, &os) != RET_SUCCESS) { return RET_FAILD; }
	
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
	s8 indexStatusFlag;
	u8 crcOffset;
	s8 writeType;
	// ---------- 输入参数条件检测---------- //
	// (1)判断index是否合法
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
		//获取当前数据通用标记
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	if (indexStatusFlag == RET_ERROR)
	{
		return RET_FAILD;
	}
	//设置是否需要CRC偏移量
	if (indexStatusFlag == CRC_Y) {
		crcOffset = DATA_CRC_SIZE;
	}
	else {
		crcOffset = 0;
	}
	//获取写入类型
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
	//找到当前的读取位置
	writeAddr = meter_get_data_status_address(index)-(dataSize- crcOffset - DATA_STATUS_SIZE);
	if (writeAddr == RET_FAILD)
	{
		return RET_FAILD;
	}
	//读取数据 TODO:CRC检验 
	if (eefs_base_readBytes(writeAddr, retData, dataSize - DATA_STATUS_SIZE - crcOffset) != RET_SUCCESS) { return RET_FAILD; }
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
	s8 indexStatusFlag;
	s8 writeType;
	u16 dataHeadAddr;
	u16 writeDataStatus;
	u16 writeDataStatusAddr;
	u8 offset;
	u16 dataSize;
	u16 writeAddr;
	// ---------- 输入参数条件检测---------- //
	// (1)判断index是否合法
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
	writeType = meter_get_write_type(index);
	if (writeType == RET_ERROR)
	{
		return RET_FAILD;
	}
	dataSize = eefs_mbr_getDataSize(index)/writeType;
	dataHeadAddr = eefs_data_getHeadAddr(index);
	//获取当前数据通用标记
	indexStatusFlag = eefs_mbr_getGenFlag(index);
	if (indexStatusFlag == RET_ERROR)
	{
		return RET_FAILD;
	}
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
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:获取当前可读数据的状态位
 * @index 索引
 * @return : 读取数据的状态位
 */
u16 meter_get_data_status_address(u16 index) {
	// ---------- 局部变量定义区---------- //
	int i;
	s8 indexStatusFlag;
	s8 writeType;
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
	if (indexStatusFlag == RET_ERROR)
	{
		return RET_FAILD;
	}
	//设置是否需要CRC偏移量
	if (indexStatusFlag == CRC_Y) {
		offset = DATA_CRC_SIZE;
	}
	else {
		offset = 0;
	}
	//获取写入类型
	writeType = meter_get_write_type(index);
	if (writeType == RET_ERROR)
	{
		return RET_FAILD;
	}
	//找到当前的写入位置
	dataHeadAddr = eefs_data_getHeadAddr(index);
	if (dataHeadAddr == RET_FAILD)
	{
		return RET_FAILD;
	}
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

