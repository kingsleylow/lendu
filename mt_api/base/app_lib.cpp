/*
 * app_lib.cpp
 *
 *  Created on: 2016��11��24��
 *      Author: J00054
 */
#include <string.h>			//��׼�����������
#include <stdio.h>			//��׼�����������
#include <stdlib.h>			//��׼�����ⶨ��
#include <math.h>			//��׼�����ⶨ��
#include "app_lib.h"
/*******************************************************************************
����: app_malloc()
����: ����һ��ָ����С���ڴ棬����¼����Ĵ�С
����:
None
����:
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
����: app_free()
����: �ͷ�ָ�����ڴ棬���ҵ���Ӧ���ڴ��ڵ㣬��սڵ���Ϣ�������ͷŽڵ㱾��ռ�õ��ڴ�
����:
None
����:
None
*******************************************************************************/
void App_lib::app_free(char *addr)
{
	free(addr);
}

/*******************************************************************************
* ������: read_file()
* ����: ��ȡ�ļ�����ָ�����ļ����ݶ���buf�У���ȡ�Ĵ�С��sizeָ��
* ����:
*		path_name: �ļ�·��
*		buf: (���)���ݻ�����
*		size: ��ȡ���ݵĳ���
* ����ֵ: ʵ�ʶ�ȡ�������ݳ���
*******************************************************************************/
int App_lib::read_file(const char *path_name, char *buf, int size)
{
	FILE *fp = NULL;	/* ���ڴ�ָ�����ļ� */
	int ret = 0;

	fp = fopen(path_name, "rb");
	if (fp != NULL)
	{
		memset(buf, 0, size);
		ret = fread(buf, sizeof(char), size, fp); /* ����ֵ�Ƕ�ȡ���������� */
		fclose(fp);
	}
	if (ret <= 0)
	{
	}
	return ret;
}

/* �����ļ���ֱ�Ӹ��ǻ򴴽� */
int App_lib::write_file(const char *path_name, char *buf, int size)
{
	FILE *fp = NULL;	/* ���ڴ�ָ�����ļ� */
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
