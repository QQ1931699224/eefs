//
//  main.c
//  CTest
//
//  Created by 吴晗帅 on 2019/5/9.
//  Copyright © 2019 吴晗帅. All rights reserved.
//  程序实现大体思路

#include <stdio.h>
#include <string.h>
#include "eefs_lib.h"

void testEefs_mbr_create(void);
void testEefs_mbr_getDataStatus(void);
void testEefs_mbr_getIndexStatus(void);
void testEefs_mbr_getNetStatus(void);
void testEefs_mbr_getGenFlagStatus(void);
void testEefs_mbr_getName(void);
void testEefs_mbr_getAddress(void);
void testEefs_data_getDesc(void);
void testEefs_mbr_create1(void);

void testEefs_data_getSys(void);

void testEefs_create(void);
void testEefs_allCreate(void);
void testEefs_setValueWithOffset(void);
void testSmallIndex(void);
void testMonthData(void);
void testLostVoltData(void);

int main(int argc, const char* argv[]) {

    testLostVoltData();
    testMonthData();
    testSmallIndex();
	testEefs_data_getSys();
	testEefs_mbr_create();
	testEefs_mbr_create1();
	//    testEefs_create();
	//    testEefs_allCreate();
	testEefs_setValueWithOffset();
	testEefs_mbr_getDataStatus();
	testEefs_mbr_getIndexStatus();
	testEefs_mbr_getNetStatus();
	testEefs_mbr_getGenFlagStatus();
	testEefs_mbr_getName();
	testEefs_mbr_getAddress();
	//printf("%s", G_STATUS_LISI);
	// 更新索引
	eefs_mbr_update(100, 256, 3);
	testEefs_data_getDesc();
	printf("%s", G_LIST);

	printf("总= %s", G_LIST);

	return 0;
}

/////////////////

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试创建索引函数
 * @paramName:无
 * @return : 无
 */
void testEefs_mbr_create(void)
{
	USERNODE userNode;
	userNode.name = 2048;
	userNode.size = 100;
	// index在0 - 128之间
	eefs_mbr_create(0, userNode);
	NODE* myNode;
	u8 data[9];
	int i;
	for (i = 0; i < 9; i++) {
		data[i] = eefs_base_readByte(164 + i);
	}
	myNode = (void*)malloc(9);
	memcpy((u8*)myNode, data, 9);
	eefs_mbr_load();
}


void testEefs_mbr_create1(void)
{
	//printf("%s", G_LIST);
	USERNODE userNode;
	userNode.name = 300;
	userNode.size = 100;
	// index在0 - 128之间
	eefs_mbr_create(1, userNode);
	NODE* myNode;
	u8 data[9];
	int i;
	for (i = 0; i < 9; i++) {

		data[i] = eefs_base_readByte(164 + i);
	}
	myNode = malloc(9);
	memcpy((u8*)myNode, data, 9);
	//printf("%s", G_LIST);

	eefs_mbr_load();
	//printf("%s", G_STATUS_LISI);

}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试获取数据状态, 设置数据状态函数
 * @paramName:无
 * @return : 无
 */
void testEefs_mbr_getDataStatus(void)
{
	u8 data;
	eefs_mbr_setDataStatus(100, 3);
	data = eefs_mbr_getDataStatus(100);
	printf("data= %d\n", data);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试获取索引状态, 设置索引状态函数
 * @paramName:无
 * @return : 无
 */
void testEefs_mbr_getIndexStatus(void)
{
	u8 data;
	// 0 <= index <= 128, 0 <= val <= 3
	eefs_mbr_setIndexStatus(100, 1);
	data = eefs_mbr_getIndexStatus(100);
	printf("index=%d\n", data);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试获取网络状态, 设置网络状态函数
 * @paramName:无
 * @return : 无
 */
void testEefs_mbr_getNetStatus(void)
{
	u8 data;
	// 0 <= index <= 128, 0 <= val <= 3
	eefs_mbr_setNetStatus(100, 3);
	data = eefs_mbr_getNetStatus(100);
	printf("net=%d\n", data);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试获取通用标记状态, 设置通用标记状态函数
 * @paramName:无
 * @return : 无
 */
void testEefs_mbr_getGenFlagStatus(void)
{
	u8 data;
	// 0 <= index <= 128, 0 <= val <= 3
	eefs_mbr_setGenFlag(100, 2);
	data = eefs_mbr_getGenFlag(100);
	printf("genflag=%d\n", data);
}

/*
 * Auth: 张添程
 * Date: 2019-5-14
 * Desc:测试获取和修改索引的name
 * @paramName:无
 * @return : 无
 */
void testEefs_mbr_getName(void)
{
	u32 data;
	eefs_mbr_setName(100, 1024);
	data = eefs_mbr_getName(100);
	printf("name=%d\n", data);
}

/*
 * Auth: 张添程
 * Date: 2019-5-14
 * Desc:测试获取和修改索引的address
 * @paramName:无
 * @return : 无
 */
void testEefs_mbr_getAddress(void)
{
	u16 data;
	eefs_mbr_setAddress(100, 3000);
	data = eefs_mbr_getAddress(100);
	printf("address=%d\n", data);
}

/*
 * Auth: 张添程
 * Date: 2019-5-14
 * Desc:测试获取和修改索引的address
 * @paramName:无
 * @return : 无
 */
void testEefs_data_getDesc(void)
{
	u16 data;
	u8 high;
	u8 low;
	eefs_data_setDesc(100, 1024);
	eefs_data_setDescHigh(100, 8);
	high = eefs_data_getDescHigh(100);
	printf("high=%d\n", high);
	eefs_data_setDescLow(100, 6);
	low = eefs_data_getDescLow(100);
	printf("low=%d\n", low);
	data = eefs_data_getDesc(100);
	printf("desc=%d\n", data);
}


/*
 * Auth: 张添程
 * Date: 2019-5-14
 * Desc:测试获取和修改系统区的相关信息
 * @paramName:无
 * @return : 无
 */
void testEefs_data_getSys(void)
{
	u16 used;
	u16 unused;
	u8 flag;
	u8 version;
	eefs_sys_setFlag(66);
	flag = eefs_sys_getFlag();
	printf("flag=%d\n", flag);
	eefs_sys_setVersion();
	version = eefs_sys_getVersion();
	printf("ver=%d\n", version);
	eefs_sys_setUsedCapacity(88);
	used = eefs_sys_getUsedCapacity();
	printf("used=%d\n", used);
	eefs_sys_setUnusedCapacity(77);
	unused = eefs_sys_getUnusedCapacity();
	printf("unused=%d\n", unused);
}


/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试创建索引和删除索引
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
void testEefs_create()
{
	USERNODE userNode;
	userNode.name = 20;
	userNode.size = 100;
	eefs_create(2, userNode);
	printf("%s", G_LIST);

	eefs_delete(2);
	printf("%s", G_LIST);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试创建全部索引和删除全部索引
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
void testEefs_allCreate()
{
	USERNODE list[10];
	int i;
	for (i = 0; i < 10; i++) {
		list[i].name = 200 + i;
		list[i].size = 100;
	}
	eefs_createAll(list, 10);
	printf("%s", G_LIST);
	eefs_deleteAll();
	printf("%s", G_LIST);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试根据偏移量给数据区赋值和取值
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
void testEefs_setValueWithOffset()
{
	u8 data;
	data = 1;
	u8 ret_data;
	eefs_setValueWithOffset(2048, 50, &data, 1);
	eefs_getValueWithOffset(2048, 50, &ret_data, 1);
	printf("%s", G_LIST);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试小索引系列
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
void testSmallIndex(void)
{
    SMALLINDEXNODE node;
    SMALLINDEXNODE node100;
    int i;
    u8 retData[SMALLINDEX_CAPACITY];
    u8 currentStatus;
    u8 sendStatus;
    u8 timeStatus;
    node.electric1 = 1;
    node.electric2 = 2;
    node.electric3 = 3;
    node.electric4 = 4;
    node.electric5 = 5;
    node.electric6 = 6;
    node100.electric1 = 1;
    node100.electric2 = 2;
    node100.electric3 = 3;
    node100.electric4 = 1;
    node100.electric5 = 2;
    node100.electric6 = 3;
    
    meter_create_breakeNetCapacity();
    for (i = 0; i < 101; i++) {
        if (i == 100) {
            meter_saveBreakeNetData(node100);
        }
        else
        {
            meter_saveBreakeNetData(node);
        }
    }
    
    // 测试根据角标获取断网数据
    meter_disconnect_getDataWithIndex(0, retData);
    
    // 测试时间状态
    meter_setSmallIndexCTimeStatus(0, 2);
    timeStatus = meter_getSmallIndexTimeStatus(0);
    
    // 测试当前状态
    meter_setSmallIndexCurrentStatus(0, 2);
    currentStatus = meter_getSmallIndexCurrentStatus(0);
    
    // 测试发送状态
    meter_setSmallIndexSendStatus(0, 2);
    sendStatus = meter_getSmallIndexSendStatus(0);
    
    // 测试获取断网数据并且改变状态
    meter_disconnect_getDataAndChangeStatus(retData);
    sendStatus = meter_getSmallIndexSendStatus(0);
    printf("%s", G_LIST);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试月份数据
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
void testMonthData(void)
{
    u8 retData[MONTHDATA_CAPACITY];
    u8 data[MONTHDATA_CAPACITY];
    // 创建月数据空间
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    meter_create_monthCapacity();
    meter_saveMonthData(2, data);
    meter_getMonthData(2, retData);
    printf("%s", G_LIST);
}

/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试失压数据
 * @paramName:xxxxx
 * @return : 1:成功 0：失败
 */
void testLostVoltData(void)
{
    u8 retData[LOSTVOLT_CAPACITY];
    u8 data[LOSTVOLT_CAPACITY];
    u8 data2[LOSTVOLT_CAPACITY];
    int i;
    // 创建失压数据空间
    data[0] = 1;
    data[1] = 1;
    data[2] = 2;
    data[3] = 2;
    data[4] = 3;
    data[5] = 3;
    data[6] = 4;
    data[7] = 4;
    data[8] = 5;
    
    data2[0] = 4;
    data2[1] = 3;
    data2[2] = 2;
    data2[3] = 1;
    data2[4] = 5;
    data2[5] = 4;
    data2[6] = 3;
    data2[7] = 2;
    data2[8] = 1;
    
    // 创建失压空间
    meter_create_lostVoltCapacity();
    for (i = 0; i < 21; i++) {
        if (i == 20) {
            meter_saveLostVoltData(data2);
        }
        else
        {
            meter_saveLostVoltData(data);
        }
    }
    // 测试根据角标获取失压数据
    meter_getLostVoltData(0, retData);
    printf("%s", G_LIST);
}
