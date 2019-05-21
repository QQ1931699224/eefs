//
//  eefs_lib.h
//  CTest
//
//  Created by 吴晗帅 on 2019/5/10. //  Copyright © 2019 吴晗帅. All rights reserved.
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

#define EE_SYS_FLAG_OFFSET 0                                                   //系统保留区标志位偏移量
#define EE_SYS_VERSION_OFFSET 1                                               //系统保留区版本号偏移量
#define EE_SYS_USEDCAPACITY_OFFSET 3                                        //系统保留区已使用空间标志偏移量
#define EE_SYS_UNUSEDCAPACITY_OFFSET 5                                     //系统保留区未使用空间标志偏移量


#define BIT_DATA_MASK 0xC0                                                       // (11000000)数据状态位掩码
#define BIT_DATA_UNMASK 0x3F                                                     // (00111111)数据状态位掩码
#define BIT_NET_UNMASK 0xF3                                                      // (11110011)网络状态位掩码
#define BIT_NET_MASK 0x0C                                                        // (00001100)网络状态位掩码
#define BIT_INDEX_MASK 0x30                                                      // (00110000)索引状态位掩码
#define BIT_INDEX_UNMASK 0xCF                                                    // (11001111)索引状态位掩码
#define BIT_GENFLAG_MASK 0x03                                                    // (00000011)通用状态位掩码
#define BIT_GENFLAG_UNMASK 0xFC                                                  // (11111100)通用状态位掩码


                                                                                 // 地址
#define TEMP_ADDRESS 2048                                                        // 模拟的数据区首地址
#define EE_START_DATA 2048                                                       // 数据区起始地址
#define EE_START_INDEX 64                                                        // 索引起始地址
                                                                                 // 空间
#define MAX_INDEX 128                                                            // 最大索引
#define EE_MAX_CAPACITY 256 * 64                                                 // 最大容量
#define EE_SYS_CAPACITY 64                                                       // 系统预留容量
#define EE_START_SYS 0						                                     // 系统保留的描述区域起始位置

                                                                                 // error返回值
#define RET_SUCCESS 1                                                            // 成功返回值
#define RET_FAILD   0                                                            // u8错误返回值
#define RET_ERROR   -1                                                           // s8错误返回值
#define ERR_INVALIDPARAM 10                                                      // 无效的参数


#define NAME_SIZE sizeof(u32)		                                             //name大小
#define ADDR_SIZE sizeof(u16)                                                    //address大小
#define SIZE_SIZE sizeof(u16)                                                    //size大小
#define STATUS_SIZE sizeof(u8)                                                   //status大小
#define NAME_OFFSET 0                                                            // name单个偏移量
#define ADDR_OFFSET (NAME_OFFSET+NAME_SIZE)			                             // 地址单个偏移量
#define SIZE_OFFSET (ADDR_OFFSET+ADDR_SIZE)                                      // 大小单个偏移量
#define STATUS_OFFSET (SIZE_OFFSET+SIZE_SIZE)                                    // status单个偏移量

#define INDEX_SIZE 9                                                             // 每个索引节点空间

#define DATA_DESCRIBE 2					                                         // 数据区描述
#define STATUS_INITVALUE 0x50										             // 状态位初始值:01010000
#define DESC_SIZE sizeof(u16)                                                    //desc大小
#define DESC_HIGH_SIZE 0					                                     //desc高位大小
#define DESC_LOW_SIZE 1						                                     //desc低位大小

#define UNUSEDCAPACITY_SIZE sizeof(u16)                                          //UnusedCapacity标识大小
#define USEDCAPACITY_SIZE sizeof(u16)                                            //UsedCapacity标识大小
#define EE_SYS_FLAG_SIZE sizeof(u8);                                             //系统保留区标志位大小
#define EE_SYS_VERSION_SIZE sizeof(u8);                                          //系统保留区版本号大小


#define ZERO_INDEXSTATUS 0x00       // 索引状态为0
#define DEFAULT_INDEXSTATUS 0x02    // 索引状态为2

                                                                                 // 索引区结构体
typedef struct node{
    u32 name;
    u16 address;
    u16 size;
    u8 status;
}NODE;

                                                                                 // 用户使用的索引区结构体
typedef struct userNode{
    u32 name;
    u16 size;
}USERNODE;

                                                                                 // 全局变量G_变量名(全大写)
                                                                                 // 局部变量小写(驼峰)
extern u8 G_LIST[EE_MAX_CAPACITY];                                               // 模拟的整个缓存区
extern u8 G_STATUS_LISI[MAX_INDEX];                                              // 索引区数组

                                                                                 // 接收数据的结构体
typedef struct dataStruct {
    u16 index;
    u16 address;
}DATAStRUCT;



u16 isEffectiveAddress(u16 address);                                             // 判断地址合法性
u8 eefs_base_writeByte(u16 address,u8* data);                                    //在eeprom的指定位置写入1个字节
u8 eefs_base_readByte(u16 address);                                              //在eeprom的指定位置读取1个字节
u8 eefs_base_writeBytes(u16 address, u8* data, u16 dataLen);                     //在eeprom的指定位置写入dataLen个字节
u8 eefs_base_readBytes(u16 address, u8* retData, u16 retLen);                    //从eeprom的指定位置读取retLen个字节


u16 getAddress(u16 size);                                                        // 获取模拟地址
u8 eefs_mbr_CheckAddress(u16 address);                                           // 检查地址合法性
u16 getIndexAddress(u16 index);                                                  // 返回索引地址
u8 eefs_mbr_CheckIndex(u16 index);                                               // 判断index是否合法
u8 writeDataToIndex(u16 address, NODE node);                                     // 写入数据到索引区
u8 eefs_mbr_load(void);                                                          // 向全局数组中载入index的status值
u8 eefs_mbr_create(u16 index, USERNODE node);                                    // 添加新索引
u8 eefs_mbr_update(u16 index, u32 name, u8 status);                              // 更新索引
u8 eefs_mbr_delete(u16 index);                                                   // 删除索引
u8 eefs_mbr_reset(u16 index);                                                    // 重置索引数据

u32 eefs_mbr_getName(u16 index);                                                 // 获取索引区名字
u8  eefs_mbr_setName(u16 index,u32 name);                                        // 设置索引区名字
u16 eefs_mbr_getAddress(u16 index);                                              // 获取索引区地址
u8  eefs_mbr_setAddress(u16 index, u16 addr);                                    // 设置索引区地址
u16 eefs_mbr_getIndexHeadAddress(u16 index);                                     // 获取索引节点首地址
u16 eefs_mbr_getIndexNameHeadAddress(u16 index);                                 // 获取索引节点首地址
u16 eefs_mbr_getIndexAddressHeadAddress(u16 index);                              // 获取索引节点首地址
u16 eefs_mbr_getIndexSizeHeadAddress(u16 index);                                 // 获取索引节点首地址
u16 eefs_mbr_getIndexStatusHeadAddress(u16 index);                               // 获取索引节点首地址

u16 eefs_mbr_getSize(u16 index);                                                 // 获取索引对应的数据区大小
u8 eefs_mbr_setSize(u16 index,u16 size);                                         // 设置索引对应的数据区大小
u16 eefs_mbr_getDataSize(u16 index);                                             // 获取索引对应的数据区大小(不包含数据描述位)
u8 eefs_mbr_check(u16 index);                                                    // 检查索引值是否合法，合法则返回索引首地址
s8 eefs_mbr_getStatus(u16 index);                                                // 获取状态
u8 eefs_mbr_setStatus(u16 index ,u8 val);                                        // 设置状态
s8 eefs_mbr_getGenFlag(u16 index);                                               // 获取通用标记值
u8 eefs_mbr_setGenFlag(u16 index , u8 val);                                      // 设置通用标记值
s8 eefs_mbr_getIndexStatus(u16 index);                                           // 获取索引状态(5,6位)
u8 eefs_mbr_setIndexStatus(u16 index , u8 val);                                  // 设置索引状态
s8 eefs_mbr_getNetStatus(u16 index);                                             // 获取网络状态(3,4位)
u8 eefs_mbr_setNetStatus(u16 index ,u8 val);                                     // 设置网络状态
s8 eefs_mbr_getDataStatus(u16 index);                                            // 获取数据状态(7,8位)
u8 eefs_mbr_setDataStatus(u16 index ,u8 val);                                    // 设置数据状态

u16 eefs_data_getHeadAddr(u16 index);                                            //获取节点数据的数据区首地址
u16 eefs_data_getTailAddr(u16 index);                                            //获取节点数据的数据区尾地址
u16 eefs_data_getTotalCapacity(void);                                            //获取数据区总容量
u16 eefs_data_getUsedCapacity(void);                                             //获取已使用总容量
u16 eefs_data_getUnusedCapacity(void);                                           //获取未使用空间总容量
u16 eefs_data_findUnusedAddr(u16 size);                                          //返回可以使用的一个合适的地址
u8 eefs_data_create(u16 addr, u16 size);                                         //创建数据区，并初始化
u8 eefs_data_update(u16 addr, u16 size);                                         //更新数据区全部内容

u16 eefs_data_getDescHeadAddress(u16 index);                                     //获取数据区描述符首地址
u16 eefs_data_getDesc(u16 index);                                                //获取数据区描述符
u8 eefs_data_setDesc(u16 index, u16 desc);                                       //设置数据区描述符
u8 eefs_data_getDescHigh(u16 index);                                             //获取数据区描述符高位
u8 eefs_data_setDescHigh(u16 index, u8 value);                                   //设置数据区描述符高位
u8 eefs_data_getDescLow(u16 index);                                              //获取数据区描述符低位
u8 eefs_data_setDescLow(u16 index, u8 value);                                    //设置数据区描述符低位


u8 eefs_createAll(USERNODE list[], u8 len);                                      //创建（申请）多个数据空间
u8 eefs_create(u16 index, USERNODE node);                                        //创建1个
u8 eefs_delete(u16 index);                                                       //删除（释放）一个数据空间
u8 eefs_deleteAll(void);
u8 eefs_reset(u16 index);                                                        //将指定1个数据的空间清零
u8 eefs_resetAll(void);
u8 eefs_init(void);                                                              //初始化全部空间
u8 eefs_setValue(u32 name, u8* data, u16 len);                                   // 设置数据区全部内容
u8 eefs_getValue(u32 name, u8* ret_data, u16 len);                              // 获取数据区全部内容
u8 eefs_setValueWithOffset(u32 name, u16 offset, u8* data, u16 len);             // 设置数据区全部内容
u8 eefs_getValueWithOffset(u32 name, u16 offset, u8* ret_data, u16 len);         // 获取数据区全部内容


u8 eefs_sys_getFlag(void);                                                           //系统保留区标志位读取
u8 eefs_sys_setFlag(u8 value);                                                   //系统保留区标志位写入
u8 eefs_sys_getVersion(void);                                                    //取得写入索引版本
u8 eefs_sys_setVersion(void);                                                        //写入索引版本
u16 eefs_sys_getUsedCapacity(void);                                                  //获取已使用空间容量
u8 eefs_sys_setUsedCapacity(u16 size);                                           //设置已使用空间容量
u16 eefs_sys_getUnusedCapacity(void);                                                //获取未使用空间容量
u8 eefs_sys_setUnusedCapacity(u16 size);                                         //设置未使用空间容量


#endif /* WriteOrReadByte_h */
