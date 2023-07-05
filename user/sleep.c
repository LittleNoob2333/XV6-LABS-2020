#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
/*
int argc:参数个数
char *argv[]:字符串数组,用来存放指向的字符参数的指针数组
argv[0]:第一个参数
argv[1]:第二个参数
以此类推
*/


int main(int argc, char *argv[])
{
    if(argc<=1)
    {
        printf("lack of parameter(Integer)\n");
        exit(1);
    }
    else
    {
        int sleep_time = atoi(argv[1]);
        sleep(sleep_time);
    }
    exit(0);
    return 0;
}