#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>

#include "head.h"
#include "control.h"
#include "input.h"
#include "output.h"
#include "queueServe.h"
#include "vip.h"

int main()
{
    vipMaintenance();
    Initialization1();

    if((diaryPtr=fopen("bankDiary.dat","w"))==NULL)
        printf("文件无法打开\n");
    else{
        //创建线程1：用于从键盘读取指令
        DWORD ThreadID1 = 1;
        HANDLE hRead1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)getInput, NULL, 0, &ThreadID1);

        //创建线程2：用于窗口对客户的服务
        DWORD ThreadID2 = 2;
        HANDLE hRead2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)queueServe, NULL, 0, &ThreadID2);
        //创建线程3：用于输出窗口状态
        DWORD ThreadID3 = 3;
        HANDLE hRead3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)printOutput, NULL, 0, &ThreadID3);


        CloseHandle(hRead1);
        CloseHandle(hRead2);
        CloseHandle(hRead3);

        while(1){     //设置死循环，每次判断一下下班
            judgeQuit();
            if(isquit==finish_working)
                break;  //下班时，跳出循环，结束程序
        }

    }
    writeDiary();       //记录日志
    Sleep(1000);
    return 0;
}
