/*
 * app_lib.h
 *
 *  Created on: 2016��11��24��
 *      Author: J00054
 */

#ifndef INCLUDE_BASE_APP_LIB_H_
#define INCLUDE_BASE_APP_LIB_H_

#include "typedef.h"
class App_lib
{
public:
	App_lib() {};
    ~App_lib() {};

    static void *app_malloc(u32 size);

    static void app_free(char *addr);

    static int read_file(const char *path_name, char *buf, int size);

    static int write_file(const char *path_name, char *buf, int size);

    /*----------------------------------------------------------------
    Name: CompareDouble
    Func: �Ƚ����� double�����ݵĴ�С,����Ϊ��0.0001
    Para:
    In
    dA: ����A,double��
    dB: ����B��double��
    Ret:
    0: ���
    1: �����
    ----------------------------------------------------------------*/
    static int double_compare(double dA, double dB);
};

#endif /* INCLUDE_BASE_APP_LIB_H_ */
