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
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:写入数据
 * @address:写入地址
 * @value:写入值
 * @len:写入长度
 * @return : 1:成功
 */

int writeByte(u16 address ,u8 *value, u16 len)
{
    memcpy(G_LIST + address, value, len);
    return 1;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:读数据
 * @address:读取地址
 * @return : 读出的数据
 */

unsigned char readByte(int address)
{
    unsigned char value = *(G_LIST + address);
    return value;
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
    u16 startIndex;         // 该索引的起始位置
    u16 statusOffset;       // status的位置
    s8 data;                // 索引的status信息
    // ---------- 输入参数条件检测---------- //
    if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
        return RET_ERROR;
    }
    // ---------- 业务处理---------- //
    // (1). 找到索引起始位置
    startIndex = EE_START_INDEX + index * INDEX_SIZE;
    // (2). 找到status的位置
    statusOffset = startIndex + STATUS_OFFSET;
    // (3). 读数据
    data = readByte(statusOffset);
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
    address = getAddress();
    if (eefs_mbr_CheckAddress(address) != RET_SUCCESS) {
        return RET_FAILD;
    }
    //(2)获取索引的位置
    indexAddress = getIndexAddress(index);
    //(3)给node结构体赋值
    node.name = userNode.name;
    node.address = address;
    node.size = userNode.size;
    node.status = 0x40;
    //(4)写入数据到索引区, 返回成功
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

u16 getAddress(void)
{
    return TEMP_ADDRESS;
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
    writeByte(myAddress, data, INDEX_SIZE);
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
    writeByte(startStatus, &val, 1);
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
 * Auth:张添程
 * Date: 2019-5-14
 * Desc:获取索引区名字
 * @index:索引
 * @return u32 名字
 */

u32 eefs_mbr_getName(u16 index) {
	// ---------- 局部变量定义区---------- //
	int i;
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
	for (i = 0; i < NAME_SIZE; i++)
	{
		//写入临时数组
		names[i] = readByte(startIndex+i);
	}
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
	writeByte(startIndex, names, NAME_SIZE);
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
	int i;
	u16 startIndex;         // 该索引的起始位置
	u16 address;                // 索引的address信息
	u8 addrs[ADDR_SIZE];
	// ---------- 输入参数条件检测---------- //
	if (eefs_mbr_CheckIndex(index) != RET_SUCCESS) {
		return RET_ERROR;
	}

	// ---------- 业务处理---------- //
	// (1). 找到索引起始位置 找到address的位置
	startIndex = eefs_mbr_getIndexAddressHeadAddress(index);
	// (2).读取地址的2字节
	for (i = 0; i < ADDR_SIZE; i++)
	{
		//写入临时数组
		addrs[i] = readByte(startIndex + i);
	}
	address = 0;
	address = *(u32*)addrs; //赋值address
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
	writeByte(startIndex, addrs, ADDR_SIZE);
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
	return eefs_mbr_getIndexAddressHeadAddress;
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
