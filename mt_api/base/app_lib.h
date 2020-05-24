/*
 * app_lib.h
 *
 *  Created on: 2016年11月24日
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
    Func: 比较两个 double型数据的大小,精度为：0.0001
    Para:
    In
    dA: 数据A,double型
    dB: 数据B，double型
    Ret:
    0: 相等
    1: 不相等
    ----------------------------------------------------------------*/
    static int double_compare(double dA, double dB);
};

#endif /* INCLUDE_BASE_APP_LIB_H_ */
