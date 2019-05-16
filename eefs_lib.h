//
//  WriteOrReadByte.h
//  CTest
//
//  Created by 吴晗帅 on 2019/5/10.
//  Copyright © 2019 吴晗帅. All rights reserved.
//

#ifndef eefs_lib_h
#define eefs_lib_h

#include <stdio.h>

typedef enum {
	data_none = 0,
	data_exist,
	data_A,
	data_B
}DATATYPE;

typedef   unsigned char   u8;
typedef   unsigned short   u16;
typedef   unsigned int   u32;

typedef int  s32;
typedef short s16;
typedef signed char  s8;

#define BIT_DATA_MASK 0xC0                  // (11000000)数据状态位掩码
#define BIT_DATA_UNMASK 0x63                // (00111111)数据状态位掩码
#define BIT_NET_UNMASK 0xF3                 // (11110011)网络状态位掩码
#define BIT_NET_MASK 0x0C                   // (00001100)网络状态位掩码
#define BIT_INDEX_MASK 0x30                 // (00110000)索引状态位掩码
#define BIT_INDEX_UNMASK 0xCF               // (11001111)索引状态位掩码
#define BIT_GENFLAG_MASK 0x03               // (00000011)通用状态位掩码
#define BIT_GENFLAG_UNMASK 0xFC             // (11111100)通用状态位掩码


// 地址
#define TEMP_ADDRESS 2048                   // 模拟的数据区首地址
#define EE_START_DATA 2048                  // 数据区起始地址
#define EE_START_INDEX 64                   // 索引起始地址
// 空间
#define MAX_INDEX 128                       // 最大索引
#define EE_MAX_CAPACITY 256 * 64            // 最大容量
#define EE_SYS_CAPACITY 64                  // 系统预留容量

											// error返回值
#define RET_SUCCESS 1                       // 成功返回值
#define RET_FAILD   0                       // u8错误返回值
#define RET_ERROR   -1                      // s8错误返回值
#define ERR_INVALIDPARAM 10                 // 无效的参数

#define INDEX_SIZE 9                        // 每个索引节点空间
#define NAME_OFFSET 0                       // name单个偏移量
#define ADDR_OFFSET 4                       // 地址单个偏移量
#define SIZE_OFFSET 6                       // 大小单个偏移量
#define STATUS_OFFSET 8                     // status单个偏移量

#define NAME_SIZE sizeof(u32)		        //name大小
#define ADDR_SIZE sizeof(u16)               //address大小
#define SIZE_SIZE sizeof(u16)               //size大小
#define STATUS_SIZE sizeof(u8)              //status大小

#define DESC_SIZE sizeof(u16)              //desc大小

// 索引区结构体
typedef struct node {
	u32 name;
	u16 address;
	u16 size;
	u8 status;
}NODE;

// 用户使用的索引区结构体
typedef struct userNode {
	u32 name;
	u16 size;
}USERNODE;

// 全局变量G_变量名(全大写)
// 局部变量小写(驼峰)
extern u8 G_LIST[EE_MAX_CAPACITY];                         // 模拟的整个缓存区
extern u8 G_STATUS_LISI[MAX_INDEX];                        // 索引区数组


u8 eefs_base_writeByte(u16 address, u8* data); //在eeprom的指定位置写入1个字节
u8 eefs_base_readByte(u16 address); //在eeprom的指定位置读取1个字节
u8 eefs_base_writeBytes(u16 address, u8* data, u16 dataLen); //在eeprom的指定位置写入dataLen个字节
u8 eefs_base_readBytes(u16 address, u8* retData, u16 retLen); //从eeprom的指定位置读取retLen个字节


u16 getAddress(u16 size);                                      // 获取模拟地址
u8 eefs_mbr_CheckAddress(u16 address);                     // 检查地址合法性
u16 getIndexAddress(u16 index);                            // 返回索引地址
u8 eefs_mbr_CheckIndex(u16 index);                         // 判断index是否合法
u8 writeDataToIndex(u16 address, NODE node);               // 写入数据到索引区
u8 eefs_mbr_load(void);                                    // 向全局数组中载入index的status值
u8 eefs_mbr_create(u16 index, USERNODE node);              // 添加新索引
u8 eefs_mbr_update(u16 index, u32 name, u8 status);        // 更新索引
u8 eefs_mbr_delete(u16 index);                             // 删除索引
u8 eefs_mbr_reset(u16 index);                              // 重置索引数据

u32 eefs_mbr_getName(u16 index);                           // 获取索引区名字
u8  eefs_mbr_setName(u16 index, u32 name);                  // 设置索引区名字
u16 eefs_mbr_getAddress(u16 index);                        // 获取索引区地址
u8  eefs_mbr_setAddress(u16 index, u16 addr);              // 设置索引区地址
u16 eefs_mbr_getIndexHeadAddress(u16 index);               // 获取索引节点首地址
u16 eefs_mbr_getIndexNameHeadAddress(u16 index);           // 获取索引节点首地址
u16 eefs_mbr_getIndexAddressHeadAddress(u16 index);        // 获取索引节点首地址
u16 eefs_mbr_getIndexSizeHeadAddress(u16 index);           // 获取索引节点首地址
u16 eefs_mbr_getIndexStatusHeadAddress(u16 index);         // 获取索引节点首地址

u16 eefs_mbr_getSize(u16 index);                            // 获取索引对应的数据区大小
u8 eefs_mbr_setSize(u16 index, u16 size);                   // 设置索引对应的数据区大小
u8 eefs_mbr_check(u16 index);                              // 检查索引值是否合法，合法则返回索引首地址
s8 eefs_mbr_getStatus(u16 index);                          // 获取状态
u8 eefs_mbr_setStatus(u16 index, u8 val);                  // 设置状态
s8 eefs_mbr_getGenFlag(u16 index);                         // 获取通用标记值
u8 eefs_mbr_setGenFlag(u16 index, u8 val);                // 设置通用标记值
s8 eefs_mbr_getIndexStatus(u16 index);                     // 获取索引状态(5,6位)
u8 eefs_mbr_setIndexStatus(u16 index, u8 val);            // 设置索引状态
s8 eefs_mbr_getNetStatus(u16 index);                       // 获取网络状态(3,4位)
u8 eefs_mbr_setNetStatus(u16 index, u8 val);               // 设置网络状态
s8 eefs_mbr_getDataStatus(u16 index);                      // 获取数据状态(7,8位)
u8 eefs_mbr_setDataStatus(u16 index, u8 val);              // 设置数据状态

u16 eefs_data_getHeadAddr(u16 index);   //获取节点数据的数据区首地址
u16 eefs_data_getTailAddr(u16 index);   //获取节点数据的数据区尾地址
u16 eefs_data_getTotalCapacity(void);   //获取数据区总容量
u16 eefs_data_getUsedCapacity(void);    //获取已使用总容量
u16 eefs_data_getUnusedCapacity(void);  //获取未使用空间总容量
u16 eefs_data_findUnusedAddr(u16 size);     //返回可以使用的一个合适的地址
u8 eefs_data_create(u16 addr, u16 size); //创建数据区，并初始化
u8 eefs_data_update(u16 addr, u16 size); //更新数据区全部内容

u16 eefs_data_getDescHeadAddress(u16 index); //获取数据区描述符首地址
u16 eefs_data_getDesc(u16 index); //获取数据区描述符
u8 eefs_data_setDesc(u16 index, u16 desc); //设置数据区描述符
u8 eefs_data_getDescHigh(u16 index); //获取数据区描述符高位
u8 eefs_data_setDescHigh(u16 index, u8 value); //设置数据区描述符高位
u8 eefs_data_getDescLow(u16 index); //获取数据区描述符低位
u8 eefs_data_setDescLow(u16 index, u8 value); //设置数据区描述符低位

#endif /* WriteOrReadByte_h */
