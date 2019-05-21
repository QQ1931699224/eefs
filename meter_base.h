#ifndef meter_base_h
#define meter_base_h
#include <stdio.h>

#define TYPE_WRITE_1 1                                                       //д������1
#define TYPE_WRITE_4 4                                                       //д������4
#define TYPE_WRITE_8 8                                                       //д������8
#define TYPE_WRITE_16 16											         //д������16

#define DATA_NEW_POS_STATUS 0x55 											 //��д��״̬
#define DATA_OLD_POS_STATUS 0xAA 											 //��д��״̬

#define DATA_CRC_SIZE 2                                                      //CRC��С
#define DATA_STATUS_SIZE 1                                                   //�¾�д��״̬��С
#define CRC_Y 1                                                              //ͨ�ñ��λ��CRC
#define CRC_N 2                                                              //ͨ�ñ��λ��CRC

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

u8 meter_register(u16 index,MEATERVAR meaterVer);                                    //����ָ�����͵����ݿռ�
u8 meter_circle_write(u16 index, u8* data, u16 len);                                 //����д��
u8 meter_circle_read(u16 index, u8* retData);                                        //���ݶ�ȡ

TYPE_WRITE meter_get_data_status(u8 WRITE_TYPE);                                     //�ж�д�����ͻ�ȡ����״̬
u8 meter_get_write_type(u16 index);                                                  //����������ȡд������
u16 meter_get_data_status_address(u16 index);                                        //��ȡ���¿ɶ���������״̬λ��ַ
u16 meter_get_write_address(u16 index);                                              //��ȡ��ǰ��д���������׵�ַ



#endif