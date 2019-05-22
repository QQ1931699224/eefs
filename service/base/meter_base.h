#ifndef meter_base_h
#define meter_base_h
#include <stdio.h>

#define TYPE_WRITE_1 1                                                       //写入类型1
#define TYPE_WRITE_4 4                                                       //写入类型4
#define TYPE_WRITE_8 8                                                       //写入类型8
#define TYPE_WRITE_16 16											         //写入类型16

#define DATA_NEW_POS_STATUS 0x55 											 //新写入状态
#define DATA_OLD_POS_STATUS 0xAA 											 //旧写入状态

#define DATA_CRC_SIZE 2                                                      //CRC大小
#define DATA_STATUS_SIZE 1                                                   //新旧写入状态大小
#define CRC_Y 1                                                              //通用标记位有CRC
#define CRC_N 2                                                              //通用标记位无CRC

typedef enum {
	tpye_write_1, 
	tpye_write_4, 
	tpye_write_8, 
	tpye_write_16
}TYPE_WRITE;



typedef struct meaterVar {
	u32 name;
	u16 size;
	u8 type;
	u8 crc;
	u8 net;
}MEATERVAR;

u8 meter_register(u16 index,MEATERVAR meaterVer);                                    //创建指定类型的数据空间
u8 meter_circle_write(u16 index, u8* data, u16 len);                                 //数据写入
u8 meter_circle_read(u16 index, u8* retData);                                        //数据读取

TYPE_WRITE meter_get_data_status(u8 WRITE_TYPE);                                     //判断写入类型获取数据状态
s8 meter_get_write_type(u16 index);                                                  //根据索引获取写入类型
u16 meter_get_data_status_address(u16 index);                                        //获取最新可读数据区的状态位地址
u16 meter_get_write_address(u16 index);                                              //获取当前可写入数据区首地址



#endif
