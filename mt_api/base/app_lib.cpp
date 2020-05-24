/*
 * app_lib.cpp
 *
 *  Created on: 2016年11月24日
 *      Author: J00054
 */
#include <string.h>			//标准输入输出定义
#include <stdio.h>			//标准输入输出定义
#include <stdlib.h>			//标准函数库定义
#include <math.h>			//标准函数库定义
#include "app_lib.h"
/*******************************************************************************
函数: app_malloc()
功能: 申请一块指定大小的内存，并记录申请的大小
参数:
None
返回:
None
*******************************************************************************/
void* App_lib::app_malloc(u32 size)
{
	u8 *mem = NULL;
	mem = (u8*)malloc(size);
	if (mem == NULL)
	{
		exit(0);
	}
	memset(mem, 0, size);
	return (void *)mem;
}

/*******************************************************************************
函数: app_free()
功能: 释放指定的内存，并找到相应的内存块节点，清空节点信息，但不释放节点本身占用的内存
参数:
None
返回:
None
*******************************************************************************/
void App_lib::app_free(char *addr)
{
	free(addr);
}

/*******************************************************************************
* 函数名: read_file()
* 功能: 读取文件，将指定的文件内容读到buf中，读取的大小由size指定
* 参数:
*		path_name: 文件路径
*		buf: (输出)内容缓冲区
*		size: 读取数据的长度
* 返回值: 实际读取到的数据长度
*******************************************************************************/
int App_lib::read_file(const char *path_name, char *buf, int size)
{
	FILE *fp = NULL;	/* 用于打开指定的文件 */
	int ret = 0;

	fp = fopen(path_name, "rb");
	if (fp != NULL)
	{
		memset(buf, 0, size);
		ret = fread(buf, sizeof(char), size, fp); /* 返回值是读取的内容数量 */
		fclose(fp);
	}
	if (ret <= 0)
	{
	}
	return ret;
}

/* 保存文件，直接覆盖或创建 */
int App_lib::write_file(const char *path_name, char *buf, int size)
{
	FILE *fp = NULL;	/* 用于打开指定的文件 */
	fp = fopen(path_name, "w+");
	if (fp != NULL)
	{
		fwrite(buf, size, 1, fp);
		fclose(fp);
	}
	return 0;
}

int App_lib::double_compare(double dA, double dB)
{
	const   double   dEpsilon   =   0.0001;
	if (fabs(dA - dB) < dEpsilon)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}
