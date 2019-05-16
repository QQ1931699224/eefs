//
//  WriteOrReadByte.c
//  CTest
//
//  Created by 吴晗帅 on 2019/5/10.
//  Copyright © 2019 吴晗帅. All rights reserved.
//

#include "eefs_lib.h"
u8 G_LIST[EE_MAX_CAPACITY];
u8 G_STATUS_LISI[MAX_INDEX];





/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc://在eeprom的指定位置读取1个字节
 * @index:地址
 * @return : u8
 */
u8 eefs_base_readByte(u16 address) {
	u8 value = *(G_LIST + address);
	return value;
} 

/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc:在eeprom的指定位置写入1个字节
 * @index:地址
 * @return : u8
 */
u8 eefs_base_writeByte(u16 address,u8 *data) {
	//
	u8 lastData;
	lastData = eefs_base_readByte(address);
	if (lastData == *data)
	{
		return RET_SUCCESS;
	}
	memcpy(G_LIST + address, data,1);
	return RET_SUCCESS;
}

/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc://在eeprom的指定位置写入dataLen个字节
 * @address:地址
 * @data u8
 * @datalen 长度
 * @return : 0,1
 */
u8 eefs_base_writeBytes(u16 address, u8* data, u16 dataLen) {
	memcpy(G_LIST + address, data, dataLen);
	return RET_SUCCESS;
}

/*
 * Auth: 张添程
 * Date: 2019-5-10
 * Desc://从eeprom的指定位置读取retLen个字节
 * @address:地址
 * @data u8
 * @datalen 长度
 * @return : 0,1
 */
u8 eefs_base_readBytes(u16 address, u8* retData, u16 retLen) {
	u16 i;
	for (i = 0; i < retLen; i++)
	{
		*(retData++) = *(G_LIST + address + i);
	}
	return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取索引区对应的状态
 * @index:索引
 * @return : s8 索引状态
 */

s8 eefs_mbr_getStatus(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u16 statusOffset;       // status的位置
    s8 data;                // 索引的status信息
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_ERROR;
    }
    // ---------- 业务处理---------- //
    // (1). 找到status的位置
    statusOffset = eefs_mbr_getIndexStatusHeadAddress(index);
    // (2). 读数据
	data = eefs_base_readByte(statusOffset);
    return data;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:向全局数组中载入index的status值
 * @paramName:无
 * @return : 1成功 0失败
 */

u8 eefs_mbr_load(void)
{
    // ---------- 局部变量定义区---------- //
    int i;
    s8 data = '\0';     //索引区对应的status
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)循环取出数据
    for (i = 0; i < MAX_INDEX; i++) {
        data = eefs_mbr_getStatus(i);
        G_STATUS_LISI[i] = data;
    }
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:添加新索引
 * @index:索引位置
 * @userNode:用户输入的结构体
 * @return : 1:成功 0：失败
 */
u8 eefs_mbr_create(u16 index, USERNODE userNode)
{
    // ---------- 局部变量定义区---------- //
    u16 address;            // 数据存放的地址
    u16 indexAddress;       // 索引在内存中的位置
    NODE node;              // 索引结构体
    // ---------- 输入参数条件检测---------- //
    // (1)判断index是否合法
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    // (2)判断node size
    // TODO:该处判断应该用剩余空间容量函数的返回值进行判断
    if (userNode.size > EE_MAX_CAPACITY - EE_SYS_CAPACITY) {
        return ERR_INVALIDPARAM;
    }
    
    // ---------- 业务处理---------- //
    //(1)获取数据存放地址
    address = getAddress(userNode.size);
    if (eefs_mbr_CheckAddress(address) != RET_SUCCESS) {
        return RET_FAILD;
    }
    //(2)获取索引的位置
    indexAddress = getIndexAddress(index);
    //(3)给node结构体赋值
    node.name = userNode.name;
    node.address = address;
    node.size = userNode.size;
    node.status = STATUS_INITVALUE;
    //(4)写入数据到索引区, 返回成功
    G_STATUS_LISI[index] = node.status;
    return writeDataToIndex(indexAddress, node);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-13
 * Desc:判断index是否合法
 * @index:索引号
 * @return : 1:成功 0：失败
 */

u8 eefs_mbr_CheckIndex(u16 index)
{
    // index在0-128之间
    if (index >= 0 && index < MAX_INDEX) {
        return RET_SUCCESS;
    }
    else
    {
        return RET_FAILD;
    }
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-13
 * Desc:获取模拟地址
 * @paramName:无
 * @return : u16模拟地址
 */

u16 getAddress(u16 size)
{
    return eefs_data_findUnusedAddr(size);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:判断地址合法性
 * @address:地址
 * @return : 1:成功 0：失败
 */

u8 eefs_mbr_CheckAddress(u16 address)
{
    // TODO:后期根据业务完善
    // 地址在系统预留空间与最大容量之间
    if (address >= EE_SYS_CAPACITY && address < EE_MAX_CAPACITY) {
        return RET_SUCCESS;
    }
    else
    {
        return RET_FAILD;
    }
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取索引在大数组中的位置
 * @index:索引号
 * @return : 索引在大数组中的位置
 */

u16 getIndexAddress(u16 index)
{
    return EE_START_INDEX + index * INDEX_SIZE;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:写入数据到索引区
 * @myAddress:地址
 * @node:写入的结构体
 * @return : 1:成功 0：失败
 */
u8 writeDataToIndex(u16 myAddress, NODE node)
{
    u8 data[INDEX_SIZE]; // 索引区中的单个索引
    memcpy(data, (u8 *)&node, INDEX_SIZE);
	eefs_base_writeBytes(myAddress, data, INDEX_SIZE);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-13
 * Desc:获取索引中的数据状态(第九字节)
 * @index:索引
 * @return : s8数据状态
 */

s8 eefs_mbr_getDataStatus(u16 index)
{
    // ---------- 局部变量定义区---------- //
    s8 statusData;  // 对应索引的status
    u8 data;        // 7,8位的数据
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_ERROR;
    }
    // ---------- 业务处理---------- //
    //(1)找到对应索引的status 11001100
    statusData = eefs_mbr_getStatus(index);
    //(2)取7, 8位数据状态: 11001100 & 11000000 = 11000000
    data = statusData & BIT_DATA_MASK;
    //(3)右移: 00000011
    data = data >> 6;
    
    return data;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:设置数据状态
 * @index:索引
 * @val:输入的数据
 * @return : 1:成功 0：失败
 */

u8 eefs_mbr_setDataStatus(u16 index ,u8 val)
{
    // ---------- 局部变量定义区---------- //
    u8 statusData;      // 对应索引的status
    u8 data;            // 设置的数据状态
    u8 newData;         // 置零后的数据
    u8 newVal;          // 移位后的数据
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_ERROR;
    }
    if (val > 3 || val < 0) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)找到对应索引的status 11001100
    statusData = eefs_mbr_getStatus(index);
    //(2)&运算, 7,8位清零: 11001100 & 00111111 = 00001100
    newData = statusData & BIT_DATA_UNMASK;
    //(3)传入的数据左移6位, 对准位置:00000011 << 6 = 11000000
    newVal = val << 6;
    //(4)数据状态: 00001100 | 11000000 = 11001100
    data = newData | newVal;
    //(5)给索引区的状态赋值
    eefs_mbr_setStatus(index, data);
    return RET_SUCCESS;
    
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:设置状态
 * @index:索引
 * @val:输入的数据
 * @return : 1:成功 0：失败
 */
u8 eefs_mbr_setStatus(u16 index ,u8 val)
{
    // ---------- 局部变量定义区---------- //
    u16 startIndex;     // 索引的起始位置
    u16 startStatus;    // 索引状态的起始位置
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)找到索引状态的起始位置
    startIndex = getIndexAddress(index);
    startStatus = startIndex + STATUS_OFFSET;
    //(2)设置索引状态
    //writeByte(startStatus, &val, 1);
	eefs_base_writeByte(startStatus, &val);
    G_STATUS_LISI[index] = val;
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取索引状态
 * @index:索引
 * @return : s8索引状态
 */

s8 eefs_mbr_getIndexStatus(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u8 statusData;      // status数据
    u8 newData;         // 5,6位的状态数据
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_ERROR;
    }
    
    // ---------- 业务处理---------- //
    //(1)获取状态位数据 11111100
    statusData = eefs_mbr_getStatus(index);
    //(2)获取五六位状态: 11111100 & 00110000 = 00110000
    newData = statusData & BIT_INDEX_MASK;
    //(3)将得到的数据右移4位:00000011
    newData >>= 4;
    return newData;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:设置索引状态
 * @index:索引
 * @return : 1:成功 0：失败
 */

u8 eefs_mbr_setIndexStatus(u16 index , u8 val)
{
    // ---------- 局部变量定义区---------- //
    u8 statusData;      // 对应索引的status
    u8 data;            // 数据状态
    u8 newData;         // 5,6位清零后的数据
    u8 newVal;          // 移位后的数据
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    if (val > 3) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)找到对应索引的status 11110000
    statusData = eefs_mbr_getStatus(index);
    //(2)&运算, 把五六位清零: 11110000 & 11001111 = 11000000
    newData = statusData & BIT_INDEX_UNMASK;
    //(3)传入的数据左移4位, 对准位置:00000011 << 4 = 00110000
    newVal = val << 4;
    //(4)设置数据状态: 11000000 | 00110000 = 11110000
    data = newData | newVal;
    //(5)写入数据状态
    eefs_mbr_setStatus(index, data);
    return RET_SUCCESS;
    
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取网络状态
 * @index:索引
 * @return : s8数据
 */

s8 eefs_mbr_getNetStatus(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u8 statusData;      // 状态位数据
    u8 newData;         // 3,4位的数据
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_ERROR;
    }
    // ---------- 业务处理---------- //
    //(1)获取状态位  11001100
    statusData = eefs_mbr_getStatus(index);
    //(2)&运算, 把除了3,4位的其他位清零:11001100 & 00001100 = 00001100
    newData = statusData & BIT_NET_MASK;
    //(3)右移两位 00001100 >> 2 = 00000011
    newData >>= 2;
    
    return newData;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:设置网络状态
 * @index:索引
 * @val:输入的数据
 * @return : 1:成功 0：失败
 */

u8 eefs_mbr_setNetStatus(u16 index ,u8 val)
{
    // ---------- 局部变量定义区---------- //
    u8 statusData;      // 状态位数据
    u8 newData;         // 清零后的数据
    u8 newVal;          // 移位后的数据
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    if (val > 3) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)获取状态位 : 11001100
    statusData = eefs_mbr_getStatus(index);
    //(2)将3,4位置清零: 11001100 & 11110011 = 11000000
    newData = statusData & BIT_NET_UNMASK;
    //(3)将val左移两位: 00000011 << 2 = 00001100
    newVal = val << 2;
    //(3)进行或运算赋值: 11000000 | 00001100 = 11001100
    newData |= newVal;
    //(4)写入
    eefs_mbr_setStatus(index, newData);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-14
 * Desc:获取通用标记值
 * @index:索引
 * @return : s8数据
 */

s8 eefs_mbr_getGenFlag(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u8 statusData;      // 状态位数据
    u8 newData;         // 1,2位数据
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_ERROR;
    }
    // ---------- 业务处理---------- //
    //(1)获取状态位 11000011
    statusData = eefs_mbr_getStatus(index);
    //(2)进行与运算, 将除了1,2位的其他位清零: 11000011 & 00000011 = 00000011
    newData = statusData & BIT_GENFLAG_MASK;
    return newData;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-14
 * Desc:设置通用标记值
 * @index:索引
 * @val:输入的数据
 * @return : 1:成功 0：失败
 */

u8 eefs_mbr_setGenFlag(u16 index , u8 val)
{
    // ---------- 局部变量定义区---------- //
    u8 statusData;      // 状态位数据
    u8 newData;         // 1,2位清零后的数据
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    if (val > 3) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)获取状态位 11000011
    statusData = eefs_mbr_getStatus(index);
    //(2)将1,2位清零:11000011 & 11111100 = 11000000
    newData = statusData & BIT_GENFLAG_UNMASK;
    //(3)进行或运算赋值: 11000000 | 00000011 = 11000011
    newData |= val;
    //(4)写入数据
    eefs_mbr_setStatus(index, newData);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:更新索引
 * @index:索引
 * @name:名字
 * @status:状态
 * @return : 1:成功 0：失败
 */
u8 eefs_mbr_update(u16 index, u32 name, u8 status)
{
    // ---------- 局部变量定义区---------- //
    
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    if (eefs_mbr_getIndexStatus(index) == 0x00) {
        eefs_mbr_setIndexStatus(index, 2);
    }
    if (eefs_mbr_getIndexStatus(index) == 0x02) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)更新名字和状态
    if (eefs_mbr_setName(index, name) == RET_SUCCESS && eefs_mbr_setStatus(index, status) == RET_SUCCESS) {
        return RET_SUCCESS;
    }
    else
    {
        return RET_FAILD;
    }
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:删除索引
 * @index:索引
 * @return : 1:成功 0：失败
 */
u8 eefs_mbr_delete(u16 index)
{
    // ---------- 局部变量定义区---------- //
    
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    if (eefs_mbr_getIndexStatus(index) == 0x00) {
        eefs_mbr_setIndexStatus(index, 2);
    }
    if (eefs_mbr_getIndexStatus(index) == 0x02) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)将indexStatus变为2
    eefs_mbr_setIndexStatus(index, 2);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:重置索引数据
 * @index:索引
 * @return : 1:成功 0：失败
 */
u8 eefs_mbr_reset(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u16 startAddress; // 起始索引位置
    int i;
    u8 data = 0x00;
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    if (eefs_mbr_getIndexStatus(index) == 0x00) {
        eefs_mbr_setIndexStatus(index, 2);
    }
    if (eefs_mbr_getIndexStatus(index) == 0x02) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)找到起始索引位置
    startAddress = getIndexAddress(index);
    
    //(2)循环赋0
    for (i = 0; i < INDEX_SIZE; i++) {
        eefs_base_writeByte(startAddress + i,&data);
    }
    return RET_SUCCESS;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取索引区名字
 * @index:索引
 * @return u32 名字
 */
u32 eefs_mbr_getName(u16 index) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u32 name;                // 索引的name信息
	u8 names[NAME_SIZE];
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到name的位置
	startIndex = eefs_mbr_getIndexHeadAddress(index);
	// (2).读取名字的四字节
	eefs_base_readBytes(startIndex, names, NAME_SIZE);
	name = 0;
	name = *(u32*)names; //赋值name
	return name;
}   

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:设置索引区名字
 * @index:索引
 * @return : 1:成功 0：失败
 */
u8 eefs_mbr_setName(u16 index, u32 name) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u8 names[NAME_SIZE];			//临时names
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到name的位置
	startIndex = eefs_mbr_getIndexHeadAddress(index);
	// (2).读取名字的四字节
	memcpy(names, (u8*)&name, NAME_SIZE);
	//writeByte(startIndex, names, NAME_SIZE);
	eefs_base_writeBytes(startIndex, names, NAME_SIZE);
	return RET_SUCCESS;
}           


/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取索引区address
 * @index:索引
 * @return u16 地址
 */
u16 eefs_mbr_getAddress(u16 index) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u16 address;                // 索引的address信息
	u8 addrs[ADDR_SIZE];
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引中找到address的位置
	startIndex = eefs_mbr_getIndexAddressHeadAddress(index);
	// (2).读取地址的2字节
	eefs_base_readBytes(startIndex, addrs, ADDR_SIZE);
	address = 0;
	address = *(u16*)addrs; //赋值address
	return address;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:设置索引区地址
 * @index:索引
 * @return : 1:成功 0：失败
 */
u8 eefs_mbr_setAddress(u16 index, u16 address) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u8 addrs[ADDR_SIZE];			//临时addrs
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到address的位置
	startIndex = eefs_mbr_getIndexAddressHeadAddress(index);
	// (2).读取address的2字节
	memcpy(addrs, (u8*)& address, ADDR_SIZE);
	//writeByte(startIndex, addrs, ADDR_SIZE);
	eefs_base_writeBytes(startIndex, addrs, ADDR_SIZE);
	return RET_SUCCESS;
}



/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取索节点首地址
 * @index:索引
 * @return : u16 地址
 */
u16 eefs_mbr_getIndexHeadAddress(u16 index) {
	return EE_START_INDEX + index * INDEX_SIZE;
}
/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取索引节点name首地址
 * @index:索引
 * @return : u16 地址
 */
u16 eefs_mbr_getIndexNameHeadAddress(u16 index) {
	return eefs_mbr_getIndexAddressHeadAddress(index);
}   
/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取索引节点address首地址
 * @index:索引
 * @return : u16 地址
 */
u16 eefs_mbr_getIndexAddressHeadAddress(u16 index) {
	return eefs_mbr_getIndexHeadAddress(index) + ADDR_OFFSET;
}  
/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取索引节点size首地址
 * @index:索引
 * @return : u16 地址
 */
u16 eefs_mbr_getIndexSizeHeadAddress(u16 index) {
	return eefs_mbr_getIndexHeadAddress(index) + SIZE_OFFSET;
}   
/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取索引节点status首地址
 * @index:索引
 * @return : u16 地址
 */
u16 eefs_mbr_getIndexStatusHeadAddress(u16 index) {
	return eefs_mbr_getIndexHeadAddress(index) + STATUS_OFFSET;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取数据节点desc首地址
 * @index:索引
 * @return : u16 地址
 */
u16 eefs_data_getDescHeadAddress(u16 index) {
   return eefs_mbr_getAddress(index) + eefs_mbr_getSize(index);
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取数据节点desc
 * @index:索引
 * @return : u16 数据区描述
 */
u16 eefs_data_getDesc(u16 index) {
	// ---------- 局部变量定义区---------- //
	int i;
	u16 startIndex;         // 该索引的起始位置
	u16 desc;                // 索引的name信息
	u8 descs[DESC_SIZE];
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到name的位置
	startIndex = eefs_data_getDescHeadAddress(index);
	// (2).读取desc的2字节
	eefs_base_readBytes(startIndex, descs, DESC_SIZE);
	desc = 0;
	desc = *(u16*)descs; //赋值desc
	return desc;
} 
/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:设置数据节点desc
 * @index:索引
 * @desc：描述
 * @return : 1 成功
 */
u8 eefs_data_setDesc(u16 index, u16 desc) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u8 descs[DESC_SIZE];			//临时names
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}
	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到desc的位置
	startIndex = eefs_data_getDescHeadAddress(index);
	// (2).读取描述的2字节
	memcpy(descs, (u8*)& desc, DESC_SIZE);
	//writeByte(startIndex, descs, DESC_SIZE);
	//eefs_base_writeBytes(startIndex, descs, DESC_SIZE);
	return RET_SUCCESS;
}
  
/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:设置数据节点desc高位
 * @index:索引
 * @desc：描述
 * @return : 1 成功
 */
u8 eefs_data_setDescHigh(u16 index, u8 value) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该数据的起始位置
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}
	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到desc高位的位置
	startIndex = eefs_data_getDescHeadAddress(index)+ DESC_HIGH_SIZE;
	// (2).写入描述的高位
	//writeByte(startIndex, &value, 1);
	eefs_base_writeByte(startIndex, &value);
	return RET_SUCCESS;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取数据节点desc高位
 * @index:索引
 * @return : u8 数据区描述高位
 */
u8 eefs_data_getDescHigh(u16 index) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u8 descHigh;                // 索引的name信息
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到描述高位的位置
	startIndex = eefs_data_getDescHeadAddress(index)+ DESC_HIGH_SIZE;
	// (2).读取描述高位的1字节
	descHigh = eefs_base_readByte(startIndex);
	return descHigh;
}
/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取数据节点desc低位
 * @index:索引
 * @return : u8 数据区描述低位
 */
u8 eefs_data_getDescLow(u16 index) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u8 descLow;                // 索引的name信息
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到描述低位的位置
	startIndex = eefs_data_getDescHeadAddress(index)+ DESC_LOW_SIZE;
	// (2).读取描述低位的1字节
	descLow = eefs_base_readByte(startIndex);
	return descLow;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:设置数据节点desc低位
 * @index:索引
 * @desc：描述
 * @return : 1 成功
 */
u8 eefs_data_setDescLow(u16 index, u8 value) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该数据的起始位置
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}
	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到desc低位的位置
	startIndex = eefs_data_getDescHeadAddress(index)+ DESC_LOW_SIZE;
	// (2).写入描述的低位
	//writeByte(startIndex, &value, 1);
	eefs_base_writeByte(startIndex, &value);
	return RET_SUCCESS;
}


/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取节点数据的数据区首地址
 * @index:索引
 * @return : 1:成功 0：失败
 */
u16 eefs_data_getHeadAddr(u16 index)
{
	// ---------- 局部变量定义区---------- //
	u16 address;
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
	//(1)获取节点数据区的地址数据
	address = eefs_mbr_getAddress(index);
	return address;
}
/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取节点数据的数据区尾地址
 * @index:索引
 * @return : 1:成功 0：失败
 */
u16 eefs_data_getTailAddr(u16 index)
{
	// ---------- 局部变量定义区---------- //
	s16 startAddress;
	s16 endAddress = 0;
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_FAILD;
	}
	// ---------- 业务处理---------- //
	//(1)获取节点数据区的首地址
	startAddress = eefs_mbr_getAddress(index);
	endAddress = startAddress + eefs_mbr_getSize(index);
	return endAddress;
}
/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取数据区总容量
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u16 eefs_data_getTotalCapacity(void)
{
	return EE_MAX_CAPACITY - TEMP_ADDRESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取已使用总容量
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u16 eefs_data_getUsedCapacity(void)
{
	// ---------- 局部变量定义区---------- //
	int i;
	u16 capacity = 0;   // 总容量
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	//(1)遍历索引区, 取出size相加
	for (i = 0; i < MAX_INDEX; i++) {
		capacity += eefs_mbr_getSize(i);
	}
	return capacity;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取索引对应的数据区大小
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u16 eefs_mbr_getSize(u16 index)
{
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u16 size;                // 索引的name信息
	u8 sizes[SIZE_SIZE];
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到size的位置
	startIndex = eefs_mbr_getIndexSizeHeadAddress(index);
	// (2).读取size的两字节
	eefs_base_readBytes(startIndex, sizes, SIZE_SIZE);
	size = 0;
	size = *(u16*)sizes + DATA_DESCRIBE; //赋值name
	return size;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取未使用空间总容量
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u16 eefs_data_getUnusedCapacity(void)
{
	// ---------- 局部变量定义区---------- //
	u16 capacity;   //数据区总容量
	u16 usedCapacity;   // 已使用数据区容量
	u16 unUsedCapacity; // 未使用数据区容量
	// ---------- 输入参数条件检测---------- //
	// ---------- 业务处理---------- //
	//(1)获取数据区总容量
	capacity = eefs_data_getTotalCapacity();
	//(2)获取已使用的容量
	usedCapacity = eefs_data_getUsedCapacity();
	//(3)总容量 - 已使用的容量
	unUsedCapacity = capacity - usedCapacity;
	return unUsedCapacity;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:返回可以使用的一个合适的地址
 * @size:数据大小
 * @return : 1:成功 0：失败
 */
u16 eefs_data_findUnusedAddr(u16 size)
{
	// ---------- 局部变量定义区---------- //
	int i;
	int n;
	int j;
    int ii, jj;
    DATAStRUCT temp;
	DATAStRUCT data;
    u16 effectiveAddress;   // 有效地址
	u16 nextAddress;        // 下一个有效地址
	DATAStRUCT dataList[MAX_INDEX];    // 结构体数组
	// ---------- 输入参数条件检测---------- //
	if (eefs_data_getUsedCapacity() == 0) { // 如果已使用空间为0
		return EE_START_DATA;
	}
	// ---------- 业务处理---------- //
	//(1)遍历索引, 获取所有address有效的首地址
    printf("%s", G_LIST);
    n = 0;
    effectiveAddress = EE_START_DATA;
	for (i = 0; i < MAX_INDEX; i++) {
		if (isEffectiveAddress(eefs_data_getHeadAddr(i)) == 1) { // 有效
			data.address = eefs_data_getHeadAddr(i);
			data.index = i;
			dataList[n] = data;
			n++;
		}
        else
        {
            data.address = 0x00;
            data.index = i;
            dataList[n] = data;
            n++;
        }
	}
	//(2)将数组从小到大排序
	jj = 0;
	for (ii = 0; ii < MAX_INDEX - 1; ii++) {

		for (jj = ii + 1; jj < MAX_INDEX; jj++) {

			if (dataList[jj].address < dataList[ii].address) {    //如果后一个元素小于前一个元素则交换

				temp = dataList[ii];

				dataList[ii] = dataList[jj];

				dataList[jj] = temp;

			}
		}
	}
	//(3)遍历地址数组
	for (j = 0; j < MAX_INDEX; j++) {
		if (dataList[j].address < effectiveAddress) {    // 第一个如果为0, 则跳出循环
            continue;      // 返回起始位置
		}
		else
		{
            nextAddress = dataList[j].address;
            if (nextAddress - effectiveAddress >= size) { // 空间够
                return effectiveAddress;
            }
            else
            {
                effectiveAddress = eefs_data_getTailAddr(dataList[j].index);
            }
		}
	}
    if (effectiveAddress + size < EE_MAX_CAPACITY) {
        return effectiveAddress;
    }
    return RET_FAILD;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:判断有效地址
 * @address:地址
 * @return : 1:有效 0：无效
 */
u16 isEffectiveAddress(u16 address)
{
	if (address < EE_START_DATA || address > EE_MAX_CAPACITY) {
		return 0;
	}
	else
	{
		return 1;
	}
}

u8 eefs_data_create(u16 addr, u16 size); //创建数据区，并初始化
u8 eefs_data_update(u16 addr, u16 size); //更新数据区全部内容

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:创建（申请）多个数据空间
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 eefs_createAll(USERNODE list[], u8 len)
{
    // ---------- 局部变量定义区---------- //
    int i;
    int j;
    j = 0;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)循环遍历数组取出结构体
    for (i = 0; i < MAX_INDEX; i++) {
        if (eefs_mbr_getIndexStatus(i) == 0x00) {
            eefs_mbr_setIndexStatus(i, 2);
        }
        if (eefs_mbr_getIndexStatus(i) == 2) { // 如果索引状态为2, 可以使用
            USERNODE node = list[j];
            eefs_create(i, node);
            if (j == len - 1) {
                return RET_SUCCESS;
            }
            j++;
        }
    }
    return RET_FAILD;
}
/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:创建1个
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 eefs_create(u16 index, USERNODE node)
{
    // ---------- 局部变量定义区---------- //
    int i;
    // ---------- 输入参数条件检测---------- //
    // (1)判断index是否合法
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    // (2)判断node size
    // TODO:该处判断应该用剩余空间容量函数的返回值进行判断
    if (node.size > EE_MAX_CAPACITY - EE_SYS_CAPACITY) {
        return ERR_INVALIDPARAM;
    }
    // ---------- 业务处理---------- //
    //(1)循环判断是否已经包含name
    for (i = 0; i < MAX_INDEX; i++) {
        if (node.name == eefs_mbr_getName(i)) {
            return RET_FAILD;
        }
    }
    //(2)创建索引
    eefs_mbr_create(index, node);
    return RET_SUCCESS;
}
/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:删除（释放）一个索引空间
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 eefs_delete(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u32 name;
    u16 address;
    u16 size;
    u8 status;
    name = 0;
    address = 0;
    size = 0;
    status = 0x20;
    // ---------- 输入参数条件检测---------- //
    // (1)判断index是否合法
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)将索引空间赋为初始值
    eefs_mbr_setName(index, name);
    eefs_mbr_setAddress(index, address);
    eefs_mbr_setSize(index, size);
    eefs_mbr_setStatus(index, status);
    eefs_reset(index);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:设置空间大小
 * @index:索引
 * @size:空间
 * @return : 1:成功 0：失败
 */
u8 eefs_mbr_setSize(u16 index, u16 size)
{
    // ---------- 局部变量定义区---------- //
    u16 startIndex;         // 该索引的起始位置
    u8 sizes[SIZE_SIZE];            //临时sizes
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_ERROR;
    }
    
    // ---------- 业务处理---------- //
    // (1). 找到索引起始位置 找到address的位置
    startIndex = eefs_mbr_getIndexSizeHeadAddress(index);
    // (2).读取address的2字节
    memcpy(sizes, (u8*)& size, SIZE_SIZE);
    //writeByte(startIndex, addrs, ADDR_SIZE);
    eefs_base_writeBytes(startIndex, sizes, SIZE_SIZE);
    return RET_SUCCESS;
}
/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:删除所有索引空间
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 eefs_deleteAll(void)
{
    // ---------- 局部变量定义区---------- //
    int i;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)循环删除所有索引空间
    for (i = 0; i < MAX_INDEX; i++) {
        eefs_delete(i);
    }
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:将指定1个数据的空间清零
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */

u8 eefs_reset(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u16 firstAddress;
    u16 size;
    u8 data;
    // ---------- 输入参数条件检测---------- //
    // (1)判断index是否合法
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)根据索引找到数据空间首地址
    firstAddress = eefs_data_getHeadAddr(index);
    //(2)根据索引获取数据size
    size = eefs_mbr_getSize(index);
    //(3)将这段空间清空
    //在指定位置写入dataLen个字节
    data = 0x00;
    eefs_base_writeBytes(firstAddress, &data, size);
    return RET_SUCCESS;
}
/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:将所有数据空间清零
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 eefs_resetAll(void)
{
    // ---------- 局部变量定义区---------- //
    u8 data;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)给所有空间赋0
    //在指定位置写入dataLen个字节
    data = 0x00;
    eefs_base_writeBytes(EE_START_DATA, &data, EE_MAX_CAPACITY - EE_START_DATA);
    return RET_SUCCESS;
}
u8 eefs_init(void);        //初始化全部空间

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:设置数据区全部内容
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 eefs_setValue(u32 name, u8 *data, u16 len)
{
    // ---------- 局部变量定义区---------- //
    
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)偏移量为0
    eefs_setValueWithOffset(name, 0, data, len);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取数据区全部内容
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 eefs_getValue(u32 name, u8 *ret_data, u16 *len)
{
    eefs_getValueWithOffset(name, 0, ret_data, len);
    return RET_SUCCESS;
}
/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:根据偏移量设置数据区全部内容
 * @name:名字
 * @offset:偏移量
 * @data:写入的数据
 * @len:写入的长度
 * @return : 1:成功 0：失败
 */
u8 eefs_setValueWithOffset(u32 name, u16 offset,u8 *data, u16 len)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u16 address;
    u16 size;
    // ---------- 输入参数条件检测---------- //
    // ---------- 业务处理---------- //
    //(1)循环获取name对应的index, address
    for (i = 0; i < MAX_INDEX; i++) {
        if (name == eefs_mbr_getName(i)) {
            address = eefs_mbr_getAddress(i);
            size = eefs_mbr_getSize(i);
            if (offset + len > size) {
                return RET_FAILD;
            }
            //(2)写入数据
            eefs_base_writeBytes(address + offset, data, len);
            return RET_SUCCESS;
        }
    }
    return RET_FAILD;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:根据偏移量获取数据区全部内容
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 eefs_getValueWithOffset(u32 name, u16 offset, u8 *ret_data, u16 len)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u16 address;
    u16 size;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)找到name对应的index, size
    for (i = 0; i < MAX_INDEX; i++) {
        if (name == eefs_mbr_getName(i)) {
            address = eefs_mbr_getAddress(i);
            size = eefs_mbr_getSize(i);
            if (offset + len > size) {
                return RET_FAILD;
            }
            //(2)读取数据
            eefs_base_readBytes(address + offset, ret_data, len);
            return RET_SUCCESS;
        }
    }
    return RET_FAILD;
    
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:系统保留区标志位读取
 * @return u16 信息
 */
u8 eefs_sys_getFlag() {
	// ---------- 局部变量定义区---------- //
	u16 flagOffset;       // 系统描述区的Flag的位置
	s8 data;                // 系统描述区的Flag信息
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	// (1). 找到SYS起始位置

	// (2). 找到FLAG的位置
	flagOffset = EE_START_SYS + EE_SYS_FLAG_OFFSET;
	// (3). 读数据
	data = eefs_base_readByte(flagOffset);
	return data;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:系统保留区标志位写入
 * @value:信息
 * @return : 1:成功
 */

u8 eefs_sys_setFlag(u8 value) {
	// ---------- 局部变量定义区---------- //
	u16 flagOffset;       // 系统描述区的Flag的位置
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	// (1). 找到SYS起始位置

	// (2). 找到FLAG的位置
	flagOffset = EE_START_SYS + EE_SYS_FLAG_OFFSET;
	// (3). 读数据
	eefs_base_writeByte(flagOffset, &value);
	return RET_SUCCESS;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:取得写入索引版本
 * @return u16 信息
 */
u8 eefs_sys_getVersion() {
	// ---------- 局部变量定义区---------- //
	u16 flagOffset;       // 系统描述区的Version的位置
	s8 data;                // 系统描述区的Version信息
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	// (1). 找到SYS起始位置

	// (2). 找到Version的位置
	flagOffset = EE_START_SYS + EE_SYS_VERSION_OFFSET;
	// (3). 读数据
	data = eefs_base_readByte(flagOffset);
	return data;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:系统保留区版本号写入
 * @return : 1:成功
 */

u8 eefs_sys_setVersion() {
	// ---------- 局部变量定义区---------- //
	u16 flagOffset;       // 系统描述区的Flag的位置
	u8 lastVersion;
	u8 nowVersion;
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	// (1). 找到SYS起始位置

	// (2). 找到FLAG的位置
	flagOffset = EE_START_SYS + EE_SYS_VERSION_OFFSET;
	// (3). 获取上个版本
	lastVersion = eefs_sys_getVersion();
	// (4). 生成新版本号
	nowVersion = lastVersion + 1;
	// (3). 写入数据
	eefs_base_writeByte(flagOffset, &nowVersion);
	return RET_SUCCESS;
}


/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取系统描述区已使用空间信息
 * @return u16 信息
 */
u16 eefs_sys_getUsedCapacity() {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该标识的起始位置
	u16 data;                // 系统的已使用空间信息
	u8 datas[USEDCAPACITY_SIZE];
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	// (1). 找到系统描述区中找到已使用空间标识的位置
	startIndex = EE_START_SYS + EE_SYS_USEDCAPACITY_OFFSET;
	// (2).读取地址后的2字节
	eefs_base_readBytes(startIndex, datas, USEDCAPACITY_SIZE);
	data = 0;
	data = *(u16*)datas;
	return data;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:设置系统描述区已使用空间信息
 * @size:信息
 * @return : 1:成功
 */
u8 eefs_sys_setUsedCapacity(u16 size) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u8 datas[USEDCAPACITY_SIZE];			//临时datas
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	// (1). 找到系统描述区中找到已使用空间标识的位置
	startIndex = EE_START_SYS + EE_SYS_USEDCAPACITY_OFFSET;
	// (2).读取地址后的2字节
	memcpy(datas, (u8*)& size, USEDCAPACITY_SIZE);
	// (3).写入数据
	eefs_base_writeBytes(startIndex, datas, USEDCAPACITY_SIZE);
	return RET_SUCCESS;
}


/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取系统描述区未使用空间信息
 * @return u16 信息
 */
u16 eefs_sys_getUnusedCapacity() {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该标识的起始位置
	u16 data;                // 系统的未使用空间信息
	u8 datas[USEDCAPACITY_SIZE];
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	// (1). 找到系统描述区中找到未使用空间标识的位置
	startIndex = EE_START_SYS + EE_SYS_USEDCAPACITY_OFFSET;
	// (2).读取地址后的2字节
	eefs_base_readBytes(startIndex, datas, USEDCAPACITY_SIZE);
	data = 0;
	data = *(u16*)datas;
	return data;
}

/*
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:设置系统描述区未使用空间信息
 * @size:信息
 * @return : 1:成功
 */
u8 eefs_sys_setUnusedCapacity(u16 size) {
	// ---------- 局部变量定义区---------- //
	u16 startIndex;         // 该索引的起始位置
	u8 datas[UNUSEDCAPACITY_SIZE];			//临时datas
	// ---------- 输入参数条件检测---------- //

	// ---------- 业务处理---------- //
	// (1). 找到系统描述区中找到已使用空间标识的位置
	startIndex = EE_START_SYS + EE_SYS_UNUSEDCAPACITY_OFFSET;
	// (2).读取地址后的2字节
	memcpy(datas, (u8*)& size, UNUSEDCAPACITY_SIZE);
	// (3).写入数据
	eefs_base_writeBytes(startIndex, datas, UNUSEDCAPACITY_SIZE);
	return RET_SUCCESS;
}
