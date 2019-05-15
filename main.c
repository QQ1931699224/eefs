//
//  main.c
//  CTest
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

int main(int argc, const char *argv[]) {
    testEefs_mbr_create();
    testEefs_mbr_getDataStatus();
    testEefs_mbr_getIndexStatus();
    testEefs_mbr_getNetStatus();
    testEefs_mbr_getGenFlagStatus();
	testEefs_mbr_getName();
	testEefs_mbr_getAddress();
	printf("%s", G_STATUS_LISI);
	printf("总= %d", G_LIST);
    return 0;
}


/*
 * Auth: 吴晗帅
 * Date: 2019-5-10
 * Desc:测试创建索引函数
 * @paramName:无
 * @return : 无
 */
#pragma mark - 测试创建索引函数
void testEefs_mbr_create(void)
{
    USERNODE userNode;
    userNode.name = 2048;
    userNode.size = 256;
    // index在0 - 128之间
    eefs_mbr_create(100, userNode);
    NODE *myNode;
    u8 data[9];
    int i;
    for (i = 0; i < 9; i++) {
        data[i] = readByte(164 + i);
    }
    myNode = malloc(9);
    memcpy((u8 *)myNode, data, 9);
    printf("%s", G_LIST);
    
    eefs_mbr_load();
    printf("%s", G_STATUS_LISI);
    
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
    // 0 <= index <= 128, 0 <= val <= 3
    eefs_mbr_setDataStatus(100, 3);
    data = eefs_mbr_getDataStatus(100);
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
    // 更新索引
    eefs_mbr_update(100, 256, 3);
    data = eefs_mbr_getIndexStatus(100);
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
	printf("name=%d", data);
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
	printf("address=%d", data);
}


