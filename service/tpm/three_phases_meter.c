#include <stdio.h>
#include <string.h>
#include "eeprom/eefs_lib.h"
#include "service/base/meter_base.h"
#include "service/tpm/three_phases_meter.h"

/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 掉电标志,5字节，不带crc，存1份
 * @data 数据接收
 * @return : 0,1
 */
u8 service_tpm_getNoPowerFlag(u8* data) {
	
 return meter_circle_read(NOPOWERFLAG_INDEX, data);
}	
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 掉电标志,5字节，不带crc，存1份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setNoPowerFalg(u8* data, u16 len) {
	return meter_circle_write(NOPOWERFLAG_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 发送参数，24字节，不带crc，存1份
 * @data 数据接收
 * @return : 0,1
 */
u8 service_tpm_getSendParameter(u8* data) {
	return meter_circle_read(SENDPARAMETER_INDEX, data);
} 
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 发送参数，24字节，不带crc，存1份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setSendParameter(u8* data, u16 len) {
	return meter_circle_write(SENDPARAMETER_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 升级参数，415字节，不带crc，存1份
 * @data 数据
 * @return : 0,1
 */
u8 service_tpm_getUpgrade(u8* data) {
	return meter_circle_read(UPGRADE_INDEX, data);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 升级参数，415字节，不带crc，存1份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setUpgrade(u8* data, u16 len) {
	return meter_circle_write(UPGRADE_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 首次判断，4字节，不带crc，存1份
 * @data 数据
 * @return : 0,1
 */
u8 service_tpm_getFirstJudge(u8* data) {
	return meter_circle_read(FIRSTJUDGE_INDEX, data);
} 
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 首次判断，4字节，不带crc，存1份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setFirstJudge(u8* data, u16 len) {
	return meter_circle_write(FIRSTJUDGE_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 电能，160字节带crc，存4份
 * @data 数据
 * @return : 0,1
 */
u8 service_tpm_getEnergy(u8* data) {
	return meter_circle_read(ENERGY_INDEX, data);
} 
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 电能，160字节带crc，存4份
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setEnergy(u8* data, u16 len) {
	return meter_circle_write(ENERGY_INDEX, data, len);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:获取 校表参数 78字节 1份 不带crc
 * @data 数据
 * @return : 0,1
 */
u8 service_tpm_getCheckMeter(u8* data) {
	return meter_circle_read(CHECKMETER_INDEX, data);
}
/*
 * Auth: 张添程
 * Date: 2019-5-21
 * Desc:写入 校表参数 78字节 1份 不带crc
 * @data 数据
 * @len 长度
 * @return : 0,1
 */
u8 service_tpm_setCheckMeter(u8* data, u16 len) {
	return meter_circle_write(CHECKMETER_INDEX, data, len);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:创建断网数据空间
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_create_breakeNetCapacity(void)
{
    // ---------- 局部变量定义区---------- //
    int i;
    USERNODE usernode;
    s8 indexStatus;
    u8 isSuccess;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)循环遍历索引区, 找到一个没使用的索引
    usernode.name = SMALLINDEX_NAME;
    usernode.size = (SMALLINDEX_CAPACITY + SMALLINDEX_STATUSCAPACITY + SMALLINDEX_CRCCAPACITY) * SMALLINDEX_DATACOUNT;
    for (i = 0; i < MAX_INDEX; i++) {
        indexStatus = eefs_mbr_getIndexStatus(i);
        if (indexStatus == RET_ERROR) {
            return RET_FAILD;
        }
        if (indexStatus == ZERO_INDEXSTATUS) {
            isSuccess = eefs_mbr_setIndexStatus(i, DEFAULT_INDEXSTATUS);
            if (isSuccess == RET_FAILD) {
                return RET_FAILD;
            }
        }
        if (indexStatus == DEFAULT_INDEXSTATUS) {
            eefs_create(i, usernode); // 创建索引区
            i = MAX_INDEX;
        }
    }
    
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:存一次断网数据
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_saveOnceBreakeNetData(u16 index, SMALLINDEXNODE smallNode)
{
    // ---------- 局部变量定义区---------- //
    u16 address;     // 真正的地址
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    address = 0;
    //(1)根据小索引算出真正的地址
    address = service_tpm_getBreakeNetDataAddress(index);
    if (address == RET_FAILD) {
        return RET_FAILD;
    }
    //(2)在newAddress处存数据
    eefs_base_writeBytes(address, (u8*)& smallNode, SMALLINDEX_CAPACITY);
    
    //(4)写入状态位(7,8位=1为当前数据, 0为以前数据, 1234位控制年月日时分, 5,6位控制发送状态,2为默认状态, 1为发送状态)
    // 写入当前状态
    if (service_tpm_setSmallIndexCurrentStatus(index, SMALLINDEX_CURRENTSTATUS) == RET_FAILD) {
        return RET_FAILD;
    }
    // 写入默认发送状态
    if (service_tpm_setSmallIndexSendStatus(index, SMALLINDEX_NOSENDSTATUS) == RET_FAILD) {
        return RET_FAILD;
    }
    if (service_tpm_setSmallIndexSendStatus(index, SMALLINDEX_NOSENDSTATUS) == RET_FAILD) {
        return RET_FAILD;
    }
    
    // 给上一个index更新状态
    if (index == 0) {
        if (service_tpm_setSmallIndexCurrentStatus(SMALLINDEX_DATACOUNT - 1, SMALLINDEX_DEFAULTSTATUS) == RET_FAILD) {
            return RET_FAILD;
        }
    }
    else
    {
        if (service_tpm_setSmallIndexCurrentStatus(index - 1, SMALLINDEX_DEFAULTSTATUS) == RET_FAILD) {
            return RET_FAILD;
        }
    }
    // 时间状态
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:保存断网数据
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_saveBreakeNetData(SMALLINDEXNODE node)
{
    // ---------- 局部变量定义区---------- //
    int j;
    u8 result;
    u8 saveResult;
    // ---------- 输入参数条件检测---------- //
    // ---------- 业务处理---------- //
    //(1)遍历小索引区, 找到status的7,8位为1的小索引
    for (j = 0; j < SMALLINDEX_DATACOUNT; j++) {
        result = service_tpm_getSmallIndexCurrentStatus(j);
        if (result == RET_FAILD) {
            return RET_FAILD;
        }
        if (result == SMALLINDEX_CURRENTSTATUS) {  // 是当前发送状态
            if (j == SMALLINDEX_DATACOUNT - 1) {// 如果是最后一个, 保存到0
                
                saveResult = service_tpm_saveOnceBreakeNetData(0, node);
                if (saveResult == RET_FAILD) {
                    return RET_FAILD;
                }
                return RET_SUCCESS;
            }
            else // 否则保存到下一个
            {
                saveResult = service_tpm_saveOnceBreakeNetData(j + 1, node);
                if (saveResult == RET_FAILD) {
                    return RET_FAILD;
                }
                return RET_SUCCESS;
            }
        }
    }
    saveResult = service_tpm_saveOnceBreakeNetData(0, node);
    if (saveResult == RET_FAILD) {
        return RET_FAILD;
    }
    return RET_SUCCESS;
    
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取小索引当前状态
 * @index:索引
 * @return : u8 status
 */
u8 service_tpm_getSmallIndexCurrentStatus(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u16 statusAddress;      // status在数据区的位置
    u16 smallIndexAddress;  // 小索引在数据区的位置
    u8 status;              // status值
    u8 newStatus;           // 右移6位后的值
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)小索引在数据区的位置
    smallIndexAddress = service_tpm_getBreakeNetDataAddress(index);
    if (smallIndexAddress == RET_FAILD) {
        return RET_FAILD;
    }
    //(2)找到status的7,8位
    statusAddress =  smallIndexAddress + SMALLINDEX_CAPACITY;
    
    status = eefs_base_readByte(statusAddress);
    //(3)将除了7,8位的其他位清零
    newStatus = status & BIT_DATA_MASK;
    newStatus = newStatus >> 6;
    return newStatus;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取小索引发送状态
 * @index:索引
 * @return : u8 status
 */
u8 service_tpm_getSmallIndexSendStatus(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u16 statusAddress;      // status在数据区的位置
    u16 smallIndexAddress;  // 小索引在数据区的位置
    u8 status;              // status值
    u8 newStatus;           // 右移4位后的值
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)小索引在数据区的位置
    smallIndexAddress = service_tpm_getBreakeNetDataAddress(index);
    if (smallIndexAddress == RET_FAILD) {
        return RET_FAILD;
    }
    //(2)找到status的5,6位
    statusAddress = smallIndexAddress + SMALLINDEX_CAPACITY;
    status = eefs_base_readByte(statusAddress);
    //(3)将除了5,6位的其他位清零
    newStatus = status & BIT_INDEX_MASK;
    newStatus = newStatus >> 4;
    return newStatus;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取小索引1234位的时间状态
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_getSmallIndexTimeStatus(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u16 statusAddress;      // status在数据区的位置
    u16 smallIndexAddress;  // 小索引在数据区的位置
    u8 status;              // status值
    u8 newStatus;           // 右移后的值
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)小索引在数据区的位置
    smallIndexAddress = service_tpm_getBreakeNetDataAddress(index);
    if (smallIndexAddress == RET_FAILD) {
        return RET_FAILD;
    }
    //(2)找到status的5,6位
    statusAddress = smallIndexAddress + SMALLINDEX_CAPACITY;
    status = eefs_base_readByte(statusAddress);
    //(3)将除了1234位的其他位清零
    newStatus = status & BIT_TIME_MASK;
    return newStatus;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获得小索引在数据区对应的地址
 * @index:索引
 * @return : u16 地址
 */
u16 service_tpm_getBreakeNetDataAddress(u16 index)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u16 address;
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    address = 0;
    for (i = 0; i < MAX_INDEX; i++) {
        if (eefs_mbr_getName(i) == SMALLINDEX_NAME) {
            address = eefs_mbr_getAddress(i);
            if (address == RET_FAILD) {
                return RET_FAILD;
            }
        }
    }
    address = address + index * SMALLINDEX_CAPACITY + index * SMALLINDEX_STATUSCAPACITY + index * SMALLINDEX_CRCCAPACITY;
    return address;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:返回最新的断网数据
 * @paramName:xxxxx
 * @return : u16 断网数据
 */
u8 service_tpm_disconnect_getData(u8 * retData)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u8 status;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)循环获取小索引数据状态
    for (i = 0; i < SMALLINDEX_DATACOUNT; i++) {
        status = service_tpm_getSmallIndexCurrentStatus(i);
        if (status == RET_FAILD) {
            return RET_FAILD;
        }
        if (status == SMALLINDEX_CURRENTSTATUS) { // 最新数据
            service_tpm_disconnect_getDataWithIndex(i, retData);
        }
    }
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:根据索引返回断网数据
 * @index:索引
 * @return : u16 断网数据
 */
u8 service_tpm_disconnect_getDataWithIndex(u16 index, u8 * retData)
{
    // ---------- 局部变量定义区---------- //
    u16 address;
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)获取输入值对应的数据区位置
    address = service_tpm_getBreakeNetDataAddress(index);
    if (address == RET_FAILD) {
        return RET_FAILD;
    }
    eefs_base_readBytes(address, retData, SMALLINDEX_CAPACITY);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:根据角标设置5,6位发送状态
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_setSmallIndexSendStatus(u16 index, u8 val)
{
    // ---------- 局部变量定义区---------- //
    u16 statusAddress;      // status在数据区的位置
    u8 status;              // status值
    u8 newStatus;           // 清零后的值
    u8 newVal;
    u8 data;
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    if (val > 3) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)获取数据首地址
    statusAddress = service_tpm_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
    if (statusAddress == RET_FAILD) {
        return RET_FAILD;
    }
    status = eefs_base_readByte(statusAddress);
    //(2)&运算, 把五六位清零: 11110000 & 11001111 = 11000000
    newStatus = status & BIT_INDEX_UNMASK;
    //(3)传入的数据左移4位, 对准位置:00000011 << 4 = 00110000
    newVal = val << 4;
    //(4)设置数据状态: 11000000 | 00110000 = 11110000
    data = newStatus | newVal;
    //(5)写入数据状态
    eefs_base_writeByte(statusAddress, &data);
    
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:根据角标设置7,8位发送状态
 * @index:角标
 * @val:状态
 * @return : 1:成功 0：失败
 */
u8 service_tpm_setSmallIndexCurrentStatus(u16 index, u8 val)
{
    // ---------- 局部变量定义区---------- //
    u16 statusAddress;      // status在数据区的位置
    u8 status;              // status值
    u8 newStatus;           // 清零后的值
    u8 newVal;
    u8 data;
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    if (val > 3) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)获取数据首地址
    statusAddress = service_tpm_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
    if (statusAddress == RET_FAILD) {
        return RET_FAILD;
    }
    status = eefs_base_readByte(statusAddress);
    //(2)&运算, 把七八位清零: 11110000 & 00111111 = 00110000
    newStatus = status & BIT_DATA_UNMASK;
    //(3)传入的数据左移6位, 对准位置:00000011 << 6 = 11000000
    newVal = val << 6;
    //(4)设置数据状态: 00110000 | 11000000 = 11110000
    data = newStatus | newVal;
    //(5)写入数据状态
    eefs_base_writeByte(statusAddress, &data);
    
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:根据角标设置1,2,3,4位的时间状态
 * @index:索引
 * @val:输入的状态值
 * @return : 1:成功 0：失败
 */
u8 service_tpm_setSmallIndexCTimeStatus(u16 index, u8 val)
{
    // ---------- 局部变量定义区---------- //
    u16 statusAddress;      // status在数据区的位置
    u8 status;              // status值
    u8 newStatus;           // 清零后的值
    u8 data;
    // ---------- 输入参数条件检测---------- //
    if (index > SMALLINDEX_DATACOUNT - 1) {
        return RET_FAILD;
    }
    if (val > 3) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)获取数据首地址
    statusAddress = service_tpm_getBreakeNetDataAddress(index) + SMALLINDEX_CAPACITY;
    if (statusAddress == RET_FAILD) {
        return RET_FAILD;
    }
    status = eefs_base_readByte(statusAddress);
    //(2)&运算, 把1234位清零: 00001111 & 11110000 = 00000000
    newStatus = status & BIT_TIME_UNMASK;
    //(3)设置数据状态: 00110000 | 11000000 = 11110000
    data = newStatus | val;
    //(4)写入数据状态
    eefs_base_writeByte(statusAddress, &data);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取角标对应的断网数据, 并改变发送状态, 数据为1是已发送, 2为未发送
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_disconnect_getDataAndChangeStatus(u8 * retData)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u8 sendStatus;
    u8 breakNetData;
    // ---------- 输入参数条件检测---------- //
    // ---------- 业务处理---------- //
    //(1)循环获取断网数据
    for (i = 0; i < SMALLINDEX_DATACOUNT; i++) {
        sendStatus = service_tpm_getSmallIndexSendStatus(i);
        if (sendStatus == RET_FAILD) {
            return RET_FAILD;
        }
        if (sendStatus == SMALLINDEX_NOSENDSTATUS) {// 为默认状态2, 证明有数据
            breakNetData = service_tpm_disconnect_getDataWithIndex(i, retData);
            if (breakNetData == RET_FAILD) {
                return RET_FAILD;
            }
            //(2)设置角标状态
            if (service_tpm_setSmallIndexSendStatus(i, SMALLINDEX_SENDTATUS) == RET_FAILD) {
                return RET_FAILD;
            }
            if (service_tpm_setSmallIndexCurrentStatus(i, SMALLINDEX_DEFAULTSTATUS)) {
                return RET_FAILD;
            }
            
            return RET_SUCCESS;
        }
    }
    return RET_FAILD;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:创建月数据空间
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_create_monthCapacity(void)
{
    // ---------- 局部变量定义区---------- //
    int i;
    USERNODE usernode;
    s8 indexStatus;
    u8 isSuccess;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)循环遍历索引区, 找到一个没使用的索引
    usernode.size = MONTHDATA_COUNT * MONTHDATA_CAPACITY;
    for (i = 0; i < MAX_INDEX; i++) {
        indexStatus = eefs_mbr_getIndexStatus(i);
        if (indexStatus == RET_ERROR) {
            return RET_FAILD;
        }
        if (indexStatus == ZERO_INDEXSTATUS) {
            isSuccess = eefs_mbr_setIndexStatus(i, DEFAULT_INDEXSTATUS);
            if (isSuccess == RET_FAILD) {
                return RET_FAILD;
            }
        }
        if (indexStatus == DEFAULT_INDEXSTATUS) {
            if (eefs_create(i, usernode) == RET_FAILD) {
                return RET_FAILD;
            }
            i = MAX_INDEX;
        }
    }
    
    return RET_SUCCESS;
    
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:保存月数据
 * @month:月份
 * @data:要保存的数据
 * @return : 1:成功 0：失败
 */
u8 service_tpm_saveMonthData(u8 month, u8 * data)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u16 address;        // 数据区首地址
    u16 saveAddress;    // 保存的位置
    u32 name;
    // ---------- 输入参数条件检测---------- //
    if (month <= 0 || month > 12) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    address = 0;
    //(1)获取月份空间对应的数据区首地址
    for (i = 0; i < MAX_INDEX; i++) {
        name = eefs_mbr_getName(i);
        if (name == RET_FAILD) {
            return RET_FAILD;
        }
        if (name == MONTHDATA_NAME) {
            address = eefs_mbr_getAddress(i);
            if (address == RET_FAILD) {
                return RET_FAILD;
            }
        }
    }
    //(2)获取保存的位置
    saveAddress = address + (month - 1) * MONTHDATA_CAPACITY;
    //(3)写入数据
    eefs_base_writeBytes(saveAddress, data, MONTHDATA_CAPACITY);
    return RET_SUCCESS;
    
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取月数据
 * @month:月份
 * @data:要保存的数据
 * @return : 1:成功 0：失败
 */
u8 service_tpm_getMonthData(u8 month, u8 * ret_data)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u16 address;        // 数据区首地址
    u16 saveAddress;    // 保存的位置
    u32 name;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    address = 0;
    //(1)获取月份空间对应的数据区首地址
    for (i = 0; i < MAX_INDEX; i++) {
        name = eefs_mbr_getName(i);
        if (name == RET_FAILD) {
            return RET_FAILD;
        }
        if (name == MONTHDATA_NAME) {
            address = eefs_mbr_getAddress(i);
            if (address == RET_FAILD) {
                return RET_FAILD;
            }
        }
    }
    //(2)获取保存的位置
    saveAddress = address + (month - 1) * MONTHDATA_CAPACITY;
    //(2)读取数据
    eefs_base_readBytes(saveAddress, ret_data, MONTHDATA_CAPACITY);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:创建失压数据空间
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_create_lostVoltCapacity(void)
{
    // ---------- 局部变量定义区---------- //
    int i;
    USERNODE usernode;
    s8 indexStatus;
    s8 isSuccess;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(1)循环遍历索引区, 找到一个没使用的索引
    usernode.name = LOSTVOLT_NAME;
    usernode.size = LOSTVOLT_COUNT * (LOSTVOLT_CAPACITY + LOSTVOLT_DESCRIBE);
    for (i = 0; i < MAX_INDEX; i++) {
        indexStatus = eefs_mbr_getIndexStatus(i);
        if (indexStatus == RET_ERROR) {
            return RET_FAILD;
        }
        if (indexStatus == ZERO_INDEXSTATUS) {
            isSuccess = eefs_mbr_setIndexStatus(i, DEFAULT_INDEXSTATUS);
            if (isSuccess == RET_FAILD) {
                return RET_FAILD;
            }
        }
        if (indexStatus == DEFAULT_INDEXSTATUS) {
            eefs_create(i, usernode); // 创建索引区
            i = MAX_INDEX;
        }
    }
    
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:保存失压数据
 * @data:传入的数据
 * @return : 1:成功 0：失败
 */
u8 service_tpm_saveLostVoltData(u8 * data)
{
    // ---------- 局部变量定义区---------- //
    int j;
    u8 currentStatus;
    u8 saveResult;
    // ---------- 输入参数条件检测---------- //
    
    // ---------- 业务处理---------- //
    //(2)循环遍历描述位, 1为当前状态, 0为以前状态
    for (j = 0; j < LOSTVOLT_COUNT; j++) {
        currentStatus = service_tpm_getLostVoltCurrentStatus(j);
        if (currentStatus == RET_FAILD) {
            return RET_FAILD;
        }
        if (currentStatus == 1) {
            if (j == LOSTVOLT_COUNT - 1) {// 如果是最后一个, 保存到0
                saveResult = service_tpm_saveOnceLostVoltData(0, data);
                if (saveResult == RET_FAILD) {
                    return RET_FAILD;
                }
                return RET_SUCCESS;
            }
            else // 否则保存到下一个
            {
                saveResult = service_tpm_saveOnceLostVoltData(j + 1, data);
                if (saveResult == RET_FAILD) {
                    return RET_FAILD;
                }
                return RET_SUCCESS;
            }
        }
    }
    
    saveResult = service_tpm_saveOnceLostVoltData(0, data);
    if (saveResult == RET_FAILD) {
        return RET_FAILD;
    }
    return RET_SUCCESS;
    
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:保存一次失压数据在index处
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_saveOnceLostVoltData(u16 index, u8 * data)
{
    // ---------- 局部变量定义区---------- //
    u16 address;     // 小索引地址
    u8 setStatusResult;
    // ---------- 输入参数条件检测---------- //
    if (index > LOSTVOLT_COUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    address = 0;
    
    //(1)根据小索引算出真正的地址
    address = service_tpm_getLostVoltDataAddress(index);
    if (address == RET_FAILD) {
        return RET_FAILD;
    }
    //(2)在newAddress处存数据
    eefs_base_writeBytes(address, data, LOSTVOLT_CAPACITY);
    
    //(3)写入状态位, 1,2为为当前状态, 1为当前, 0为以前
    setStatusResult = service_tpm_setLostVoltCurrentStatus(index, LOSTVOLT_CURRENTSTATUS);
    if (setStatusResult == RET_FAILD) {
        return RET_FAILD;
    }
    // 给上一个index更新状态
    if (index == 0) {
        setStatusResult = service_tpm_setLostVoltCurrentStatus(LOSTVOLT_COUNT - 1, LOSTVOLT_DEFAULTSTATUS);
        if (setStatusResult == RET_FAILD) {
            return RET_FAILD;
        }
    }
    else
    {
        setStatusResult = service_tpm_setLostVoltCurrentStatus(index - 1, LOSTVOLT_DEFAULTSTATUS);
        if (setStatusResult == RET_FAILD) {
            return RET_FAILD;
        }
    }
    // 时间状态
    
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取失压小索引数据区首地址
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u16 service_tpm_getLostVoltDataAddress(u16 index)
{
    // ---------- 局部变量定义区---------- //
    int i;
    u16 address;
    u32 name;
    // ---------- 输入参数条件检测---------- //
    if (index > LOSTVOLT_COUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    address = 0;
    for (i = 0; i < LOSTVOLT_COUNT; i++) {
        name = eefs_mbr_getName(i);
        if (name == RET_FAILD) {
            return RET_FAILD;
        }
        if (name == LOSTVOLT_NAME) {
            address = eefs_mbr_getAddress(i);
            if (address == RET_FAILD) {
                return RET_FAILD;
            }
        }
    }
    address = address + index * LOSTVOLT_CAPACITY + index * LOSTVOLT_DESCRIBE;
    return address;
}
/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:获取失压1,2位的当前状态
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_getLostVoltCurrentStatus(u16 index)
{
    // ---------- 局部变量定义区---------- //
    u16 statusAddress;      // status在数据区的位置
    u8 status;              // status值
    u8 newStatus;           // 右移后的值
    u16 smallIndexAddress;  // 小索引地址
    // ---------- 输入参数条件检测---------- //
    if (index > LOSTVOLT_COUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)找到status
    smallIndexAddress = service_tpm_getLostVoltDataAddress(index);
    if (smallIndexAddress == RET_FAILD) {
        return RET_FAILD;
    }
    statusAddress = smallIndexAddress + LOSTVOLT_CAPACITY;
    status = eefs_base_readByte(statusAddress);
    //(3)将除了12位的其他位清零
    newStatus = status & BIT_GENFLAG_MASK;
    return newStatus;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc: 设置失压1,2位的当前状态
 * @index:索引
 * @val:输入的状态
 * @return : 1:成功 0：失败
 */
u8 service_tpm_setLostVoltCurrentStatus(u16 index, u8 val)
{
    // ---------- 局部变量定义区---------- //
    u16 statusAddress;      // status在数据区的位置
    u8 status;              // status值
    u8 newStatus;           // 清零后的值
    u8 data;
    u16 smallIndexAddress;  // 小索引地址
    // ---------- 输入参数条件检测---------- //
    if (index > LOSTVOLT_COUNT - 1) {
        return RET_FAILD;
    }
    if (val > 3) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)获取数据首地址
    smallIndexAddress = service_tpm_getLostVoltDataAddress(index);
    if (smallIndexAddress == RET_FAILD) {
        return RET_FAILD;
    }
    statusAddress = smallIndexAddress + SMALLINDEX_CAPACITY;
    status = eefs_base_readByte(statusAddress);
    //(2)&运算, 把1,2位清零: 00001111 & 11111100 = 00001100
    newStatus = status & BIT_GENFLAG_UNMASK;
    //(3)设置数据状态: 00110000 | 11000000 = 11110000
    data = newStatus | val;
    //(4)写入数据状态
    eefs_base_writeByte(statusAddress, &data);
    return RET_SUCCESS;
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:根据角标取失压数据
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
u8 service_tpm_getLostVoltData(u16 index, u8 * retData)
{
    // ---------- 局部变量定义区---------- //
    u16 address;
    // ---------- 输入参数条件检测---------- //
    if (index > LOSTVOLT_COUNT - 1) {
        return RET_FAILD;
    }
    // ---------- 业务处理---------- //
    //(1)获取输入值对应的数据区位置
    address = service_tpm_getLostVoltDataAddress(index);
    if (address == RET_FAILD) {
        return RET_FAILD;
    }
    eefs_base_readBytes(address, retData, LOSTVOLT_CAPACITY);
    return RET_SUCCESS;
}
